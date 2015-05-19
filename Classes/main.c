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

#define LOCAL_MULTICAST "224.0.0.1"

void receive(void) {
    size_t iterator;
    char buffer[1500];
    RSocket *receiver = c(RSocket)(nil);
    $(receiver, m(bindPort, RSocket)), 8888);

    $(receiver, m(joinMulticastGroup, RSocket)), LOCAL_MULTICAST);
    forAll(iterator, 20) {
        $(receiver, m(receive, RSocket)), buffer, 1500);
        printf("Received - %s", buffer);
        printf(" from %s\n", addressToString(&receiver->address));
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

    $(socket, m(setPort,         RSocket)), 8888);
    $(socket, m(setAddress,      RSocket)), LOCAL_MULTICAST);

    forAll(iterator, 19) {
        $(socket, m(sendString, RSocket)), RS("Some udp multicast for hello bro"));
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


