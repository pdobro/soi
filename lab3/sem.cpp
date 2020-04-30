#include"sem.hpp"

Semaphore::Semaphore(int beg) {
    if (sem_init(&semaphore, 0 , beg))
        throw"init fail";
}

//Semaphore::~Semaphore() {sem_destroy(&semaphore);}

 void Semaphore::wait(){
    if (sem_wait(&semaphore))
        throw "wait fail";
}

void Semaphore::signal() {
    if(sem_post(&semaphore))
        throw "signal fail";
}