#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <fstream>
#include <vector>
#include <mutex>

#define MAX 999999
#define PORT 8012
#define SA struct sockaddr


int x =0;
std::mutex mx;
using namespace std;

class user
{
public:
	int acn, bal;
	string name;
};

user u[20];

void func(int sockfd)
{
	char buff[MAX];
	int n;
	while (1)
	{
		bzero(buff, MAX);

		// read the message from client and copy it in buffer
		read(sockfd, buff, sizeof(buff));
		// print buffer which contains the client contents
		printf("From client: %s\t To client : ", buff);
		bzero(buff, MAX);
		n = 0;
		// copy server message in the buffer
		while ((buff[n++] = getchar()) != '\n');

		// and send that buffer to client
		write(sockfd, buff, sizeof(buff));
	}
}


class actions
{
public:
	int acn,amt;
	string type;

};

int stringline(int x)
{

	int q = x;
	return q;
}
void*call (void * param)
{
	int r = stringline(x);
	sleep(0.5);
	int val_read, new_socket;
	actions b;
	char buffer[1024]={0};
	pthread_detach(pthread_self());
	new_socket = (long) param;
	vector <string> a;
	int i;
	cout<<"new_socket"<<endl;
	const char *hello = "Connection established";
	while(val_read = read( new_socket , buffer, 1024) > 0)
	{
		printf("%s\n",buffer );
		send(new_socket , hello , strlen(hello) , 0 );
	}
	string s;
	s = string(buffer);
	std::string delimiter = " ";
	size_t pos = 0;
	std::string token;
	while ((pos = s.find(delimiter)) != std::string::npos)
	{
		token = s.substr(0, pos);
		a.push_back(token);
		s.erase(0, pos + delimiter.length());
	}

	b.acn = atoi(a.at(0).c_str());
	b.type = a.at(1);
	b.amt = atoi(a.at(2).c_str());
	mx.lock();
	for(int i = 0; i<r; i++)
	{
		if(b.acn == u[i].acn)
		{

			if(b.type=="d" or b.type =="D")
			{
				u[i].bal = u[i].bal+b.amt;
				cout<<"Money deposited in account "<<u[i].acn<<" is: "<<u[i].bal<<endl ;
			}

			if(b.type == "w" or b.type == "W")
			{
				if (b.amt > u[i].bal)
				{
					cout<<"Insufficient balance, please try smaller amount than "<<endl;
				}
				else
				{
					u[i].bal  = u[i].bal - b.amt;
					cout<<"Money withdrawn from account "<<u[i].acn<<" is "<<u[i].bal<<endl;
				}
			}
			//send(new_socket,"Transactions Done",50);
		}
		mx.lock();
	}
}



int records()
{
	// This fuction adds Users for the client and also returns the size of the file i.e. number of transactions
	int a,c;
	string b;
	fstream transaction;
	transaction.open("./Records.txt");
	string line;
	int i = 0;
	if(transaction.is_open())
	{
		while( transaction >>u[i].acn>>u[i].name>>u[i].bal)
		{
			cout<<"User added";
			cout<<u[i].acn<<endl;
			i++;
			x++;

	    }
		cout<<"Data recieved from Client"<<endl;
	}
	stringline(x);
}

void* interest(void* param)
{
	// This function is used to add interest to the client's accounts
	int r = stringline(x);
	while(1)
	{
		mx.lock();
		sleep(5);
	    cout<<"-------------------------------"<<endl;

		for(int y = 0; y<r ; y++)
		{
			u[y].bal = u[y].bal*1.2;
			cout<<"Interest added to account "<<u[y].acn<<" is "<<u[y].bal<<endl;
		}
		mx.unlock();
	}
}


int main()
{
	pthread_t t[99999];
	int count = 0;
	int optval =1;

	records();

	int sockfd, connfd, len;
	struct sockaddr_in servaddr, cli;

	// socket create and verification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	setsockopt((sockfd), SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));
	if (sockfd == -1)
	{
		cout<<"socket creation failed...\n";
		exit(0);
	}
	else
	//printf("Socket successfully created..\n");
	bzero(&servaddr, sizeof(servaddr));

	// assign IP address and port

	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &optval, sizeof(optval)))
	    {
	        perror("setsockopt");
	        exit(EXIT_FAILURE);
	    }
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);

	// Binding newly created socket to given IP and verification
	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0)
	{
		cout<<"socket bind failed...\n";
		exit(0);
	}
	else
		cout<<"Socket successfully binded..\n";

	// Now server is ready to listen and verification
	if ((listen(sockfd, 5)) != 0) {
		cout<<"Listen failed...\n";
		exit(0);
	}
	else
		cout<<"Server listening..\n";
	len = sizeof(cli);

	pthread_t interestrate;
	pthread_create(&interestrate,0,interest,(void*)1);
	while(1)
	{
		connfd = accept(sockfd, (SA*)&cli,(socklen_t*) &len);
		pthread_create(&t[count],0,call,(void*)connfd);
		cout<<"thread created "<<count<<endl;
		count++;
	}
	if (connfd < 0)
	{
		cout<<"server acccept failed...\n";
		exit(0);
	}
	else
		cout<<"server acccept the client...\n";

	close(sockfd);
}
