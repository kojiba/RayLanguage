#include "RNetwork.h"
#include "../RClassTable/RClassTable.h"

constructor(RSocket), int domain, int type, int protocol) {
#ifdef _WIN32
    // init wsa
    static rbool firstTime = YES;
    if(firstTime == YES) {
//    testme, fixme
        WSADATA wsaData;
        if(WSAStartup(wVersionRequested, &wsaData) != 0) {
            RError("RSocket. Windows. WSA startup.");
            WSACleanup();
        }
        firstTime = NO;
    }
#endif
    object = allocator(RSocket);
    if(object != NULL) {
        object->classId = registerClassOnce(toString(RSocket));
        object->socket  = RSocketOpen(domain, type, protocol);
    }
    return object;
}

