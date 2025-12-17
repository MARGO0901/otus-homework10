#include "worker.h"

#include <ctime>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <chrono>
#include <mutex>
#include <ostream>
#include <string>
#include <thread>
#include <utility>

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

std::string GlobalWorker::currentTime{""};

void GlobalWorker::start() {
    if (!running) {
        running = true;

        std::time_t t = std::time(nullptr);
        std::tm tm = *std::localtime(&t);
        std::stringstream ss;
        ss << std::put_time(&tm, "%Y-%m-%d_%H:%M:%S");
        currentTime = ss.str();

        consoleFuture = std::async(std::launch::async, []() {
            while(running) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));

                std::lock_guard<std::mutex> lock(consoleMutex);       
                while (!consoleQueue.empty()) {
                    auto msg = std::move(consoleQueue.front());
                    consoleQueue.pop();
                    std::cout << msg.data << std::endl;
                }
            }
        });

        file1Future = std::async(std::launch::async, [](){fileWorker(1);});

        file2Future = std::async(std::launch::async, []() {fileWorker(2);});
    }
}

void GlobalWorker::fileWorker(int workerId) {

    std::string filename = currentTime + "_" + std::to_string(workerId) + ".log";
    std::ofstream file(filename, std::ios::app);

    while(running) {
        std::queue<Message> messages;
        bool hasMessage = false;

        {
            std::lock_guard<std::mutex> lock(GlobalWorker::fileMutex);
            messages.swap(fileQueue);
        }
                               
        if (!messages.empty() > 0 && file.is_open()) {
            while (!messages.empty()) {
                auto msg = std::move(messages.front());
                messages.pop();
                file << msg.data << std::endl;
            }
            file.flush();
            
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));     
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