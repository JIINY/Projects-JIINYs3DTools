#pragma once
#include <random>
#include <cstdint>
#include <mutex>


class IDGenerator
{
public:
    IDGenerator() = delete;

    inline static uint64_t generate()
    {
        static std::mt19937_64 engine{ std::random_device{}() };
        static std::uniform_int_distribution<uint64_t> dist;

        static std::mutex mtx;
        std::lock_guard<std::mutex> lock(mtx);

        return dist(engine);
    }
};
