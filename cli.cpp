#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <arpa/inet.h>
#include <vector>
#include <cstring>
#define MAX 80
#define PORT 8012
#define SA struct sockaddr
using namespace std;


int myconnection(string instn)
{

	char inst[1024];
	strcpy(inst,instn.c_str());
	cout<<inst;
	int sockfd, connfd;
	struct sockaddr_in servaddr, cli;
	// socket create and verification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{
		cout<<"socket creation failed...\n";
		exit(0);
	}
	else
		cout<<"Socket successfully created..\n";
	bzero(&servaddr, sizeof(servaddr));

	// assign IP address and port number
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); //machine's local ip
	servaddr.sin_port = htons(PORT);

	// connect the client socket to server socket
	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0)
	{
		cout<<"connection with the server failed.\n";
		exit(0);
	}
	else
		cout<<"connected to the server \n";

	send(sockfd,inst,strlen(inst),0);

	// close the socket
	close(sockfd);
	return 0;
}

int main()
{
	int acn,tamt,timestamp;
	string action;
	 	fstream transaction;
	 	fstream logfile;
	    transaction.open("./Transactions.txt");
	    logfile.open("./logfile.txt", ofstream::app);
	    if(transaction.is_open())
	    {
	    	while( transaction >>timestamp>>acn>>action>>tamt)
	    	{
	    	string x = to_string(acn) + " " + action + " " + to_string(tamt) + " ";
	    	logfile <<x<<endl;
	    	myconnection(x);
	    	}
	    }
	    return 0;
}
