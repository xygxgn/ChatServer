#include "server/net/ChatServer.hh"
#include "server/service/ChatService.hh"

#include <signal.h>
#include <iostream>

void resetHandler(int)
{
    ChatService::instance()->reset();
    exit(0);
}

int main(int argc, char **argv)
{
    signal(SIGINT, resetHandler);

    if (argc < 3)
    {
        std::cerr << "command invalid! example: ./ChatClient 127.0.0.1 6000" << std::endl;
        ::exit(-1);
    }
    char *ip = argv[1];
    uint16_t port = ::atoi(argv[2]);

    EventLoop loop;
    InetAddress addr(ip, port);
    ChatServer server(&loop, addr, "ChatServer");
    server.start();
    loop.loop();
    
    return 0;
}