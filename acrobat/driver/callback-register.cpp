AVAppRegisterNotification(AVDocWillOpenFromFileNSEL,
    gExtensionID,
    (void *)onAVDocWillOpenFromFile,
    NULL);
AVAppRegisterNotification(AVDocDidOpenNSEL,
    gExtensionID,
    (void *)onAVDocDidOpen,
    NULL);
AVAppRegisterNotification(AVDocDidActivateNSEL,
    gExtensionID,
    (void *)onAVDocDidActivate,
    NULL);
AVAppRegisterNotification(AVDocDidDeactivateNSEL,
    gExtensionID,
    (void *)onAVDocDidDeactivate,
    NULL);
AVAppRegisterNotification(AVDocWillCloseNSEL,
    gExtensionID,
    (void *)onAVDocWillClose,
    NULL);
AVAppRegisterNotification(AVDocDidCloseNSEL,
    gExtensionID,
    (void *)onAVDocDidClose,
    NULL);
AVAppRegisterNotification(AVDocWillClearSelectionNSEL,
    gExtensionID,
    (void *)onAVDocWillClearSelection,
    NULL);
AVAppRegisterNotification(AVDocDidSetSelectionNSEL,
    gExtensionID,
    (void *)onAVDocDidSetSelection,
    NULL);
AVAppRegisterNotification(AVDocWillPerformActionNSEL,
    gExtensionID,
    (void *)onAVDocWillPerformAction,
    NULL);
AVAppRegisterNotification(AVDocDidPerformActionNSEL,
    gExtensionID,
    (void *)onAVDocDidPerformAction,
    NULL);
AVAppRegisterNotification(AVPageViewDidDrawNSEL,
    gExtensionID,
    (void *)onAVPageViewDidDraw,
    NULL);
AVAppRegisterNotification(AVPageViewDidChangeNSEL,
    gExtensionID,
    (void *)onAVPageViewDidChange,
    NULL);
AVAppRegisterNotification(PDDocWillInsertPagesNSEL,
    gExtensionID,
    (void *)onPDDocWillInsertPages,
    NULL);
AVAppRegisterNotification(PDDocDidInsertPagesNSEL,
    gExtensionID,
    (void *)onPDDocDidInsertPages,
    NULL);
AVAppRegisterNotification(PDDocWillReplacePagesNSEL,
    gExtensionID,
    (void *)onPDDocWillReplacePages,
    NULL);
AVAppRegisterNotification(PDDocDidReplacePagesNSEL,
    gExtensionID,
    (void *)onPDDocDidReplacePages,
    NULL);
AVAppRegisterNotification(PDDocWillMovePagesNSEL,
    gExtensionID,
    (void *)onPDDocWillMovePages,
    NULL);
AVAppRegisterNotification(PDDocDidMovePagesNSEL,
    gExtensionID,
    (void *)onPDDocDidMovePages,
    NULL);
AVAppRegisterNotification(PDDocWillDeletePagesNSEL,
    gExtensionID,
    (void *)onPDDocWillDeletePages,
    NULL);
AVAppRegisterNotification(PDDocDidDeletePagesNSEL,
    gExtensionID,
    (void *)onPDDocDidDeletePages,
    NULL);
AVAppRegisterNotification(PDDocWillChangePagesNSEL,
    gExtensionID,
    (void *)onPDDocWillChangePages,
    NULL);
AVAppRegisterNotification(PDDocDidChangePagesNSEL,
    gExtensionID,
    (void *)onPDDocDidChangePages,
    NULL);
AVAppRegisterNotification(PDDocDidChangeThumbsNSEL,
    gExtensionID,
    (void *)onPDDocDidChangeThumbs,
    NULL);
AVAppRegisterNotification(PDDocWillPrintPagesNSEL,
    gExtensionID,
    (void *)onPDDocWillPrintPages,
    NULL);
