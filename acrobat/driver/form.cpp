#include "defs.h"

static ASBool enumPromField(PDField field, void *clientData)
{
  CosObj cos = AFPDFieldGetCosObj(field);
  char *name = AFPDFieldGetName(field);
  CosObj val = AFPDFieldGetValue(field);

  char *cosStr = CosObjToStr(cos);
  char *valStr = CosObjToStr(val);

  LOG_FORM("[%s]\"%s\"=\"%s\"", cosStr, name, valStr);

  ASfree(cosStr);
  ASfree(valStr);

  return true;
}

/* checks */
ASBool ExamineAcroForm(PDDoc doc)
{
  if(!IsPDDocAcroForm(doc)){
    return true;
  }

  LOG_SEC_BEGIN("FormFields");
  AFPDDocEnumPDFields(doc, false, false, enumPromField, NULL);
  LOG_SEC_END("FormFields");

  return true;
}
