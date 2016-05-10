/**
 * InterpreterTemp.c
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
 * 5/10/16 Ukraine Kharkiv
 *  _         _ _ _
 * | |       (_|_) |
 * | | _____  _ _| |__   __ _
 * | |/ / _ \| | | '_ \ / _` |
 * |   < (_) | | | |_) | (_| |
 * |_|\_\___/| |_|_.__/ \__,_|
 *          _/ |
 *         |__/
 **/

#include "InterpreterTemp.h"




constructor(Variable)) {
object = allocator(Variable);
if(object != nil) {
object->type = 0;
object->value = makeRDataBytes(1, 0);
object->name = nil;
}
return object;
}

destructor(Variable) {
    nilDeleter(object->name, RString);
    deleter(object->value, RData);
    object->type = 0;
}

generateDeleter(Variable)

#define UIntFromVariable(object) *( (size_t*)object->value->data)

        printer(Variable) {
    purePrint(object->name);
    RPrintf(" = ");
    if(object->type == 1) {
        RPrintf("%li \n", *((ssize_t*) object->value->data) );
        return;
    } else if(object->type == 2) {
        RPrintf("%lu \n", UIntFromVariable(object) );
        return;
    } else {
        RError1("Unknown type %lu \n", object, object->type);
    }
}


#define equalStrings(string1, string2) ($(string1, m(isEqualTo, RString)), string2))

        method(rbool, stringsTokenEnumerator,        Interpreter), RString *token, size_t index);
method(rbool, linesEnumerator,               Interpreter), RString *line, size_t index);
method(rbool, variablesEnumerator, Interpreter), Variable *variable, size_t index);

constructor(Interpreter)) {
object = allocator(Interpreter);
if(object != nil) {
flushAllToByte((byte *) object, sizeof(Interpreter), 0);

object->typesTable = makeRDictionary();
object->typesSizes = makeRDictionary();

if(object->typesTable != nil
&& object->typesSizes) {
object->classId = registerClassOnce(toString(Interpreter));

$(object->typesSizes, m(initDelegate, RDictionary)), (ComparatorDelegate) m(compareWith, RString));
$(object->typesTable, m(initDelegate, RDictionary)), (ComparatorDelegate) m(compareWith, RString));

// register some basic c types
$(object->typesTable, m(setObjectForKey, RDictionary)), (pointer) object->typesTable->keys->count, (pointer) RS(toString(ERROR_TYPE_UNUSED)) ); // must be 0
$(object->typesTable, m(setObjectForKey, RDictionary)), (pointer) object->typesTable->keys->count, (pointer) RS("Int")); // must be 1 - ssize_t
$(object->typesTable, m(setObjectForKey, RDictionary)), (pointer) object->typesTable->keys->count, (pointer) RS("UInt")); // size_t
$(object->typesTable, m(setObjectForKey, RDictionary)), (pointer) object->typesTable->keys->count, (pointer) RS("String"));

$(object->typesTable, m(setObjectForKey, RDictionary)), (pointer) object->typesTable->keys->count, (pointer) RS("u8"));
$(object->typesTable, m(setObjectForKey, RDictionary)), (pointer) object->typesTable->keys->count, (pointer) RS("u16"));
$(object->typesTable, m(setObjectForKey, RDictionary)), (pointer) object->typesTable->keys->count, (pointer) RS("u32"));
$(object->typesTable, m(setObjectForKey, RDictionary)), (pointer) object->typesTable->keys->count, (pointer) RS("u64"));

$(object->typesTable, m(setObjectForKey, RDictionary)), (pointer) object->typesTable->keys->count, (pointer) RS("i8"));
$(object->typesTable, m(setObjectForKey, RDictionary)), (pointer) object->typesTable->keys->count, (pointer) RS("i16"));
$(object->typesTable, m(setObjectForKey, RDictionary)), (pointer) object->typesTable->keys->count, (pointer) RS("i32"));
$(object->typesTable, m(setObjectForKey, RDictionary)), (pointer) object->typesTable->keys->count, (pointer) RS("i64"));

