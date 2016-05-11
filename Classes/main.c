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

#include <RayFoundation/RayFoundation.h>
#include "Tests.h"


int main(int argc, const char *argv[]) {
    enablePool(RPool);

    ComplexTest();

    const RString *string = RS("1, 2, 3, 4, 5, 6, 7, 8, 9, 10."
                               "11, 12, 13, 14, 15, 16, 17."
                               "18, 19, 20, 21, 22, 23, 24.");

    RArray *separators = RA(RS("."), RS(", "), nil);
    separators->printerDelegate = (PrinterDelegate) p(RString);

    RArray *separated = $(string, m(separatedByStrings, RString)), separators);

    p(RArray)(separators);

    if(separated != nil) {
        p(RArray)(separated);
        deleter(separated, RArray);
    }

    deleter(separators, RArray);

    endRay();
}