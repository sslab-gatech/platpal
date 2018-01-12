#include "defs.h"

/* globals */
ASFileSysRec fileSys;
static ASFileSys defFileSys;
static ASAtom rplcFileSys_K;

/* utils */
static void LogOpByPath(const char *op, ASPathName p,
    const char *fmt, ...)
{
  va_list ap;
  int rv;
  char *args = NULL;

  if(fmt){
    va_start(ap, fmt);
    rv = ASvasprintf(&args, fmt, ap);
    va_end(ap);
    ACROASSERT(rv > 0);
  }

  char *pn = fileSys.urlFromPath(p);

  if(args){
    LOG_AS("%s(%s, %s)", op, pn, args);
    ASfree(args);
  } else {
    LOG_AS("%s(%s)", op, pn);
  }

  ASfree(pn);
}

static void LogOpByFile(const char *op, ASMDFile f,
    const char *fmt, ...)
{
  va_list ap;
  int rv;
  char *args = NULL;

  if(fmt){
    va_start(ap, fmt);
    rv = ASvasprintf(&args, fmt, ap);
    va_end(ap);
    ACROASSERT(rv > 0);
  }

  char *pn;
  ASFile fobj;

  if(ASFileFromMDFile(f, &fileSys, &fobj)){
    pn = ASFileGetURL(fobj);
  } else {
    pn = ASstrdup("UNKNOWN");
  }

  if(args){
    LOG_AS("%s(%s, %s)", op, pn, args);
    ASfree(args);
  } else {
    LOG_AS("%s(%s)", op, pn);
  }

  ASfree(pn);
}

/*-------------------------------------------------------
  File System Callbacks
  -------------------------------------------------------*/

/* MyASFileSysOpenProc
 ** ------------------------------------------------------
 ** */
/** Called when opening a file.
 **
 ** @return 0 if success, else a platform-dependent error code
 ** */
static ACCB1 ASInt32 ACCB2 MyASFileSysOpenProc (ASPathName pathName, ASUns16 mode, ASMDFile *fP)
{
  LogOpByPath("open", pathName, "0x%x", mode);
  return defFileSys->open (pathName, mode, fP);
}

static ACCB1 ASInt32 ACCB2 MyASFileSysOpen64Proc (ASPathName pathName, ASUns16 mode, ASMDFile *fP)
{
  LogOpByPath("open", pathName, "0x%x", mode);
  return defFileSys->open64 (pathName, mode, fP);
}

/* MyASFileSysCloseProc
 ** ------------------------------------------------------
 ** */
/** Called when closing a file.
 **
 ** @return 0 if success, else a platform-dependent error code
 ** */
static ACCB1 ASInt32 ACCB2 MyASFileSysCloseProc (ASMDFile f)
{
  LogOpByFile("close", f, NULL);
  return defFileSys->close (f);
}

/* MyASFileSysFlushProc
 ** ------------------------------------------------------
 ** */
/** Called when flushing a file.
 **
 ** @return 0 if success, else a platform-dependent error code
 ** */
static ACCB1 ASInt32 ACCB2 MyASFileSysFlushProc (ASMDFile f)
{
  return defFileSys->flush (f);
}

/* MyASFileSysSetPosProc
 ** ------------------------------------------------------
 ** */
/** Called when setting a position within a file.
 **
 ** @return 0 if success, else a platform-dependent error code
 ** */
static ACCB1 ASInt32 ACCB2 MyASFileSysSetPosProc (ASMDFile f, ASUns32 pos)
{
  return defFileSys->setpos (f,pos);
}

static ACCB1 ASInt32 ACCB2 MyASFileSysSetPos64Proc (ASMDFile f, ASUns64 pos)
{
  return defFileSys->setpos64 (f,pos);
}

/* MyASFileSysGetPosProc
 ** ------------------------------------------------------
 ** */
/** For getting the marker position in a file.
 **
 ** @return 0 if success, else a platform-dependent error code
 ** */
static ACCB1 ASInt32 ACCB2 MyASFileSysGetPosProc (ASMDFile f, ASUns32 *pos)
{
  return defFileSys->getpos (f, pos);
}

