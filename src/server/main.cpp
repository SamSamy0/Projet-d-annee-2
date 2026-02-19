#include <iostream>
#include <thread>
#include <chrono>
#include <memory>
#include "messages.hpp"
#include "messagequeue.hpp"
#include "servernetwork.hpp"
#include "worker.hpp"

int main() {
    MessageQueue m_queue;
    ReponseQueue r_queue;

    ServerNetworkManager servManager(m_queue,r_queue);
    Worker worker(m_queue, r_queue);

    servManager.run();
    std::thread workerThread(&Worker::run, &worker);
}
