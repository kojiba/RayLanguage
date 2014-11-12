#ifndef __BASE_C_LEXEMS_H__
#define __BASE_C_LEXEMS_H__

char *basicLexems[] = {
        "auto",     "break",   "case",     "char",    "const",
        "continue", "default", "do",       "double",  "int",
        "else",     "enum",    "extern",   "float",   "long",
        "for",      "goto",    "if",       "inline",  "register",
        "restrict", "return",  "short",    "signed",  "sizeof",
        "static",   "struct",  "switch",   "typedef", "union",
        "unsigned", "void",    "volatile", "while"
};

char basicSeparators[] = {
        '.', ',', ' ', '[', ']', '{', '}', ';'
};

char *pointersOperations[] = {
        "->", "*", "&"
};

char defineLexem = '#';

char *commentLexems[] = {
        "//", "/*"
};

#endif /*__BASE_C_LEXEMS_H__*/