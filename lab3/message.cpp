#include "message.hpp"
void Synchronization::printBuffers(){

    for(auto it = buffer1.begin(); it != buffer1.end(); it++){
        std::cout<<"|"<< it->message_id;
    }
    std::cout<<std::endl;
        for(auto it = buffer2.begin(); it != buffer2.end(); it++){
        std::cout<<"|"<< it->message_id;
    }
    std::cout<<std::endl;

}
void Synchronization::read () {

    reader reader;
    reader.reader_id = curr_reader_id++;
    reader.queue_id = reader.reader_id % 2 + 1;//
    while (true) {
        if ((reader.queue_id == 1) && (!buffer1.empty())) {
            full_q1.wait();
            mutex_q1.lock();
            mutex_stream.lock();
            std::cout<<"reader "<<reader.reader_id<<" read msg nr: "<< buffer1.back().message_id<<" first buffer"<<std::endl;
            printBuffers();
            mutex_stream.unlock();
            std::this_thread::sleep_for(std::chrono::seconds(4));
            mutex_q1.unlock();
            full_q1.signal();
        } else if ((reader.queue_id == 2) && (!buffer2.empty()))  {
            full_q2.wait();  
            mutex_q2.lock();
            mutex_stream.lock();
            std::cout<<"reader "<<reader.reader_id<<" read msg nr: "<< buffer2.back().message_id<<" second buffer"<<std::endl;
                  printBuffers();
   mutex_stream.unlock();
            std::this_thread::sleep_for(std::chrono::seconds(4));
            mutex_q2.unlock();
            full_q2.signal();
        }
        

        std::this_thread::sleep_for(std::chrono::seconds(2 * SLEEPTIME));
    }   

}



void Synchronization::produce() {
int temp = curr_prod_id++;
while(true) {

    message m;
    m.producer_id = temp;
    m.queue_id = rand() % 2 + 1;
    m.message_id = curr_message_id++;
    if(m.queue_id == 1){
        empty_q1.wait();
        mutex_q1.lock();
        buffer1.push_front(m);
        mutex_stream.lock();
        std::cout<<"producent "<<m.producer_id<<" produced msg: "<<m.message_id<<"to queue: " << m.queue_id<<std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(4));

        mutex_stream.unlock();
        mutex_q1.unlock();
        full_q1.signal();


    } else {
        empty_q2.wait();
        mutex_q2.lock();
        buffer2.push_front(m);
            std::this_thread::sleep_for(std::chrono::seconds(4));

        mutex_stream.lock();
        std::cout<<"producent "<<m.producer_id<<" produced msg: "<<m.message_id<<"to queue: " << m.queue_id<<std::endl;
        mutex_stream.unlock();
        mutex_q2.unlock();
        full_q2.signal();

    }

    std::this_thread::sleep_for(std::chrono::seconds(2* SLEEPTIME));

}
}

void Synchronization::consume() {

    while(true) {
        if ((!buffer1.empty()) && (buffer2.size() < S)) {
        full_q1.wait();
        mutex_q1.lock();
        buffer1.pop_back();
        mutex_q1.unlock();
        empty_q1.signal();

        mutex_stream.lock();
        std::cout<<"deleted msg from first que. current size "<< buffer1.size()<<std::endl;
  
        mutex_stream.unlock();
        }  
        if((!buffer2.empty()) && (buffer1.size() < S)) {
        full_q2.wait();
        mutex_q2.lock();
        buffer2.pop_back();
        mutex_q2.unlock();
        empty_q2.signal();

        mutex_stream.lock();
        std::cout<<"deleted msg from second que. current size "<< buffer1.size()<<std::endl;
  
        mutex_stream.unlock();
                    std::this_thread::sleep_for(std::chrono::seconds(4));

        }

        std::this_thread::sleep_for(std::chrono::seconds(3 * SLEEPTIME));

    }

}