#include "epoll_server.h"

int main(void){
	EpollServer esrv;
	esrv.listenAccept("127.0.0.1",7777);
	esrv.run();
	return 0;
}
