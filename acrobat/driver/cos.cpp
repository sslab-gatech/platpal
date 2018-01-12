#include "defs.h"

/* to string */
static const char *CosTypeToStr(CosType type)
{
  switch(type){
    case CosNull:
      return "Null";
    case CosInteger:
      return "Integer";
    case CosFixed:
      return "Fixed";
    case CosBoolean:
      return "Boolean";
    case CosName:
      return "Name";
    case CosString:
      return "String";
    case CosDict:
      return "Dict";
    case CosArray:
      return "Array";
    case CosStream:
      return "Stream";
    default:
      return "Invalid";
  }
}

static char *CosIndirectToStr(CosObj obj)
{
  CosID id = CosObjGetID(obj);

  char *str;
  int rv = ASasprintf(&str, "(Indirect)%d", id);

  ACROASSERT(rv > 0);
  return str;
}

static char *CosScalaToStr(CosObj obj)
{
  char *tmp;
  ASTCount len;

  char *str;
  int rv;

  switch(CosObjGetType(obj)){
    case CosNull:
      rv = ASasprintf(&str,
          "(Null)NULL");
      break;
    case CosInteger:
      rv = ASasprintf(&str,
          "(Integer)%lld", CosInteger64Value(obj));
      break;
    case CosFixed:
      rv = ASasprintf(&str,
          "(Fixed)%d", CosFixedValue(obj));
      break;
    case CosBoolean:
      rv = ASasprintf(&str,
          "(Boolean)%s", CosBooleanValue(obj) ? "true" : "false");
      break;
    case CosName:
      rv = ASasprintf(&str,
          "(Name)%s", ASAtomGetString(CosNameValue(obj)));
      break;
    case CosString:
      tmp = CosStringValue(obj, &len);
      rv = ASasprintf(&str,
          "(String)[%d]\n<<<\n\"%s\"\n>>>\n", len, tmp);
      break;
    default:
      rv = ASasprintf(&str,
          "(Invalid)NULL");
      break;
  }

  ACROASSERT(rv > 0);
  return str;
}

static ASBool enumCosDictForStr(CosObj key, CosObj val, void *clientData)
{
  char *keystr = CosScalaToStr(key);
  char *valstr = CosObjToStr(val);

  char *str;
  int rv;

  rv = ASasprintf(&str, "\"%s\":\"%s\"", keystr, valstr);
  ACROASSERT(rv > 0);

  ArrListAdd((ArrList *)clientData, str);

  ASfree(keystr);
  ASfree(valstr);

  return true;
}

static char *CosDictToStr(CosObj obj)
{
  char *buf = NULL;
  char *cur;

  int i;
  int total = 0;

  bool rv;

  ArrList list;
  ArrListInit(&list);

  rv = CosObjEnum(obj, enumCosDictForStr, &list);
  ACROASSERT(rv);

  for(i = 0; i < list.count; i++){
    total += strlen((char *)list.ptrs[i]);
  }

  buf = (char *)ASmalloc(total + list.count + 4);
  cur = buf;

  *(cur++) = '{';
  for(i = 0; i < list.count; i++){
    cur += sprintf(cur, "%s,", (char *)list.ptrs[i]);
  }
  *(cur++) = '}';
  *(cur++) = '\0';

  for(i = 0; i < list.count; i++){
    ASfree(list.ptrs[i]);
  }

  ArrListFini(&list);
  return buf;
}

static char *CosStreamToStr(CosObj obj)
{
  ASTCount len;
  ASStm stream;

  DURING;
  stream = CosStreamOpenStm(obj, cosOpenFiltered);
  HANDLER;
  stream = NULL;
  LOG_ERR_FUNC_ENO("CosStreamOpenStm-Filtered", ERRORCODE);
  END_HANDLER;

  if(!stream){
    DURING;
    stream = CosStreamOpenStm(obj, cosOpenRaw);
    HANDLER;
    stream = NULL;
    LOG_ERR_FUNC_ENO("CosStreamOpenStm-Raw", ERRORCODE);
    END_HANDLER;
  }

  char *str;
  ASTArraySize size = CosStreamLength(obj) * OPT_FILTER_FACTOR;

  if(!stream){
    str = ASstrdup("OBFUSCATED");
  } else {
    size = CosStreamLength(obj) * OPT_FILTER_FACTOR;
    str = (char *)ASmalloc(size);
    len = ASStmRead(str, 1, size, stream);

    if(len > 0){
      str[len - 1] = '\0';
    } else if(len == 0){
      sprintf(str, "%s", "EMPTY");
    } else {
      sprintf(str, "%s", "UNAVAILABLE");
    }

    ASStmClose(stream);
  }

  char *dict = CosDictToStr(CosStreamDict(obj));

  char *buf;
  int rv;

  rv = ASasprintf(&buf, "(Stream)[%s]\n<<<\n\"%s\"\n>>>\n", dict, str);

  ASfree(str);
  ASfree(dict);

  return buf;
}

