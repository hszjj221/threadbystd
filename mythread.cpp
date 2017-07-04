#include <iostream>
#include <thread>
#include <vector>
#include <functional>

#include "thread_guard.h"
#include "thread_pool.h"

using namespace std;

int main()
{
	tp::Thread_pool tp;
	vector<std::future<int>> v;
	for (int i = 0; i != 10; ++i) {
		auto ans = tp.Submit([](int answer) {return answer; }, i);
		v.push_back(std::move(ans));
	}
	for (int i = 0; i != v.size(); ++i) {
		cout << v[i].get() << endl;
	}
	system("pause");
	return 0;
}