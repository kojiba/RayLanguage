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

#include <RayFoundation.h>
#include <unistd.h>
#include "Tests.h"

#include "RVirtualMachine/RVirtualFunction/RVirtualFunction.h"
#include "RVirtualMachine/RVirtualCompiler/RVirtualCompiler.h"
#include "RVirtualMachine/RVirtualMachine/RVirtualMachine.h"

void receive(void) {
    size_t iterator;
    char buffer[1500];
    RSocket *receiver = c(RSocket)(nil);
    $(receiver, m(bindPort, RSocket)), 8888);
    forAll(iterator, 20) {
        $(receiver, m(receive, RSocket)), buffer, 1500);
        printf("Received - %s from %u \n", buffer, receiver->address.sin_addr);
    }

    deleter(receiver, RSocket);
}

int main(int argc, const char *argv[]) {
    size_t iterator;
    enablePool(RPool);
    ComplexTest();

    RThread receiver;
    RThreadCreate(&receiver, nil, (RThreadFunction) receive, nil);

    RSocket *socket = c(RSocket)(nil);
//    sleep(1);

    $(socket, m(setPort,         RSocket)), 8888);
//    $(socket, m(enableBroadCast, RSocket)), yes);
    $(socket, m(setAddress,      RSocket)), "127.0.0.1");

    forAll(iterator, 19) {
        $(socket, m(sendString, RSocket)), RS("Hello to other planet! UDP works."));
        sleep(1);
    }
    $(socket, m(sendString, RSocket)), RS("END CONNECTION"));

    deleter(socket, RSocket);

    RThreadJoin(&receiver);
    endSockets();


//
//    // [ ] ,  [ [ [ ] ] ],  [ [ ] [] [ ] ]
//    RCString *source = RS(" Hello : + [>-<-]");
//
//    // brainfuck hard(with [, ]) hello world on RVM
//    RVirtualFunction *function = $(RVC, m(createFunctionFromBrainFuckSourceCode, RVirtualCompiler)), source );
//
//    p(RVirtualFunction)(function);
//
//    executeRay(function);
//
//    deleter(function, RVirtualFunction);
//    deleter(RVM, RVirtualMachine);
//    deleter(RVC, RVirtualCompiler);



    endRay();
}


