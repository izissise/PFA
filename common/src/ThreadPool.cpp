#include "ThreadPool.hpp"

ThreadPool::ThreadPool(unsigned int nbThreads) :
  _active(true)
{
  for(unsigned int i = 0; i < nbThreads; ++i)
    _threads.emplace_back(
			 [this]
			 {
			   while (true)
			     {
			       std::function<void()> task;

			       {
			       	 std::unique_lock<std::mutex> lock(this->_qmutex);
			       	 this->_condvar.wait(lock,
						     [this]
						     {
						       return !this->_active || !this->_tasks.empty();
						     });
			       	 if (!this->_active && this->_tasks.empty())
			       	   return;
			       	 task = std::move(this->_tasks.front());
			       	 this->_tasks.pop();
			       }
			       task();
 			     }
 			 });
}

ThreadPool::~ThreadPool()
{
  {  // scoped to unlock the mutex
    std::unique_lock<std::mutex> lock(_qmutex);
    _active = false;
  }

  _condvar.notify_all();
  for (std::thread &thread: _threads)
    thread.join();
}
