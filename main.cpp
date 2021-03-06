#include <cstdlib>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <mutex>
#include <chrono>
#include <vector>
#include <iomanip>

#include "Buffer.h"

using namespace std;

/* buffer */
Buffer *buffer;

/* mutex */
mutex mtx;

/* thread */
vector<thread> threads;

/* sequence number */
vector<int> producer_sn;
vector<int> consumer_sn;

/* ID */
vector<int> pids;
vector<int> cids;

void producer(void *);
void consumer(void *);

ofstream fout("output.txt");
ofstream p_out("producer_output.txt");
ofstream c_out("consumer_output.txt");

int main(int argc, char* argv[])
{
	/* 1. Get command line arguments argv[1], argv[2], argv[3] */
/*	if(argc < 4)
		return -1;

	int sleep_time = atoi(argv[1]);
	int producer_num = atoi(argv[2]), consumer_num = atoi(argv[3]);
/*/
	int sleep_time = 100;
	int producer_num = 5, consumer_num = 5;

	/* 2. Initialize buffer */
	buffer = new Buffer();
	for(int id = 0; id <= producer_num; id++) pids.push_back(id);
	for(int id = 0; id <= consumer_num; id++) cids.push_back(id);
	threads.resize(0);
	producer_sn.resize(producer_num + 1);
	consumer_sn.resize(consumer_num + 1);

	/* 3. Create producer thread(s) */
	for(int id = 1; id <= producer_num; id++) {
		threads.push_back(thread(producer, &pids[id]));
		producer_sn.push_back(0);
	}

	/* 4. Create consumer thread(s) */
	for(int id = 1; id <= consumer_num; id++) {
		threads.push_back(thread(consumer, &cids[id]));
		consumer_sn.push_back(0);
	}

	/* synchronizing all threads */
	for(auto& th : threads) th.join();

	/* 5. Sleep */
	this_thread::sleep_for(std::chrono::seconds(sleep_time));

	/* 6. Exit */
	if(fout) fout.close();
	if(p_out) p_out.close();
	if(c_out) c_out.close();

    return 0;
}

void producer(void *param) {
	int producerID = *(int *)param;
    while(true) {
		/* generate a random number */
		int item = rand() % 100;
		int period = rand() % min((int)pow(2, min(producerID, 31)), 99) + 1;
        
		mtx.lock();
		stringstream ss;
		ss << setw(2) << setfill('0') << producerID
			<< setw(3) << setfill('0') << ++producer_sn[producerID]
			<< setw(2) << setfill('0') << period;

		string pack;
		ss >> pack;

		buffer->add(pack);
		
        cout << "ProducerID: " << producerID
			<< ", SequenceNum: " << setw(3) << setfill('0') << producer_sn[producerID]
			<< ", Random: " << setw(3) << period
			<< ", Produce: " << item << endl;
        
		if(fout)
			fout << "producer " << producerID
				<< " produced " << pack << endl;

		if(p_out)
			p_out << "producer " << producerID
				<< " produced " << pack << endl;

        mtx.unlock();
		/* sleep for a random period of time */
		this_thread::sleep_for(std::chrono::seconds(period));
    }
}

void consumer(void *param) {
	int consumerID = *(int *)param;
    while(true) {
		string item = buffer->remove();
		int period = rand() % min((int)pow(2, min(consumerID, 31)), 99) + 1;
        mtx.lock();
        cout << "ConsumerID: " << consumerID
			<< ", SequenceNum: " << setw(3) << setfill('0') << ++consumer_sn[consumerID]
			<< ", Random: " << setw(3) << period
			<< ", Consume: " << item << endl;
        
		if(fout)
			fout << "consumer " << consumerID
				<< " consumed " << item << endl;

		if(c_out)
			c_out << "consumer " << consumerID
				<< " consumed " << item << endl;

		mtx.unlock();

		/* sleep for a random period of time */
		this_thread::sleep_for(std::chrono::seconds(period));
    }
}
