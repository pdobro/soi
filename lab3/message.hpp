#ifndef MESSAGE_H
#define MESSAGE_H


#include<iostream>
#include<deque>
#include<thread>
#include<mutex>
#include<chrono>

#include"sem.hpp"


#define  S 10//queue size

#define SLEEPTIME 6//in seconds

//wiadomość
struct message {
    int producer_id;
    int message_id;
    int queue_id;
};

//czytelnik sparametryzowany na kolejkę
struct reader {
    int reader_id;
    int queue_id;
};

//główna klasa służaca do synchronizacji
class Synchronization {
std::deque<message> buffer1;//double ended queue
std::deque<message> buffer2;

int curr_message_id = 1;
int curr_reader_id = 1;
int curr_prod_id = 1;


std::mutex mutex_q1, mutex_q2, mutex_stream;//mutex buforów i wyjścia
Semaphore empty_q1 = Semaphore(S);//czy bufor pusty
Semaphore empty_q2 = Semaphore(S);
Semaphore full_q1 = Semaphore(0);//czy bufor peły
Semaphore full_q2 = Semaphore(0);


public:

void printBuffers();
//
void read();
void produce();
void consume();
    

};



#endif