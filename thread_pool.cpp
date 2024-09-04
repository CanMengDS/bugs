#include "thread_pool.h"

ThreadPool::ThreadPool(int threadNum):isStop(false) {
	for (int i = 0; i < threadNum; i++) {
	threads.emplace_back([this]{
		while (true) {
			std::unique_lock<std::mutex> ul(tex);
			t_cv.wait(ul, [this] {
				return !tasks.empty() || isStop;
				});
			
			if (isStop && tasks.empty()) {  //判断是否stop，如stop则立即结束这次循环
				return;
			}
			/*std::function<void()> task(std::move(tasks.front()));*/
			auto task(std::move(tasks.front()));
			tasks.pop();
			ul.unlock();
			task();
		}
		});
}
}

ThreadPool::~ThreadPool() {
	{
		std::unique_lock<std::mutex> ul(tex);
		isStop = true;
	}
	t_cv.notify_all(); //唤醒并使所有线程都关闭
	for (auto& task : threads) {
		task.join();  //让主线程等待所有线程的关闭
	}
}

//template<class T>
//void ThreadPool::enqueue(T&& t) {
//	std::function<void()> task(t);
//	{
//		std::unique_lock<std::mutex> ul(tex);
//		tasks.emplace(task);
//	}
//	t_cv.notify_all();
//}