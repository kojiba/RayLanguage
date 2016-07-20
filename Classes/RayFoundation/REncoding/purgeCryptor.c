/**
 *  __________ ____ _____________  ___________________
 *  \______   \    |   \______   \/  _____/\_   _____/
 *   |     ___/    |   /|       _/   \  ___ |    __)_
 *   |    |   |    |  / |    |   \    \_\  \|        \
 *   |____|   |______/  |____|_  /\______  /_______  /
 *                            \/        \/        \/
 **/

#include <RayFoundation/RayFoundation.h>
#include "Tests.h"

static const char *versionString = "1.2";

void printCryptorVersion() {
    RPrintf("Purge, evasion util\nv%s\n(c) kojiba 2016\n", versionString);
}

void printCryptorHelp() {
    printCryptorVersion();
    RPrintf("Options:\n"
                    "        -pf passwordFileName passwordFileName with password"
                    "        -f filename          filename to decrypt/encrypt\n"
                    "        -p password          password to decrypt/encrypt\n"
                    "        -e                   option for manual encrypt mode\n"
                    "        -d                   option for manual decrypt mode\n"
                    "        -v                   prints version string\n"
                    "        -h                   prints base64 hash of file\n"
                    "        -hh                  prints hex hash of file\n");
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
    rbool isHash = no;
    rbool isStringHash = no;
    rbool isHexHash = no;

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

        if (!RStringCompare(currentArgument, "-f")) { // encr/decr/hash file
            if (iterator + 1 <= argc) {
                filename = argv[iterator + 1];
                ++error;
            } else {
                RPrintf("Error. File name not found.");
                goto help;
            }
            continue;
        }

        if (!RStringCompare(currentArgument, "-p")) { // password string
            if (iterator + 1 <= argc) {
                passwordString = argv[iterator + 1];
                ++error;
            } else {
                RPrintf("Error. Password string not found.");
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

        if (!RStringCompare(currentArgument, "-h")) { // base64 hash
            isHash = yes;
            continue;
        }

        if (!RStringCompare(currentArgument, "-hh")) { // hex hash
            isHash = yes;
            isHexHash = yes;
            continue;
        }
    }

    if(filename == nil) {
        RPrintf("Error. Filename empty.");
        ++error;
        goto help;
    }

    if(isHash) {
        data = contentOfFile(filename);
        ++error;
        if(data == nil) {
            RPrintf("Error. Can't read file.");
            goto help;
        }

        RData *hexData = $(data, m(evasionHash, RData)));

        ++error;
        if (hexData == nil) {
            deleter(data, RData);
            RPrintf("Error. Can't get hash.");
            goto help;
        }

        if (!isHexHash) {
            RString *hexString = $(hexData, m(encodeBase64, RString)));
            $(hexString, p(RString)));
            deleter(hexString, RString);
        } else {
            printByteArrayInHex(hexData->data, hexData->size);
        }

        deleter(data, RData);
        deleter(hexData, RData);

    } else {
        isSetManualMode = (rbool) (isManualEncrypt || isManualDecrypt);

        if(passwordFilename != nil) {
            key = contentOfFile(passwordFilename);
        } else {
            ++error;
            if(passwordString == nil) {
                RPrintf("Error. Empty password.");
                goto help;
            }
            key = $(RS(passwordString), m(copy, RString)));
        }

        ++error;
        if(key == nil) {
            RPrintf("Error. Can't read key.");
            goto help;
        }

        if(isPasswordEncryption) {
            RString *tempKeyToDelete = key;
            key = $(key, m(evasionHash, RData)));
            deleter(tempKeyToDelete, RData);
        }

        ++error;
        if(key == nil) {
            RPrintf("Error. Can't hash key.");
            goto help;
        }

        data = contentOfFile(filename);
        if(data == nil) {
            deleter(key, RData);
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
            ++error;
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
                goto help;
            }
        } else {
            RData *encrypted = $(data, m(encryptPurgeEvasion, RData)), key);
            ++error;
            if (encrypted != nil) {
                $(fileName, m(concatenate, RString)), RS(".purge"));
                appendArray(&fileName->data, &fileName->size, &nullTerminator, 1); // add \0

                $(encrypted, m(saveToFile, RData)), (const char *) fileName->data);
                deleter(encrypted, RData);
            } else {
                RPrintf("Error. Can't encrypt.");
                goto help;
            }
        }

        deleter(data, RData);
        deleter(key, RData);
        deleter(fileName, RData);
    }

    help:
    if (error) {
        RPrintf("\nUse \"purge help\" option for help.\n");
    }

//    endRay();
    return error;
}