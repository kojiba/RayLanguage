/**
 * InterpreterTemp.h
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

#ifndef __InterpreterTemp__
#define __InterpreterTemp__

#include "../../RayFoundation/RayFoundation.h"

class(Variable)
        RString *name;
        RData *value;
        size_t type;
endOf(Variable)


typedef enum InterpreterOperation {
            InterpreterOperationNone,

            InterpreterOperationAssigment,

            InterpreterOperationAddition,
            InterpreterOperationSubstraction,
            InterpreterOperationMultiplication
} InterpreterOperation;

class(Interpreter)
        RDictionary *typesTable;
        RDictionary *typesSizes;

        REnumerateDelegate linesEnumerator;
        REnumerateDelegate tokensEnumerator;
        REnumerateDelegate variablesEnumerator;

        RArray *variables;

        RString *variableNameToFind;

// raw rayTokens, that be separated only by ' ' and '\n'
        RArray *lineTokens;
        RArray *linesStatements;

// string added when file is read
        RString *sourceFileString;

        InterpreterOperation currentOperation;
        Variable *lastOperatingVariable;
        Variable *lastOperatingResult;

        rbool isInDeclarationCode;
        rbool isInExecutionCode;

        size_t lastVariableType;
        rbool nextTokenIsVariableName;
        rbool nextTokenIsVariableValue;
endOf(Interpreter)

#endif /*__InterpreterTemp__*/
