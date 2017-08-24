#include "./agent.h"
#include <iostream>
#include <boost/bind.hpp>

Agent::Agent(Conn* conn):
recive_buf_(Buffer(1024)) {
    conn_=conn;
    messageCallBack_=boost::bind(&Agent::defaultMessageCallBack, this, _1,_2,_3);
};

Agent::~Agent(){
    delete conn_;
};

void Agent::setMessageCallBack(message_callback_t callback){
    messageCallBack_ = callback;
}

void Agent::defaultMessageCallBack(Agent *agent,Buffer *buf,unsigned int len){
    unsigned char msg[len+1];
    memset(msg,0,len+1);
    buf->read(msg,len);
    std::cout<<msg<<std::endl;
};


void Agent::reciveBufWrite(unsigned char *msg,unsigned int len){
    recive_buf_.write(msg,len);
    while(1){
        if(!head_len_read_ && recive_buf_.len()<2){
            return;
        };
        if(!head_len_read_){
           unsigned char head[2]; 
           recive_buf_.read(head,2);
           head_len_ = *head; 
           std::cout<<"msg len:"<<head_len_<<std::endl;
           head_len_read_ = true;
        };
        if(recive_buf_.len()<head_len_){
           return;
        };
        head_len_read_ = false;
        messageCallBack_(this,&recive_buf_,head_len_);
    };
};


void Agent::sendMessage(unsigned char *msg,unsigned int len){
   Buffer buf(2+len);
   unsigned char wlen[2];
   *wlen=(unsigned short)len;
   buf.write(wlen,2);
   buf.write(msg,len);
   conn_->write(buf.buf(),buf.len());
};
