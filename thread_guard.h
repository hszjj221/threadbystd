#ifndef _THREAD_GUARD_H_
#define _THREAD_GUARD_H_

#include <vector>

namespace tp {

template<class ThreadType>
class Thread_guard {
private:
	std::vector<ThreadType>& threads_;
public:
	Thread_guard(std::vector<ThreadType>& v) : threads_(v) {}
	Thread_guard(Thread_guard&& tg) = delete;
	Thread_guard& operator = (Thread_guard&& tg) = delete;
	Thread_guard(const Thread_guard&) = delete;
	Thread_guard& operator = (const Thread_guard&) = delete;

	~Thread_guard() {
		for (size_t i = 0; i != threads_.size(); ++i) {
			if (threads_[i].joinable()) {
				threads_[i].join();
			}
		}
	}
};

}

#endif	// !_THREAD_GUARD_H_
