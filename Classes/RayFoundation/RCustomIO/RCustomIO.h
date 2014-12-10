/**
 * RCustomIO.h.h
 * Some additions to stdio.
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
 
#ifndef __R_CUSTOM_IO_H__
#define __R_CUSTOM_IO_H__

#ifdef _WIN32
    #include <conio.h>
#else
//    #include <curses.h>
#endif

#include "../RCString/RCString.h"

#if !defined(getch)
    int getch();
#endif

RCString *readConsolePasswordUnixStyle();

#endif /*__R_CUSTOM_IO_H__*/