AVAppRegisterNotification(PDDocDidPrintPagesNSEL,
    gExtensionID,
    (void *)onPDDocDidPrintPages,
    NULL);
AVAppRegisterNotification(PDDocWillPrintPageNSEL,
    gExtensionID,
    (void *)onPDDocWillPrintPage,
    NULL);
AVAppRegisterNotification(PDDocDidPrintPageNSEL,
    gExtensionID,
    (void *)onPDDocDidPrintPage,
    NULL);
AVAppRegisterNotification(PDDocDidAddThreadNSEL,
    gExtensionID,
    (void *)onPDDocDidAddThread,
    NULL);
AVAppRegisterNotification(PDDocDidRemoveThreadNSEL,
    gExtensionID,
    (void *)onPDDocDidRemoveThread,
    NULL);
AVAppRegisterNotification(PDDocWillSaveNSEL,
    gExtensionID,
    (void *)onPDDocWillSave,
    NULL);
AVAppRegisterNotification(PDDocDidSaveNSEL,
    gExtensionID,
    (void *)onPDDocDidSave,
    NULL);
AVAppRegisterNotification(PDPageContentsDidChangeNSEL,
    gExtensionID,
    (void *)onPDPageContentsDidChange,
    NULL);
AVAppRegisterNotification(PDPageWillAddAnnotNSEL,
    gExtensionID,
    (void *)onPDPageWillAddAnnot,
    NULL);
AVAppRegisterNotification(PDPageDidAddAnnotNSEL,
    gExtensionID,
    (void *)onPDPageDidAddAnnot,
    NULL);
AVAppRegisterNotification(PDPageWillRemoveAnnotNSEL,
    gExtensionID,
    (void *)onPDPageWillRemoveAnnot,
    NULL);
AVAppRegisterNotification(PDPageDidRemoveAnnotNSEL,
    gExtensionID,
    (void *)onPDPageDidRemoveAnnot,
    NULL);
AVAppRegisterNotification(PDAnnotWasCreatedNSEL,
    gExtensionID,
    (void *)onPDAnnotWasCreated,
    NULL);
AVAppRegisterNotification(PDAnnotWillChangeNSEL,
    gExtensionID,
    (void *)onPDAnnotWillChange,
    NULL);
AVAppRegisterNotification(PDAnnotDidChangeNSEL,
    gExtensionID,
    (void *)onPDAnnotDidChange,
    NULL);
AVAppRegisterNotification(PDThreadDidChangeNSEL,
    gExtensionID,
    (void *)onPDThreadDidChange,
    NULL);
AVAppRegisterNotification(PDBookmarkWasCreatedNSEL,
    gExtensionID,
    (void *)onPDBookmarkWasCreated,
    NULL);
AVAppRegisterNotification(PDBookmarkDidChangePositionNSEL,
    gExtensionID,
    (void *)onPDBookmarkDidChangePosition,
    NULL);
AVAppRegisterNotification(PDBookmarkWillChangeNSEL,
    gExtensionID,
    (void *)onPDBookmarkWillChange,
    NULL);
AVAppRegisterNotification(PDBookmarkDidChangeNSEL,
    gExtensionID,
    (void *)onPDBookmarkDidChange,
    NULL);
AVAppRegisterNotification(PDBookmarkWillDestroyNSEL,
    gExtensionID,
    (void *)onPDBookmarkWillDestroy,
    NULL);
AVAppRegisterNotification(PDBookmarkDidDestroyNSEL,
    gExtensionID,
    (void *)onPDBookmarkDidDestroy,
    NULL);
AVAppRegisterNotification(PDDocWillRemoveThreadNSEL,
    gExtensionID,
    (void *)onPDDocWillRemoveThread,
    NULL);
AVAppRegisterNotification(PDPageContentsDidChangeExNSEL,
    gExtensionID,
    (void *)onPDPageContentsDidChangeEx,
    NULL);