static ACCB1 ASInt32 ACCB2 MyASFileSysGetPos64Proc (ASMDFile f, ASUns64 *pos)
{
  return defFileSys->getpos64 (f, pos);
}

/* MyASFileSysSetEofProc
 ** ------------------------------------------------------
 ** */
/** For setting the end of file marker
 **
 ** @return 0 if success, else a platform-dependent error code
 */
static  ACCB1 ASInt32 ACCB2 MyASFileSysSetEofProc (ASMDFile f, ASUns32 pos)
{
  LogOpByFile("eof", f, "%u", pos);
  return defFileSys->seteof (f, pos);
}

static  ACCB1 ASInt32 ACCB2 MyASFileSysSetEof64Proc (ASMDFile f, ASUns64 pos)
{
  LogOpByFile("eof", f, "%lu", pos);
  return defFileSys->seteof64 (f, pos);
}


/* MyASFileSysGetEofProc
 ** ------------------------------------------------------
 ** */
/** For getting the end of file marker
 **
 ** @return 0 if success, else a platform-dependent error code
 ** */
static ACCB1 ASInt32 ACCB2 MyASFileSysGetEofProc (ASMDFile f, ASUns32 *pos)
{
  return defFileSys->geteof (f, pos);
}

static ACCB1 ASInt32 ACCB2 MyASFileSysGetEof64Proc (ASMDFile f, ASUns64 *pos)
{
  return defFileSys->geteof64 (f, pos);
}

/* MyASFileSysReadProc
 ** ------------------------------------------------------
 ** */
/** Used for reading from the file
 **
 ** @return the number of bytes read, or 0 if there was an error
 */
static ACCB1 ASSize_t ACCB2 MyASFileSysReadProc (void *ptr, ASSize_t size, ASSize_t count, ASMDFile f, ASInt32 *pError)
{
  ASUns32 pos;
  ACROASSERT(MyASFileSysGetPosProc(f, &pos) == 0);
  LogOpByFile("read", f, "%u, %zu, %zu", pos, size, count); 
  return defFileSys->read (ptr, size, count, f, pError);
}


/* MyASFileSysWriteProc
 ** ------------------------------------------------------
 ** */
/** Used for writing to a file
 **
 ** @return the number of bytes written, or 0 if there was an error
 ** */
static ACCB1 ASSize_t ACCB2 MyASFileSysWriteProc (void *ptr, ASSize_t size, ASSize_t count, ASMDFile f, ASInt32 *pError)
{
  ASUns32 pos;
  ACROASSERT(MyASFileSysGetPosProc(f, &pos) == 0);
  LogOpByFile("write", f, "%u, %zu, %zu", pos, size, count);
  return defFileSys->write (ptr, size, count, f, pError);
}


/* MyASFileSysRemoveProc
 ** ------------------------------------------------------
 ** */
/** Called when removing a file
 **
 ** @return 0 if success, else a platform-dependent error code
 ** */
static ACCB1 ASInt32 ACCB2 MyASFileSysRemoveProc (ASPathName pathName)
{
  return defFileSys->remove (pathName);
}

/* MyASFileSysRenameProc
 ** ------------------------------------------------------
 ** */
/** Called when renaming a file.
 **
 ** @return 0 if success, else a platform-dependent error code
 ** */
static ACCB1 ASInt32 ACCB2 MyASFileSysRenameProc (ASMDFile* f, ASPathName oldPath, ASPathName newPath)
{
  return defFileSys->rename (f, oldPath, newPath);
}

/* MyASFileSysIsSameFileProc
 ** ------------------------------------------------------
 ** */
/** Used for comparing two ASPathNames to see if they are the same file
 **
 ** @return true if they are the same file
 ** */
static ACCB1 ASBool ACCB2 MyASFileSysIsSameFileProc (ASMDFile f, ASPathName pathName, ASPathName newPathName)
{
  return defFileSys->isSameFile(f, pathName, newPathName);
}

/* MyASFileSysGetNameProc
 ** ------------------------------------------------------
 ** */
/** Called to get the name of a file, (not the full path)
 **
 ** @return 0 if success, else a platform-dependent error code
 */
