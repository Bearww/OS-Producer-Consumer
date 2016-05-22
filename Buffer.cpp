#include "Buffer.h"

void Buffer::add(int num) {
	while (true) {
		sem.wait(PRODUCE);
        buffer.push(num);
        sem.notify();
        return;
    }
}

int Buffer::remove() {
    while (true) {
		sem.wait(CONSUME);
        int item = buffer.front();
        buffer.pop();
		sem.notify();
        return item;
    }
}
