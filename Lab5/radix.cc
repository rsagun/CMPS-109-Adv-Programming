/*
 * Copyright (C) 2018 David C. Harrison. All right reserved.
 *
 * You may not use, distribute, publish, or modify this code without 
 * the express written permission of the copyright holder.
 */

/* https://www.geeksforgeeks.org/quick-sort/
 * Implemented the psuedo for quickSort to sort elements into lexographic order.
 */

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
#include <pthread.h>

static unsigned int threadMax;

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

RadixSort::RadixSort(const unsigned int cores) 
{ 
	std::cout << "Cores: " << cores << std::endl;
	threadMax = cores;
}

/*
 * https://canvas.ucsc.edu/courses/12881/discussion_topics/41114
 * Psuedo for limiting cores from David Harrison.
 */
void RadixSort::msd(std::vector<std::reference_wrapper<std::vector<unsigned int>>> &lists) 
{ 
    std::vector<std::thread> activeThreads;
    unsigned int threadCount = 0;
    for(std::vector<unsigned int> &list : lists)
	{
		if (activeThreads.size() < threadMax)
		{
			activeThreads.push_back(std::thread(quickSort, std::ref(list), 0, list.size()-1));
	    	threadCount += 1;
		}
	    if (activeThreads.size() == threadMax || threadCount == lists.size())
	    {
	        for (auto &thread : activeThreads)
	        {
	        	thread.join();
	        }
	        activeThreads.erase(activeThreads.begin(), activeThreads.end());
	    }	
	}
}