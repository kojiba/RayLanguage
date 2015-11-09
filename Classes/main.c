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


void receive() {
    uint64_t masterKey[8] = {};
    RSocket *listener = openListenerOnPort(3000, 10);
    while(listener != nil) {
        RPrintLn("Listen");
        RSocket *client = acceptRSocket(listener);

        if(client != nil) {
            RPrintLn("Accepted");
            RData *result;
            PEConnection *connection = PEConnectionInit(client, initPEContext(masterKey));
            PEConnectionReceive(connection, &result);

            if(result != nil) {
                RPrintLn("Received bufffer");
                p(RData)(result);

                $(result, m(validateToASCII, RString)));
                p(RData)(result);

                deleter(result, RData);

            } else {
                RPrintLn("Error receive");
            }

            deleter(connection, PEConnection);
            deleter(listener, RSocket);
        } else {
            break;
        }
    }

}

int main(int argc, const char *argv[]) {
    enablePool(RPool);
    ComplexTest(); // lib test

    uint64_t masterKey[8] = {};

    RThread receiver = makeRThread((RThreadFunction) receive);

    sleep(1);

    RSocket *socket = socketConnectedTo("127.0.0.1", 3000);
    RPrintLn("Connected");
    PEConnection *connection = PEConnectionInit(socket, initPEContext(masterKey));

    if(connection != nil) {
        RData *toSend = $(RS("Hello world from crypto maniac 1!\n"
                             "Hello world from crypto maniac 2!\n"
                             "Hello world from crypto maniac 3!\n"
                             "Hello world from crypto maniac 4!\n"), m(copy, RData)));

        toSend->type = RDataTypeBytes;

        PEConnectionSend(connection, toSend);
        RPrintLn("Sended");
        p(RData)(toSend);
        deleter(toSend, RData);
        deleter(connection, PEConnection);

        RThreadJoin(receiver);
    }

    RThreadCancel(receiver);

    endRay();
}