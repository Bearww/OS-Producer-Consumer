#include "Buffer.h"

void Buffer::add(std::string &item) {
	while (true) {
		sem.wait(PRODUCE);
        buffer.push(item);
        sem.notify();
        return;
    }
}

std::string Buffer::remove() {
    while (true) {
		sem.wait(CONSUME);
        std::string item = buffer.front();
        buffer.pop();
		sem.notify();
        return item;
    }
}
