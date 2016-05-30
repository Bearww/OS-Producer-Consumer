#include <mutex>
#include <condition_variable>
#include <queue>

#include "Semaphore.h"

#define BUFFER_SIZE 5

class Buffer {
private:
    std::queue<std::string> buffer;
    const unsigned int size;
	Semaphore sem;
public:
	Buffer() : size(BUFFER_SIZE), sem(BUFFER_SIZE) {}
    void add(std::string &);
    std::string remove();
};
