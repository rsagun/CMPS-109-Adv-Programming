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

#include "radix.h"

static std::vector<std::thread> activeThreads;

static int partition(std::vector<unsigned int> &list, int low, int high)
{
	int j;
	unsigned int temp;
	unsigned int pivot = list[high];
	std::string pivotStr = std::to_string(pivot);

	int i = (low - 1);

	for(j = low; j <= high-1; j++)
	{
		std::string listEle = std::to_string(list[j]);
		if(listEle.compare(pivotStr) <= 0)
		{
			i++;
			temp = list[j];
			list[j] = list[i];
			list[i] = temp;
		}
	}
	temp = list[high];
	list[high] = list[i + 1];
	list[i + 1] = temp;
	return i+1;
}

static void quickSort(std::vector<unsigned int> &list, int low, int high)
{
	if(low < high)
	{
		int partIndex = partition(list, low, high);

		quickSort(list, low, partIndex - 1);
		quickSort(list, partIndex + 1, high);
	}
}

static void bucketSort(std::vector<unsigned int> &list, unsigned int n, unsigned int max_cores)
{
	std::vector<std::vector<unsigned int>> buckets;
	for(int i = 0; i < 10; i++)
	{
		std::vector<unsigned int> bucket;
		buckets.push_back(bucket);
	}

	for(unsigned int i = 0; i < n; i++)
	{
		std::string number = std::to_string(list[i]);
		int bucketNumber = number[0] - 48;
		buckets[bucketNumber].push_back(list[i]);
	}

	// Sorting Into Sublists
	unsigned int threadCount = 0;
	for(unsigned int i = 0; i < 10; i++)
	{
		if(activeThreads.size() < max_cores)
		{
			activeThreads.push_back(std::thread(quickSort, std::ref(buckets[i]), 0, buckets[i].size()-1));
			threadCount += 1;
		}
		if(activeThreads.size() == max_cores || threadCount == buckets.size())
		{
			for(auto &thread : activeThreads)
			{
				thread.join();
			}
			activeThreads.erase(activeThreads.begin(), activeThreads.end());
		}
	}

	// This is where you merge the sublists back to one.
	int index = 0;
	for(unsigned int i = 0; i < 10; i++)
	{
		for(unsigned int j = 0; j < buckets[i].size(); j++)
		{
			list[index++] = buckets[i][j];
		}
	}
}

void ParallelRadixSort::msd(std::vector<std::reference_wrapper<std::vector<unsigned int>>> &lists, const unsigned int cores) { 
    for(std::vector<unsigned int> &list : lists)
    {
    	bucketSort(std::ref(list), list.size(), cores);
    }
}

RadixServer::RadixServer(const int port, const unsigned int cores) {
	 /*
     *	This server will receive unsigned ints from the client server. After receiving all of the
     *	numbers for the given list (terminated by a 0), this server will sort the list.
     *
     *	After the server is done sorting the list. This server will send out one unsigned int
     *	at a time back to the client.
     */
	//std::cout << "Socket" << std::endl;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)
    {
    	exit(-1);
    }
    struct sockaddr_in server_addr;
    bzero((char *) &server_addr, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);	// Can be network small (converts byte ordering) CANNOT CHEAT THIS ONE

    //std::cout << "Binding" << std::endl;
    if(bind(sockfd, (struct sockaddr *)
    	&server_addr, sizeof(server_addr)) < 0)
    {
    	exit(-1);
    }

	//std::cout << "Listening" << std::endl;
    listen(sockfd, 1);	// 1 Clients

    std::vector<unsigned int> listToSort;
    // Read what the client sends to us (unsigned ints)
	struct sockaddr_in client_addr;
	socklen_t len = sizeof(client_addr);

	//std::cout << "Accepting" << std::endl;
	// ??? CODE IS BREAKING HERE ???
	int newsockfd = accept(sockfd, (struct sockaddr *) &client_addr, &len);
	//std::cout << "newsockfd: " << newsockfd << std::endl;
	if(newsockfd < 0)
	{
		exit(-1);
	}
	//std::cout << "Connection Successful" << std::endl;

    // Makes the server run infinitely receiving what ever the client sends it.
    for(;;)
    {
    	// Receive the unsigned int from the client
    	unsigned int on_wireR = 0;
    	int n = recv(newsockfd, (void*)&on_wireR, sizeof(unsigned int), 0);	// Recieve to socket returned by accept

    	if(n < 0)
    	{
    		exit(-1);
    	}
    	else if(n == 0)
    	{
    		break;
    	}

    	unsigned int localR = ntohl(on_wireR);	// The number received from client. Store in array to sort?
    	//std::cout << "Local Receive: " << n << std::endl;
    	if(localR == 0)
    	{

    		bucketSort(std::ref(listToSort), listToSort.size(), cores);
    		for(unsigned int i = 0; i < listToSort.size(); i++)
    		{
    			// std::cout << "NUMBER: " << listToSort[i] << std::endl;
    			unsigned int on_wireS = htonl(listToSort[i]);
    			int n = send(newsockfd, (void*)&on_wireS, sizeof(unsigned int), 0);
    			if(n < 0)
    			{
    				exit(-1);
    			}
    		}
    		unsigned int zero = 0;
    		unsigned int on_wireZ = htonl(zero);
    		n = send(newsockfd, (void*)&on_wireZ, sizeof(unsigned int), 0);
			if(n < 0)
			{
				exit(-1);
			}
			listToSort.clear();
			// break;
    	}
    	else
    	{
    		//unsigned int localR = ntohl(on_wireR);	// The number received from client. Store in array to sort?
    		listToSort.push_back(localR);
    	}
    }
    close(sockfd);
}

