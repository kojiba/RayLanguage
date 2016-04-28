/**
 *  _____
 * |  __ \
 * | |__) | __ _  _   _
 * |  _  / / _` || | | |
 * | | \ \| (_| || |_| |
 * |_|  \_\\__,_| \__, |
 *                 __/ |
 *                |___/
 *  ______                        _         _    _
 * |  ____|                      | |       | |  (_)
 * | |__  ___   _   _  _ __    __| |  __ _ | |_  _   ___   _ __
 * |  __|/ _ \ | | | || '_ \  / _` | / _` || __|| | / _ \ | '_ \
 * | |  | (_) || |_| || | | || (_| || (_| || |_ | || (_) || | | |
 * |_|   \___/  \__,_||_| |_| \__,_| \__,_| \__||_| \___/ |_| |_|
 *
 **/

#include <RayFoundation/RayFoundation.h>
#include "Tests.h"

class(Variable)
    RString *name;
    RData *value;
    size_t type;
endOf(Variable)

constructor(Variable)) {
    object = allocator(Variable);
    if(object != nil) {
        object->type = 0;
        object->value = makeRDataAllocated(0);
    }
    return object;
}

destructor(Variable) {
    nilDeleter(object->name, RString);
    deleter(object->value, RData);
    object->type = 0;
}

generateDeleter(Variable)

printer(Variable) {
    purePrint(object->name);
    RPrintf(" : ");
    if(object->type == 2) { // size_t
        RPrintf("%lu \n", (size_t) *object->value->data);
        return;
    } else {
        RError1("Unknown type %lu \n", object, object->type);
    }
}

class(Interpreter)
    RDictionary *typesTable;

    REnumerateDelegate linesEnumerator;
    REnumerateDelegate tokensEnumerator;

    RArray *variables;

// raw rayTokens, that be separated only by ' ' and '\n'
    RArray *lineTokens;
    RArray *linesStatements;

// string added when file is read
    RString *sourceFileString;

    rbool isInDeclarationCode;
    rbool nextTokenIsVariableName;
    rbool nextTokenIsVariableValue;
endOf(Interpreter)

#define equalStrings(string1, string2) ($(string1, m(isEqualTo, RString)), string2))

method(rbool, stringsTokenEnumerator, Interpreter), RString *token, size_t index);
    method(rbool, linesEnumerator, Interpreter), RString *line, size_t index);

constructor(Interpreter)) {
    object = allocator(Interpreter);
    if(object != nil) {
        flushAllToByte(object, sizeof(Interpreter), 0);

        object->typesTable = makeRDictionary();

        if(object->typesTable != nil) {
            object->classId = registerClassOnce(toString(Interpreter));

            object->linesEnumerator.virtualEnumerator = (EnumeratorDelegate) m(linesEnumerator, Interpreter);
            object->tokensEnumerator.virtualEnumerator = (EnumeratorDelegate) m(stringsTokenEnumerator, Interpreter);

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

        }
    }

    return object;
}

method(void, setValueFromToken, Interpreter), Variable *var, RString *token, rbool *isConverted) {
    if(var->type == 2) { // size_t
        size_t value = $(token, m(toSystemUInt, RString)), isConverted);
        if (*isConverted) {
            appendArray(&var->value->data, &var->value->size, (const byte *) &value, sizeof(size_t));
        }
    }
}

method(rbool, stringsTokenEnumerator, Interpreter), RString *token, size_t index) {

    rbool isCommaSeparator = no;
    rbool isEqualsSeparator = no;

    if equalStrings(token, RS(",")) {
        isCommaSeparator = yes;
    }

    if(object->nextTokenIsVariableName && !isCommaSeparator) { // set var name
        Variable *lastDeclaredVar = $(object->variables, m(lastObject, RArray)));

        if(lastDeclaredVar != nil) {
            lastDeclaredVar->name = $(token, m(copy, RString)));

            object->nextTokenIsVariableName = no;
            object->nextTokenIsVariableValue = yes;
            return yes;
        } else {
            RError("Something wrong with last var", object);
            return no;
        }
    }

    if equalStrings(token, RS("+")) {

    }
    if equalStrings(token, RS("-")) {

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
        object->nextTokenIsVariableValue = yes;
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

    if(type != 0 && object->isInDeclarationCode) { // add var and set var type

        Variable *var = c(Variable)(nil);
        if (var != nil) {

            var->type = type;
            $(object->variables, m(addObject, RArray)), var);
            object->nextTokenIsVariableName = yes;

            return yes;
        } else {
            RError("Not enought memory for variables.", object);
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
        RFindResult enumerationResult = $(object->lineTokens, m(enumerate, RArray)), &object->tokensEnumerator, yes);
        if(enumerationResult.object != nil) {
            RPrintf("Token -> ");
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

            object->linesEnumerator.context = object;
            object->tokensEnumerator.context = object;
            RFindResult enumerationResult = $(object->linesStatements, m(enumerate, RArray)), &object->linesEnumerator, yes);
            if(enumerationResult.object != nil) {
                RPrintf("Line -> ");
                p(RString)(enumerationResult.object);
            }

            p(RArray)(object->variables);

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
}

int main(int argc, const char *argv[]) {
    enablePool(RPool);

    ComplexTest();

//
    Interpreter *interpreter = c(Interpreter)(nil);
    RString *string = stringFromFile("code_demo.int");

    $(interpreter, m(runString, Interpreter)), string);

    deleter(string, RString);
    deleter(interpreter, Interpreter);

    endRay();
}