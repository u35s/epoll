gcc_flags = --std=c++11  
gcc = /usr/bin/g++ $(gcc_flags)

.PHONY:all base client server clean
objs = buffer.o conn.o agent.o

defalult: all

all: base server client

base: src/base/buffer.cpp src/base/conn.cpp src/base/agent.cpp
	$(gcc) -c $^

client: src/client/main.cpp
	$(gcc) $(objs) $^ -o $@

server: src/server/epoll_server.cpp src/server/main.cpp
	$(gcc) -pthread $(objs)  $^ -o $@

clean:
	rm *.o client server
