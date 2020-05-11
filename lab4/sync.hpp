
#include<iostream>
#include<deque>
#include<thread>
#include<mutex>
#include<chrono>

#include"monitor.h"

#define  S 5//queue size

#define SLEEPTIME 7//in seconds


struct message {
    int producer_id;
    int message_id;
    int queue_id;
};

struct reader {
    int reader_id;
    int queue_id;
};

//klasa Monitor w pliku monitor.h
class Synchronization : Monitor{

std::deque<message> buffer1;
std::deque<message> buffer2;

Condition full_q1, full_q2, empty_q1, empty_q2;
std::mutex mutex_stream;

int curr_message_id = 1;
int curr_reader_id = 1;
int curr_prod_id = 1;

public:
void produce();
void consume();
void read();
void printBuffers();

};