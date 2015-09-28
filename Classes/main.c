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
#include <RayFoundation/Utils/PurgeEvasionConnection.h>
#include <unistd.h>

#include "Tests.h"

void receive() {
    uint64_t masterKey[8] = {};
    RSocket *listener = openListenerOnPort(3000, 10);
    while(listener != nil) {
        RPrintf("Listen\n");
        RSocket *client = acceptRSocket(listener);
        if(client != nil) {
            RPrintf("Accepted \n");
            RByteArray *result;
            PEConnection *connection = PEConnectionInit(client, initPEContext(masterKey));
            PEConnectionReceive(connection, &result);

            if(result != nil) {
                RPrintf("Received bufffer\n");
                p(RByteArray)(result);

                RPrintf("ASCII: \n%s\n",result->array);
                deleter(result, RByteArray);

            } else {
                RPrintf("Error receive\n");
            }

            deleter(connection, PEConnection);
        } else {
            break;
        }
    }

}

int main(int argc, const char *argv[]) {
    enablePool(RPool);
    ComplexTest(); // lib test

    size_t iterator;

    uint64_t masterKey[8] = {};

    RThread receiver = makeRThread((RThreadFunction) receive);

    sleep(1);

    RSocket *socket = socketConnectedTo("127.0.0.1", 3000);
    RPrintf("Connected\n");
    PEConnection *connection = PEConnectionInit(socket, initPEContext(masterKey));

    if(connection != nil) {
        RByteArray *toSend = RBfromRCS(RS("Hello world from crypto maniac 1!\n"
                             "Hello world from crypto maniac 2!\n"
                             "Hello world from crypto maniac 3!\n"
                             "Hello world from crypto maniac 4!\n"
                             "Hello world from crypto maniac 5!\n"
                             "Hello world from crypto maniac 6!\n"
                             "Hello world from crypto maniac 7!\n"
                             "Hello world from crypto maniac 8!\n"
                             "Hello world from crypto maniac 9!"));

        PEConnectionSend(connection, toSend);
        RPrintf("Sended\n");

        sleep(1);
        deleter(connection, PEConnection);
    }

    RThreadCancel(receiver);

    endRay();
}