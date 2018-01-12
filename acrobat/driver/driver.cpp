#include "defs.h"

/* hfts */
HFT gAcroFormHFT = NULL;
HFT gConsultantHFT = NULL;

/* doc layers */
AVDoc gAVDoc = NULL;
PDDoc gPDDoc = NULL;
ASFile gASDoc = NULL;

CosDoc gCosDoc;

/* callback logic */
#include "callback-logic.cpp"

/* execution */
static ASBool RunSample(const char *path)
{
  int i, j;

  AVPageView pv;
  ASCount pvnum;
  ASInt32 pgnum;

  /* prepare path */
  ASPathName pn =
    ASFileSysCreatePathFromCString(&fileSys, path);

  if(pn == NULL){
    LOG_ERR_FUNC_MSG("ASFileSysCreatePathFromCString", OPT_ERR_NULL);
    return false;
  }

  ASErrorCode rv;

  /* open AS-level doc */
  LOG_SEC_BEGIN("ASLoad");

  rv = ASFileSysOpenFile64(&fileSys, pn, ASFILE_READ, &gASDoc);
  if(rv != 0){
    LOG_ERR_FUNC_ENO("ASFileSysOpenFile64", rv);
    goto err_as;
  }

  LOG_SEC_END("ASLoad");

  /* open PD-level doc */
  LOG_SEC_BEGIN("PDLoad");

  DURING;
  gPDDoc = PDDocOpenFromASFileEx(gASDoc, NULL, NULL, true);
  if(gPDDoc == NULL){
    LOG_ERR_FUNC_MSG("PDDocOpenFromASFileEx", OPT_ERR_NULL);
    ASRaise(1);
  }
  rv = 0;
  HANDLER;
  rv = ERRORCODE;
  END_HANDLER;

  if(rv != 0){
    LOG_ERR_FUNC_ENO("PDDocOpenFromASFileEx", rv);
    goto err_pd;
  }

  LOG_SEC_END("PDLoad");

  /* grab Cos-level doc */
  gCosDoc = PDDocGetCosDoc(gPDDoc);
  if(gCosDoc == NULL){
    LOG_ERR_FUNC_MSG("PDDocGetCosDoc", OPT_ERR_NULL);
    goto err_pd;
  }

  if(!ExamineCosDoc(gCosDoc)){
    goto err_av;
  }

  /* check consultant */
  if(!ExamineConsultant(gCosDoc)){
    goto err_av;
  }
  
  /* check PD doc */
  if(!ExaminePDDoc(gPDDoc)){
    goto err_av;
  }
  
  /* check Acro form */
  if(!ExamineAcroForm(gPDDoc)){
    goto err_av;
  }

#ifdef OPT_UI_CHECK
  /* open AV-level doc */
  LOG_SEC_BEGIN("AVLoad");

  AVDocOpenParamsRec params;
  memset(&params, 0, sizeof(AVDocOpenParamsRec));

  params.size = sizeof(AVDocOpenParamsRec);
  params.useVisible = true;
  params.visible = true;
  params.useMinimize = true;
  params.minimize = false;
  params.suppressDialogs = true;

  params.useFrame = false;
  params.useServerType = false;
  params.useSourceDoc = false;
  params.useReadOnly = false;
  params.useViewType = false;
  params.useViewDef = false;
  params.usePermReqProc = false;
  params.useCollectionPref = false;

  DURING;
  gAVDoc = AVDocOpenFromPDDocWithParams(gPDDoc, NULL, &params);
  if(gAVDoc == NULL){
    ASRaise(1);
  }
  rv = 0;
  HANDLER;
  rv = ERRORCODE;
  END_HANDLER;

  if(rv != 0){
    LOG_ERR_FUNC_ENO("AVDocOpenFromPDDoc", rv);
    goto err_av;
  }

  LOG_SEC_END("AVLoad");

  /* done opening */
  ASFileSysReleasePath(NULL, pn);
  
#ifdef OPT_FULL_CHECK
  /* drive document */
  LOG_SEC_BEGIN("Drive");

  pgnum = PDDocGetNumPages(gPDDoc);
  pvnum = AVDocGetNumPageViews(gAVDoc);

  for(i = 0; i < pvnum; i++){
    pv = AVDocGetNthPageView(gAVDoc, i);
    AVPageViewZoomTo(pv, AVZoomFitPage, 0);

    if(pgnum > OPT_PAGE_LIMIT){
      continue;
    }

    for(j = 0; j < pgnum; j++){
      AVPageViewBeginOperation(pv);

      AVPageViewGoTo(pv, j);
      AVPageViewDrawNow(pv);

      AVPageViewEndOperation(pv);
    }
  }

  LOG_SEC_END("Drive");
#else
  /* perform actions only */
  ExamineAction(gCosDoc);
#endif

  /* done driving */
  AVDocClose(gAVDoc, true);
  gAVDoc = NULL;
  gPDDoc = NULL;
#endif

  if(gPDDoc){
    PDDocClose(gPDDoc);
    gPDDoc = NULL;
  }

  ASFileClose(gASDoc);
  gASDoc = NULL;

  return true;

err_av:
  PDDocClose(gPDDoc);

err_pd:
  ASFileClose(gASDoc);

err_as:
  ASFileSysReleasePath(NULL, pn);
  return false;
}

static void onAppInit(void *clientData)
{
  /* run sample */
  if(!RunSample(OPT_FILE_SAMPLE)){
#ifdef OPT_FULL_CHECK 
    AVAlertNote("Error running sample, check log for details");
#endif
  }

  /* unregister file system */
  UninitializeFileSystem();

  /* close log file */
  ASFileFlush(gLog);
  ASFileClose(gLog);

  /* force exit */
  exit(0);
}

