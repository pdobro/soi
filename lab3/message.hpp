#ifndef MESSAGE_H
#define MESSAGE_H


#include<iostream>
#include<deque>
#include<thread>
#include<mutex>
#include<chrono>

#include"sem.hpp"


#define  S 20//queue size

#define SLEEPTIME 2000//in milliseconds

struct message {
    int producer_id;
    int message_id;
    int queue_id;
};

struct reader {
    int reader_id;
    int queue_id;
};


class Synchronization {
std::deque<message> buffer1;
std::deque<message> buffer2;

int curr_message_id = 0;
int curr_reader_id = 0;
int curr_prod_id = 0;


std::mutex mutex_q1, mutex_q2;
Semaphore empty_q1 = Semaphore(S);
Semaphore empty_q2 = Semaphore(S);
Semaphore full_q1 = Semaphore(0);
Semaphore full_q2 = Semaphore(0);
public:

void read();
void produce();
void consume();
    

};



#endif