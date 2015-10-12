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
#include <unistd.h>
#include "Tests.h"

int main(int argc, const char *argv[]) {
    enablePool(RPool);
    ComplexTest();

    char string[] = "|lol|lol|lol|";

    size_t size = sizeof(string) - 1;
    byte *data = getByteArrayCopy((const byte *) string, size);

    char substring[] = "lol";
    size_t subSize = sizeof(substring) - 1;

    deleteAllSubArrays(data, &size, substring, subSize);

    data[size] = 0;
    RPrintf("%s\n", data);

    deleteRepetitionsOfByte(data, &size, '|');
    data[size] = 0;
    RPrintf("%s\n", data);

//    deleteRepetitionsOfSubArray(data, &size, (const byte *) "bro", sizeof("bro") - 1);

//    data[size] = 0;
//    RPrintf("%s\n", data);



    deallocator(data);

    endRay();
}