/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.1
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"

void freeObject(Object* obj);
void freeScope(Scope* scope);
void freeObjectList(ObjectNode *objList);
void freeReferenceList(ObjectNode *objList);

SymTab* symtab;
Type* intType;
Type* charType;

/******************* Type utilities ******************************/

Type* makeIntType(void) {
  Type* type = (Type*) malloc(sizeof(Type));
  type->typeClass = TP_INT;
  return type;
}

Type* makeCharType(void) {
  Type* type = (Type*) malloc(sizeof(Type));
  type->typeClass = TP_CHAR;
  return type;
}

Type* makeArrayType(int arraySize, Type* elementType) {
  Type* type = (Type*) malloc(sizeof(Type));
  type->typeClass = TP_ARRAY;
  type->arraySize = arraySize;
  type->elementType = elementType;
  return type;
}

Type* duplicateType(Type* type) {
  // TODO
  switch(type->typeClass){
    case TP_INT:
      return makeIntType();
      break;
    case TP_CHAR:
      return makeCharType();
      break;
    case TP_ARRAY:
      return NULL;
      break;
    default:
      return NULL;
  }
}

int compareType(Type* type1, Type* type2) {
  // TODO
  return type1->typeClass==type2->typeClass?1:0;
}

void freeType(Type* type) {
  // TODO
  if(type->typeClass==TP_ARRAY && type->elementType != NULL){
    freeType(type->elementType);
  }
  free(type);
}

/******************* Constant utility ******************************/

ConstantValue* makeIntConstant(int i) {
  // TODO
  ConstantValue* cv = (ConstantValue*)malloc(sizeof(ConstantValue));
  cv->type = TP_INT;
  cv->intValue = i;
  return cv;
}

ConstantValue* makeCharConstant(char ch) {
  // TODO
  ConstantValue* cv = (ConstantValue*)malloc(sizeof(ConstantValue));
  cv->type = TP_CHAR;
  cv->charValue = ch;
  return cv;
}

ConstantValue* duplicateConstantValue(ConstantValue* v) {
  // TODO
  ConstantValue* cv = (ConstantValue*)malloc(sizeof(ConstantValue));
  cv->type = v->type;
  switch(v->type){
    case TP_INT:
      cv->intValue = v->intValue;
      break;
    case TP_CHAR:
      cv->charValue = v->charValue;
      break;
    case TP_ARRAY:
    return NULL;
      break;
    default:
    return NULL;
  }
  return cv;
}

/******************* Object utilities ******************************/

Scope* CreateScope(Object* owner, Scope* outer) {
  Scope* scope = (Scope*) malloc(sizeof(Scope));
  scope->objList = NULL;
  scope->owner = owner;
  scope->outer = outer;
  return scope;
}

Object* CreateProgramObject(char *programName) {
  Object* program = (Object*) malloc(sizeof(Object));
  strcpy(program->name, programName);
  program->kind = OBJ_PROGRAM;
  program->progAttrs = (ProgramAttributes*) malloc(sizeof(ProgramAttributes));
  program->progAttrs->scope = CreateScope(program,NULL);
  symtab->program = program;
  return program;
}


Object* CreateConstantObject(char *name) {
  // TODO
  Object* cons = (Object*) malloc(sizeof(Object));
  strcpy(cons->name, name);
  cons->kind = OBJ_CONSTANT;
  cons->constAttrs = (ConstantAttributes*) malloc(sizeof(ConstantAttributes));
  cons->constAttrs->value = (ConstantValue*) malloc(sizeof(ConstantValue));
  return cons;
}

Object* CreateTypeObject(char *name) {
  // TODO
  Object* type = (Object*) malloc(sizeof(Object));
  strcpy(type->name, name);
  type->kind = OBJ_TYPE;
  type->typeAttrs = (TypeAttributes*)malloc(sizeof(TypeAttributes));
  return type;
}

Object* CreateVariableObject(char *name) {
  // TODO
  Object* var = (Object*) malloc(sizeof(Object));
  strcpy(var->name, name);
  var->kind = OBJ_VARIABLE;
  var->varAttrs = (VariableAttributes*)malloc(sizeof(VariableAttributes));
  var->varAttrs->scope = CreateScope(var,symtab->currentScope);
  return var;
}

Object* CreateFunctionObject(char *name) {
  // TODO
  Object* funObj = (Object*) malloc(sizeof(Object));
  strcpy(funObj->name, name);
  funObj->kind = OBJ_FUNCTION;
  funObj->funcAttrs = (FunctionAttributes*)malloc(sizeof(FunctionAttributes));
  funObj->funcAttrs->paramList = NULL;
  funObj->funcAttrs->scope = CreateScope(funObj,symtab->currentScope);
  return funObj;
}

Object* CreateProcedureObject(char *name) {
  // TODO
  Object* proObj = (Object*) malloc(sizeof(Object));
  strcpy(proObj->name, name);
  proObj->kind = OBJ_PROCEDURE;
  proObj->procAttrs = (ProcedureAttributes*)malloc(sizeof(ProcedureAttributes));
  proObj->procAttrs->paramList = NULL;
  proObj->procAttrs->scope = CreateScope(proObj,symtab->currentScope);
  return proObj;
}

