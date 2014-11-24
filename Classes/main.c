/**
 * main.c
 * Ray additions test.
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
 */

#include "Tests.h"
#include "RayFoundation/RayFoundation.h"

int main(int argc, const char *argv[]) {
    initPointers();
    ComplexTest();
    initPointers();
    size_t iterator;

    RBuffer *buff = $(nil, c(RBuffer)));

//    forAll(iterator, 1024) {
//        RCString *res = randomRCString();
//        $(buff, m(addData, RBuffer)), res->baseString, res->size);
//        deleter(res, RCString);
//    }
    $(buff, m(addData, RBuffer)), "1", sizeof("1"));
    $(buff, m(addData, RBuffer)), "2", sizeof("2"));
    $(buff, m(addData, RBuffer)), "3", sizeof("3"));


    $(buff, p(RBuffer)));
    $(buff, m(deleteDataAt, RBuffer)), 1);
    $(buff, p(RBuffer)));

    deleter(buff, RBuffer);
    return 0;
}
