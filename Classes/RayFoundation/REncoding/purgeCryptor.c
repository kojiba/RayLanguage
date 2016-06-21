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

static const char *versionString = "1.0";

void printCryptorVersion() {
    RPrintf("Purge, evasion ciphering util\n v%s \n(c) kojiba 2016", versionString);
}

void printCryptorHelp() {
    printCryptorVersion();
    RPrintf("Options:\n"
                    "        -pf passwordFileName passwordFileName with password"
                    "        -f filename          filename to decrypt/encrypt\n"
                    "        -p password          password to decrypt/encrypt\n"
                    "        -e                   option for manual encrypt mode\n"
                    "        -d                   option for manual decrypt mode\n"
                    "        -v                   prints version string\n");
}

int main(int argc, const char *argv[]) {
//    enablePool(RPool);
//    ComplexTest();

    const char *filename = nil;
    const char *passwordFilename = nil;
    const char *passwordString = nil;

    rbool isManualEncrypt = no;
    rbool isManualDecrypt = no;
    rbool isSetManualMode = no;
    rbool isPasswordEncryption = yes;

    rbool decrypt = yes;
    RData *data = nil;
    RData *key = nil;
    RString *fileName = nil;
    byte nullTerminator = 0;
    int error = 0;

    int iterator;
    forAll(iterator, argc) {
        const char *currentArgument = argv[iterator];
        if (!RStringCompare(currentArgument, "-pf")) { // password file
            if (iterator + 1 <= argc) {
                passwordFilename = argv[iterator + 1];
            } else {
                RPrintf("Error. Password filename not found.");
                ++error; //  1;
                goto help;
            }
            continue;
        }

        if (!RStringCompare(currentArgument, "-f")) { // encr/decr file
            if (iterator + 1 <= argc) {
                filename = argv[iterator + 1];
            } else {
                RPrintf("Error. File name not found.");
                ++error; //  2;
                goto help;
            }
            continue;
        }

        if (!RStringCompare(currentArgument, "-p")) { // password string
            if (iterator + 1 <= argc) {
                passwordString = argv[iterator + 1];
            } else {
                RPrintf("Error. Password string not found.");
                ++error; //  3;
                goto help;
            }
            continue;
        }

        if (!RStringCompare(currentArgument, "-e")) { // manual encrypt
            isManualEncrypt = yes;
            continue;
        }

        if (!RStringCompare(currentArgument, "-d")) { // manual decrypt
            isManualDecrypt = yes;
            continue;
        }

        if (!RStringCompare(currentArgument, "help")) { // help
            printCryptorHelp();
            return 0;
        }

        if (!RStringCompare(currentArgument, "-v")) { // version
            printCryptorVersion();
            return 0;
        }
    }

    isSetManualMode = (rbool) (isManualEncrypt || isManualDecrypt);

    if(passwordFilename != nil) {
        key = contentOfFile(passwordFilename);
    } else {
        key = $(RS(passwordString), m(copy, RString)));
    }

    if(key == nil) {
        RPrintf("Error. Can't read key.");
        ++error; //  4;
        goto help;
    }

    if(isPasswordEncryption) {
        RString *tempKeyToDelete = key;
        key = $(key, m(evasionHash, RData)));
        deleter(tempKeyToDelete, RData);
    }

    if(key == nil) {
        RPrintf("Error. Can't hash key.");
        ++error; //  4;
        goto help;
    }

    data = contentOfFile(filename);
    if(data == nil) {
        RPrintf("Error. Can't read file.");
        ++error; //  5;
        goto help;
    }


    fileName = $(RS(filename), m(copy, RString)));

    if(!isSetManualMode) {
        if (!$(fileName, m(endsOn, RString)), RS(".purge"))) { // decrypt
            decrypt = no;
        }
    } else {
        if(isManualDecrypt) {
            decrypt = yes;
        }
        if(isManualEncrypt) {
            decrypt = no;
        }
    }

    if (decrypt) {

        RData *decrypted = $(data, m(decryptPurgeEvasion, RData)), key);
        if (decrypted != nil) {
            if (isManualDecrypt) {
                $(fileName, m(concatenate, RString)), RS(".purgeDecrypted"));
            } else {
                $(fileName, m(trimHead, RString)), 6); // remove .purge
            }
            appendArray(&fileName->data, &fileName->size, &nullTerminator, 1); // add \0

            $(decrypted, m(saveToFile, RData)), (const char *) fileName->data);
            deleter(decrypted, RData);
        } else {
            RPrintf("Error. Can't decrypt.");
            ++error; //  6;
            goto help;
        }
    } else {
        RData *encrypted = $(data, m(encryptPurgeEvasion, RData)), key);
        if (encrypted != nil) {
            $(fileName, m(concatenate, RString)), RS(".purge"));
            appendArray(&fileName->data, &fileName->size, &nullTerminator, 1); // add \0

            $(encrypted, m(saveToFile, RData)), (const char *) fileName->data);
            deleter(encrypted, RData);
        } else {
            RPrintf("Error. Can't encrypt.");
            ++error; //  7;
            goto help;
        }
    }

    deleter(data, RData);
    deleter(key, RData);
    deleter(fileName, RData);

    help:
    if (error) {
        RPrintf("\nUse \"purge help\" option for help.\n");
    }

//    endRay();
    return error;
}