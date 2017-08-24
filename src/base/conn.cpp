#include <sys/socket.h>
#include <arpa/inet.h>  
#include <unistd.h>

#include <sstream>
#include <iostream>

#include "./conn.h"

Conn::Conn(int sockfd){
    sockfd_ = sockfd;
};


Conn::~Conn(){
   ::close(sockfd_);
};


int Conn::sockfd(){
   return sockfd_;
};

int Conn::read(void *data,int len){
    len=::recv(sockfd_,data,len,0);
    return len;
};

int Conn::write(const void *data,int len){
    len=::send(sockfd_,data,len,0); 
    return len;
};


std::string Conn::local_addr(){
    if(local_addr_str_ != ""){
        return local_addr_str_;
    };
    socklen_t addr_len = sizeof(local_addr_);
    getsockname(sockfd_,(struct sockaddr *)&local_addr_,&addr_len);
    std::ostringstream oss;
    oss << inet_ntoa(local_addr_.sin_addr) << ":" << ntohs(local_addr_.sin_port); 
    local_addr_str_ = oss.str();
    return local_addr_str_;
};

std::string Conn::remote_addr(){
    if(remote_addr_str_ != ""){
        return remote_addr_str_;
    };
    socklen_t addr_len = sizeof(remote_addr_);
    getpeername(sockfd_,(struct sockaddr *)&remote_addr_,&addr_len);
    std::ostringstream oss;
    oss << inet_ntoa(remote_addr_.sin_addr) << ":" << ntohs(remote_addr_.sin_port); 
    remote_addr_str_ = oss.str();
    return remote_addr_str_;
};