Object* CreateParameterObject(char *name, enum ParamKind kind, Object* owner) {
  // TODO
  Object* paramObj = (Object*) malloc(sizeof(Object));
  strcpy(paramObj->name, name);
  paramObj->kind = OBJ_PARAMETER;
  paramObj->paramAttrs = (ParameterAttributes*)malloc(sizeof(ParameterAttributes));
  paramObj->paramAttrs->kind = kind;
  paramObj->paramAttrs->function = NULL;
  return paramObj;
}

void freeObject(Object* obj) {
  // TODO
  switch(obj->kind){
    case OBJ_PARAMETER:
      freeType(obj->paramAttrs->type);
      free(obj->paramAttrs);
      break;
    case OBJ_PROCEDURE:
      freeObjectList(obj->procAttrs->paramList);
      freeScope(obj->procAttrs->scope);
      free(obj->procAttrs);
      break;
    case OBJ_FUNCTION:
      freeObjectList(obj->funcAttrs->paramList);
      freeType(obj->funcAttrs->returnType);
      freeScope(obj->funcAttrs->scope);
      free(obj->funcAttrs);
      break;
    case OBJ_VARIABLE:
      freeType(obj->varAttrs->type);
      freeScope(obj->varAttrs->scope);
      free(obj->varAttrs);
      break;
    case OBJ_TYPE:
      freeType(obj->typeAttrs->actualType);
      free(obj->typeAttrs);
      break;
    case OBJ_CONSTANT:
      free(obj->constAttrs->value);
      free(obj->constAttrs);
      break;
    case OBJ_PROGRAM:
      freeScope(obj->progAttrs->scope);
      free(obj->progAttrs);
      break;
  }
  free(obj);
}

void freeScope(Scope* scope) {
  // TODO
  freeObjectList(scope->objList);
  free(scope);
}

void freeObjectList(ObjectNode *objList) {
  // TODO
  if(objList->next!=NULL){
    freeObjectList(objList->next);
  }
  freeObject(objList->object);
  free(objList);
}

void freeReferenceList(ObjectNode *objList) {
  // TODO

}

void AddObject(ObjectNode **objList, Object* obj) {
  ObjectNode* node = (ObjectNode*) malloc(sizeof(ObjectNode));
  node->object = obj;
  node->next = NULL;
  if ((*objList) == NULL) 
    *objList = node;
  else {
    ObjectNode *n = *objList;
    while (n->next != NULL) 
      n = n->next;
    n->next = node;
  }
}

Object* findObject(ObjectNode *objList, char *name) {
  // TODO
  ObjectNode* p = objList;
  while(p!=NULL){
    if(strcmp(p->object->name,name)==0) return p->object;
  }
  return NULL;
}

/******************* others ******************************/

void initSymTab(void) {
  Object* obj;
  Object* param;

  symtab = (SymTab*) malloc(sizeof(SymTab));
  symtab->globalObjectList = NULL;
  
  obj = CreateFunctionObject("READC");
  obj->funcAttrs->returnType = makeCharType();
  AddObject(&(symtab->globalObjectList), obj);

  obj = CreateFunctionObject("READI");
  obj->funcAttrs->returnType = makeIntType();
  AddObject(&(symtab->globalObjectList), obj);

  obj = CreateProcedureObject("WRITEI");
  param = CreateParameterObject("i", PARAM_VALUE, obj);
  param->paramAttrs->type = makeIntType();
  AddObject(&(obj->procAttrs->paramList),param);
  AddObject(&(symtab->globalObjectList), obj);

  obj = CreateProcedureObject("WRITEC");
  param = CreateParameterObject("ch", PARAM_VALUE, obj);
  param->paramAttrs->type = makeCharType();
  AddObject(&(obj->procAttrs->paramList),param);
  AddObject(&(symtab->globalObjectList), obj);

  obj = CreateProcedureObject("WRITELN");
  AddObject(&(symtab->globalObjectList), obj);

  intType = makeIntType();
  charType = makeCharType();
}

void cleanSymTab(void) {
  freeObject(symtab->program);
  freeObjectList(symtab->globalObjectList);
  free(symtab);
  freeType(intType);
  freeType(charType);
}

void enterBlock(Scope* scope) {
  symtab->currentScope = scope;
}

void exitBlock(void) {
  symtab->currentScope = symtab->currentScope->outer;
}

void declareObject(Object* obj) {
  if (obj->kind == OBJ_PARAMETER) {
    Object* owner = symtab->currentScope->owner;
    switch (owner->kind) {
    case OBJ_FUNCTION:
      AddObject(&(owner->funcAttrs->paramList), obj);
      break;
    case OBJ_PROCEDURE:
      AddObject(&(owner->procAttrs->paramList), obj);
      break;
    default:
      break;
    }
  }
  AddObject(&(symtab->currentScope->objList), obj);
}