static ACCB1 ASInt32 ACCB2 MyASFileSysGetNameProc (ASPathName pathName, char *name, ASInt32 maxLength)
{
  return defFileSys->getName (pathName, name, maxLength);
}

/*MyASFileSysGetNameAsASTextProc
 ** -------------------------------------------------------
 ** */
/** Called to get the file name as an ASText object
 **
 ** @return 0 if success, else a platform-dependent error code
 */
static ACCB1 ASInt32 ACCB2 MyASFileSysGetNameAsASTextProc (ASPathName pathName, ASText name)
{
  return defFileSys->getNameAsASText (pathName, name);
}

/* MyASFileSysGetTempPathNameProc
 ** ------------------------------------------------------
 ** */
/** Called to get a valid temporary file name, if pathName is not null, will return a path that
 ** allows for renaming of the file
 **
 ** @return ASPathName pointing at file, or NULL.
 ** */
static ACCB1 ASPathName ACCB2 MyASFileSysGetTempPathNameProc (ASPathName pathName)
{
  return defFileSys->getTempPathName (pathName);
}

/* MyASFileSysCopyPathNameProc
 ** ------------------------------------------------------
 ** */
/** Creates a copy of the path
 **
 ** @return new ASPathName object
 */
static ACCB1 ASPathName ACCB2 MyASFileSysCopyPathNameProc (ASPathName pathName)
{
  return defFileSys->copyPathName (pathName);
}

/* MyASFileSysDiPathFromPathProc
 ** ------------------------------------------------------
 ** */
/** Returns a Device independent path.
 ** This string should be freed with ASfree when the
 ** caller is done with it
 **
 ** @return a string with that path.
 ** */
static ACCB1 char * ACCB2 MyASFileSysDiPathFromPathProc (ASPathName path, ASPathName relativeToThisPath)
{
  return defFileSys->diPathFromPath (path, relativeToThisPath);
}

/* MyASFileSysPathFromDIPathProc
 ** ------------------------------------------------------
 ** */
/** Converts a device independent path to a file system dependent path
 **
 ** @return ASPathName for the path
 ** */
static ACCB1 ASPathName ACCB2 MyASFileSysPathFromDIPathProc (const char * diPath, ASPathName relativeToThisPath)
{
  return defFileSys->pathFromDIPath (diPath, relativeToThisPath);
}

/* MyASFileSysDisposePathNameProc
 ** ------------------------------------------------------
 ** */
/** Frees the ASPathName object. Does not do file-level operations.
*/
static ACCB1 void ACCB2 MyASFileSysDisposePathNameProc (ASPathName pathName)
{
  defFileSys->disposePathName (pathName);
  return;
}

/* MyASFileSysGetFileSysNameProc
 ** ------------------------------------------------------
 ** */
/** Get the name of the file system
 **
 ** @return an ASAtom containing the file system name
 ** */
static ACCB1 ASAtom ACCB2 MyASFileSysGetFileSysNameProc (void)
{
  return rplcFileSys_K;
}

/* MyASFileSysGetStorageFreeSpaceProc
 ** ------------------------------------------------------
 ** */
/** Gets the free space on the same volume as the pathname
 **
 ** @return the free space in bytes. The upper limit is 4 GB, because it returns an ASInt32
 ** */
static ACCB1 ASUns32 ACCB2 MyASFileSysGetStorageFreeSpaceProc (ASPathName pathName)
{
  return defFileSys->getStorageFreeSpace (pathName);
}

/* MyASFileSysFlushVolumeProc
 ** ------------------------------------------------------
 ** */
/** FlushVolume is not present in default filesys on
 ** Windows, but it is present on the Macintosh.
 **
 ** @return 0 if success, otherwise returns a platform-
 ** dependent error code.
 */
static ACCB1 ASInt32 ACCB2 MyASFileSysFlushVolumeProc (ASPathName pathName)
{
#if MAC_PLATFORM
  return defFileSys->flushVolume (pathName);
#endif
  return 0;

}

/* MyASFileSysCreatePathNameProc
 ** ------------------------------------------------------
 ** */
