#ifndef __BASE_C_LEXEMS_H__
#define __BASE_C_LEXEMS_H__

char *basicLexems[] = {
        "inline", "extern",

        "restrict", "register", "const", "volatile", "auto", "static",

        "void", "int", "char", "float", "double",

        "long", "short", "signed", "unsigned",

        "enum", "union", "struct",

        "if", "else", "switch",

        "for", "do", "while",

        "goto",  "case", "default",

        "break", "continue", "return",

        "sizeof", "typedef"
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