#include "message.hpp"

void Synchronization::read () {

    reader reader;
    reader.reader_id = ++curr_reader_id;
    reader.queue_id = rand() % 2 + 1;//popraw
    while (true) {
        if ((reader.queue_id == 1) && (!buffer1.empty())) {
            mutex_q1.lock();
            std::cout<<"reader "<<reader.reader_id<<" read msg nr: "<< buffer1.back().message_id<<std::endl;
            mutex_q1.unlock();
        } else if (!buffer2.empty()) {
            mutex_q2.lock();
            std::cout<<"reader "<<reader.reader_id<<" read msg nr"<< buffer2.back().message_id<<std::endl;
            mutex_q2.unlock();
        }
        

        std::this_thread::sleep_for(std::chrono::milliseconds(rand()% SLEEPTIME));
    }   

}



void Synchronization::produce() {
int temp = ++curr_prod_id;
while(true) {

    message m;
    m.producer_id = temp;
    m.queue_id = rand() % 2 + 1;
    m.message_id = ++curr_message_id;
    if(m.queue_id == 1){
        empty_q1.wait();
        mutex_q1.lock();
         buffer1.push_front(m);
        mutex_q1.unlock();
        full_q1.signal();
        std::cout<<"produced msg: "<<m.message_id<<std::endl;
    } else {
        empty_q2.wait();
        mutex_q2.lock();
        buffer2.push_front(m);
        mutex_q2.unlock();
        full_q2.signal();
        std::cout<<"produced msg: "<<m.message_id<<std::endl;     
    }

    //add here sleep
    std::this_thread::sleep_for(std::chrono::milliseconds(rand()% 2* SLEEPTIME));

}
}

void Synchronization::consume() {

    while(true) {
        full_q1.wait();
        mutex_q1.lock();
        buffer1.pop_back();
        mutex_q1.unlock();
        empty_q1.signal();
        std::cout<<"deleted msg from first que. current size "<< buffer1.size()<<std::endl;

        full_q2.wait();
        mutex_q2.lock();
        buffer2.pop_back();
        mutex_q2.unlock();
        empty_q2.signal();
        std::cout<<"deleted msg from second que. current size "<< buffer2.size()<<std::endl;


        std::this_thread::sleep_for(std::chrono::milliseconds(rand()% 3 * SLEEPTIME));

    }

    ///sleep
}