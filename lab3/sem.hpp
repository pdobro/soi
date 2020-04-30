#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include<semaphore.h>



class Semaphore {
    sem_t semaphore;

    public:

    Semaphore(int beg);
    //~Semaphore();
    void wait();
    void signal();

};

#endif