static ASBool enumCosArrayForStr(CosObj val, CosObj none, void *clientData)
{
  char *str = CosObjToStr(val);
  ArrListAdd((ArrList *)clientData, str);

  return true;
}

static char *CosArrayToStr(CosObj obj)
{
  char *buf = NULL;
  char *cur;

  int i;
  int total = 0;

  bool rv;

  ArrList list;
  ArrListInit(&list);

  rv = CosObjEnum(obj, enumCosArrayForStr, &list);
  ACROASSERT(rv);

  for(i = 0; i < list.count; i++){
    total += strlen((char *)list.ptrs[i]);
  }

  buf = (char *)ASmalloc(total + list.count + 4);
  cur = buf;

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
  return buf;
}

static char *CosObjToStrWorker(CosObj obj, bool force)
{
  if(!force && CosObjIsIndirect(obj)){
    return CosIndirectToStr(obj);
  }

  switch(CosObjGetType(obj)){
    case CosDict:
      return CosDictToStr(obj);
    case CosArray:
      return CosArrayToStr(obj);
    case CosStream:
      return CosStreamToStr(obj);
    default:
      return CosScalaToStr(obj);
  }
}

char *CosObjToStr(CosObj obj)
{
  return CosObjToStrWorker(obj, false);
}

/* collections */
static ASBool enumCosCollectionForStr(CosObj obj, CosObj none, void *clientData)
{
  char *str = CosObjToStr(obj);
  ArrListAdd((ArrList *)clientData, str);

  return true;
}

static char *CosCollectionToStr(CosObjCollection coll)
{
  char *buf = NULL;
  char *cur;

  int i;
  int total = 0;

  bool rv;

  ArrList list;
  ArrListInit(&list);

  rv = CosObjCollectionEnum(coll, enumCosCollectionForStr, &list);
  ACROASSERT(rv);

  for(i = 0; i < list.count; i++){
    total += strlen((char *)list.ptrs[i]);
  }

  buf = (char *)ASmalloc(total + list.count + 4);
  cur = buf;

  *(cur++) = '(';
  for(i = 0; i < list.count; i++){
    cur += sprintf(cur, "%s,", (char *)list.ptrs[i]);
  }
  *(cur++) = ')';
  *(cur++) = '\0';

  for(i = 0; i < list.count; i++){
    ASfree(list.ptrs[i]);
  }

  ArrListFini(&list);
  return buf;
}

/* indirect objs */
static ASBool enumCosIndirect(CosObj obj, CosObj none, void *clientData)
{
  CosID id = CosObjGetID(obj);
  CosType type = CosObjGetType(obj);
  char *msg = CosObjToStrWorker(obj, true);

  CosObjCollection coll;
  char *collstr;

  DURING;
  coll = CosObjGetCollection(obj);
  collstr = CosCollectionToStr(coll);
  HANDLER;
  collstr = ASstrdup("()");
  END_HANDLER;

  LOG_COS("%d %s %s %s", id, CosTypeToStr(type), msg, collstr);

  ASfree(msg);
  ASfree(collstr);

  return true;
}

/* checks */
ASBool ExamineCosDoc(CosDoc doc)
{
  bool rv;

  /* build indirect obj table */
  LOG_SEC_BEGIN("COSIndirect");
  rv = CosDocEnumIndirect(doc, enumCosIndirect, NULL);
  ACROASSERT(rv);
  LOG_SEC_END("COSIndirect");

  /* check the meta items */
  LOG_SEC_BEGIN("COSMetadata");

  CosObj info = CosDocGetInfoDict(doc);
  LOG_COS("Info:%s", CosObjToStr(info));

  CosObj root = CosDocGetRoot(doc);
  LOG_COS("Root:%s", CosObjToStr(root));
      
  LOG_SEC_END("COSMetadata");
  
  return true;
}
