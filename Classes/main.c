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
#include <RayFoundation/REncoding/purge.h>

int main(int argc, const char *argv[]) {
    enablePool(RPool);


    byte array[purgeBytesCount] = {};
    byte key[purgeBytesCount] = {};


    printByteArrayInHexWithScreenSize(array, purgeBytesCount, 32);

    byte cipherText[purgeBytesCount] = {};

    initRClock();
    purgeEncrypt((uint64_t *) array, (uint64_t *) key);
    tickRClock();
    printByteArrayInHexWithScreenSize(array, purgeBytesCount, 32);
    memset(key, 0, purgeBytesCount);

    tickRClock();
    purgeDecrypt((uint64_t *) array, (uint64_t *) key);
    tickRClock();
    printByteArrayInHexWithScreenSize(array, purgeBytesCount, 32);

    /* Etalon
     *
     * Encr Elapsed: 0.000053 seconds
       Decr Elapsed: 0.000085 seconds

     * 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
       00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
       B8 FF E8 11 EB 46 0F FE 12 AE 7F 34 E7 7A 03 49 18 B8 F8 AA EA F4 D6 3D 1F A8 98 35 35 C7 5C 42
       91 42 7E 4C CF EA E4 30 56 6E 4B 28 19 4D D0 FA 72 55 FE DB 48 D5 79 FA 5A 1D 9B 47 10 9D E1 7E
       00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
       00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
     */

    /* PURGE_DECRYPT_SPEEDUP
     *
     * Encr Elapsed: 0.000060 seconds
       Decr Elapsed: 0.000061 seconds

     * 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
       00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
       B8 FF E8 11 EB 46 0F FE 12 AE 7F 34 E7 7A 03 49 18 B8 F8 AA EA F4 D6 3D 1F A8 98 35 35 C7 5C 42
       91 42 7E 4C CF EA E4 30 56 6E 4B 28 19 4D D0 FA 72 55 FE DB 48 D5 79 FA 5A 1D 9B 47 10 9D E1 7E
       00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
       00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
     */


    endRay();
}