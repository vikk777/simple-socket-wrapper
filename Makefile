FLAGS = -pthread

all: server client

server: server.o sockets.o
	g++ -o $@ $^ $(FLAGS)

server.o: server.cpp
	g++ -c $< 

client: client.o sockets.o
	g++ -o $@ $^ $(FLAGS)

client.o: client.cpp
	g++ -c $< 

sockets.o: sockets.cpp sockets.h
	g++ -c $< 

clean:
	rm *.o