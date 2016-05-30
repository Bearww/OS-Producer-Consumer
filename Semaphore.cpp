#include "Semaphore.h"

void Semaphore::notify()
{
    std::unique_lock<std::mutex> lock(mtx);
    cv.notify_one();
}

void Semaphore::wait(int type)
{
    std::unique_lock<std::mutex> lock(mtx);
    if(type == PRODUCE) {
		cv.wait(lock, [this]() { return count < size; });
		count++;
	}
	if(type == CONSUME) {
		cv.wait(lock, [this]() { return count > 0; });
		count--;
	}
}