void RadixClient::msd(const char *hostname, const int port, std::vector<std::reference_wrapper<std::vector<unsigned int>>> &lists) { 
    /*
     *	This client will take each number in a list and send them one by one to the server to be sorted.
     *	After a full list is sent a '0' is sent to signal the server that the list is finished and it
     *	can start sorting the list.
     *
     *	After the list is sorted the server will send back the list one unsigned int at at time.
     *	This client will receive the unsigned int and overwrite the reference of the list with the
     * 	respective unsigned int. 
     */

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
	{
		exit(-1);
	}

	struct hostent *server = gethostbyname(hostname);	// Converts 
	if(server == NULL)
	{
		exit(-1);
	}

	struct sockaddr_in serv_addr;
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);

	serv_addr.sin_port = htons(port);	// Network Byte Number

	// Block til find connection
	if(connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
	{
		exit(-1);
	}

	for(std::vector<unsigned int> &list : lists)
	{
		// Run through the list and for each unsigned int send it to the server
		for(unsigned int i = 0; i < list.size(); i++)
		{
			unsigned int localS = list[i];
			//std::cout << "Local Send: " << localS << std::endl;
			unsigned int on_wireS = htonl(localS);
			int n = send(sockfd, (void*)&on_wireS, sizeof(unsigned int), 0);
			if(n < 0)
			{
				exit(-1);
			}
		}

		// Send 0 to server to start terminate and sorting of the list.
		std::vector<unsigned int> sortedList;
		unsigned int zero = 0;
		unsigned int on_wireZ = htonl(zero);
		int n = send(sockfd, (void*)&on_wireZ, sizeof(unsigned int), 0);
		if(n < 0)
		{
			exit(-1);
		}

		// Read the response from the server terminated by a zero.
		// Should receive one unsigned int every time it is read.
		for(unsigned int i = 0; i < list.size()+1; i++)
		{
			unsigned int on_wireR = 0;
			//unsigned int i = 0;
			n = recv(sockfd, (void*)&on_wireR, sizeof(unsigned int), 0);
			// std::cout << "Local Receive: " << on_wireR << std::endl;
			// std::cout << "n: " << n << std::endl;
			if(n < 0)
			{
				exit(-1);
			}
			unsigned int localR = ntohl(on_wireR);
			//std::cout << "Local Receive: " << localR << std::endl;

			if(localR == 0)
			{
				for(unsigned int i = 0; i < sortedList.size(); i++)
				{
					// std::cout << "NUMBER: " << sortedList[i] << std::endl;
					list[i] = sortedList[i];
				}
				break;
			}
			else
			{
				sortedList.push_back(localR);
			}
		}
	}
	close(sockfd);
}