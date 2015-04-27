/**
 * RSyntax.h
 * A ray of light in the realm of darkness.
 * Some syntax additions to C.
 * If You don't like it,
 * You can preprocess files, to get pure-C code.
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
 * 2014 Ukraine Kharkiv
 *  _         _ _ _
 * | |       (_|_) |
 * | | _____  _ _| |__   __ _
 * | |/ / _ \| | | '_ \ / _` |
 * |   < (_) | | | |_) | (_| |
 * |_|\_\___/| |_|_.__/ \__,_|
 *          _/ |
 *         |__/
 **/

#ifndef __R_ERRORS_H
#define __R_ERRORS_H

#include <RayConfig.h>

#ifdef RAY_ERRORS_ON
    #ifdef RAY_ASSERT_ON_ERRORS
        #define endPrivateError                       ; assert(nil)
    #else
        #define endPrivateError
    #endif

    #define RPrivateErrStr                            RFPrintf(stderr, RRed "%p ERROR. "

    #define RError(string, object)                    RPrivateErrStr string "\n" RNC, object) endPrivateError
    #define RError1(string, object, arg1)             RPrivateErrStr string "\n" RNC, object, arg1) endPrivateError
    #define RError2(string, object, arg1, arg2)       RPrivateErrStr string "\n" RNC, object, arg1, arg2) endPrivateError
    #define RError3(string, object, arg1, arg2, arg3) RPrivateErrStr string "\n" RNC, object, arg1, arg2, arg3) endPrivateError

    #define elseError(error)                          else { error; }
    #define ifError(condition, error)                 if(condition) { error; }
#else
    #define RError(string, object)

    #define RError1(string, object, arg1)
    #define RError2(string, object, arg1, arg2)
    #define RError3(string, object, arg1, arg2, arg3)

    #define elseError(error)
    #define ifError(condition, error)
#endif



#ifdef RAY_WARNINGS_ON
    #define RPleaseRemoveString            " Please, remove function call, or fix it!\n"
    #define RWarning(string, object)       RPrintf(RYellow "%p Warning. " string RPleaseRemoveString RNC, object)

    #define ifWarning(condition, warning)  if(condition) { warning; }
    #define elseWarning(warning)           else { warning; }
#else
    #define RWarning(string, object)

    #define ifWarning(condition, warning)
    #define elseWarning(warning)
#endif

#endif /*__R_ERRORS_H*/
