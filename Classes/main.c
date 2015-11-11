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
        RData *result;
        PEConnection *connection = PEConnectionInit(acceptRSocket(listener), initPEContext(masterKey));

        if(connection != nil) {
            result = PEConnectionReceive(connection, nil);

            if (result != nil) {
                RPrintLn("Received buffer");
                p(RData)(result);

                $(result, m(validateToASCII, RString)));
                p(RData)(result);

                deleter(result, RData);

            } else {
                RPrintLn("Error receive");
            }

            deleter(connection, PEConnection);
            deleter(listener, RSocket);
            listener = nil;
        }
    }

}

int main(int argc, const char *argv[]) {
    enablePool(RPool);
    ComplexTest(); // lib test

//    uint64_t masterKey[8] = {};
//
//    RThread receiver = makeRThread((RThreadFunction) receive);
//
//    sleep(1);
//
//    RSocket *socket = socketConnectedTo("127.0.0.1", 3000);
//    RPrintLn("Connected");
//    PEConnection *connection = PEConnectionInit(socket, initPEContext(masterKey));
//
//    if(connection != nil) {
//        RData *toSend = $(RS("Hello world from crypto maniac 1!\n"
//                             "Hello world from crypto maniac 2!\n"
//                             "Hello world from crypto maniac 3!\n"
//                             "Hello world from crypto maniac 4!\n"
//                             "Hello world from crypto maniac 5!\n"
//                             "Hello world from crypto maniac 6!\n"
//                             "Hello world from crypto maniac 7!\n"
//                             "Hello world from crypto maniac 8!\n"
//                             "Hello world from crypto maniac 9!\n"
//                             "Hello world from crypto maniac 10!\n"
//                             "Hello world from crypto maniac 11!\n"
//                             "Hello world from crypto maniac 12!\n"
//                             "Hello world from crypto maniac 13!\n"
//
//        ), m(copy, RData)));
//
//        toSend->type = RDataTypeBytes;
//
//        PEConnectionSend(connection, toSend);
//        RPrintLn("Sended");
//        p(RData)(toSend);
//        deleter(toSend, RData);
//        deleter(connection, PEConnection);
//
//        RThreadJoin(receiver);
//    }
//
//    RThreadCancel(receiver);

    RString *temp = RSC("øˆ˙(+397)¬ˆ˙¥ˆ¥œ∑∂");
    $(temp, m(deleteAllSubstrings, RString)), RS("(+397)"));
    p(RString)(temp);

    deleter(temp, RString);

    endRay();
}