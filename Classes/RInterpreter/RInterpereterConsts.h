#ifndef __R_INTERPRETER_CONSTS_H__
#define __R_INTERPRETER_CONSTS_H__

static const char  property_qualifier_start_symbol = '(';
static const char  property_qualifier_end_symbol   = ')';
static const char  property_type_separator         = ' ';
static const char* property_names_separator_string = ", ";

typedef enum RayPropertyType {
    ReadOnly = 0,     // start must be 0,
    ReadWrite,
    Inner,

    PropertyTypeCount // hack for count
} RayPropertyType;

static const RayPropertyType default_property_qualifier = ReadOnly;

static const char* const propertyTypesConsts[PropertyTypeCount] = {
        "readonly",
        "readwrite",
        "inner",
};

static const char* const property_type_prefix_postfix = "_";

//--------------------------------------------------------------------------

// toadd
// MTInner
// MTStatic
// MTVirtual

typedef enum RayMethodType {
    MTSetter  = 0,
    MTGetter,

    MTConstructor,
    MTDestructor ,

    MTOperator,

    MethodTypesCount

} RayMethodType;

static unsigned const maskToPower[] = {0, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, };


static const char* const methodTypesConsts[MethodTypesCount] = {
        "setter",
        "getter",
        "constructor",
        "destructor",
        "operator",
};

static const char* const method_type_prefix_postfix = "_";
static const char* const namespace_name_prefix = "@";

typedef enum RayOperatorType {
    OTPrefix = 0,
    OTPostfix = 1,

// one operand
    OTPlusPlus,
    OTMinusMinus,

// two operands
    OTMinus,
    OTPlus,
    OTMultiplication,
    OTDivision,
    OTModulo,

    OperatorTypesCount
} RayOperatorType;

static const char* const operatorTypesConsts[OperatorTypesCount] = {
        "OTPrefix",
        "OTPostfix",
        "OTPlusPlus",
        "OTMinusMinus",
        "OTMinus",
        "OTPlus",
        "OTMultiplication",
        "OTDivision",
        "OTModulo",
};

#endif /*__R_INTERPRETER_CONSTS_H__*/
