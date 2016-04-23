/*

//Author: Mayur Agarkar
//Last Edited: 4/14/2015

//Thread Pool with independent queues

*/
#include "poolThread.h"
#include <iostream>
#include <deque>
#include <vector>
#include <stdio.h>
#include <pthread.h>
#include <queue>
#include <chrono>
#include <time.h>
#include <ctime>
#include "stdlib.h"
#include <iomanip>
#include <fstream>
#include<cstdlib>

using namespace chrono;

//MAIN STARTS HERE 
int main(){
  thread_pool *tp = new thread_pool(3);
  pthread_t threads[3];
  tp->initialize(threads);
  high_resolution_clock::time_point t1 = high_resolution_clock::now();
  int i=0;
  for(i=0;i<10000;i++){
    worker *w = new worker(i);
  	tp->assignNext(w);
  }
  while(tp->empty(i)!=1);
  high_resolution_clock::time_point t2 = high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
  cout << "\n Overall Program Time: " << duration << " microseconds";
  
  
  std::getchar();
  return 0;
}
