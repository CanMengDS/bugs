#include "thread_pool.h"

ThreadPool::ThreadPool(int threadNum):isStop(false) {
	for (int i = 0; i < threadNum; i++) {
	threads.emplace_back([this]{
		while (true) {
			std::unique_lock<std::mutex> ul(tex);
			t_cv.wait(ul, [this] {
				return !tasks.empty() || isStop;
				});
			
			if (isStop && tasks.empty()) {  //�ж��Ƿ�stop����stop�������������ѭ��
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
	t_cv.notify_all(); //���Ѳ�ʹ�����̶߳��ر�
	for (auto& task : threads) {
		task.join();  //�����̵߳ȴ������̵߳Ĺر�
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