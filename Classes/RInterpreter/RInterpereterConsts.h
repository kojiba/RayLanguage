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

static const char* const propertyTypesConst[PropertyTypeCount] = {
        "readonly",
        "write",
        "inner",
};

static const char* const property_type_prefix_postfix = "_";




#endif /*__R_INTERPRETER_CONSTS_H__*/
