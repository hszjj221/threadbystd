#ifndef _THREAD_POOL_H_
#define _THREAD_POOL_H_

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <future>
#include <functional>
#include <vector>
#include <queue>
#include <utility>
#include <iostream>
#include <type_traits>

#include "thread_guard.h"

namespace tp {

class Thread_pool {
private:
	typedef std::function<void()> task_type;
private:
	std::atomic<bool> stop_;
	std::mutex mtx_;
	std::condition_variable cond_;
	std::queue<task_type> tasks_;
	std::vector<std::thread> threads_;
	tp::Thread_guard<std::thread> tg_;
public:
	Thread_pool();
};

Thread_pool::Thread_pool()
	:stop_(false)
	, tg_(threads_) {
	size_t nthreads = std::thread::hardware_concurrency();
	nthreads = nthreads == 0 ? 2 : nthreads;

	for (size_t i = 0; i != nthreads; ++i) {
		threads_.push_back(std::thread(
			[this] {
			while (!stop_) {
				task_type task;
				{
					std::unique_lock<std::mutex> lock(this->mtx_);
					this->cond_.wait(lock, [this] {return !this->tasks_.empty(); });
					task = std::move(this->tasks_.front());
					this->tasks_.pop();
				}
				task();
			}
		}
		));
	}
}

}

#endif // !_THREAD_POOL_H_

