#include <sys/epoll.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <fcntl.h>  
#include <unistd.h> 
#include <arpa/inet.h>  
#include <iostream>  
#include <pthread.h> 
#include "epoll_server.h"
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "../base/conn.h"
#include "../base/agent.h"
#include "boost/function.hpp"
#include <boost/bind.hpp>

using namespace std;

EpollServer::EpollServer(){
};

EpollServer::~EpollServer(){
    close(sock_fd_);
};

void EpollServer::onMessage(Agent *agent,Buffer *buf,unsigned int len){
    unsigned char msg[len+1];
    memset(msg,0,len+1);
    buf->read(msg,len);
    std::cout<<msg<<endl;
    agent->sendMessage(msg,len);
};

bool EpollServer::listenAccept(const char* ip,const int port){
    epoll_fd_ = epoll_create(MAX_SOCKFD_COUNT);
    if (fcntl(epoll_fd_,F_SETFL,O_NONBLOCK)<0) {
        cout << "set non-blocking err," << strerror(errno) << endl;
        return false;
    }

    if((sock_fd_=socket(AF_INET,SOCK_STREAM,0))<0){    
        cout << "socket err," << strerror(errno) << endl;
        return false;  
    }  
    int reuse_port = 1;//支持端口复用  
    setsockopt(sock_fd_, SOL_SOCKET, SO_REUSEADDR, &reuse_port, sizeof(reuse_port));

    sockaddr_in listen_addr;
    listen_addr.sin_family = AF_INET;
    listen_addr.sin_addr.s_addr = inet_addr(ip);
    listen_addr.sin_port = htons(port);
    if(bind(sock_fd_,(struct sockaddr*)&listen_addr,sizeof(struct sockaddr))<0){
        cout << "bind err," << strerror(errno) << endl;
        return false;
    }; 

    if(listen(sock_fd_,9999)<0){
        cout << "listen err," << strerror(errno) << endl;
        return false;
    }  
    if(pthread_create(&accept_thread_id_,0,(void* (*)(void*))acceptThread,this) !=0){
        cout << "create thread err," << strerror(errno) << endl;
        return false;    
    };
    cout << "服务器监听" << ip << ":" << port << "中..." << endl;
    return true;
};


void EpollServer::acceptThread(void* srv){
    
    EpollServer *esrv = (EpollServer*)srv;
    while(true){
        //sockaddr_in *remote_addr = new(sockaddr_in);  
        //int len = sizeof(remote_addr);  
        //int client_socket = accept(esrv->sock_fd_, (sockaddr *)&remote_addr,(socklen_t*)&len);  
        int client_socket = accept(esrv->sock_fd_, NULL,NULL);  
        if ( client_socket < 0 ){  
            cout << "accept err," << client_socket << strerror(errno) << endl;
            break;
        } else {  
    	    Conn *conn = new Conn(client_socket);
            cout << "new conn,local addr, " << conn->local_addr() << endl;
            cout << "new conn,remote addr, " << conn->remote_addr() << endl;
	    Agent *agent = new Agent(conn);
            agent->setMessageCallBack(boost::bind(&EpollServer::onMessage, esrv, _1,_2,_3));
            struct epoll_event  ev;  
            ev.events = EPOLLIN | EPOLLERR | EPOLLHUP;  
            ev.data.ptr = agent;
            epoll_ctl(esrv->epoll_fd_, EPOLL_CTL_ADD, client_socket, &ev);  
        };     
    };
};


void EpollServer::run(){
    while(1){
        struct epoll_event  events[MAX_SOCKFD_COUNT]; 
        cout << "to wait" << endl;
        int nfds = epoll_wait(epoll_fd_, events, MAX_SOCKFD_COUNT, -1);  
        for(int i=0;i<nfds;i++){
            unsigned int buf_len = 1024;
            unsigned char buffer[buf_len];//每次收发的字节数小于1024字节  
            memset(buffer, 0, buf_len);  
            Agent *agent = (Agent *)events[i].data.ptr;
            if(events[i].events & EPOLLIN){
                unsigned int rev_size = agent->conn_->read(buffer,buf_len);
                if(rev_size > 0){
		    agent->reciveBufWrite(buffer,rev_size);
                }else if (errno == EAGAIN || errno == EINTR){
                    cout << "epoll continue:" << strerror(errno) << endl;     
                    continue;
                }else{
                    cout << "epoll err:" << strerror(errno) << endl;     
                    epoll_ctl(epoll_fd_, EPOLL_CTL_DEL,agent->conn_->sockfd(), &events[i]);  
                    delete agent;
                }
            }else if(events[i].events & EPOLLOUT){
                    cout << "out";    
            }else if(events[i].events & EPOLLHUP){
                    cout << "hup";    
                    return;
            }
        }
    }
}
