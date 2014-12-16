#ifndef _POOLTHREAD_H_
#define _POOLTHREAD_H_

#include <vector>
#include <queue>
#include <mutex>
#include <thread>
#include <future>
#include <type_traits>

class ThreadPool
{
public:
  ThreadPool(unsigned int nbThreads);
  ~ThreadPool();

  template<class F, class... Args>
  auto	addTask(F&& f, Args&&... args)
    -> std::future<typename std::result_of<F(Args...)>::type>;

private:
  std::vector<std::thread>		_threads;
  std::queue<std::function<void()> >	_tasks;
  std::mutex				_qmutex;
  std::condition_variable		_condvar;
  bool					_active;
};

template<class F, class... Args>
auto	ThreadPool::addTask(F&& f, Args&&... args)
  -> std::future<typename std::result_of<F(Args...)>::type>
{
  using	return_type = typename std::result_of<F(Args...)>::type;
  auto	task = std::make_shared< std::packaged_task<return_type()> >
    (std::bind(std::forward<F>(f), std::forward<Args>(args)...));
  std::future<return_type>	res = task->get_future();

  { // scoped to unlock the mutex
    std::unique_lock<std::mutex> lock(_qmutex);
    if (!_active)
      throw std::runtime_error("addTask on stopped ThreadPool");
   _tasks.emplace([task]()
		   {
		     (*task)();
		   });
  }
  _condvar.notify_one();
  return res;
}

#endif /* _POOLTHREAD_H_ */
