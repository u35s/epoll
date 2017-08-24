#ifndef BUFFER_H
#define BUFFER_H

class Buffer {
public:
	Buffer(const unsigned int cap);
	~Buffer();

	void write(const unsigned char *w,const unsigned int wlen);
	unsigned int read(unsigned char *r,const unsigned int rlen);
        unsigned char *buf();
	unsigned int len();
	unsigned int cap();
private:
    void  reset();
    void  grow(unsigned int n);
    bool  tryGrowByRebuf(unsigned int n);
	unsigned char  *buf_; // contents are the bytes buf[off_ : len_]
	unsigned int   off_;  // read at buf[off_], write at buf[len_]
	unsigned int   len_;
	unsigned int   cap_;
};
#endif
