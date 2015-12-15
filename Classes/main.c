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

    rbool end = no;
    do {
        RData *received = PEConnectionReceive(((RTCPDataStructPE *) data->context)->connection, nil);

        if (received) {
            $(received, m(validateToASCII, RString)));
            RPrintf("%lu -> ", data->identifier);
            p(RData)(received);
            end = $(received, m(isEqualTo, RString)), RS("End"));
            deleter(received, RData);
        } else {
            end = yes;
        }

    } while(!end);

    RPrintf("%lu disconnected\n", data->identifier);

    if(((RTCPDataStructPE*)data->context)->connection != nil) {
        PEConnectionDeleter(((RTCPDataStructPE *) data->context)->connection);
        ((RTCPDataStructPE *) data->context)->connection = nil;
    }
    data->socket = nil;

    return nil;
}

RTCPHandlerPE *globalHandler;
RTCPDelegate  *globalDelegate;
PEKeyGeneratorDelegate *globalKeyGenerator;

RThread clientThread = nil;

void startPEClient();

void startedNotifier(RTCPHandler *handler) {
    if(handler == globalHandler->handler) {
        RPrintf("RTCPHandlerPE server started %p on port %u\n", handler, 3000);

        if(clientThread == nil) {
            clientThread = makeRThread((RThreadFunction) startPEClient);
        }
    } else {
        RPrintf("RTCPHandlerPE connector started %p on port %u\n", handler, 3000);
    }
}

void startPEServer(){
    RTCPHandlerPE **server = &globalHandler;
    RTCPDelegate **delegate = &globalDelegate;
    PEKeyGeneratorDelegate **keyGenerator = &globalKeyGenerator;

    *keyGenerator = allocator(PEKeyGeneratorDelegate);
    (*keyGenerator)->context = nil;
    (*keyGenerator)->keyForConnectionData = EmptyKeyForConnectionData;

    *server = c(RTCPHandlerPE)(nil);

    if(*server != nil) {

        $((*server), m(set_keyGeneratorDelegate, RTCPHandlerPE)), *keyGenerator);

        (*server)->dataStructContextDestructor = nil;


        *delegate = allocator(RTCPDelegate);
        if(*delegate != nil) {
            (*delegate)->delegateFunction = TempExec;
            (*delegate)->context          = nil;

            (*server)->handler->handlerStartedNotifier = (RThreadFunction) startedNotifier;
            $(*server,  m(set_delegate, RTCPHandlerPE)), *delegate);
            $(*server,  m(startOnPort,  RTCPHandlerPE)), 3000);
        }
    }
}

pointer TempExec2(RTCPDataStruct *data) {
    RPrintf("%lu connecting...\n", data->identifier);

    PEConnectionSend(((RTCPDataStructPE*)data->context)->connection, RS("Hello bro=)"));

    sleep(1);
    PEConnectionSend(((RTCPDataStructPE*)data->context)->connection, RS("Come, get some!"));
    PEConnectionSend(((RTCPDataStructPE*)data->context)->connection, RS("End"));

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

            $(*server, m(set_delegate, RTCPHandlerPE)), *delegate);
            (*server)->handler->handlerStartedNotifier = (RThreadFunction) startedNotifier;
            $(*server, m(startWithHost,  RTCPHandlerPE)), RS("127.0.0.1"), 3000, 10);
            $(*server, m(waitConnectors, RTCPHandlerPE)));

            deleter(*server, RTCPHandlerPE);
            deallocator(*delegate);
            deallocator(*keyGenerator);
        }
    }
}

int main(int argc, const char *argv[]) {
    enablePool(RPool);
    stringConstantsTable();
    RCTSingleton;

    ComplexTest(); // lib test

    startPEServer();
    sleep(2);

    RThreadJoin(clientThread);

    if(globalHandler) {
        $(globalHandler, m(terminate, RTCPHandlerPE)));

        deleter(globalHandler, RTCPHandlerPE);
        deallocator(globalDelegate);
        deallocator(globalKeyGenerator);
    }

    endRay();
}