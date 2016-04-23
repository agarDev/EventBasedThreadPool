# ThreadPool
A Threadpool generated with POSIX threads for C++.
A sample code is given in Source.cpp file which demonstrates the working of this library.


Usage:

Step 0:
Import necessary files
`#include "poolThread.h"`
`#include <pthread.h>`

Step 1: 
Create ThreadPool using the following sample code
```c++
thread_pool *threadPool = new thread_pool(3);
```

Step 2:
Create pThreads and Assign to the Thread Pool created in Step 1
```c++
pthread_t threads[3];
threadPool->initialize(threads);
```

Step 3:
Assign work for Threads to do in Following function in PoolThread.cpp
```c++
void worker::workerDoWork(size_t tNum){
	pthread_mutex_lock(&mutualLocker->at(tNum));
	//Add your code below this line
	pthread_mutex_unlock(&mutualLocker->at(tNum));
}
```