/*-------------------------------------------------------
  Core Initialization Callbacks
  -------------------------------------------------------*/

/**
 * Create and register the HFT's.
 *
 * @return true to continue loading plug-in,
 * false to cause plug-in loading to stop.
 */
ACCB1 ASBool ACCB2 PluginExportHFTs(void)
{
  return true;
}

/**
 * The application calls this function to allow it to
 *  - Import plug-in supplied HFTs.
 *  - Replace functions in the HFTs you're using (where allowed).
 *  - Register to receive notification events.
 *
 *  @return true to continue loading plug-in,
 *  false to cause plug-in loading to stop.
 */
ACCB1 ASBool ACCB2 PluginImportReplaceAndRegister(void)
{
  return true;
}

/**
 * The main initialization routine.
 *
 * @return true to continue loading the plug-in,
 * false to cause plug-in loading to stop.
 */
ACCB1 ASBool ACCB2 PluginInit(void)
{
  /* set hft */
  gAcroFormHFT = Init_AcroFormHFT;
  if(!gAcroFormHFT){
    return false;
  }

  gConsultantHFT = Init_PDFConsultantHFT;
  if(!gConsultantHFT){
    return false;
  }

  /* create log file */
  ASErrorCode rv;

  ASPathName logpath =
    ASFileSysCreatePathFromCString(NULL, OPT_FILE_RESULT);

  if(logpath == NULL){
    return false;
  }

  rv = ASFileSysOpenFile64(NULL, logpath,
      ASFILE_CREATE | ASFILE_WRITE | ASFILE_SERIAL, &gLog);

  ASFileSysReleasePath(NULL, logpath);

  if(rv != 0){
    return false;
  }

  /* setup filesystem */
  if(!InitializeFileSystem()){
    AVAlertNote("Error initializing filesystem");
    return false;
  }

  /* wait for app initialize */
  AVAppRegisterNotification(AVAppDidInitializeNSEL,
      gExtensionID, 
      (void *)onAppInit, 
      NULL);

  /* register callbacks */
  #include "callback-register.cpp"

  return true;
}

/**
 * The application calls this function when the plug-in is being unloaded
 * (e.g., when the application quits).
 * Use this routine to release any system resources allocated.
 *
 * @return true to indicate the plug-in unloaded,
 * false will cause an alert to display that unloading failed
 */
ACCB1 ASBool ACCB2 PluginUnload(void)
{
  /* clean up */
  if(gAVDoc != NULL){
    AVDocClose(gAVDoc, true);
    gAVDoc = NULL;
  }

  if(gPDDoc != NULL){
    PDDocClose(gPDDoc);
    gPDDoc = NULL;
  }

  if(gASDoc != NULL){
    ASFileClose(gASDoc);
    gASDoc = NULL;
  }

  /* remove filesystem */
  UninitializeFileSystem();

  /* close log file */
  ASFileFlush(gLog);
  ASFileClose(gLog);

  return true;
}

/**
 * Returns the unique ASAtom associated with your plug-in.
 *
 * @return the plug-in's name as an ASAtom.
 */
ASAtom GetExtensionName()
{
  return ASAtomFromString("ADBE:Driver");
}

/**
 * This function provides the initial interface between the plug-in and
 * the application.
 * This function provides the callback functions to the application that
 * allow it to register the plug-in with the application environment.
 *
 * @param handshakeVersion the version this plug-in works with.
 *
 * There are two versions possible, the plug-in version and the application
 * version. The application calls the main entry point for this plug-in with
 * its version.
 *
 * The main entry point will call this function with the version that is
 * earliest.
 *
 * @param handshakeData OUT the data structure used to provide the primary
 * entry points for the plug-in. These entry points are used in registering
 * the plug-in with the application and allowing the plug-in to register for
 * other plug-in services and offer its own.
 *
 * @return true on success, false otherwise (the plug-in will not load).
 */
ACCB1 ASBool ACCB2 PIHandshake(Uns32 handshakeVersion, void *handshakeData)
{
  if (handshakeVersion == HANDSHAKE_V0200) {
    /* Cast handshakeData to the appropriate type */
    PIHandshakeData_V0200 *hsData = (PIHandshakeData_V0200 *)handshakeData;

    /* Set the name we want to go by */
    hsData->extensionName = GetExtensionName();

    /* If you export your own HFT, do so in here */
    hsData->exportHFTsCallback =
      (void *)ASCallbackCreateProto(PIExportHFTsProcType, &PluginExportHFTs);

    /*
     ** If you import plug-in HFTs, replace functionality, and/or want to
     register for notifications before the user has a chance to do anything,
     do so in here.
     */
    hsData->importReplaceAndRegisterCallback =
      (void *)ASCallbackCreateProto(PIImportReplaceAndRegisterProcType,
          &PluginImportReplaceAndRegister);

    /* Perform your plug-in's initialization in here */
    hsData->initCallback =
      (void *)ASCallbackCreateProto(PIInitProcType, &PluginInit);

    /* Perform any memory freeing or state saving on "quit" in here */
    hsData->unloadCallback =
      (void *)ASCallbackCreateProto(PIUnloadProcType, &PluginUnload);

    /* All done */
    return true;
  }
  /* Each time the handshake version changes, add a new "else if" branch */

  /*
   * If we reach here, then we were passed a handshake version number we
   * don't know about. This shouldn't ever happen since our main() routine chose
   * the version number.
   */

  return false;
}

