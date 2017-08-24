#include <string.h>
#include <iostream>
#include "../base/conn.h"
#include "../base/agent.h"
#include <arpa/inet.h>
#include "sys/socket.h"
#include <sstream>
  
using namespace std;

int main(int argc, char *argv[])  
{  

    sockaddr_in remote_addr;
    remote_addr.sin_family = AF_INET;
    remote_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    remote_addr.sin_port = htons(7777); 
    int client_sockfd;
    if((client_sockfd=socket(AF_INET,SOCK_STREAM,0))<0){
        cout << "socket error," << strerror(errno) << endl;
        return 0;
    }
    if(connect(client_sockfd,(struct sockaddr *)&remote_addr,sizeof(struct sockaddr))<0){
        cout << "connect error," << strerror(errno) << endl;
        return 0;
    }
    Conn *conn = new Conn(client_sockfd);
    cout << "connected to server,local addr, " << conn->local_addr() << endl;
    cout << "connected to server,remote addr, " << conn->remote_addr() << endl;
    unsigned int len;
    char buf[1024]; 
    memset(buf,0,1024);
    //len=conn->read(buf,1024);
    //buf[len]='\0';  
    //cout << "server say " << buf <<endl;
    Agent *agent= new Agent(conn); 
    while(1){  
        memset(buf,0,1024);
        cout << "Enter string to send:" << endl; 
        cin.getline(buf,1024);
	    //cout << buf <<endl;
        agent->sendMessage((unsigned char*)buf,strlen(buf));
        //cout << "write len " << len << endl;
        memset(buf,0,1024);
        len=conn->read(buf,1024);
        //cout <<"read len " << len << ",server say " << buf <<endl;
        agent->reciveBufWrite((unsigned char*)buf,len);
    }  
    delete conn;  
    return 0;  
}  
