#include "worker.h"

#include <iostream>
#include <fstream>
#include <chrono>
#include <mutex>
#include <thread>

namespace async {

// Определение статических членов
std::queue<GlobalWorker::Message> GlobalWorker::consoleQueue;
std::queue<GlobalWorker::Message> GlobalWorker::fileQueue;

std::mutex GlobalWorker::consoleMutex;
std::mutex GlobalWorker::fileMutex;

std::atomic<bool> GlobalWorker::running{false};
std::future<void> GlobalWorker::consoleFuture;
std::future<void> GlobalWorker::file1Future;
std::future<void> GlobalWorker::file2Future;


void GlobalWorker::start() {
    if (!running) {
        running = true;

        consoleFuture = std::async(std::launch::async, []() {
            while(running) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));

                std::lock_guard<std::mutex> lock(consoleMutex);       
                while (!consoleQueue.empty()) {
                    auto msg = std::move(consoleQueue.front());
                    consoleQueue.pop();
                    std::cout << "[" << msg.handlerId << "] " << msg.data << std::endl;
                }
            }
        });

        file1Future = std::async(std::launch::async, [](){fileWorker(1);});

        file2Future = std::async(std::launch::async, []() {fileWorker(2);});
    }
}

void GlobalWorker::fileWorker(int workerId) {
    while(running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        
        // Берем и пишем под одним мьютексом
        {
            std::lock_guard<std::mutex> lock(GlobalWorker::fileMutex);
            if (!fileQueue.empty()) {
                auto msg = std::move(GlobalWorker::fileQueue.front());
                fileQueue.pop();
                
                std::ofstream file("bulk.log", std::ios::app);
                if (file.is_open()) {
                    file << "[thread " << workerId << ": " << msg.handlerId << "] " << msg.data << std::endl;
                }
            }
        }
    }
}

void GlobalWorker::stop() {
    running = false;
    if (consoleFuture.valid()) consoleFuture.wait();
    if (file1Future.valid()) file1Future.wait();
    if (file2Future.valid()) file2Future.wait();
}

void GlobalWorker::logToConsole(int handlerId, const std::string& data) {
    std::lock_guard<std::mutex> lock(consoleMutex);
    consoleQueue.push({handlerId, data});
}

void GlobalWorker::logToFile(int handlerId, const std::string& data) {
    std::lock_guard<std::mutex> lock(fileMutex);
    fileQueue.push({handlerId, data});
}


}