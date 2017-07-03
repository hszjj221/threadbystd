#include <iostream>
#include <functional>
#include <chrono>
#include <thread>
#include <mutex>
#include <future>
#include <condition_variable>
#include <atomic>


std::atomic<bool> ready(false);
std::atomic_flag winner = ATOMIC_FLAG_INIT;

void test(int id) {
	while (!ready) {
		std::this_thread::yield();
	}
	for (int i = 0; i < 1000000; ++i){}
	if (!winner.test_and_set()) {
		std::cout << id << " won!" << std::endl;
	}
}

int main()
{
	std::vector<std::thread> threads;
	for (int i = 0; i < 10; ++i) {
		threads.push_back(std::thread(test, i));
	}
	ready = true;
	for (auto& th : threads) {
		th.join();
	}
	system("pause");
	return 0;
}