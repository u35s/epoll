#ifndef AGENT_H
#define AGENT_H

#include "./buffer.h"
#include "./conn.h"
#include "boost/function.hpp"
class Agent;
typedef boost::function<void(Agent*,Buffer*,unsigned int)> message_callback_t;

class Agent{
public:
    Agent(Conn *conn);
    ~Agent();
    void reciveBufWrite(unsigned char *msg,unsigned int len); 
    void sendMessage(unsigned char *msg,unsigned int len);
    void setMessageCallBack(message_callback_t);
    Conn   *conn_;
private:
    void defaultMessageCallBack(Agent* agent,Buffer *buf,unsigned int len);
    Buffer recive_buf_;
    bool head_len_read_;
    unsigned int head_len_;
    message_callback_t messageCallBack_;
};
#endif
