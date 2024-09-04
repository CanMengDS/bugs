#include <iostream>
#include "thread_pool.h"

int main() {
	ThreadPool threadPool;
	for (int i = 0; i <= 999999999; i++) {
		threadPool.enqueue([i]() {
			std::cout << "thread: " << i << " is running" << std::endl;
			/*std::this_thread::sleep_for(std::chrono::seconds(1));*/
			std::cout << "thread: " << i << " is ended" << std::endl;
			});
		
	}
	return 0;
}