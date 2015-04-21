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

int main(int argc, const char *argv[]) {
//    size_t iterator;

    enablePool(RPool);
    ComplexTest();
    char buffer[1500];

    RCString *html = RCStringFromFile("hello.html");

    if(html == nil)
        goto exit;
    p(RCString)(html);
    printf("html size %lu\n", html->size);

    RCString *content = stringWithFormat("HTTP/1.1 200 OK\n"
                            "Content-Type: text/html\n"
                            "Content-Length: %lu\n"
                            "\n"
                            "%s", (html->size), html->baseString);
    p(RCString)(content);
    if(content != nil) {
        RSocket *socket = makeRSocket(nil, SOCK_STREAM, IPPROTO_TCP);
        if($(socket, m(bindPort, RSocket)), 5000) == no) {
            goto exit;
        } // open on http port

        $(socket->socket, listen), 10);
        for(;;) {
            RSocket * child = $(socket, m(accept, RSocket)));

            int port;
            struct sockaddr_in *ipv4 = &child->address;
            char ipAddress[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &(ipv4->sin_addr), ipAddress, INET_ADDRSTRLEN);
            port = ntohs(child->address.sin_port);

            printf("[%s:%d] connected\n", ipAddress, port);

            if (child != nil) {
                $(child, m(receive, RSocket)), &buffer, 1500);

                printf("%s\n", buffer);

                $(child, m(setAddress, RSocket)), "127.0.0.1");
                if($(child, m(send, RSocket)), content->baseString, content->size) == networkOperationSuccessConst) {
                    printf("send sucess\n");
                } elseError (
                      RError("Send data to socket", nil)
                );
                deleter(child, RSocket);

            } elseError(
                    RError("Bad create child socket.", nil)
            );
        }

        deleter(socket, RSocket);
        deleter(content, RCString);
    }

    exit:

    endSockets();
    endRay();
}
