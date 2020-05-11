#include "sync.hpp"

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

void Synchronization::read() {
    reader reader;
    reader.reader_id = curr_reader_id++;
    reader.queue_id = reader.reader_id % 2 + 1;//czytelnik pracuje na jednej kolejce
    while (true){
        if ((reader.queue_id == 1) && (!buffer1.empty())) {
            mutex_stream.lock();//zablokuj mutex i wypisz wiadomość
            std::cout<<"reader "<<reader.reader_id<<" read msg nr: "<< buffer1.back().message_id<<" first buffer"<<std::endl;
            std::this_thread::sleep_for(std::chrono::seconds( rand() % SLEEPTIME + 1));
            mutex_stream.unlock();
        }
        else if ((reader.queue_id == 2) && (!buffer2.empty())) {
            mutex_stream.lock();
            std::cout<<"reader "<<reader.reader_id<<" read msg nr: "<< buffer2.back().message_id<<" second buffer"<<std::endl;
            std::this_thread::sleep_for(std::chrono::seconds( rand() % SLEEPTIME + 1));
            mutex_stream.unlock();
        }

        std::this_thread::sleep_for(std::chrono::seconds(rand() % SLEEPTIME + 1));
    }
}

void Synchronization::produce() {
    int temp = curr_prod_id++;
    while(true) {
        message m;
        m.producer_id = temp;
        m.queue_id = rand() % 2 + 1;//wylosuj kolejkę
        m.message_id = curr_message_id++;
        if (m.queue_id == 1) {
            enter_q1();//wejdź do monitora
            if(buffer1.size() == S) {//jeśli pełne to czekaj
                wait(full_q1, m.queue_id);
            }
            buffer1.push_front(m);
            if (buffer1.size() == 1) {//jeśli jeden element w buforze zasygnalizuj że pusty
                signal(empty_q1, m.queue_id);
            }
            mutex_stream.lock();//zablokuj strumień wyjścia i wypisz bufory
            std::cout<<"producent "<<m.producer_id<<" produced msg: "<<m.message_id<<"to queue: " << m.queue_id<<std::endl;
            printBuffers();
            mutex_stream.unlock();

            leave_q1();//wyjdź z monitora

        } else {
            enter_q2();
            if(buffer2.size() == S) {
                wait(full_q2, m.queue_id);
            }
            buffer2.push_front(m);
            if (buffer2.size() == 1) {
                signal(empty_q2, m.queue_id);
            }
            mutex_stream.lock();
            std::cout<<"producent "<<m.producer_id<<" produced msg: "<<m.message_id<<"to queue: " << m.queue_id<<std::endl;
            printBuffers();
            mutex_stream.unlock();

            leave_q2(); 
        }
            std::this_thread::sleep_for(std::chrono::seconds( rand() % SLEEPTIME + 1));
    }

}


void Synchronization ::consume(){


    while (true) {
        enter_q1();//wejdź do monitora
        if(buffer1.empty()) {//jeśli bufor pusty to czekaj
            wait(empty_q1, 1);
        }

        buffer1.pop_back();

        if (buffer1.size() == S - 1) {//jeśli bufor pełny to zasygnalizuj 
            signal(full_q1, 1);
        }
        leave_q1();

        mutex_stream.lock();//zablokuj mutex wyjścia i wypisz
        std::cout<<"deleted msg from first que. current size "<< buffer1.size()<<std::endl;
        mutex_stream.unlock();



        enter_q2();
        if(buffer2.empty()){
            wait(empty_q2, 2);
        }

        buffer2.pop_back();

        if (buffer1.size() == S - 1) {
            signal(full_q2, 2);
        }
        leave_q2();

        mutex_stream.lock();
        std::cout<<"deleted msg from second que. current size "<< buffer2.size()<<std::endl;
        mutex_stream.unlock();
   

               std::this_thread::sleep_for(std::chrono::seconds(rand() % SLEEPTIME + 2));
    }
}