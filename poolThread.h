/*

//Author: Mayur Agarkar
//Last Edited: 4/14/2015

//Thread Pool with independent queues

*/

#include <iostream>
#include <deque>
#include <vector>
#include <stdio.h>
#include <pthread.h>
#include <queue>

using namespace std;

class worker{
public:
	//Constructor used for storing actual data of worker object
	worker(int i){
		this->i = i;
		threadNum = 0;
	}
	//Constructor used for passing thread number while execution
	worker(size_t threadNum){
		this->threadNum = threadNum;
	}
	//Destroy that allocated memory :D
	virtual ~worker(){}
	void virtual workerDoWork(size_t tNum);
	size_t virtual getThreadNum(){ return threadNum; }
private:
	int i;
	size_t threadNum;
};

//structure to create parameters to pass to threads
struct threadParams{
	threadParams(){
		params = NULL;
	}
	void *params;
	size_t tNum;
};



//Threadpool Class with app definitions 
class thread_pool
{
public:
    thread_pool(){ thread_pool(2); }
    thread_pool(int num) : numOfThreads(num) {numOfJobs=0;}
    virtual ~thread_pool() { while(!workerQueue.empty()) workerQueue.pop(); };
    void initialize(pthread_t*);
    void assignNext(worker *worker);
    bool loadNext(worker *&worker);
    static void *threadExecute(void *);
	size_t numOfJobs;
	int empty(size_t workGiven);
	int getOptimalQueue(thread_pool*);
private:
	std::queue<worker*> workerQueue;
	vector<queue<worker*>> *QueueArray;
    int numOfThreads;
};


