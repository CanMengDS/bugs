#pragma once
#include <iostream>
#include <queue>
#include <vector>
#include <functional>
#include <condition_variable>
#include <mutex>

class ThreadPool {
public:
	ThreadPool(int threadNum = 10);
	~ThreadPool();

	template<class T>
	void enqueue(T&& t);
private:
	bool isStop;
	std::mutex tex;
	std::vector<std::thread> threads;
	std::queue <std::function<void()>> tasks;
	std::condition_variable t_cv;
};

template<class T>
void ThreadPool::enqueue(T&& t) {
	std::function<void()> task(t);
	{
		std::unique_lock<std::mutex> ul(tex);
		tasks.emplace(task);
	}
	t_cv.notify_all();
}