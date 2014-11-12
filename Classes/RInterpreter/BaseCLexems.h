#ifndef __BASE_C_LEXEMS_H__
#define __BASE_C_LEXEMS_H__

const char const *funcTypeLexems[] = {
        "inline", "extern" };

const char const *accessLexems[] = {
        "restrict", "register", "const", "volatile", "auto", "static" };

const char const *typesLexems[] = {
        "void", "int", "char", "float", "double" };

const char const *typeOfTypesLexems[] = {
        "long", "short", "signed", "unsigned" };

const char const *declarationLexems[] = {
        "enum", "union", "struct", "typedef" };

const char const *logicLexems[] = {
        "if", "else", "switch" };

const char const *cyclesLexems[] = {
        "for", "while" };

const char const *caseLexems[] = {
        "case", "default" };

const char const *actionsLexems[] = {
        "break", "continue", "return", "do", "goto" };

const char const *sizeLexems[] = {
        "sizeof" };

const char const basicSeparators[] = {
        '.', ',', ' ', '[', ']', '{', '}', ';'
};

const char const thernarSeparators[] = {
        '?', ':'
};

const char const *mathematicalSeparators[] = {
        "+", "-", "*", "/", "%", "++", "--"
};

const char const *logicalSeparators[] = {
        "!", "&&", "||", "==", ">=", "<=", "!="
};

const char const *boolSeparators[] = {
        "~", "&", "|", ">>", "<<" , "^"
};

const char const *pointersOperations[] = {
        "->", "*", "&", "="
};

const char const *compilerLexems[] =  {
        "#include", "#pragma", "#define"
};

const char const *commentLexems[] = {
        "//", "/*"
};

#endif /*__BASE_C_LEXEMS_H__*/