$(object->typesTable, m(setObjectForKey, RDictionary)), (pointer) object->typesTable->keys->count, (pointer) RS("float")); // 32 bit
$(object->typesTable, m(setObjectForKey, RDictionary)), (pointer) object->typesTable->keys->count, (pointer) RS("f32")); // 32 bit
$(object->typesTable, m(setObjectForKey, RDictionary)), (pointer) object->typesTable->keys->count, (pointer) RS("f64")); // 64 bit

// register some sizes
$(object->typesSizes, m(setObjectForKey, RDictionary)), (pointer) 0, (pointer) RS(toString(ERROR_TYPE_UNUSED)) ); // must be 0
$(object->typesSizes, m(setObjectForKey, RDictionary)), (pointer) sizeof(ssize_t), (pointer) RS("Int")); // must be 1 - ssize_t
$(object->typesSizes, m(setObjectForKey, RDictionary)), (pointer) sizeof(size_t),  (pointer) RS("UInt")); // size_t
$(object->typesSizes, m(setObjectForKey, RDictionary)), (pointer) sizeof(RString), (pointer) RS("String"));

$(object->typesSizes, m(setObjectForKey, RDictionary)), (pointer) sizeof(uint8_t),  (pointer) RS("u8"));
$(object->typesSizes, m(setObjectForKey, RDictionary)), (pointer) sizeof(uint16_t), (pointer) RS("u16"));
$(object->typesSizes, m(setObjectForKey, RDictionary)), (pointer) sizeof(uint32_t), (pointer) RS("u32"));
$(object->typesSizes, m(setObjectForKey, RDictionary)), (pointer) sizeof(uint64_t), (pointer) RS("u64"));

$(object->typesSizes, m(setObjectForKey, RDictionary)), (pointer) sizeof(int8_t),  (pointer) RS("i8"));
$(object->typesSizes, m(setObjectForKey, RDictionary)), (pointer) sizeof(int16_t), (pointer) RS("i16"));
$(object->typesSizes, m(setObjectForKey, RDictionary)), (pointer) sizeof(int32_t), (pointer) RS("i32"));
$(object->typesSizes, m(setObjectForKey, RDictionary)), (pointer) sizeof(int64_t), (pointer) RS("i64"));

$(object->typesSizes, m(setObjectForKey, RDictionary)), (pointer) sizeof(float),    (pointer) RS("float"));
$(object->typesSizes, m(setObjectForKey, RDictionary)), (pointer) sizeof(uint32_t), (pointer) RS("f32")); // 32 bit
$(object->typesSizes, m(setObjectForKey, RDictionary)), (pointer) sizeof(uint64_t), (pointer) RS("f64")); // 64 bit


object->linesEnumerator.virtualEnumerator = (EnumeratorDelegate) m(linesEnumerator, Interpreter);
object->tokensEnumerator.virtualEnumerator = (EnumeratorDelegate) m(stringsTokenEnumerator, Interpreter);
object->variablesEnumerator.virtualEnumerator = (EnumeratorDelegate) m(variablesEnumerator, Interpreter);

object->linesEnumerator.context = object;
object->tokensEnumerator.context = object;
object->variablesEnumerator.context = object;
}
}

return object;
}

printer(Interpreter) {
    RPrintf("%p - %s object {\n", object, toString(Interpreter));
    RPrintf("Variables : ");
    p(RArray)(object->variables);

    RPrintf("} - %p\n", object);
}

method(void, setValueFromToken, Interpreter), Variable *var, RString *token, rbool *isConverted) {
RString *typeString = $(object->typesTable->keys, m(objectAtIndex, RArray)), var->type);
size_t typeSize = (size_t) $(object->typesSizes, m(getObjectForKey, RDictionary)), typeString);

pointer value = nil;

if equalStrings(typeString, RS("UInt")) { // size_t
size_t uintValue = $(token, m(toSystemUInt, RString)), isConverted);
if (*isConverted) {
value = &uintValue;
RMemCpy(var->value->data, value, typeSize);
return;
}
}
if equalStrings(typeString, RS("Int")) { // size_t
ssize_t intValue = $(token, m(toSystemInt, RString)), isConverted);
if (*isConverted) {
value = &intValue;
RMemCpy(var->value->data, value, typeSize);
return;
}

}
}

