/**
 * RTCPHandler.c
 * Threaded TCP connection handler.
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
 * 6/16/15 Ukraine Kharkiv
 *  _         _ _ _
 * | |       (_|_) |
 * | | _____  _ _| |__   __ _
 * | |/ / _ \| | | '_ \ / _` |
 * |   < (_) | | | |_) | (_| |
 * |_|\_\___/| |_|_.__/ \__,_|
 *          _/ |
 *         |__/
 **/

#include "RTCPHandler.h"

constructor(RTCPHandler)) {
    object = allocator(RTCPHandler);
    if(object != nil) {
        object->delegate = nil;
        object->listener = makeRSocket(nil, SOCK_STREAM, IPPROTO_TCP);
        if(object->listener != nil) {
            object->threads = c(RThreadPool)(nil);
        }
    }
    return object;
}

destructor(RTCPHandler) {
    deleter(object->listener, RSocket);
    deleter(object->threads,  RThreadPool);
}

getterImpl(delegate, RThreadFunction, RTCPHandler)
setterImpl(delegate, RThreadFunction, RTCPHandler)

method(void, start, RTCPHandler), pointer context) {
    object->runningThread = currentThread();

    object->terminateFlag = no;
    $(object->threads,  m(setDelegateFunction, RThreadPool)), object->delegate);
    $(object->listener, m(listen,              RSocket)),     50);

    while(!object->terminateFlag) {
        RTCPDataStruct *argument = allocator(RTCPDataStruct);

        if(argument != nil) {
            argument->context = context;
            argument->socket  = $(object->listener, m(accept, RSocket)));
            $(object->threads, m(addWithArg, RThreadPool)), argument, yes);
        } elseError(
                RError("RTCPHandler. Can't allocate thread argument.", object)
        );
    }
}

method(void, terminate,  RTCPHandler)) {
    RThreadKill(&object->runningThread);
    $(object->threads, m(cancel, RThreadPool)));
}