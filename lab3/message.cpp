#include "message.hpp"

//print both queues 
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
    reader.queue_id = reader.reader_id % 2 + 1;//przydzielnie do kolejki nie jest losowe, czytelnik działa tylko na jednej kolejce
    while (true) {
        if ((reader.queue_id == 1) && (!buffer1.empty())) {
            //full_q1.wait();

            mutex_stream.lock();//mutex strumienia wyjścia zablokowany, wypisz wiadomość z tyłu kolejki
            std::cout<<"reader "<<reader.reader_id<<" read msg nr: "<< buffer1.back().message_id<<" first buffer"<<std::endl;
            mutex_stream.unlock();
            std::this_thread::sleep_for(std::chrono::seconds(rand() % 10 + 1));//czytanie może trwać do 10 sekund

            //full_q1.signal();
        } else if ((reader.queue_id == 2) && (!buffer2.empty()))  {
            //full_q2.wait();  
 
            mutex_stream.lock();
            std::cout<<"reader "<<reader.reader_id<<" read msg nr: "<< buffer2.back().message_id<<" second buffer"<<std::endl;
             mutex_stream.unlock();
            std::this_thread::sleep_for(std::chrono::seconds(rand() % 10 + 1));

            //full_q2.signal();
        }

        std::this_thread::sleep_for(std::chrono::seconds( SLEEPTIME));
    }   

}



void Synchronization::produce() {
int temp = curr_prod_id++;
while(true) {
    message m;
    m.producer_id = temp;
    m.queue_id = rand() % 2 + 1;//jeden producent będzie produkować do różnych kolejek
    m.message_id = curr_message_id++;

    if(m.queue_id == 1){
        empty_q1.wait();
        mutex_q1.lock();//semafor empty i mutex bufora zablkowane

        buffer1.push_front(m);//dodaj na przód kolejki

        mutex_stream.lock();//mutex strumienia wyjścia zablokowany
        std::cout<<"producent "<<m.producer_id<<" produced msg: "<<m.message_id<<"to queue: " << m.queue_id<<std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(4));
        printBuffers();//wypisz całe dwie kolejki
        mutex_stream.unlock();

        mutex_q1.unlock();
        full_q1.signal();
    } else {
        empty_q2.wait();
        mutex_q2.lock();

        buffer2.push_front(m);

        mutex_stream.lock();
        std::cout<<"producent "<<m.producer_id<<" produced msg: "<<m.message_id<<"to queue: " << m.queue_id<<std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(4));
        printBuffers();
        mutex_stream.unlock();

        mutex_q2.unlock();
        full_q2.signal();
    }

    std::this_thread::sleep_for(std::chrono::seconds(rand() %  SLEEPTIME + 1));//zaśnij na losowy okres

}
}

void Synchronization::consume() {

    while(true) {
        if ((!buffer1.empty()) /*&& (buffer2.size() < S)*/) {
        full_q1.wait();
        mutex_q1.lock();//semafor full i mutex bufora zablokowane, usuń wiadomość z tyłu kolejki

        buffer1.pop_back();

        mutex_q1.unlock();
        empty_q1.signal();

        mutex_stream.lock();//strumień wyjścia zablokowany wypisz informacje i aktualny rozmiar bufora
        std::cout<<"deleted msg from first que. current size "<< buffer1.size()<<std::endl;
        mutex_stream.unlock();
        }  
        if((!buffer2.empty()) /*&& (buffer1.size() < S)*/) {
        full_q2.wait();
        mutex_q2.lock();

        buffer2.pop_back();

        mutex_q2.unlock();
        empty_q2.signal();

        mutex_stream.lock();
        std::cout<<"deleted msg from second que. current size "<< buffer2.size()<<std::endl;
        mutex_stream.unlock();

        }

        std::this_thread::sleep_for(std::chrono::seconds(rand() % SLEEPTIME + 2));//zaśnij na losowy okres

    }

}