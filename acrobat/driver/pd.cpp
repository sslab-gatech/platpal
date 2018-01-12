#include "defs.h"

/* elements */
static char *PDFontToStr(PDFont font)
{
  char *name = (char *)ASmalloc(PSNAMESIZE);

  DURING;
  PDFontGetName(font, name, PSNAMESIZE);
  HANDLER;
  name = ASstrdup("UNKNOWN");
  END_HANDLER;

  return name; 
}

static ASBool enumTextString(PDFont font, 
    char *string, ASInt32 stringLen, ASFixed delta, void *clientData)
{
  char *str;
  int rv;

  rv = ASasprintf(&str, "-%d|%d-", stringLen, delta);
  ACROASSERT(rv > 0);

  ArrListAdd((ArrList *)clientData, str);

  return true;
}

static char *PDTextToStr(PDText text)
{
  int rv;
  char *stateStr = NULL;

  PDTextState state;
  PDTextGetState(text, &state, sizeof(PDTextState));

  char *fontStr = PDFontToStr(state.font);
  rv = ASasprintf(&stateStr, "{font:%s}", fontStr);
  ACROASSERT(rv > 0);

  ASfree(fontStr);

  char *contentStr = NULL;
  char *cur;

  int i;
  int total = 0;

  ArrList list;
  ArrListInit(&list);

  PDTextEnum(text, enumTextString, &list);

  for(i = 0; i < list.count; i++){
    total += strlen((char *)list.ptrs[i]);
  }

  contentStr = (char *)ASmalloc(total + list.count + 4);
  cur = contentStr;

  *(cur++) = '[';
  for(i = 0; i < list.count; i++){
    cur += sprintf(cur, "%s,", (char *)list.ptrs[i]);
  }
  *(cur++) = ']';
  *(cur++) = '\0';

  for(i = 0; i < list.count; i++){
    ASfree(list.ptrs[i]);
  }

  ArrListFini(&list);

  char *buf;
  rv = ASasprintf(&buf, "(Text)%s%s", stateStr, contentStr);
  ACROASSERT(rv > 0);

  ASfree(stateStr);
  ASfree(contentStr);

  return buf;
}

static char *PDPathToStr(PDPath path)
{
  int rv;
  char *buf;
  
  rv = ASasprintf(&buf, "(Path)%x", PDPathGetPaintOp(path));
  ACROASSERT(rv > 0);

  return buf;
}

static char *PDInlineImageToStr(PDInlineImage image)
{
  PDImageAttrs attrs;
  memset(&attrs, 0, sizeof(PDImageAttrs));

  PDInlineImageGetAttrs(image, &attrs, sizeof(PDImageAttrs));

  int rv;
  char *buf;

  rv = ASasprintf(&buf, "(Image)%d,%d,%d", 
      attrs.width, attrs.height, attrs.dataLen);
  ACROASSERT(rv > 0);

  return buf;
}

/* page */
static ASBool enumPDGraphicText(PDText obj, void *clientData)
{
  DURING;

  char *str = PDTextToStr(obj);
  LOG_PD("%s", str);
  ASfree(str);

  HANDLER;
  LOG_ERR_FUNC_ENO("PDTextToStr", ERRORCODE);
  END_HANDLER;

  return true;
}

static ASBool enumPDGraphicPath(PDPath obj, void *clientData)
{
  DURING;

  char *str = PDPathToStr(obj);
  LOG_PD("%s", str);
  ASfree(str);

  HANDLER;
  LOG_ERR_FUNC_ENO("PDPathToStr", ERRORCODE);
  END_HANDLER;

  return true;
}

static ASBool enumPDGraphInlineImage(PDInlineImage obj, void *clientData)
{
  DURING;

  char *str = PDInlineImageToStr(obj);
  LOG_PD("%s", str);
  ASfree(str);

  HANDLER;
  LOG_ERR_FUNC_ENO("PDInlineImageToStr", ERRORCODE);
  END_HANDLER;

  return true;
}

static ASBool enumPDXObjectRef(char *name, ASFixedRect *bbox, void *clientData)
{
  LOG_PD("(XObjectRef)%s", name);
  return true;
}

static ASBool CheckPage(int index, PDPage page)
{
  /* check meta */
  char *pageCosStr = CosObjToStr(PDPageGetCosObj(page));
  char *resourceCosStr = CosObjToStr(PDPageGetCosResources(page));

  LOG_PD("PageCos:%s", pageCosStr);
  LOG_PD("ResourceCos:%s", resourceCosStr);

  ASfree(pageCosStr);
  ASfree(resourceCosStr);

  /* enum contents */
  PDGraphicEnumMonitorRec monitor;

  memset(&monitor, 0, sizeof(PDGraphicEnumMonitorRec));
  monitor.size = sizeof(PDGraphicEnumMonitorRec);
  monitor.EnumText = enumPDGraphicText;
  monitor.EnumPath = enumPDGraphicPath;
  monitor.EnumImage = enumPDGraphInlineImage;
  monitor.EnumXObjectRef = enumPDXObjectRef;

  PDPageEnumContents(page, &monitor, NULL);

  return true;
}

/* checks */
ASBool ExaminePDDoc(PDDoc doc)
{
  /* check meta */
  LOG_SEC_BEGIN("PDDocMetadata");
  LOG_PD("Flags:%x", PDDocGetFlags(doc));
  LOG_PD("Pages:%d", PDDocGetNumPages(doc));
  LOG_PD("Threads:%d", PDDocGetNumThreads(doc));
  LOG_PD("OCGs:%d", PDDocGetNumOCGs(doc));
  LOG_PD("Errors:%d", PDDocGetNumErrors(doc));
  LOG_SEC_END("PDDocMetadata");

#ifdef OPT_PAGE_CHECK
  /* check pages */
  int i;
  PDPage page;
  PDEContent content;
  int pgnum = PDDocGetNumPages(doc);

  for(i = 0; i < pgnum ; i++){
    LOG_SEC_BEGIN_FMT("PDPage-%d", i);

    page = PDDocAcquirePage(doc, i);
    content = PDPageAcquirePDEContent(page, gExtensionID);

    CheckPage(i, page);

    PDPageReleasePDEContent(page, gExtensionID);
    PDPageRelease(page);

    LOG_SEC_END_FMT("PDPage-%d", i);
  }
#endif

  return true;
}