/** Prompt user for a file.
 **
 ** @return ASPathName pointing at file, or NULL
 ** */
static ACCB1 ASPathName ACCB2 MyASFileSysCreatePathNameProc (ASAtom pathSpecType, const void *pathSpec, const void *mustBeZero)
{
  return defFileSys->createPathName (pathSpecType, pathSpec, mustBeZero);
}

/* MyASFileSysGetItemPropsProc
 ** ------------------------------------------------------
 **
 **
 **
 **
 */
static ACCB1 ASInt32 ACCB2 MyASFileSysGetItemPropsProc (ASPathName pathName, ASFileSysItemProps props)
{
  return defFileSys->getItemProps (pathName, props);
}

/* MyASFileSysFirstFolderItemProc
 ** ------------------------------------------------------
 **
 **
 **
 **
 */
static ACCB1 ASFolderIterator ACCB2 MyASFileSysFirstFolderItemProc (ASPathName folderPath, ASFileSysItemProps props, ASPathName *itemPath)
{
  return defFileSys->firstFolderItem (folderPath, props, itemPath);
}

/* MyASFileSysNextFolderItemProc
 ** ------------------------------------------------------
 **
 **
 **
 **
 */
static ACCB1 ASBool ACCB2 MyASFileSysNextFolderItemProc (ASFolderIterator folderIter, ASFileSysItemProps props, ASPathName *itemPath)
{
  return defFileSys->nextFolderItem (folderIter, props, itemPath);
}

/* MyASFileSysDestroyFolderIteratorProc
 ** ------------------------------------------------------
 **
 **
 **
 **
 */
static ACCB1 void ACCB2 MyASFileSysDestroyFolderIteratorProc (ASFolderIterator folderIter)
{
  defFileSys->destroyFolderIterator (folderIter);
}

/* MyASFileSysURLFromPathProc
 ** ------------------------------------------------------
 **
 **
 **
 **
 */
static ACCB1 char * ACCB2 MyASFileSysURLFromPathProc (ASPathName path)
{
  return defFileSys->urlFromPath (path);
}

/* MyASFileSysGetParentProc
 ** ------------------------------------------------------
 **
 **
 **
 **
 */
static ACCB1 ASPathName ACCB2 MyASFileSysGetParentProc (ASPathName path)
{
  return defFileSys->getParent (path);
}

/* MyASFileSysCreateFolderProc
 ** ------------------------------------------------------
 **
 **
 **
 **
 */
static ACCB1 ASInt32 ACCB2 MyASFileSysCreateFolderProc (ASPathName path)
{
  return defFileSys->createFolder (path);
}

/* MyASFileSysRemoveFolderProc
 ** ------------------------------------------------------
 **
 **
 **
 **
 */
static ACCB1 ASInt32 ACCB2 MyASFileSysRemoveFolderProc (ASPathName path)
{
  return defFileSys->removeFolder (path);
}

/* MyASFileSysDisplayStringFromPathProc
 ** ------------------------------------------------------
 **
 **
 **
 **
 */
static ACCB1 char * ACCB2 MyASFileSysDisplayStringFromPathProc (ASPathName path)
{
  return defFileSys->displayStringFromPath (path);
}

/* MyASFileSysSetTypeAndCreatorProc
 ** ------------------------------------------------------
 **
 **
 **
 **
 */
static ACCB1 void ACCB2 MyASFileSysSetTypeAndCreatorProc (ASPathName path, ASUns32 type, ASUns32 creator)
{
#if MAC_PLATFORM
  defFileSys->setTypeAndCreator (path, type, creator);
#endif
}

/* MyASFileSysGetTypeAndCreatorProc
 ** ------------------------------------------------------
 **
 **
 **
 **
 */
static ACCB1 void ACCB2 MyASFileSysGetTypeAndCreatorProc (ASPathName path, ASUns32 *type, ASUns32 *creator)
{
#if MAC_PLATFORM
  defFileSys->getTypeAndCreator (path, type, creator);
#endif
}

