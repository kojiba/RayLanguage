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
#include <RayFoundation/Utils/PurgeEvasionTCPHandler.h>
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

RData* EmptyKeyForConnectionData(RTCPDataStruct *connectionData, pointer context) {
    return nil;
}

pointer TempExec(RTCPDataStruct *data) {
    RPrintf("one connected");
}

void startPEServer(){
    RTCPHandlerPE **server;
    RTCPDelegate **delegate;
    PEKeyGeneratorDelegate **keyGenerator;

    *keyGenerator = allocator(PEKeyGeneratorDelegate);
    (*keyGenerator)->keyForConnectionData = EmptyKeyForConnectionData;

    *server = c(RTCPHandlerPE)(nil);

    if(*server != nil) {

        $((*server), m(set_keyGeneratorDelegate, RTCPHandlerPE)), *keyGenerator);

        (*server)->dataStructContextDestructor = nil;


        *delegate = allocator(RTCPDelegate);
        if(*delegate != nil) {
            (*delegate)->delegateFunction = TempExec;
            (*delegate)->context          = nil;

            $(*server,  m(set_delegate, RTCPHandlerPE)), *delegate);
            $(*server,  m(startOnPort,  RTCPHandlerPE)), 3000);

            RPrintf("RTCPHandlerPE server started %p on port %u\n", *server, 3000);
        }
    }
}

int main(int argc, const char *argv[]) {
    enablePool(RPool);
    ComplexTest(); // lib test


    endRay();
}