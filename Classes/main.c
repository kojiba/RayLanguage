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
    ComplexTest(); // lib test

    initRClock();

    rbool decrypt = yes;

    RPrintf("Enter filename\n");
    RString *input = getInputString();
    RString *name = input;
    RString *key = nil;

    if(input != nil) {
        RData *data = contentOfFile((const char *) name->data);
        if(data != nil) {
            RPrintf("Readed\n");
            tickRClock();
            RPrintf("Enter key\n");
            input = getInputString();
            key = $(input, m(evasionHash, RData)));

            decrypt = $(name, m(endsOn, RString)), RS(".purge"));

            if(decrypt) {

                RData *decrypted = $(data, m(decryptPurgeEvasion, RData)), key);
                if(decrypted != nil) {
                    $(name, m(trimHead, RString)), 5); // remove .purge
                    name->data[name->size - 1] = 0;

                    if($(decrypted, m(saveToFile, RData)), (const char *) name->data)){
                        tickRClock();
                        RPrintf("Saved\n");
                    }

                    deleter(decrypted, RData);
                }
            } else {
                RData *encrypted = $(data, m(encryptPurgeEvasion, RData)), key);
                if(encrypted != nil) {
                    $(name, m(concatenate, RString)), RS(".purge"));

                    if($(encrypted, m(saveToFile, RData)), (const char *) name->data)){
                        tickRClock();
                        RPrintf("Saved\n");
                    }

                    deleter(encrypted, RData);
                }
            }
            deleter(data, RData);

            deleter(name, RString);
        }

        nilDeleter(key, RString);
        deleter(input, RString);
    }

    endRay();
}