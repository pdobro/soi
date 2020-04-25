#include"message.hpp"
#include"sem.hpp"

Lista list;

void runProducent() {
    list.produce();
}

void runReader() {
    list.read();
}

void runConsumer() {
    list.consume();
}


int main(){
srand(time(0));
int i;
std::thread producent[4], reader[5], consumer[2];


for(i = 0; i < 4; i++)
    producent[i] = std::thread(runProducent);
for(i = 0; i < 5; i++)
    reader[i] = std::thread(runReader);
for(i = 0; i < 3; i++)
    consumer[i] = std::thread(runConsumer);

  
for(i = 0; i < 4; i++)  
    producent[i].join();

for(i = 0; i < 4; i++)
    reader[i].join();

for(i = 0; i < 4; i++)
    consumer[i].join();
//list.read();
//list.read();
//list.read();

    return 0;
}