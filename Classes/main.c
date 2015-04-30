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

#include <RayFoundation.h>
#include "Tests.h"

rbool process(pointer string, size_t iterator) {
    $(string, m(trimAfterString, RCString)), RS("//")); // trim comment
    if(((RCString*)string)->size != 0) { // if not fully comment
        RArray *keyValue = $(string, m(substringsSeparatedByString, RCString)), RS("="));
        if(keyValue != nil) {
            p(RArray)(keyValue);
            deleter(keyValue, RArray);
        }
    } else {
        RPrintf("String at index %lu is comment.\n", iterator);
    }
    return yes;
}

int main(int argc, const char *argv[]) {
    enablePool(RPool);
    ComplexTest();

    REnumerateDelegate delegate;
    delegate.virtualCheckObject = (EnumeretorDelegate) process;

    RCString *locales = stringFromFile("Localizable.strings");
    if(locales != nil) {
        $(locales, m(removeRepetitionsOfString, RCString)), RS("\n"));                  // trim some
        RArray *temp = $(locales, m(substringsSeparatedByString, RCString)), RS("\n")); // get lines

        if(temp != nil) {
            p(RArray)(temp);
            $(temp, m(enumerate, RArray)), &delegate, yes);
            deleter(temp, RArray);
        }
        deleter(locales, RCString);
    }

    endRay();
}