AVAppRegisterNotification(AVDocWillOpenFromPDDocNSEL,
    gExtensionID,
    (void *)onAVDocWillOpenFromPDDoc,
    NULL);
AVAppRegisterNotification(AVDocDidAddToSelectionNSEL,
    gExtensionID,
    (void *)onAVDocDidAddToSelection,
    NULL);
AVAppRegisterNotification(AVDocDidRemoveFromSelectionNSEL,
    gExtensionID,
    (void *)onAVDocDidRemoveFromSelection,
    NULL);
AVAppRegisterNotification(AVDocWantsToDieNSEL,
    gExtensionID,
    (void *)onAVDocWantsToDie,
    NULL);
AVAppRegisterNotification(PDDocWillPrintDocNSEL,
    gExtensionID,
    (void *)onPDDocWillPrintDoc,
    NULL);
AVAppRegisterNotification(PDDocWillSaveExNSEL,
    gExtensionID,
    (void *)onPDDocWillSaveEx,
    NULL);
AVAppRegisterNotification(AVDocWillPrintNSEL,
    gExtensionID,
    (void *)onAVDocWillPrint,
    NULL);
AVAppRegisterNotification(AVDocDidPrintNSEL,
    gExtensionID,
    (void *)onAVDocDidPrint,
    NULL);
AVAppRegisterNotification(PDDocWillCloseNSEL,
    gExtensionID,
    (void *)onPDDocWillClose,
    NULL);
AVAppRegisterNotification(PDDocPageLabelDidChangeNSEL,
    gExtensionID,
    (void *)onPDDocPageLabelDidChange,
    NULL);
AVAppRegisterNotification(PDDocWillExportAnnotsNSEL,
    gExtensionID,
    (void *)onPDDocWillExportAnnots,
    NULL);
AVAppRegisterNotification(PDDocWillImportAnnotsNSEL,
    gExtensionID,
    (void *)onPDDocWillImportAnnots,
    NULL);
AVAppRegisterNotification(PDDocDidExportAnnotsNSEL,
    gExtensionID,
    (void *)onPDDocDidExportAnnots,
    NULL);
AVAppRegisterNotification(PSPrintAfterBeginPrologNSEL,
    gExtensionID,
    (void *)onPSPrintAfterBeginProlog,
    NULL);
AVAppRegisterNotification(PSPrintAfterBeginSetupNSEL,
    gExtensionID,
    (void *)onPSPrintAfterBeginSetup,
    NULL);
AVAppRegisterNotification(PSPrintBeforeEndSetupNSEL,
    gExtensionID,
    (void *)onPSPrintBeforeEndSetup,
    NULL);
AVAppRegisterNotification(PSPrintAfterBeginPageSetupNSEL,
    gExtensionID,
    (void *)onPSPrintAfterBeginPageSetup,
    NULL);
AVAppRegisterNotification(PSPrintAfterPageTrailerNSEL,
    gExtensionID,
    (void *)onPSPrintAfterPageTrailer,
    NULL);
AVAppRegisterNotification(PSPrintAfterTrailerNSEL,
    gExtensionID,
    (void *)onPSPrintAfterTrailer,
    NULL);
AVAppRegisterNotification(PSPrintBeforeEndCommentsNSEL,
    gExtensionID,
    (void *)onPSPrintBeforeEndComments,
    NULL);
AVAppRegisterNotification(PSPrintAfterEmitExtGStateNSEL,
    gExtensionID,
    (void *)onPSPrintAfterEmitExtGState,
    NULL);
AVAppRegisterNotification(AVDocDidClickNameNSEL,
    gExtensionID,
    (void *)onAVDocDidClickName,
    NULL);
AVAppRegisterNotification(PDDocDidImportAnnotsNSEL,
    gExtensionID,
    (void *)onPDDocDidImportAnnots,
    NULL);
AVAppRegisterNotification(PDNameTreeNameAddedNSEL,
    gExtensionID,
    (void *)onPDNameTreeNameAdded,
    NULL);
