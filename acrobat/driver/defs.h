#ifndef DEFS_H
#define DEFS_H

/* acrobat headers */
#ifndef MAC_PLATFORM
#include "PIHeaders.h"
#endif

/* acrobat fixes */
#ifndef PDPageEnumContents
#define PDPageEnumContents \
  (ACROASSERT(gPDModelVersion >=PDModelHFT_VERSION_7), \
   *((PDPageEnumContentsSELPROTO)(gPDModelHFT[PDPageEnumContentsSEL])))
#endif

/* defs */
#define OPT_PROJ          "Maldoc"

/* configs */
#define OPT_SIZE_BUF      1024

#define OPT_TIME_VIEW     1
#define OPT_PAGE_LIMIT    10
#define OPT_FILTER_FACTOR 256

/* paths */
#if defined(MAC_PLATFORM)
#define OPT_DIR_HOME      "/Users/bft/"
#elif defined(WIN_PLATFORM)
#define OPT_DIR_HOME      "C:\\Users\\bft\\"
#endif

#define OPT_FILE_RESULT   OPT_DIR_HOME OPT_PROJ ".log"
#define OPT_FILE_SAMPLE   OPT_DIR_HOME OPT_PROJ ".pdf"

/* errors */
#define OPT_ERR_NEVER     "This error should never happen"
#define OPT_ERR_NULL      "NULL"
#define OPT_ERR_TYPE      "Wrong type"

/* platform dependencies */
#if defined(MAC_PLATFORM)
#ifndef SYS_thread_selfid
#define SYS_thread_selfid 372
#endif
#endif

/* structs */
typedef struct _ArrList {
  int size;
  int count;
  void **ptrs;
} ArrList;

/* hfts */
extern HFT gAcroFormHFT;
extern HFT gConsultantHFT;

/* logs */
extern ASFile gLog;

/* docs */
extern AVDoc gAVDoc;
extern PDDoc gPDDoc;
extern ASFile gASDoc;

/* misc */
extern ASFileSysRec fileSys;

/* helpers */
int ASvasprintf(char **strp, const char *fmt, va_list ap);
int ASasprintf(char **strp, const char *fmt, ...);
char *ASstrdup(const char *src);

void ArrListInit(ArrList *list);
void ArrListFini(ArrList *list);
void ArrListAdd(ArrList *list, char *ptr);

/* logs */
void LOG_STR(const char *buf);
void LOG_MSG(const char *tag, const char *fmt, ...);
void LOG_ERR(const char *fmt, ...);
void LOG_ERR_FUNC_MSG(const char* func, const char *msg);
void LOG_ERR_FUNC_ENO(const char* func, ASErrorCode eno);
void LOG_SEC_BEGIN(const char *section);
void LOG_SEC_BEGIN_FMT(const char *fmt, ...);
void LOG_SEC_END(const char *section);
void LOG_SEC_END_FMT(const char *fmt, ...);
void LOG_CBK(const char *name);
void LOG_AS(const char *fmt, ...);
void LOG_COS(const char *fmt, ...);
void LOG_CONS(const char *fmt, ...);
void LOG_PD(const char *fmt, ...);
void LOG_FORM(const char *fmt, ...);
void LOG_JS(const char *fmt, ...);

/* messaging */
void AVAlertFmt(const char *fmt, ...);

/* filesystem */
ASBool InitializeFileSystem();
void UninitializeFileSystem();

/* cos layer helpers */
char *CosObjToStr(CosObj obj);

/* checks */
ASBool ExamineCosDoc(CosDoc doc);
ASBool ExamineConsultant(CosDoc doc);
ASBool ExaminePDDoc(PDDoc doc);
ASBool ExamineAcroForm(PDDoc doc);
ASBool ExamineScript(CosDoc doc);
ASBool ExamineAction(CosDoc doc);

#endif /* DEFS_H */
