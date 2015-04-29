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
#include "Tests.h"

#define Equals(rstring, rstring2) ($(rstring, m(compareWith, RCString)), rstring2) == equals)

void func(pointer arg) {
    printf("Im a thread %p\n", arg);
    printf("hello\n");
}

int main(int argc, const char *argv[]) {
    enablePool(RPool);
    ComplexTest();

    rbool exit = no;

    RThread thread1, thread2;
    RThreadCreate(&thread1, nil, (RThreadFunction) func, nil);
    RThreadCreate(&thread2, nil, (RThreadFunction) func, (pointer) 1);

    while(!exit) {
        RString *command = getInputString();
        if(Equals(command, RS("exit"))) { // RS("exit") creates once
            exit = yes;
        } else if(Equals(command, RS("hello"))) { // RS("hello") creates once
            RPrintLn("Hello to You, %username%\n");
        }
        deleter(command, RCString);
    }

    RThreadJoin(&thread1);
    RThreadJoin(&thread2);


    endRay();
}