AVAppRegisterNotification(PDNameTreeNameRemovedNSEL,
    gExtensionID,
    (void *)onPDNameTreeNameRemoved,
    NULL);
AVAppRegisterNotification(PDDocDidCloseNSEL,
    gExtensionID,
    (void *)onPDDocDidClose,
    NULL);
AVAppRegisterNotification(PDDocDidInsertPagesExNSEL,
    gExtensionID,
    (void *)onPDDocDidInsertPagesEx,
    NULL);
AVAppRegisterNotification(PDDocWillInsertPagesExNSEL,
    gExtensionID,
    (void *)onPDDocWillInsertPagesEx,
    NULL);
AVAppRegisterNotification(PDNumTreeNumAddedNSEL,
    gExtensionID,
    (void *)onPDNumTreeNumAdded,
    NULL);
AVAppRegisterNotification(PDNumTreeNumRemovedNSEL,
    gExtensionID,
    (void *)onPDNumTreeNumRemoved,
    NULL);
AVAppRegisterNotification(PDBookmarkDidUnlinkNSEL,
    gExtensionID,
    (void *)onPDBookmarkDidUnlink,
    NULL);
AVAppRegisterNotification(AVDocDidDeleteSelectionNSEL,
    gExtensionID,
    (void *)onAVDocDidDeleteSelection,
    NULL);
AVAppRegisterNotification(PDDocWillPrintTiledPageNSEL,
    gExtensionID,
    (void *)onPDDocWillPrintTiledPage,
    NULL);
AVAppRegisterNotification(PDDocPrintingTiledPageNSEL,
    gExtensionID,
    (void *)onPDDocPrintingTiledPage,
    NULL);
AVAppRegisterNotification(PDDocDidPrintTiledPageNSEL,
    gExtensionID,
    (void *)onPDDocDidPrintTiledPage,
    NULL);
AVAppRegisterNotification(PDPageWillPrintAnnotsNSEL,
    gExtensionID,
    (void *)onPDPageWillPrintAnnots,
    NULL);
AVAppRegisterNotification(PDPageDidPrintAnnotsNSEL,
    gExtensionID,
    (void *)onPDPageDidPrintAnnots,
    NULL);
AVAppRegisterNotification(PDDocDidOpenNSEL,
    gExtensionID,
    (void *)onPDDocDidOpen,
    NULL);
AVAppRegisterNotification(PDDocCalculateMetadataNSEL,
    gExtensionID,
    (void *)onPDDocCalculateMetadata,
    NULL);
AVAppRegisterNotification(PDDocDidChangePageAreasNSEL,
    gExtensionID,
    (void *)onPDDocDidChangePageAreas,
    NULL);
AVAppRegisterNotification(PSPrintBeforeAcrobatProcsetsNSEL,
    gExtensionID,
    (void *)onPSPrintBeforeAcrobatProcsets,
    NULL);
AVAppRegisterNotification(AVPageViewWillDrawNSEL,
    gExtensionID,
    (void *)onAVPageViewWillDraw,
    NULL);
AVAppRegisterNotification(AVPageViewAnnotDidPerformOpNSEL,
    gExtensionID,
    (void *)onAVPageViewAnnotDidPerformOp,
    NULL);
AVAppRegisterNotification(PDPageWillPrintAnnotNSEL,
    gExtensionID,
    (void *)onPDPageWillPrintAnnot,
    NULL);
AVAppRegisterNotification(PDPageDidPrintAnnotNSEL,
    gExtensionID,
    (void *)onPDPageDidPrintAnnot,
    NULL);
AVAppRegisterNotification(AVDocActivePageViewDidChangeNSEL,
    gExtensionID,
    (void *)onAVDocActivePageViewDidChange,
    NULL);
AVAppRegisterNotification(AVPageViewWasCreatedNSEL,
    gExtensionID,
    (void *)onAVPageViewWasCreated,
    NULL);
