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

#include <RString.h>

int main(int argc, const char *argv[]) {
//    size_t iterator;

    enablePool(RPool);
    ComplexTest();
//
//    char buffer[1500];
//
//    RCString *html = stringFromFile("../Resources/hello.html");
//
//    if(html == nil)
//        goto exit;
//
//    printf("Html content begin =================\n");
//    p(RCString)(html);
//    printf("Html content end ===================\n");
//
//    printf("Html page size %lu\n", html->size);
//
//    RCString *content = stringWithFormat("HTTP/1.1 200 OK\n"
//                                                 "Content-Type: text/html\n"
//                                                 "Content-Length: %lu\n"
//                                                 "\n"
//                                                 "%s", (html->size), html->baseString);
//
//
//    if(content != nil) {
//        RSocket *socket = makeRSocket(nil, SOCK_STREAM, IPPROTO_TCP);
//        if($(socket, m(bindPort, RSocket)), 5000) == no) {
//            goto exit;
//        } // open on http port
//
//        $(socket->socket, listen), 10);
//        for(;;) {
//            RSocket * child = $(socket, m(accept, RSocket)));
//
//            if (child != nil) {
//                char *addr = addressToString(&child->address);
//                ushort port = ntohs(child->address.sin_port);
//                printf("%s:%u connected\n", addr, port);
//
//                $(child, m(receive, RSocket)), &buffer, 1500);
//
//                printf("REQ ============\n"
//                       "%s\n"
//                       "REQ END ========\n", buffer);
//
//                $(child, m(setAddress, RSocket)), "127.0.0.1");
//                if($(child, m(send, RSocket)), content->baseString, content->size) == networkOperationSuccessConst) {
//                    printf("send sucess\n");
//                } else {
//                    RError("Send data to socket", nil);
//                    goto exit;
//                }
//                deleter(child, RSocket);
//
//            } else {
//                RError("Bad create child socket.", nil);
//                goto exit;
//            }
//        }
//
//        deleter(socket, RSocket);
//        deleter(content, RCString);
//    }
//    deleter(html, RCString);
//
//    exit:
//
//    endSockets();

    byte *str = "Пример ютф-8 строки џўќ†њѓ";
    char *character = (char *) str;
    size_t size = strlen((const char*) str);
    size_t length = utf8Length(str, size);
    RString *string = RSC("Пример");

    $(string, m(replaceСSubstrings, RCString)), "им", "_replacer_");

    p(RCString)(string);

    if(length != size) {
        printf("Length = %lu, size = %lu\n", length, size);

        while(*character != 0) {
            printf("%c", *character);
            ++character;
        }
    }

    return EXIT_SUCCESS;

    endRay();
}


