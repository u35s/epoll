#ifndef CONN_H
#define CONN_H

#include <netinet/in.h>  
#include <string>

class Conn {
public:
   Conn(const int sockfd);
   ~Conn();
   int write(const void *data,int len);
   int read(void *data,int len);
   int sockfd();
   void close();

   std::string local_addr();
   std::string remote_addr();

private:
   sockaddr_in remote_addr_;
   sockaddr_in local_addr_;

   std::string      remote_addr_str_;
   std::string      local_addr_str_;

   int         sockfd_;
};

#endif