method(Variable*, performAdditionOperation, Interpreter), Variable *first, Variable *second) {
Variable *var = nil;
if (first->type == second->type && second->type == 2) {
size_t firstValue = UIntFromVariable(first);
size_t secondValue = UIntFromVariable(second);
size_t result = firstValue + secondValue;

var = c(Variable)(nil);
if (var != nil) {
RMemCpy(var->value->data, &result, sizeof(size_t));
} else {
RError("Can't allocate result var", nil);
}
}
return var;
}

method(Variable*, performOperationOnVariables, Interpreter), Variable *first, Variable *second, InterpreterOperation operation) {
if (operation == InterpreterOperationAddition) {
return $(object, m(performAdditionOperation, Interpreter)), first, second);
}

}

method(Variable*, variableWithName, Interpreter), RString *name) {
object->variableNameToFind = name;
RFindResult result = $(object->variables, m(enumerate, RArray)), &object->variablesEnumerator, yes);
return result.object;
}

method(Variable*, addValueFromToken, Interpreter), RString *varName, size_t type) {
if ( $(object, m(variableWithName, Interpreter)), varName) == nil ) {
Variable *var = c(Variable)(nil);
if (var != nil) {
$(object->variables, m(addObject, RArray)), var);
var->type = type;
var->name = $(varName, m(copy, RString)));

RString *typeString = $(object->typesTable->keys, m(objectAtIndex, RArray)), type);
size_t typeSize = (size_t) $(object->typesSizes, m(getObjectForKey, RDictionary)), typeString);

if(typeSize != 0) {
var->value->data = arrayAllocator(byte, typeSize);
var->value->size = typeSize;
return var;
} else {
RError1("Bad type [%lu] size 0.", object, type);
p(RString)(typeString);
}
} else {
RError("Not enough memory for variables.", object);
}
} else {
RError1("Variable with name %s already exists", object, varName->data);
}
return nil;
}

method(rbool, variablesEnumerator, Interpreter), Variable *variable, size_t index) {
if equalStrings(object->variableNameToFind, variable->name) {
return no;
}
return yes;
}

method(rbool, stringsTokenEnumerator, Interpreter), RString *token, size_t index) {

rbool isCommaSeparator = no;
rbool isEqualsSeparator = no;
rbool isPlusSeparator = no;

if equalStrings(token, RS(",")) {
isCommaSeparator = yes;
}

if(object->isInDeclarationCode && isCommaSeparator) {
object->nextTokenIsVariableName = yes;
return yes;
}

if(object->nextTokenIsVariableName && !isCommaSeparator) { // add var and set name

Variable *var = $(object, m(addValueFromToken, Interpreter)), token, object->lastVariableType);
if(var != nil) {
object->nextTokenIsVariableName = no;
object->nextTokenIsVariableValue = yes;
return yes;
} else {
return no;
}
}

if equalStrings(token, RS("+")) {
isPlusSeparator = yes;
if (object->isInExecutionCode) {
object->currentOperation = InterpreterOperationAddition;
}
}
if equalStrings(token, RS("-")) {
if (object->isInExecutionCode) {
object->currentOperation = InterpreterOperationSubstraction;
}
}
if equalStrings(token, RS("/")) {

}
if equalStrings(token, RS("*")) {

}
if equalStrings(token, RS("%")) {

}
if equalStrings(token, RS("^")) {

}
if equalStrings(token, RS("*")) {

}

if equalStrings(token, RS("=")) {
isEqualsSeparator = yes;
if (object->isInExecutionCode) {
object->currentOperation = InterpreterOperationAssigment;
} else {
object->nextTokenIsVariableValue = yes;
}
}

if (object->isInExecutionCode) {

// if current token is var name we can be in begin of operation, or in some
Variable *variable = $(object, m(variableWithName, Interpreter)), token);

