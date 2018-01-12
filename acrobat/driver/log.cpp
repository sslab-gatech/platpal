#include "defs.h"

/* globals */
ASFile gLog;

void LOG_STR(const char *buf)
{
  int len = strlen(buf);
  if(ASFileWrite(gLog, buf, len) != len){
    AVAlertNote("Error writing to logs");
    AVAlertNote(buf);
  }

  ASFileWrite(gLog, "\n", 1);
  ASFileFlush(gLog);
}

static void __LOG_MSG(const char *tag, const char *fmt, va_list args) 
{
  int rv;
  char *msg;
  char *buf;

  rv = ASvasprintf(&msg, fmt, args);
  ACROASSERT(rv > 0);

  /* get timestamp */
  ASDate t = ASDateNew();
  ASDateSetToCurrentLocalTime(t);
  char *tstr = ASDateGetTimeString(t, kASTimeUniversal);

  /* get pid, tid */
  unsigned int pid;
  unsigned long tid;

#if defined(MAC_PLATFORM)
  pid = getpid();
  tid = syscall(SYS_thread_selfid);
#elif defined(WIN_PLATFORM)
  pid = GetCurrentProcessId();
  tid = GetCurrentThreadId();
#endif

  /* format output */
  rv = ASasprintf(&buf, "[%s] %u/%lu %s %s\n", tstr, pid, tid, tag, msg);
  ACROASSERT(rv > 0);

  if(ASFileWrite(gLog, buf, rv) != rv){
    AVAlertNote("Error writing to log");
    AVAlertNote(buf);
  }

  ASFileFlush(gLog);
  ASfree(msg);
  ASfree(tstr);
}

void LOG_MSG(const char *tag, const char *fmt, ...)
{
  va_list args;
  va_start(args, fmt);

  __LOG_MSG(tag, fmt, args);

  va_end(args);
}

void LOG_ERR(const char *fmt, ...)
{
  va_list args;
  va_start(args, fmt);

  __LOG_MSG("ERR", fmt, args);

  va_end(args);
}

void LOG_ERR_FUNC_MSG(const char* func, const char *msg)
{
  LOG_ERR("%s: %s", func, msg);
}

void LOG_ERR_FUNC_ENO(const char* func, ASErrorCode eno)
{
  char msg[OPT_SIZE_BUF];
  memset(msg, 0, OPT_SIZE_BUF);
  ASGetErrorString(eno, msg, OPT_SIZE_BUF);

  LOG_ERR("%s: %s", func, msg);
}

void LOG_SEC_BEGIN(const char *section) 
{
  LOG_MSG("SEC", "%s:BEGIN", section);
}

void LOG_SEC_END(const char *section) 
{
  LOG_MSG("SEC", "%s:END", section);
}

void LOG_SEC_BEGIN_FMT(const char *fmt, ...)
{
  int r;
  char *buf;

  va_list args;
  va_start(args, fmt);

  r = ASvasprintf(&buf, fmt, args);
  ACROASSERT(r > 0);

  va_end(args);

  LOG_SEC_BEGIN(buf);

  ASfree(buf);
}

void LOG_SEC_END_FMT(const char *fmt, ...)
{
  int r;
  char *buf;

  va_list args;
  va_start(args, fmt);

  r = ASvasprintf(&buf, fmt, args);
  ACROASSERT(r > 0);

  va_end(args);

  LOG_SEC_END(buf);

  ASfree(buf);
}

void LOG_CBK(const char *name)
{
  LOG_MSG("CBK", "%s", name);
}

void LOG_AS(const char *fmt, ...)
{
  va_list args;
  va_start(args, fmt);

  __LOG_MSG("AS", fmt, args);

  va_end(args);
}

void LOG_COS(const char *fmt, ...)
{
  va_list args;
  va_start(args, fmt);

  __LOG_MSG("COS", fmt, args);

  va_end(args);
}

void LOG_CONS(const char *fmt, ...)
{
  va_list args;
  va_start(args, fmt);

  __LOG_MSG("CONS", fmt, args);

  va_end(args);
}

void LOG_PD(const char *fmt, ...)
{
  va_list args;
  va_start(args, fmt);

  __LOG_MSG("PD", fmt, args);

  va_end(args);
}

void LOG_FORM(const char *fmt, ...)
{
  va_list args;
  va_start(args, fmt);

  __LOG_MSG("FORM", fmt, args);

  va_end(args);
}

void LOG_JS(const char *fmt, ...)
{
  va_list args;
  va_start(args, fmt);

  __LOG_MSG("JS", fmt, args);

  va_end(args);
}
