#include "defs.h"

/* configs */
#define OPT_SIZE_REC      64

/* string */
int ASvasprintf(char **strp, const char *fmt, va_list ap)
{
  int r = -1;

  va_list bkp;
  va_copy(bkp, ap);

#if defined(MAC_PLATFORM)
  int size = vsnprintf(0, 0, fmt, bkp);
#elif defined(WIN_PLATFORM)
  int size = _vscprintf(fmt, bkp);
#endif

  if (size >= 0){
    *strp = (char *)ASmalloc(size + 1);
    r = vsnprintf(*strp, size + 1, fmt, ap);
    if ((r < 0) || (r > size)){
      ASfree(*strp);
      *strp = NULL;

      r = -1;
    }
  } else {
    *strp = NULL;
  }

  return r;
}

int ASasprintf(char **strp, const char *fmt, ...)
{
  int r;
  va_list ap;

  va_start(ap, fmt);
  r = ASvasprintf(strp, fmt, ap);
  va_end(ap);

  return r;
}

char *ASstrdup(const char *src)
{
  char *str;
  char *p;
  int len = strlen(src);

  str = (char *)ASmalloc(len + 1);
  p = str;

  while(*src){
    *p++ = *src++;
  }

  *p = '\0';
  return str;
}

/* data structs */
void ArrListInit(ArrList *list)
{
  list->ptrs = (void **)ASmalloc(sizeof(void *) * OPT_SIZE_REC);
  list->size = OPT_SIZE_REC;
  list->count = 0;
}

void ArrListFini(ArrList *list)
{
  ASfree(list->ptrs);
  list->size = 0;
  list->count = 0;
}

void ArrListAdd(ArrList *list, char *ptr)
{
  if(list->count == list->size){
    list->size *= 2;
    list->ptrs = (void **)ASrealloc(list->ptrs, sizeof(void *) * list->size);
  }

  list->ptrs[list->count++] = ptr;
}

/* messaging */
void AVAlertFmt(const char *fmt, ...)
{
  char *buf;

  int r;
  va_list ap;

  va_start(ap, fmt);
  r = ASvasprintf(&buf, fmt, ap);
  va_end(ap);

  ACROASSERT(r > 0);

  AVAlertNote(buf);

  ASfree(buf);
}