/* MyASFileSysAcquirePlatformPathProc
 ** ------------------------------------------------------
 **
 **
 **
 **
 */
  static ACCB1 ASInt32 ACCB2 MyASFileSysAcquirePlatformPathProc
(ASPathName path, ASAtom platformPathType, ASPlatformPath *platformPath)
{
  return defFileSys->acquirePlatformPath (path, platformPathType, platformPath);
}

/* MyASFileSysReleasePlatformPathProc
 ** ------------------------------------------------------
 **
 **
 **
 **
 */
  static ACCB1 void ACCB2 MyASFileSysReleasePlatformPathProc
(ASPlatformPath platformPath)
{
  return defFileSys->releasePlatformPath (platformPath);
}

/* InitializeFileSystem
 ** ------------------------------------------------------
 ** */
/** Sets up all the callbacks for the file system.
 **
 ** @return true if the file system was registered
 ** successfully.
 ** */
ASBool InitializeFileSystem ()
{
  defFileSys = ASGetDefaultFileSys();
  rplcFileSys_K = ASAtomFromString("ADBE:RplcFileSys");

  memset (&fileSys, 0, sizeof(ASFileSysRec));

  fileSys.size = sizeof(ASFileSysRec);

  fileSys.open = ASCallbackCreateProto (ASFileSysOpenProc, MyASFileSysOpenProc);
  fileSys.open64 = ASCallbackCreateProto (ASFileSysOpen64Proc, MyASFileSysOpen64Proc);
  fileSys.close = ASCallbackCreateProto (ASFileSysCloseProc, MyASFileSysCloseProc);
  fileSys.flush = ASCallbackCreateProto (ASFileSysFlushProc, MyASFileSysFlushProc);
  fileSys.setpos = ASCallbackCreateProto (ASFileSysSetPosProc, MyASFileSysSetPosProc);
  fileSys.setpos64 = ASCallbackCreateProto (ASFileSysSetPos64Proc, MyASFileSysSetPos64Proc);
  fileSys.getpos = ASCallbackCreateProto (ASFileSysGetPosProc, MyASFileSysGetPosProc);
  fileSys.getpos64 = ASCallbackCreateProto (ASFileSysGetPos64Proc, MyASFileSysGetPos64Proc);
  fileSys.seteof = ASCallbackCreateProto (ASFileSysSetEofProc, MyASFileSysSetEofProc);
  fileSys.seteof64 = ASCallbackCreateProto (ASFileSysSetEof64Proc, MyASFileSysSetEof64Proc);
  fileSys.geteof = ASCallbackCreateProto (ASFileSysGetEofProc, MyASFileSysGetEofProc);
  fileSys.geteof64 = ASCallbackCreateProto (ASFileSysGetEof64Proc, MyASFileSysGetEof64Proc);
  fileSys.read = ASCallbackCreateProto (ASFileSysReadProc, MyASFileSysReadProc);
  fileSys.write = ASCallbackCreateProto (ASFileSysWriteProc, MyASFileSysWriteProc);
  fileSys.remove = ASCallbackCreateProto (ASFileSysRemoveProc, MyASFileSysRemoveProc);
  fileSys.rename = ASCallbackCreateProto (ASFileSysRenameProc, MyASFileSysRenameProc);
  fileSys.isSameFile = ASCallbackCreateProto (ASFileSysIsSameFileProc, MyASFileSysIsSameFileProc);
  fileSys.getName = ASCallbackCreateProto (ASFileSysGetNameProc, MyASFileSysGetNameProc);
  fileSys.getNameAsASText = ASCallbackCreateProto (ASFileSysGetNameAsASTextProc, MyASFileSysGetNameAsASTextProc);
  fileSys.getTempPathName = ASCallbackCreateProto (ASFileSysGetTempPathNameProc, MyASFileSysGetTempPathNameProc);
  fileSys.copyPathName = ASCallbackCreateProto (ASFileSysCopyPathNameProc, MyASFileSysCopyPathNameProc);
  fileSys.diPathFromPath = ASCallbackCreateProto (ASFileSysDiPathFromPathProc, MyASFileSysDiPathFromPathProc);
  fileSys.pathFromDIPath = ASCallbackCreateProto (ASFileSysPathFromDIPathProc, MyASFileSysPathFromDIPathProc);
  fileSys.disposePathName = ASCallbackCreateProto (ASFileSysDisposePathNameProc, MyASFileSysDisposePathNameProc);
  fileSys.getFileSysName = ASCallbackCreateProto (ASFileSysGetFileSysNameProc, MyASFileSysGetFileSysNameProc);
  fileSys.getStorageFreeSpace = ASCallbackCreateProto (ASFileSysGetStorageFreeSpaceProc, MyASFileSysGetStorageFreeSpaceProc);
  fileSys.flushVolume = ASCallbackCreateProto (ASFileSysFlushVolumeProc, MyASFileSysFlushVolumeProc);
  fileSys.createPathName = ASCallbackCreateProto (ASFileSysCreatePathNameProc, MyASFileSysCreatePathNameProc);

  fileSys.getItemProps = ASCallbackCreateProto (ASFileSysGetItemPropsProc, MyASFileSysGetItemPropsProc);
  fileSys.firstFolderItem = ASCallbackCreateProto (ASFileSysFirstFolderItemProc, MyASFileSysFirstFolderItemProc);
  fileSys.nextFolderItem = ASCallbackCreateProto (ASFileSysNextFolderItemProc, MyASFileSysNextFolderItemProc);
  fileSys.destroyFolderIterator = ASCallbackCreateProto (ASFileSysDestroyFolderIteratorProc, MyASFileSysDestroyFolderIteratorProc);
  fileSys.urlFromPath = ASCallbackCreateProto (ASFileSysURLFromPathProc, MyASFileSysURLFromPathProc);
  fileSys.getParent = ASCallbackCreateProto (ASFileSysGetParentProc, MyASFileSysGetParentProc);
  fileSys.createFolder = ASCallbackCreateProto (ASFileSysCreateFolderProc, MyASFileSysCreateFolderProc);
  fileSys.removeFolder = ASCallbackCreateProto (ASFileSysRemoveFolderProc, MyASFileSysRemoveFolderProc);
  fileSys.displayStringFromPath = ASCallbackCreateProto (ASFileSysDisplayStringFromPathProc, MyASFileSysDisplayStringFromPathProc);
  fileSys.setTypeAndCreator = ASCallbackCreateProto (ASFileSysSetTypeAndCreatorProc, MyASFileSysSetTypeAndCreatorProc);
  fileSys.getTypeAndCreator = ASCallbackCreateProto (ASFileSysGetTypeAndCreatorProc, MyASFileSysGetTypeAndCreatorProc);

  //These need to be implemented for 7.0 Windows
  fileSys.acquirePlatformPath = ASCallbackCreateProto (ASFileSysAcquirePlatformPathProc, MyASFileSysAcquirePlatformPathProc);
  fileSys.releasePlatformPath = ASCallbackCreateProto (ASFileSysReleasePlatformPathProc, MyASFileSysReleasePlatformPathProc);

  /* The following are other callbacks we don't implement.

     fileSys.getFileFlags
     fileSys.readAsync
     fileSys.writeAsync
     fileSys.abortAsync
     fileSys.yield
     fileSys.mreadRequest
     fileSys.getStatus
     fileSys.acquireFileSysPath
     fileSys.clearOutstandingMReads
     fileSys.setFileMode
     fileSys.reopen
     fileSys.hardFlush
     fileSys.getItemPropsAsCab;
     fileSys.canPerformOpOnItem;
     fileSys.performOpOnItem;

     fileSys.displayASTextFromPath;
     fileSys.rangeArrived;
     fileSys.canSetEof;
     fileSys.diPathFromPathEx;
     fileSys.pathFromDIPathEx;
     fileSys.getfileposlimit;
     fileSys.getNameForDisplay;
     fileSys.ASFileSysGetStorageFreeSpace64Proc
     */

  return ASFileRegisterFileSys (gExtensionID, &fileSys);
}

/* UninitializeFileSystem
 ** ------------------------------------------------------
 */
void UninitializeFileSystem()
{
  ASFileUnregisterFileSys (gExtensionID, &fileSys);
}
