#!/bin/sh

USAGE="Usage: $0 <outfile> <program> <...args...>"

# opts
OPT_BUFSIZE="4m"

# args
if [[ $# < 2 ]]; then
  echo $USAGE; exit
fi

ARG_OUT=$1
shift

ARG_CMD="$*"

# main program, DTrace
dtrace='
#pragma D option quiet

/* args */
inline int af_inet = 2; 

dtrace:::BEGIN
{
  trackedpid[pid] = 0;
  self->child = 0;
  this->type = 0;
}

/* upon fork (child) */
syscall:::entry
/trackedpid[ppid] == -1 && 0 == self->child/
{
  self->child = 1;

  printf("%5d/0x%x:  ", pid, tid);
  printf("%s() = %d %d\n", "fork",
    0,
    (int)errno);
}

/* upon vfork (child and parent) */
syscall:::entry
/trackedpid[ppid] > 0 && 0 == self->child/
{
  this->vforking_tid = trackedpid[ppid];
  self->child = (this->vforking_tid == tid) ? 0 : 1;

  printf("%5d/0x%x:  ", pid, tid);
  printf("%s() = %d %d]n", "vfork", 
    (this->vforking_tid == tid) ? pid : 0,
    (int)errno);
}

/* generic entry */
syscall:::entry
/(pid == $target) || (self->child)/
{
  self->start = 1;
  self->arg0 = arg0;
  self->arg1 = arg1;
  self->arg2 = arg2;
}

/* follow children */
syscall::fork:entry
/(self->start)/
{
  trackedpid[pid] = -1;
}

syscall::vfork:entry
/(self->start)/
{
  trackedpid[pid] = tid;
}

syscall::exit:entry
{
  self->child = 0;
  trackedpid[pid] = 0;
}

syscall::execve:return,
syscall::posix_spawn:return
/(self->start)/
{
  printf("%5d/0x%x:  ", pid, tid);
  printf("%s(\"%s\") = %d %d\n", probefunc,
    execname,
    (int)arg0, (int)errno);

  self->start = 0;
  self->arg0 = 0;
  self->arg1 = 0;
  self->arg2 = 0;
}

/* The following code is written in an intentionally repetetive way */

/* (path) syscall */
syscall::unlink:return,
syscall::truncate:return,
syscall::mkdir:return,
syscall::open:return,
syscall::open_extended:return,
syscall::open_nocancel:return
/(self->start)/
{
  printf("%5d/0x%x:  ", pid, tid);
  printf("%s(\"%s\") = %d %d\n", probefunc, 
    copyinstr(self->arg0),
    (int)arg0, (int)errno);

  self->start = 0;
  self->arg0 = 0;
  self->arg1 = 0;
  self->arg2 = 0;
}

/* (fd, path) syscall */
syscall::unlinkat:return,
syscall::mkdirat:return,
syscall::openat:return
/(self->start)/
{
  printf("%5d/0x%x:  ", pid, tid);
  printf("%s(0x%X, \"%s\") = %d %d\n", probefunc,
    (int)self->arg0, copyinstr(self->arg1),
    (int)arg0, (int)errno);

  self->start = 0;
  self->arg0 = 0;
  self->arg1 = 0;
  self->arg2 = 0;
}

/* (path, path) syscall */
syscall::rename:return,
syscall::symlink:return,
syscall::link:return
/(self->start)/
{
  printf("%5d/0x%x:  ", pid, tid);
  printf("%s(\"%s\", \"%s\") = %d %d\n", probefunc,
    copyinstr(self->arg0), copyinstr(self->arg1),
    (int)arg0, (int)errno);

  self->start = 0;
  self->arg0 = 0;
  self->arg1 = 0;
  self->arg2 = 0;
}

/* (fd, path, path) syscall */
syscall::renameat:return,
syscall::symlinkat:return,
syscall::linkat:return
/(self->start)/
{
  printf("%5d/0x%x:  ", pid, tid);
  printf("%s(0x%X, \"%s\", \"%s\") = %d %d\n", probefunc,
    (int)self->arg0, copyinstr(self->arg1), copyinstr(self->arg2),
    (int)arg0, (int)errno);

  self->start = 0;
  self->arg0 = 0;
  self->arg1 = 0;
  self->arg2 = 0;
}

ip:::send
/(pid == $target) || (self->child)/
{
  printf("%5d/0x%x:  ", pid, tid);
  printf("%s(%s) = 0 0\n", "connect", args[2]->ip_daddr);
}
'

# run program
/usr/sbin/dtrace \
  -x dynvarsize=$OPT_BUFSIZE \
  -x evaltime=exec \
  -n "$dtrace" \
  -c "$ARG_CMD" \
  -o "$ARG_OUT"

