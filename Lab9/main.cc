/*
 *	server.cc - Testing server to send and receive pws
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <iostream>
#include <math.h>
#include <vector>
#include <functional>
#include <thread>
#include <future>
#include <limits>
#include <pthread.h>
#include <stdint.h>

#include "crack.h"

int main(int argc, char *argv[]) 
{
	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0)
	{
		exit(-1);
	}

	struct sockaddr_in server_addr;
	bzero((char *) &server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(get_multicast_port());

	if(bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
	{
		exit(-1);
	}

	 struct ip_mreq multicastRequest;
	 multicastRequest.imr_multiaddr.s_addr = get_multicast_address();
	 multicastRequest.imr_interface.s_addr = htonl(INADDR_ANY);
	 if (setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void *) &multicastRequest, sizeof(multicastRequest)) < 0)
	 {		
	 	exit(-1);
	 }

	struct sockaddr_in remote_addr;
	socklen_t len = sizeof(remote_addr);

	Message message;
	int n = recvfrom(sockfd, (void *)&message, sizeof(message), 0, (struct sockaddr *)&remote_addr, &len);
	if(n < 0)
	{
		std::cout << "Exception thrown (receive) exiting program" << std::endl;
		exit(-1);
	}
	// std::cout << "Recieved Message" << std::endl;

	close(sockfd);

	int sockst = socket(AF_INET, SOCK_STREAM, 0);
	if (sockst < 0)
	{
		exit(-1);
	}

	struct hostent *server = gethostbyname(message.hostname);
	// std::cout << "hostname: " << message.hostname << std::endl;
	if(server == NULL)
	{
		std::cout << "No server to connect to exiting program." << std::endl;
		exit(-1);
	}

	bzero((char *) &server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, (char *)&server_addr.sin_addr.s_addr, server->h_length);

	server_addr.sin_port = message.port;
	// std::cout << "Port Number: " << message.port << std::endl;
	if(connect(sockst, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
	{
		exit(-1);
	}

	std::vector<std::thread> activeThreads;

	Message cracked;
	strcpy(cracked.cruzid, message.cruzid);
	cracked.num_passwds = message.num_passwds;
	strcpy(cracked.hostname, message.hostname);
	cracked.port = message.port;

	for(unsigned int i = 0; i < ntohl(message.num_passwds); i++)
	{
		// std::cout << "hash: " << message.passwds[i] << std::endl;
		activeThreads.push_back(std::thread(crack, message.passwds[i], cracked.passwds[i]));
	}

	for(auto &thread : activeThreads)
	{
		thread.join();
	}
	activeThreads.erase(activeThreads.begin(), activeThreads.end());

	for(unsigned int j = 0; j < ntohl(message.num_passwds); j++)
	{
		// std::cout << "Cracked PW: " << cracked.passwds[j] << std::endl;
	}

	n = send(sockst, (void*)&cracked, sizeof(Message), 0);
	if(n < 0)
	{
		std::cout << "Sending Failure exiting program." << std::endl;
		exit(-1);
	}
	close(sockst);
}