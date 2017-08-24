#include "./buffer.h"
#include <iostream>
#include <string.h>

Buffer::Buffer(const unsigned int size){
    buf_ = new unsigned char[size];
    off_ = 0;
    len_ = 0;
    cap_ = size;
};


Buffer::~Buffer(){
    delete []buf_;
};

unsigned int Buffer::len(){return len_ - off_;};
unsigned int Buffer::cap(){return cap_;};
unsigned char *Buffer::buf(){return buf_+off_; }

void Buffer::write(const unsigned char *w,const unsigned int wlen){
    if (!tryGrowByRebuf(wlen)){
        grow(wlen);
    };
    memcpy(buf_+len_-wlen,w,wlen);
};

unsigned int Buffer::read(unsigned char *r,unsigned int rlen){
	if(off_+rlen>len_){
		rlen=len(); 
	};
   	memcpy(r,buf_+off_,rlen);
	off_ += rlen;
	return rlen;
};

void Buffer::reset(){
	off_ = 0;
	len_ = 0;
}

void Buffer::grow(unsigned int n){
	 unsigned int m = len();
     if (m == 0 && off_ != 0){
	     reset();
	 };
	 if(tryGrowByRebuf(n)){
	 	return;
	 };
	 if (m+n <= cap_ / 2){
		memcpy(buf_,buf_+off_,m); 
	 }else{
		cap_ = 2 * cap_ + n;
		unsigned char *temp = new unsigned char[cap_]; 
		memcpy(temp,buf_+off_,m);
		delete []buf_;
		buf_ = temp;
	 };
	 off_ = 0;
	 len_ = m + n;
}

bool Buffer::tryGrowByRebuf(unsigned int n){
	if(len_+n<=cap_){
        len_ += n;	
		return true;
	};
	return false;
};
