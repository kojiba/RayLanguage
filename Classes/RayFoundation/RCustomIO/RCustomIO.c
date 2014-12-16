/**
 * RCustomIO.c
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
 * 12/10/14 2014 Ukraine Kharkiv
 *  _         _ _ _
 * | |       (_|_) |
 * | | _____  _ _| |__   __ _
 * | |/ / _ \| | | '_ \ / _` |
 * |   < (_) | | | |_) | (_| |
 * |_|\_\___/| |_|_.__/ \__,_|
 *          _/ |
 *         |__/
 **/


#include <RCustomIO.h>

#if !defined(getch) && !defined(_WIN32)
    #include <unistd.h>
    #include <sys/termios.h>

    int getch() {
        int    character;
        struct termios old;
        struct termios tmp;

        if (tcgetattr(STDIN_FILENO, &old))
        {
            return -1;
        }

        memcpy(&tmp, &old, sizeof(old));

        tmp.c_lflag &= ~ICANON & ~ECHO;

        if (tcsetattr(STDIN_FILENO, TCSANOW, (const struct termios*) &tmp))
        {
            return -1;
        }

        character = getchar();

        tcsetattr(STDIN_FILENO, TCSANOW, (const struct termios*) &old);

        return character;
    }
#endif

RCString *readConsolePasswordUnixStyle() {
    int ch;
    RCString *result = RSC("");
    while ((ch = getch()) != '\n') {
        $(result, m(append, RCString)), (char const) ch);
        puts("\b \b");
    }
    printf("\n");
    return result;
}