AVAppRegisterNotification(AVPageViewWillDestroyNSEL,
    gExtensionID,
    (void *)onAVPageViewWillDestroy,
    NULL);
AVAppRegisterNotification(AVPageViewDidChangeExNSEL,
    gExtensionID,
    (void *)onAVPageViewDidChangeEx,
    NULL);
AVAppRegisterNotification(PDBookmarkWillChangeOpenStateNSEL,
    gExtensionID,
    (void *)onPDBookmarkWillChangeOpenState,
    NULL);
AVAppRegisterNotification(PDBookmarkDidChangeOpenStateNSEL,
    gExtensionID,
    (void *)onPDBookmarkDidChangeOpenState,
    NULL);
AVAppRegisterNotification(PDOCContextWillChangeNSEL,
    gExtensionID,
    (void *)onPDOCContextWillChange,
    NULL);
AVAppRegisterNotification(PDOCContextDidChangeNSEL,
    gExtensionID,
    (void *)onPDOCContextDidChange,
    NULL);
AVAppRegisterNotification(PDDocOCWillChangeNSEL,
    gExtensionID,
    (void *)onPDDocOCWillChange,
    NULL);
AVAppRegisterNotification(PDDocOCDidChangeNSEL,
    gExtensionID,
    (void *)onPDDocOCDidChange,
    NULL);
AVAppRegisterNotification(PDPageDidRemoveAnnotExNSEL,
    gExtensionID,
    (void *)onPDPageDidRemoveAnnotEx,
    NULL);
AVAppRegisterNotification(PDDocWillPrintDocInModeNSEL,
    gExtensionID,
    (void *)onPDDocWillPrintDocInMode,
    NULL);
AVAppRegisterNotification(AVPageViewDocDidChangeNSEL,
    gExtensionID,
    (void *)onAVPageViewDocDidChange,
    NULL);
AVAppRegisterNotification(AVDocDidClearSelectionNSEL,
    gExtensionID,
    (void *)onAVDocDidClearSelection,
    NULL);
AVAppRegisterNotification(AVDocWindowWasAddedNSEL,
    gExtensionID,
    (void *)onAVDocWindowWasAdded,
    NULL);
AVAppRegisterNotification(AVDocWindowWasRemovedNSEL,
    gExtensionID,
    (void *)onAVDocWindowWasRemoved,
    NULL);
AVAppRegisterNotification(AVDocAVWindowDidChangeNSEL,
    gExtensionID,
    (void *)onAVDocAVWindowDidChange,
    NULL);
AVAppRegisterNotification(PDNameTreeNameWillRemoveNSEL,
    gExtensionID,
    (void *)onPDNameTreeNameWillRemove,
    NULL);
AVAppRegisterNotification(PDDocPageDirectionDidChangeNSEL,
    gExtensionID,
    (void *)onPDDocPageDirectionDidChange,
    NULL);
AVAppRegisterNotification(AVDocPopupContentsWillChangeNSEL,
    gExtensionID,
    (void *)onAVDocPopupContentsWillChange,
    NULL);
AVAppRegisterNotification(PDDocXAPMetadataDidChangeNSEL,
    gExtensionID,
    (void *)onPDDocXAPMetadataDidChange,
    NULL);
AVAppRegisterNotification(CosDictXAPMetadataDidChangeNSEL,
    gExtensionID,
    (void *)onCosDictXAPMetadataDidChange,
    NULL);
AVAppRegisterNotification(AVDocPrintDialogWasCancelledNSEL,
    gExtensionID,
    (void *)onAVDocPrintDialogWasCancelled,
    NULL);
AVAppRegisterNotification(PDDocInksDidChangeNSEL,
    gExtensionID,
    (void *)onPDDocInksDidChange,
    NULL);
AVAppRegisterNotification(PDPageDidRedrawNSEL,
    gExtensionID,
    (void *)onPDPageDidRedraw,
    NULL);