#include <mutex>
#include <condition_variable>

#define PRODUCE 0
#define CONSUME 1

class Semaphore {
private:
    std::mutex mtx;
    std::condition_variable cv;
    int count, size;
public:
    Semaphore(int _size = 1) : count(0), size(_size) {}
    void notify();
    void wait(int);
};
