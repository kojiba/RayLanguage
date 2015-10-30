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
//
//    RString* result = $(RS("hello"), m(encryptPurgeEvasionBase64, RString)), RS("key"));
//    $(result, p(RString)));
//
//    RString *hash = $(result, m(evasionHashBase64, RString)));
//    p(RString)(hash);
//
//    deleter(hash, RString);
//    deleter(result, RString);

    RString *some = RSC("hello");
    some->data[1] = 0;
    some->data[4] = (byte) -56;

    $(some, m(validateToASCII, RString)));
    p(RString)(some);



    endRay();
}