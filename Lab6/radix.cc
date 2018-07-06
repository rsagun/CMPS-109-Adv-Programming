#include "radix.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <functional>
#include <thread>
#include <future>
#include <limits>
#include <pthread.h>

/*
 * https://www.geeksforgeeks.org/bucket-sort-2/
 * Used part of their bucket sorting algorithm as a basis
 */

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

void ParallelRadixSort::msd(std::vector<std::reference_wrapper<std::vector<unsigned int>>> &lists, unsigned int cores) { 
    for(std::vector<unsigned int> &list : lists)
    {
    	bucketSort(std::ref(list), list.size(), cores);
    }
}