if(variable != nil) {
object->nextTokenIsVariableName = no;
object->nextTokenIsVariableValue = no;
object->isInDeclarationCode = no;

if (object->lastOperatingVariable == nil) {
object->lastOperatingVariable = variable;
object->currentOperation = InterpreterOperationNone;
return yes;
} else {

if (object->currentOperation == InterpreterOperationAddition) {

if (object->lastOperatingVariable != nil) {

}

if (object->lastOperatingResult != nil) {
Variable *var = $(object, m(variableWithName, Interpreter)), token);
if(var != nil) {
object->lastOperatingResult = $(object, m(performOperationOnVariables, Interpreter)), object->lastOperatingResult, var, object->currentOperation);
object->currentOperation = InterpreterOperationNone;
}
}

} else if (object->currentOperation == InterpreterOperationAssigment) {
object->lastOperatingVariable = object->lastOperatingResult;
}
}
}
}

if (object->nextTokenIsVariableValue && !isEqualsSeparator) { // set value here

Variable *lastDeclaredVar = $(object->variables, m(lastObject, RArray)));

if(lastDeclaredVar != nil) {
rbool isValidValueToken = yes;
$(object, m(setValueFromToken, Interpreter)), lastDeclaredVar, token, &isValidValueToken);
if(isValidValueToken) {
object->nextTokenIsVariableName = no;
object->nextTokenIsVariableValue = no;
return yes;
} else {
RError2("Invalid number token \"%s\" at index %lu", object, token->data, index);
return no;
}
} else {
RError("Something wrong with last var", object);
return no;
}
}

size_t type = (size_t) $(object->typesTable, m(getObjectForKey, RDictionary)), token);

if(type != 0 && object->isInDeclarationCode) { // set type
object->nextTokenIsVariableName = yes;
object->isInExecutionCode = no;
if(object->lastVariableType == 0) {
object->lastVariableType = type;
return yes;

} else {
RError("Another type declared, when last contains", object);
return no;
}
}

return yes;

}

method(rbool, linesEnumerator, Interpreter), RString *line, size_t index) {
object->lineTokens = $(line, m(substringsSeparatedByString, RString)), RS(" "));
if(object->lineTokens != nil) {
object->lineTokens->destructorDelegate = (DestructorDelegate) RStringDeleter;
object->lineTokens->printerDelegate = (PrinterDelegate) p(RString);

object->isInDeclarationCode = yes;
object->isInExecutionCode = yes;
object->lastVariableType = 0;
RFindResult enumerationResult = $(object->lineTokens, m(enumerate, RArray)), &object->tokensEnumerator, yes);
if(enumerationResult.object != nil) {
RPrintf("Token: ");
p(RString)(enumerationResult.object);
return no;
}

deleter(object->lineTokens, RArray);
object->lineTokens = nil;
return yes;
}
return no;
}

method(void, setup, Interpreter)) {
object->variables = makeRArray();
object->variables->printerDelegate = (PrinterDelegate) p(Variable);
object->variables->destructorDelegate = (DestructorDelegate) deleterName(Variable);
}

method(void, runString, Interpreter), RString *source) {
if (source != nil) {
$(object, m(setup, Interpreter)));

object->sourceFileString = $(source, m(copy, RString)));
object->linesStatements = $(object->sourceFileString, m(substringsSeparatedByString, RString)), RS("\n"));

if(object->linesStatements != nil
&& object->linesStatements->count > 0) {
RFindResult enumerationResult = $(object->linesStatements, m(enumerate, RArray)), &object->linesEnumerator, yes);
if(enumerationResult.object != nil) {
RPrintf("Line [%lu]: ", enumerationResult.index);
p(RString)(enumerationResult.object);
}

p(Interpreter)(object);

deleter(object->linesStatements, RArray);
object->linesStatements = nil;
return;
}
}
RError("Empty code, or lines count is 0", object);
}

destructor(Interpreter) {
    nilDeleter(object->sourceFileString, RString)
    nilDeleter(object->lineTokens, RArray)
    nilDeleter(object->variables, RArray)
    deleter(object->typesTable, RDictionary);
    deleter(object->typesSizes, RDictionary);
}