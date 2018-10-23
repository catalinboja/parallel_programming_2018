#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

static int counter;
static const int num_threads = 10;

class Counter {
private:
	int counter = 0;
	std::mutex mutex;
public:
	void increment() {
		mutex.lock();
		counter += 1;
		mutex.unlock();
	}
};

void incrementCounter() {
	counter += 1;
}

void thread_function() {
	std::cout << "Hello World from another thread";
}

// long operation to time
int fib(int n) {
	if (n < 2) {
		return n;
	}
	else {
		return fib(n - 1) + fib(n - 2);
	}
}


int main() {
	//start a thread
	std::thread t1(thread_function);

	//join the thread with the main thread
	t1.join();


	auto start_time = std::chrono::high_resolution_clock::now();

	fib(30);

	auto end_time = std::chrono::high_resolution_clock::now();
	auto time = end_time - start_time;

	std::cout << "fib(30) took " <<
		std::chrono::duration_cast<std::chrono::microseconds>(time).count() << " ms to run.\n";


	return 0;
}