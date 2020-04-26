#include"message.hpp"


#define N 4//readers
#define M 5//procucents
#define P 2//consumers


Synchronization example;

void runReader() {
    example.read();
}

void runProducent() {
    example.produce();
}

void runConsumer() {
    example.consume();
}


int main(){


srand(time(0));
int i;

std::thread producents_array[M], readers_array[N], consumers_array[P];

for(i = 0; i < N; i++)
    readers_array[i] = std::thread(runReader);
for(i = 0; i < M; i++)
    producents_array[i] = std::thread(runProducent);
for(i = 0; i < P; i++)
    consumers_array[i] = std::thread(runConsumer);


for(i = 0; i < N; i++)  
    readers_array[i].join();

for(i = 0; i < M; i++)
    producents_array[i].join();

for(i = 0; i < P; i++)
    consumers_array[i].join();




    return 0;
}