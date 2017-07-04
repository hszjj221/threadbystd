#include <iostream>
#include <thread>
#include <vector>
#include <functional>
#include <string>
#include <mutex>

#include "thread_guard.h"
#include "thread_pool.h"

int main()
{
	std::mutex mtx;
	try {
		tp::Thread_pool tp;
		std::vector<std::future<int>> v;
		std::vector<std::future<void>> v1;
		for (int i = 0; i != 10; ++i) {
			auto ans = tp.Submit([](int answer) {return answer; }, i);
			v.push_back(std::move(ans));
		}
		for (int i = 0; i != 5; ++i) {
			auto ans = tp.Submit([&mtx](const std::string& str1, const std::string& str2) {
				std::lock_guard<std::mutex> lck(mtx);
				std::cout << (str1 + str2) << std::endl;
				return;
			}, "hello", "thread");
			v1.push_back(std::move(ans));
		}
		for (int i = 0; i != v.size(); ++i) {
			std::cout << v[i].get() << std::endl;
		}
		for (int i = 0; i != v1.size(); ++i) {
			v1[i].get();
		}
	} catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}
	system("pause");
	return 0;
}