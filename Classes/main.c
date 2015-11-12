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

RData* EmptyKeyForConnectionData(RTCPDataStruct *connectionData, pointer context) {
    return nil;
}

pointer TempExec(RTCPDataStruct *data) {
    RPrintf("%lu connected\n", data->identifier);

    RData *received = PEConnectionReceive(((RTCPDataStructPE*)data->context)->connection, nil);

    if(received) {
        received->type = RDataTypeASCII;
        p(RData)(received);
    }
    nilDeleter(received, RData);

    PEConnectionDeleter(((RTCPDataStructPE*)data->context)->connection);
    ((RTCPDataStructPE*)data->context)->connection = nil;
    data->socket = nil;
    data->context = nil;

    return nil;
}

RTCPHandlerPE *globalHandler;
RTCPDelegate *globalDelegate;

void startPEServer(){

    RTCPHandlerPE *server1;
    RTCPDelegate *delegate1;
    PEKeyGeneratorDelegate *keyGenerator1;

    RTCPHandlerPE **server = &globalHandler;
    RTCPDelegate **delegate = &globalDelegate;
    PEKeyGeneratorDelegate **keyGenerator = &keyGenerator1;

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

pointer TempExec2(RTCPDataStruct *data) {
    RPrintf("%lu connecting...\n", data->identifier);

    PEConnectionSend(((RTCPDataStructPE*)data->context)->connection, RS("Hello bro=)"));
    data->socket = nil;

    return nil;
}

void startPEClient(){
    RTCPHandlerPE *server1;
    RTCPDelegate *delegate1;
    PEKeyGeneratorDelegate *keyGenerator1;

    RTCPHandlerPE **server = &server1;
    RTCPDelegate **delegate = &delegate1;
    PEKeyGeneratorDelegate **keyGenerator = &keyGenerator1;

    *keyGenerator = allocator(PEKeyGeneratorDelegate);
    (*keyGenerator)->keyForConnectionData = EmptyKeyForConnectionData;

    *server = c(RTCPHandlerPE)(nil);

    if(*server != nil) {

        $((*server), m(set_keyGeneratorDelegate, RTCPHandlerPE)), *keyGenerator);

        (*server)->dataStructContextDestructor = nil;


        *delegate = allocator(RTCPDelegate);
        if(*delegate != nil) {
            (*delegate)->delegateFunction = TempExec2;
            (*delegate)->context          = nil;

            $(*server,  m(set_delegate, RTCPHandlerPE)), *delegate);
            $(*server,  m(startWithHost,  RTCPHandlerPE)), RS("127.0.0.1"), 3000, 5);

            RPrintf("RTCPHandlerPE connector started %p on port %u\n", *server, 3000);

            $(*server, m(waitConnectors, RTCPHandlerPE)));


            deleter(*server, RTCPHandlerPE);
            deallocator(*delegate);
            deallocator(*keyGenerator);
        }
    }
}


int main(int argc, const char *argv[]) {
    enablePool(RPool);
    ComplexTest(); // lib test

    RThread thread = makeRThread((RThreadFunction) startPEServer);
    sleep(2);

    RThread threadConnector = makeRThread((RThreadFunction) startPEClient);

    RThreadJoin(threadConnector);

    $(globalHandler, m(terminate, RTCPHandlerPE)));
    deleter(globalHandler, RTCPHandlerPE);

    deallocator(globalDelegate);

    endRay();
}