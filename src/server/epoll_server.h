#ifndef EPOLL_SERVER_H
#define EPOLL_SERVER_H

#include <pthread.h> 
#include "../base/agent.h" 
#include "../base/buffer.h" 

#define MAX_SOCKFD_COUNT 65535 

class EpollServer {
public:
    EpollServer();
    ~EpollServer();

    bool listenAccept(const char* ip,const int port);
    void onMessage(Agent*,Buffer*,unsigned int);
    void run();
private:
    int epoll_fd_;
    int sock_fd_;
    pthread_t accept_thread_id_;

    static void acceptThread(void* srv);
};
#endif


