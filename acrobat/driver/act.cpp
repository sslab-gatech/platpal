#include "defs.h"

/* utils */
static int testActionType(ConsStack stack){
  PDFObjType type;
  int i;
  ASUns32 count = ConsStackGetCount(stack);

  for(i = count - 1; i >= 0; i--){
    type = ConsStackIndexGetTypeAt(stack, i, 0);
    switch(type){
      case PT_PAGE:
        return 0;
      case PT_NAMEDJAVASCRIPTTREE:
        return 1;
      case PT_CATALOG:
        return 2;
    }
  }

  return -1;
}

/* execution */
static void executeAction(PDAction act)
{
  ASBool rv;

  DURING;
  AVDocPerformAction(gAVDoc, act);
  rv = true;
  HANDLER;
  rv = false;
  LOG_ERR_FUNC_ENO("AVDocPerformAction", ERRORCODE);
  END_HANDLER;

  LOG_JS("EXEC:%s\n<<<\nNULL\n>>>", rv ? "true" : "false");
}

/* javascript scanner */
static void onCycleFinish(struct tagConsultantAgent *agent)
{
  return;
}

static void handleAction(CosObj obj, ConsStack stack)
{
  int type = testActionType(stack);
  if(type <= 0){
    return;
  }

  PDAction act = PDActionFromCosObj(obj);
  executeAction(act);
}

static ConsultantAgentAction onFindObj(struct tagConsultantAgent *agent, 
    CosObj obj, const PDFObjType *typeList, const ASUns32 typeSize, 
    ConsStack stack, CosObj *objRet)
{
  handleAction(obj, stack);

  return AC_NOCHANGE;
}

/* checks */
ASBool ExamineAction(CosDoc doc)
{
  PDFObjType allTypes[4] = {
    PT_JAVASCRIPTACTION, 
    PT_LAUNCHACTION, 
    PT_RENDITIONACTION,
    PT_NOPACTION
  };

  struct tagConsultantAgent agent;
  memset(&agent, 0, sizeof(struct tagConsultantAgent));

  agent.Size = sizeof(struct tagConsultantAgent);
  agent.pFindObjects = allTypes;
  agent.NumFindObjects = 4;
  agent.PostProcess = onCycleFinish;
  agent.ObjFound = onFindObj;
  agent.WantSubclasses = true;

  Consultant cons = ConsultantCreate(NULL);
  ConsultantRegisterAgent(cons, &agent, REG_REVISITRECLASS_ALWAYSUNKNOWN);

  LOG_SEC_BEGIN("Action");
  ConsultantTraverseFrom(cons, CosDocGetRoot(doc), PT_CATALOG);
  LOG_SEC_END("Action");

  ConsultantDestroy(cons);

  return true;
}
