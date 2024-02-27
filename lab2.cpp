#include <iostream>
#include <vector>
#include <string>
#include <mutex>
#include <future>
#include <chrono>
const int NUM_THREADS = 10;
std::vector<std::string> colors;
std::mutex mtx;
void assignColorSynchronous(int threadIndex) 
{
    std::vector<std::string> availableColors = {"Red", "Green", "Blue", "Yellow", "Orange", "Purple"};
    std::string selectedColor = availableColors[threadIndex % availableColors.size()];
    {
        std::lock_guard<std::mutex> lock(mtx);
        colors[threadIndex] = selectedColor;
        std::cout << "Синхронный: Поток " << threadIndex << " записал цвет: ";
        for(const auto& color : colors) {
            std::cout << color << " ";
        }
        std::cout << std::endl;
    }
}
void assignColorAsynchronous(int threadIndex) 
{
    std::vector<std::string> availableColors = {"Red", "Green", "Blue", "Yellow", "Orange", "Purple"};
    std::string selectedColor = availableColors[threadIndex % availableColors.size()];
    {
        std::lock_guard<std::mutex> lock(mtx);
        colors[threadIndex] = selectedColor;
        std::cout << "Асинхронный: Поток " << threadIndex << " записал цвет: ";
        for(const auto& color : colors) 
        {
            std::cout << color << " ";
        }
        std::cout << std::endl;
    }
}
int main() 
{
    colors.resize(NUM_THREADS);
    auto startTimeSync = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < NUM_THREADS; ++i) 
    {
        assignColorSynchronous(i);
    }
    auto endTimeSync = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> timeSync = endTimeSync - startTimeSync;
    std::cout << "Время синхронной записи: " << timeSync.count() << " секунд" << std::endl;
    auto startTimeAsync = std::chrono::high_resolution_clock::now();
    std::vector<std::future<void>> futures;
    for (int i = 0; i < NUM_THREADS; ++i) 
    {
        futures.emplace_back(std::async(std::launch::async, assignColorAsynchronous, i));
    }
    for (auto& future : futures) {
        future.wait();
    }
    auto endTimeAsync = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> timeAsync = endTimeAsync - startTimeAsync;
    std::cout << "Время асинхронной записи: " << timeAsync.count() << " секунд" << std::endl;
    return 0;
}
