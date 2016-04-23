/*

//Author: Mayur Agarkar
/Last Edited: 4/14/2015

//Thread Pool with independent queues

*/

#include "poolThread.h"
#include <chrono>
#include <time.h>
#include <ctime>
#include "stdlib.h"
#include <iomanip>
#include <fstream>
#include<cstdlib>

using namespace chrono;

pthread_mutex_t workerLock = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t workerQueue_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t workerQueue_cond = PTHREAD_COND_INITIALIZER;
volatile size_t workDone = 0;

std::vector<pthread_mutex_t> *mutualLocker;
std::vector<pthread_cond_t> *mutualCondLocker;


void worker::workerDoWork(size_t tNum){
	pthread_mutex_lock(&mutualLocker->at(tNum));
	workDone++;
	_sleep(0);
	pthread_mutex_unlock(&mutualLocker->at(tNum));
}


//initialize threads
void thread_pool::initialize(pthread_t *threads)
{
	mutualLocker = new std::vector<pthread_mutex_t>(numOfThreads);
	mutualCondLocker = new std::vector<pthread_cond_t>(numOfThreads);
	this->QueueArray = new vector<queue<worker*>>(numOfThreads);
	for(int i = 0; i < numOfThreads; i++)
	{ 
		threadParams *tParams = new threadParams();
		tParams->params = this;
		tParams->tNum = (size_t)i;
		mutualLocker->at(i) = PTHREAD_MUTEX_INITIALIZER;
		mutualCondLocker->at(i) = PTHREAD_COND_INITIALIZER;
		pthread_create(&threads[i], NULL, &thread_pool::threadExecute, (void *)tParams);
	}
}

//returns queue num which is least filled 
int thread_pool::getOptimalQueue(thread_pool* tp){
	int queueNum = 0;
	size_t minSize = tp->QueueArray->at(0).size();
	for(size_t i=1; i<tp->QueueArray->size(); i++){
		if(tp->QueueArray->at(i).size() < minSize){
			minSize = tp->QueueArray->at(i).size();
			queueNum = i;
		}
	}
	return queueNum;
}


//Push job into the queue
void thread_pool::assignNext(worker* worker)
{
	//get free or less loaded queue
	size_t queueNum = getOptimalQueue(this);
	pthread_mutex_lock(&mutualLocker->at(queueNum));
	QueueArray->at(queueNum).push(worker);
	pthread_mutex_unlock(&mutualLocker->at(queueNum));
	pthread_cond_signal(&mutualCondLocker->at(queueNum));
}

//load job from queue and assign to threads
bool thread_pool::loadNext(worker*& worker)
{
	size_t threadNum = worker->getThreadNum();
	pthread_mutex_lock(&mutualLocker->at(threadNum));
	while(QueueArray->at(threadNum).empty())
		pthread_cond_wait(&mutualCondLocker->at(threadNum), &mutualLocker->at(threadNum));
	worker = QueueArray->at(threadNum).front();
	QueueArray->at(threadNum).pop();
	numOfJobs--;
	pthread_mutex_unlock(&mutualLocker->at(threadNum));
	return true;
}

//Actual thread execution with fetched job from hash queue
void *thread_pool::threadExecute(void *param)
{
	threadParams *passedParams = (threadParams *)param;
	thread_pool *p = (thread_pool *)passedParams->params;
	worker *oneworker = new worker(passedParams->tNum);
	while(p->loadNext(oneworker))
	{
		oneworker->workerDoWork(passedParams->tNum);
		delete oneworker;
		oneworker = new worker(passedParams->tNum);
	}
	return NULL;
}
 

int thread_pool::empty(size_t workGiven){
	if(workGiven>workDone){
		return 0;
	}else{
		return 1;
	}
}

//End of ThreadPool



