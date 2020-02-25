CC=g++

compile : server client
server: server.o
	$(CC) -o server serv1.cpp -lpthread
client: client.o
	$(CC) -o client cli.cpp
server.o:serv1.cpp
	$(CC) -c serv1.cpp
client.o: cli.cpp
	$(CC) -c cli.cpp
clean :
	rm server client *.o

