#include "packetQueue.h"
#include <iostream>
#include <sstream>

const int nbConsumers = 1;
const int nbToConsume = 24;
const int nbToProduce = nbToConsume * nbConsumers;

void print(std::string x) {
  static std::mutex mutex;
  std::unique_lock<std::mutex> locker(mutex);
  std::cout << x << " \n";
}

void produce(PacketQueue<int>& q) {
  for (int i = 1; i <= nbToProduce; ++i) {
    std::ostringstream tmp;
    tmp << "--> " << i;
    print(tmp.str());
    q.push(i);
  }
}

void consume(PacketQueue<int>& q, unsigned int id) {
  for (int i = 0; i < nbToConsume; ++i) {
    auto item = q.pop();
    std::ostringstream tmp;
    tmp << "        " << item << " --> C" << id;
    print(tmp.str());
  }
}

#include <thread>
int main()
{
  PacketQueue<int> PQ;

  // Start the producer thread.
  std::thread prod1(std::bind(produce, std::ref(PQ)));

  // Start the consumer threads.
  std::vector<std::thread> consumers;
  for (int i = 0 ; i < nbConsumers ; ++i) {
    std::thread consumer(std::bind(&consume, std::ref(PQ), i + 1));
    consumers.push_back(std::move(consumer));
  }

  prod1.join();

  for (auto& consumer : consumers) {
    consumer.join();
  }
}