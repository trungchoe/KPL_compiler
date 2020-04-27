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
}

int compareType(Type* type1, Type* type2) {
  // TODO
}

void freeType(Type* type) {
  // TODO
}

/******************* Constant utility ******************************/

ConstantValue* makeIntConstant(int i) {
  // TODO
}

ConstantValue* makeCharConstant(char ch) {
  // TODO
}

ConstantValue* duplicateConstantValue(ConstantValue* v) {
  // TODO
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
}

Object* CreateTypeObject(char *name) {
  // TODO
}

Object* CreateVariableObject(char *name) {
  // TODO
}

Object* CreateFunctionObject(char *name) {
  // TODO
}

Object* CreateProcedureObject(char *name) {
  // TODO
}

Object* CreateParameterObject(char *name, enum ParamKind kind, Object* owner) {
  // TODO
}

void freeObject(Object* obj) {
  // TODO
}

void freeScope(Scope* scope) {
  // TODO
}

void freeObjectList(ObjectNode *objList) {
  // TODO
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


