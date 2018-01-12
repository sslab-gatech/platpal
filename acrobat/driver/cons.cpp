#include "defs.h"

/* naieve scanner */
static void onCycleFinish(struct tagConsultantAgent *agent)
{
  return;
}

static ConsultantAgentAction onFindObj(struct tagConsultantAgent *agent, 
    CosObj obj, const PDFObjType *typeList, const ASUns32 typeSize, 
    ConsStack stack, CosObj *objRet)
{
  LOG_CONS("[%d-%d][%d]%s", typeList[0], typeList[typeSize - 1], 
      ConsStackGetCount(stack), CosObjToStr(obj));

  return AC_NOCHANGE;
}

/* checks */
ASBool ExamineConsultant(CosDoc doc)
{
  PDFObjType allTypes[1] = {PT_ALL};

  struct tagConsultantAgent agent;
  memset(&agent, 0, sizeof(struct tagConsultantAgent));

  agent.Size = sizeof(struct tagConsultantAgent);
  agent.pFindObjects = allTypes;
  agent.NumFindObjects = 1;
  agent.PostProcess = onCycleFinish;
  agent.ObjFound = onFindObj;
  agent.WantSubclasses = true;

  Consultant cons = ConsultantCreate(NULL);
  ConsultantRegisterAgent(cons, &agent, REG_REVISITRECLASS_ALWAYSUNKNOWN);

  LOG_SEC_BEGIN("Consultant");
  ConsultantTraverseFrom(cons, CosDocGetRoot(doc), PT_CATALOG);
  LOG_SEC_END("Consultant");

  ConsultantDestroy(cons);

  return true;
}
