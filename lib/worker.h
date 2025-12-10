#pragma once

#include <future>
#include <mutex>
#include <queue>

namespace async {

class GlobalWorker {

public:
    static void start(); 
    static void stop();

    static void logToConsole(int handlerId, const std::string& data);
    static void logToFile(int handlerId, const std::string& data);

private:
    static void fileWorker(int workerId);

    struct Message {
        int handlerId;
        std::string data;
    };
    static std::atomic<bool> running;
    static std::future<void> consoleFuture;
    static std::future<void> file1Future;
    static std::future<void> file2Future;

    static std::mutex consoleMutex;
    static std::mutex fileMutex;
    static std::mutex fileWriteMutex;

    static std::queue<Message> consoleQueue;
    static std::queue<Message> fileQueue;
};
}