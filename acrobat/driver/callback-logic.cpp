ACCB1 void onAVDocWillOpenFromFile(
    ASPathName fileName,
    ASFileSys fileSys,
    void * aux)
{
  LOG_CBK("AVDocWillOpenFromFile");
}
ACCB1 void onAVDocDidOpen(
    AVDoc doc,
    ASInt32 error,
    void * aux)
{
  LOG_CBK("AVDocDidOpen");
}
ACCB1 void onAVDocDidActivate(
    AVDoc doc,
    void * aux)
{
  LOG_CBK("AVDocDidActivate");
}
ACCB1 void onAVDocDidDeactivate(
    AVDoc doc,
    void * aux)
{
  LOG_CBK("AVDocDidDeactivate");
}
ACCB1 void onAVDocWillClose(
    AVDoc doc,
    void * aux)
{
  LOG_CBK("AVDocWillClose");
}
ACCB1 void onAVDocDidClose(
    AVDoc doc,
    void * aux)
{
  LOG_CBK("AVDocDidClose");
}
ACCB1 void onAVDocWillClearSelection(
    AVDoc doc,
    ASAtom selType,
    void* selData,
    void * aux)
{
  LOG_CBK("AVDocWillClearSelection");
}
ACCB1 void onAVDocDidSetSelection(
    AVDoc doc,
    ASAtom selType,
    void* selData,
    void * aux)
{
  LOG_CBK("AVDocDidSetSelection");
}
ACCB1 void onAVDocWillPerformAction(
    AVDoc doc,
    PDAction action,
    void * aux)
{
  LOG_CBK("AVDocWillPerformAction");
}
ACCB1 void onAVDocDidPerformAction(
    AVDoc doc,
    PDAction action,
    ASInt32 err,
    void * aux)
{
  LOG_CBK("AVDocDidPerformAction");
}
ACCB1 void onAVPageViewDidDraw(
    AVPageView pageView,
    void * aux)
{
  LOG_CBK("AVPageViewDidDraw");
}
ACCB1 void onAVPageViewDidChange(
    AVPageView pageView,
    ASInt16 how,
    void * aux)
{
  LOG_CBK("AVPageViewDidChange");
}
ACCB1 void onPDDocWillInsertPages(
    PDDoc doc,
    ASInt32 insertAfterThisPage,
    PDDoc srcDoc,
    ASInt32 srcFromPage,
    ASInt32 srcToPage,
    void * aux)
{
  LOG_CBK("PDDocWillInsertPages");
}
ACCB1 void onPDDocDidInsertPages(
    PDDoc doc,
    ASInt32 insertAfterThisPage,
    PDDoc srcDoc,
    ASInt32 srcFromPage,
    ASInt32 srcToPage,
    ASInt32 error,
    void * aux)
{
  LOG_CBK("PDDocDidInsertPages");
}
ACCB1 void onPDDocWillReplacePages(
    PDDoc doc,
    ASInt32 fromPage,
    ASInt32 toPage,
    PDDoc srcDoc,
    ASInt32 srcFromPage,
    ASInt32 srcToPage,
    void * aux)
{
  LOG_CBK("PDDocWillReplacePages");
}
ACCB1 void onPDDocDidReplacePages(
    PDDoc doc,
    ASInt32 fromPage,
    ASInt32 toPage,
    PDDoc srcDoc,
    ASInt32 srcFromPage,
    ASInt32 srcToPage,
    ASInt32 error,
    void * aux)
{
  LOG_CBK("PDDocDidReplacePages");
}
ACCB1 void onPDDocWillMovePages(
    PDDoc doc,
    ASInt32 moveAfterThisPage,
    ASInt32 fromPage,
    ASInt32 toPage,
    void * aux)
{
  LOG_CBK("PDDocWillMovePages");
}
ACCB1 void onPDDocDidMovePages(
    PDDoc doc,
    ASInt32 moveAfterThisPage,
    ASInt32 fromPage,
    ASInt32 toPage,
    ASInt32 error,
    void * aux)
{
  LOG_CBK("PDDocDidMovePages");
}
ACCB1 void onPDDocWillDeletePages(
    PDDoc doc,
    ASInt32 fromPage,
    ASInt32 toPage,
    void * aux)
{
  LOG_CBK("PDDocWillDeletePages");
}
ACCB1 void onPDDocDidDeletePages(
    PDDoc doc,
    ASInt32 fromPage,
    ASInt32 toPage,
    ASInt32 error,
    void * aux)
{
  LOG_CBK("PDDocDidDeletePages");
}
ACCB1 void onPDDocWillChangePages(
    PDDoc doc,
    PDOperation op,
    ASInt32 fromPage,
    ASInt32 toPage,
    void * aux)
{
  LOG_CBK("PDDocWillChangePages");
}
ACCB1 void onPDDocDidChangePages(
    PDDoc doc,
    PDOperation op,
    ASInt32 fromPage,
    ASInt32 toPage,
    ASInt32 error,
    void * aux)
{
  LOG_CBK("PDDocDidChangePages");
}
ACCB1 void onPDDocDidChangeThumbs(
    PDDoc doc,
    void * aux)
{
  LOG_CBK("PDDocDidChangeThumbs");
}
ACCB1 void onPDDocWillPrintPages(
    PDDoc doc,
    ASInt32 fromPage,
    ASInt32 toPage,
    ASInt32 psLevel,
    ASBool binaryOK,
    void * aux)
{
  LOG_CBK("PDDocWillPrintPages");
}
ACCB1 void onPDDocDidPrintPages(
    PDDoc doc,
    ASInt32 fromPage,
    ASInt32 toPage,
    ASInt32 error,
    void * aux)
{
  LOG_CBK("PDDocDidPrintPages");
}
ACCB1 void onPDDocWillPrintPage(
    PDDoc doc,
    ASInt32 page,
    ASStm stm,
    void * aux)
{
  LOG_CBK("PDDocWillPrintPage");
}
ACCB1 void onPDDocDidPrintPage(
    PDDoc doc,
    ASInt32 page,
    ASStm stm,
    ASInt32 error,
    void * aux)
{
  LOG_CBK("PDDocDidPrintPage");
}
ACCB1 void onPDDocDidAddThread(
    PDDoc doc,
    PDThread thread,
    void * aux)
{
  LOG_CBK("PDDocDidAddThread");
}
ACCB1 void onPDDocDidRemoveThread(
    PDDoc doc,
    ASInt32 index,
    void * aux)
{
  LOG_CBK("PDDocDidRemoveThread");
}
ACCB1 void onPDDocWillSave(
    PDDoc doc,
    void * aux)
{
  LOG_CBK("PDDocWillSave");
}
ACCB1 void onPDDocDidSave(
    PDDoc doc,
    ASInt32 err,
    void * aux)
{
  LOG_CBK("PDDocDidSave");
}
ACCB1 void onPDPageContentsDidChange(
    PDPage page,
    void * aux)
{
  LOG_CBK("PDPageContentsDidChange");
}
ACCB1 void onPDPageWillAddAnnot(
    PDPage page,
    ASInt32 addAfter,
    PDAnnot annot,
    void * aux)
{
  LOG_CBK("PDPageWillAddAnnot");
}
ACCB1 void onPDPageDidAddAnnot(
    PDPage page,
    PDAnnot annot,
    ASInt32 error,
    void * aux)
{
  LOG_CBK("PDPageDidAddAnnot");
}
ACCB1 void onPDPageWillRemoveAnnot(
    PDPage page,
    ASInt32 annotIndex,
    void * aux)
{
  LOG_CBK("PDPageWillRemoveAnnot");
}
ACCB1 void onPDPageDidRemoveAnnot(
    PDPage page,
    ASInt32 annotIndex,
    ASInt32 error,
    void * aux)
{
  LOG_CBK("PDPageDidRemoveAnnot");
}
ACCB1 void onPDAnnotWasCreated(
    PDAnnot annot,
    PDPage page,
    void * aux)
{
  LOG_CBK("PDAnnotWasCreated");
}
ACCB1 void onPDAnnotWillChange(
    PDAnnot annot,
    ASAtom key,
    void * aux)
{
  LOG_CBK("PDAnnotWillChange");
}
ACCB1 void onPDAnnotDidChange(
    PDAnnot annot,
    ASAtom key,
    ASInt32 error,
    void * aux)
{
  LOG_CBK("PDAnnotDidChange");
}
ACCB1 void onPDThreadDidChange(
    PDThread thread,
    void * aux)
{
  LOG_CBK("PDThreadDidChange");
}
ACCB1 void onPDBookmarkWasCreated(
    PDBookmark bookmark,
    void * aux)
{
  LOG_CBK("PDBookmarkWasCreated");
}
ACCB1 void onPDBookmarkDidChangePosition(
    PDBookmark bookmark,
    void * aux)
{
  LOG_CBK("PDBookmarkDidChangePosition");
}
ACCB1 void onPDBookmarkWillChange(
    PDBookmark bookmark,
    ASAtom key,
    void * aux)
{
  LOG_CBK("PDBookmarkWillChange");
}
ACCB1 void onPDBookmarkDidChange(
    PDBookmark bookmark,
    ASAtom key,
    ASInt32 err,
    void * aux)
{
  LOG_CBK("PDBookmarkDidChange");
}
ACCB1 void onPDBookmarkWillDestroy(
    PDBookmark bookmark,
    void * aux)
{
  LOG_CBK("PDBookmarkWillDestroy");
}
ACCB1 void onPDBookmarkDidDestroy(
    PDBookmark bookmark,
    ASInt32 err,
    void * aux)
{
  LOG_CBK("PDBookmarkDidDestroy");
}
ACCB1 void onPDDocWillRemoveThread(
    PDDoc doc,
    ASInt32 index,
    void * aux)
{
  LOG_CBK("PDDocWillRemoveThread");
}
ACCB1 void onPDPageContentsDidChangeEx(
    PDPage page,
    ASBool invalidateViews,
    void * aux)
{
  LOG_CBK("PDPageContentsDidChangeEx");
}
ACCB1 void onAVDocWillOpenFromPDDoc(
    PDDoc pdDoc,
    void * aux)
{
  LOG_CBK("AVDocWillOpenFromPDDoc");
}
ACCB1 void onAVDocDidAddToSelection(
    AVDoc doc,
    ASAtom selType,
    void* selData,
    void* addData,
    void * aux)
{
  LOG_CBK("AVDocDidAddToSelection");
}
ACCB1 void onAVDocDidRemoveFromSelection(
    AVDoc doc,
    ASAtom selType,
    void* selData,
    void* remData,
    void * aux)
{
  LOG_CBK("AVDocDidRemoveFromSelection");
}
ACCB1 void onAVDocWantsToDie(
    AVDoc doc,
    void * aux)
{
  LOG_CBK("AVDocWantsToDie");
}
ACCB1 void onPDDocWillPrintDoc(
    PDDoc doc,
    ASStm stm,
    ASInt32 psLevel,
    void * aux)
{
  LOG_CBK("PDDocWillPrintDoc");
}
ACCB1 void onPDDocWillSaveEx(
    PDDoc doc,
    PDDocSaveParams params,
    void * aux)
{
  LOG_CBK("PDDocWillSaveEx");
}
ACCB1 void onAVDocWillPrint(
    AVDoc doc,
    void * aux)
{
  LOG_CBK("AVDocWillPrint");
}
ACCB1 void onAVDocDidPrint(
    AVDoc doc,
    void * aux)
{
  LOG_CBK("AVDocDidPrint");
}
ACCB1 void onPDDocWillClose(
    PDDoc doc,
    void * aux)
{
  LOG_CBK("PDDocWillClose");
}
ACCB1 void onPDDocPageLabelDidChange(
    PDDoc doc,
    ASInt32 firstPage,
    ASInt32 lastPage,
    void * aux)
{
  LOG_CBK("PDDocPageLabelDidChange");
}
ACCB1 void onPDDocWillExportAnnots(
    PDDoc doc,
    void * aux)
{
  LOG_CBK("PDDocWillExportAnnots");
}
ACCB1 void onPDDocWillImportAnnots(
    PDDoc doc,
    void * aux)
{
  LOG_CBK("PDDocWillImportAnnots");
}
ACCB1 void onPDDocDidExportAnnots(
    PDDoc doc,
    void * aux)
{
  LOG_CBK("PDDocDidExportAnnots");
}
ACCB1 void onPSPrintAfterBeginProlog(
    PDDoc doc,
    ASStm stm,
    void * aux)
{
  LOG_CBK("PSPrintAfterBeginProlog");
}
ACCB1 void onPSPrintAfterBeginSetup(
    PDDoc doc,
    ASStm stm,
    void * aux)
{
  LOG_CBK("PSPrintAfterBeginSetup");
}
ACCB1 void onPSPrintBeforeEndSetup(
    PDDoc doc,
    ASStm stm,
    void * aux)
{
  LOG_CBK("PSPrintBeforeEndSetup");
}
ACCB1 void onPSPrintAfterBeginPageSetup(
    PDDoc doc,
    ASInt32 page,
    ASStm stm,
    void * aux)
{
  LOG_CBK("PSPrintAfterBeginPageSetup");
}
ACCB1 void onPSPrintAfterPageTrailer(
    PDDoc doc,
    ASInt32 page,
    ASStm stm,
    void * aux)
{
  LOG_CBK("PSPrintAfterPageTrailer");
}
ACCB1 void onPSPrintAfterTrailer(
    PDDoc doc,
    ASStm stm,
    void * aux)
{
  LOG_CBK("PSPrintAfterTrailer");
}
ACCB1 void onPSPrintBeforeEndComments(
    PDDoc doc,
     ASStm,
    void * aux)
{
  LOG_CBK("PSPrintBeforeEndComments");
}
ACCB1 void onPSPrintAfterEmitExtGState(
    ASStm stm,
    void * aux)
{
  LOG_CBK("PSPrintAfterEmitExtGState");
}
ACCB1 void onAVDocDidClickName(
    AVDoc doc,
    CosObj NameObj,
    void * aux)
{
  LOG_CBK("AVDocDidClickName");
}
ACCB1 void onPDDocDidImportAnnots(
    PDDoc doc,
    void * aux)
{
  LOG_CBK("PDDocDidImportAnnots");
}
ACCB1 void onPDNameTreeNameAdded(
    PDNameTree NameTree,
    CosObj Key,
    CosObj Value,
    void * aux)
{
  LOG_CBK("PDNameTreeNameAdded");
}
ACCB1 void onPDNameTreeNameRemoved(
    PDNameTree NameTree,
    CosObj Key,
    void * aux)
{
  LOG_CBK("PDNameTreeNameRemoved");
}
ACCB1 void onPDDocDidClose(
    PDDoc doc,
    void * aux)
{
  LOG_CBK("PDDocDidClose");
}
ACCB1 void onPDDocDidInsertPagesEx(
    PDDocInsertPagesParams params,
    void * aux)
{
  LOG_CBK("PDDocDidInsertPagesEx");
}
ACCB1 void onPDDocWillInsertPagesEx(
    PDDocInsertPagesParams params,
    void * aux)
{
  LOG_CBK("PDDocWillInsertPagesEx");
}
ACCB1 void onPDNumTreeNumAdded(
    PDNumTree numTree,
    ASInt32 key,
    CosObj value,
    void * aux)
{
  LOG_CBK("PDNumTreeNumAdded");
}
ACCB1 void onPDNumTreeNumRemoved(
    PDNumTree numTree,
    ASInt32 key,
    void * aux)
{
  LOG_CBK("PDNumTreeNumRemoved");
}
ACCB1 void onPDBookmarkDidUnlink(
    PDBookmark bookmark,
    void * aux)
{
  LOG_CBK("PDBookmarkDidUnlink");
}
ACCB1 void onAVDocDidDeleteSelection(
    AVDoc doc,
    ASAtom selType,
    void* selData,
    void * aux)
{
  LOG_CBK("AVDocDidDeleteSelection");
}
ACCB1 void onPDDocWillPrintTiledPage(
    PDDoc doc,
    ASInt32 page,
    ASStm stm,
    ASInt32 whichTile,
    ASInt32 numTiles,
    ASFixedRect *cropRegion,
    PDTile tile,
    void * aux)
{
  LOG_CBK("PDDocWillPrintTiledPage");
}
ACCB1 void onPDDocPrintingTiledPage(
    PDDoc doc,
    ASInt32 page,
    ASStm stm,
    ASInt32 whichRow,
    ASInt32 whichColumn,
    ASInt32 numRows,
    ASInt32 numColumns,
    ASFixedRect *cropRegion,
    PDTile tile,
    void * aux)
{
  LOG_CBK("PDDocPrintingTiledPage");
}
ACCB1 void onPDDocDidPrintTiledPage(
    PDDoc doc,
    ASInt32 page,
    ASStm stm,
    ASInt32 error,
    ASInt32 whichTile,
    ASInt32 numTiles,
    void * aux)
{
  LOG_CBK("PDDocDidPrintTiledPage");
}
ACCB1 void onPDPageWillPrintAnnots(
    PDDoc doc,
    ASInt32 page,
    ASStm stm,
    void * aux)
{
  LOG_CBK("PDPageWillPrintAnnots");
}
ACCB1 void onPDPageDidPrintAnnots(
    PDDoc doc,
    ASInt32 page,
    ASStm stm,
    void * aux)
{
  LOG_CBK("PDPageDidPrintAnnots");
}
ACCB1 void onPDDocDidOpen(
    PDDoc doc,
    void * aux)
{
  LOG_CBK("PDDocDidOpen");
}
ACCB1 void onPDDocCalculateMetadata(
    PDDoc doc,
    void * aux)
{
  LOG_CBK("PDDocCalculateMetadata");
}
ACCB1 void onPDDocDidChangePageAreas(
    PDDoc pdDoc,
    ASInt32 areaMask,
    ASInt32 firstPage,
    ASInt32 lastPage,
    void * aux)
{
  LOG_CBK("PDDocDidChangePageAreas");
}
ACCB1 void onPSPrintBeforeAcrobatProcsets(
    PDDoc doc,
    ASStm stm,
    void * aux)
{
  LOG_CBK("PSPrintBeforeAcrobatProcsets");
}
ACCB1 void onAVPageViewWillDraw(
    AVPageView pageView,
    void * aux)
{
  LOG_CBK("AVPageViewWillDraw");
}
ACCB1 void onAVPageViewAnnotDidPerformOp(
    AVPageView pageView,
    PDAnnot pdAnnot,
    AVAnnotOp annotOp,
    void * aux)
{
  LOG_CBK("AVPageViewAnnotDidPerformOp");
}
ACCB1 void onPDPageWillPrintAnnot(
    PDAnnot annot,
    ASInt32 page,
    void * aux)
{
  LOG_CBK("PDPageWillPrintAnnot");
}
ACCB1 void onPDPageDidPrintAnnot(
    PDAnnot annot,
    ASInt32 page,
    ASInt32 status,
    void * aux)
{
  LOG_CBK("PDPageDidPrintAnnot");
}
ACCB1 void onAVDocActivePageViewDidChange(
    AVDoc avDoc,
    AVPageView newPageView,
    AVPageView oldPageView,
    void * aux)
{
  LOG_CBK("AVDocActivePageViewDidChange");
}
ACCB1 void onAVPageViewWasCreated(
    AVPageView pageView,
    void * aux)
{
  LOG_CBK("AVPageViewWasCreated");
}
ACCB1 void onAVPageViewWillDestroy(
    AVPageView pageView,
    void * aux)
{
  LOG_CBK("AVPageViewWillDestroy");
}
ACCB1 void onAVPageViewDidChangeEx(
    AVPageView pageView,
    ASInt16 how,
    const AVDevRectP,
    void * aux)
{
  LOG_CBK("AVPageViewDidChangeEx");
}
ACCB1 void onPDBookmarkWillChangeOpenState(
    PDBookmark bookmark,
    ASBool open,
    void * aux)
{
  LOG_CBK("PDBookmarkWillChangeOpenState");
}
ACCB1 void onPDBookmarkDidChangeOpenState(
    PDBookmark bookmark,
    ASBool open,
    void * aux)
{
  LOG_CBK("PDBookmarkDidChangeOpenState");
}
ACCB1 void onPDOCContextWillChange(
    PDOCContext ocContext,
    PDOCContextChangeType whatWillHappen,
    void* objects,
    void * aux)
{
  LOG_CBK("PDOCContextWillChange");
}
ACCB1 void onPDOCContextDidChange(
    PDOCContext ocContext,
    PDOCContextChangeType whatHappened,
    void* objects,
    ASErrorCode error,
    void * aux)
{
  LOG_CBK("PDOCContextDidChange");
}
ACCB1 void onPDDocOCWillChange(
    PDDoc doc,
    PDDocOCChangeType whatWillHappen,
    void* object,
    void * aux)
{
  LOG_CBK("PDDocOCWillChange");
}
ACCB1 void onPDDocOCDidChange(
    PDDoc doc,
    PDDocOCChangeType whatHappened,
    void* object,
    ASErrorCode error,
    void * aux)
{
  LOG_CBK("PDDocOCDidChange");
}
ACCB1 void onPDPageDidRemoveAnnotEx(
    PDPage page,
    PDAnnot removedAnnot,
    ASInt32 error,
    void * aux)
{
  LOG_CBK("PDPageDidRemoveAnnotEx");
}
ACCB1 void onPDDocWillPrintDocInMode(
    PDDoc doc,
    PDPrintWhat printMode,
    void * aux)
{
  LOG_CBK("PDDocWillPrintDocInMode");
}
ACCB1 void onAVPageViewDocDidChange(
    AVPageView pageView,
    AVDoc newDoc,
    AVDoc oldDoc,
    void * aux)
{
  LOG_CBK("AVPageViewDocDidChange");
}
ACCB1 void onAVDocDidClearSelection(
    AVDoc doc,
    void * aux)
{
  LOG_CBK("AVDocDidClearSelection");
}
ACCB1 void onAVDocWindowWasAdded(
    AVDoc doc,
    AVWindow window,
    void * aux)
{
  LOG_CBK("AVDocWindowWasAdded");
}
ACCB1 void onAVDocWindowWasRemoved(
    AVDoc doc,
    AVWindow window,
    void * aux)
{
  LOG_CBK("AVDocWindowWasRemoved");
}
ACCB1 void onAVDocAVWindowDidChange(
    AVDoc doc,
    AVWindow oldWindow,
    AVWindow newWindow,
    void * aux)
{
  LOG_CBK("AVDocAVWindowDidChange");
}
ACCB1 void onPDNameTreeNameWillRemove(
    PDNameTree tree,
    const char,
    ASInt32 keyLen,
    void * aux)
{
  LOG_CBK("PDNameTreeNameWillRemove");
}
ACCB1 void onPDDocPageDirectionDidChange(
    PDDoc doc,
    void * aux)
{
  LOG_CBK("PDDocPageDirectionDidChange");
}
ACCB1 void onAVDocPopupContentsWillChange(
    AVDoc doc,
    PDAnnot annot,
    void * aux)
{
  LOG_CBK("AVDocPopupContentsWillChange");
}
ACCB1 void onPDDocXAPMetadataDidChange(
    PDDoc pdDoc,
    ASText newMetadata,
    void * aux)
{
  LOG_CBK("PDDocXAPMetadataDidChange");
}
ACCB1 void onCosDictXAPMetadataDidChange(
    CosObj dict,
    ASText newMetadata,
    void * aux)
{
  LOG_CBK("CosDictXAPMetadataDidChange");
}
ACCB1 void onAVDocPrintDialogWasCancelled(
    AVDoc doc,
    void * aux)
{
  LOG_CBK("AVDocPrintDialogWasCancelled");
}
ACCB1 void onPDDocInksDidChange(
    PDDoc doc,
    void * aux)
{
  LOG_CBK("PDDocInksDidChange");
}
ACCB1 void onPDPageDidRedraw(
    PDPage page,
    void * aux)
{
  LOG_CBK("PDPageDidRedraw");
}