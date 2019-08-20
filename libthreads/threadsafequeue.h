#ifndef _THREAD_SAFE_QUEUE_H_
#define _THREAD_SAFE_QUEUE_H_

#include <mutex>
#include <deque>
#include <memory>
#include <atomic>


namespace httpsys
{

	template <typename T, typename Container = std::deque<T> >
	class ThreadSafeQueue
	{
	public:
		ThreadSafeQueue()
		{
			isExit_ = false;
		}

		ThreadSafeQueue(ThreadSafeQueue const&) = delete;
		ThreadSafeQueue& operator=(ThreadSafeQueue const&) = delete;

		void Enqueue(T const& item)
		{
			{
				std::lock_guard<std::mutex> guard(mtx_);
				q_.emplace_back(item);
			}
			cv_.notify_one();
		}

		std::shared_ptr<T> Dequeue()
		{
			std::unique_lock<std::mutex> lock(mtx_);
			cv_.wait(lock, [this]() {
				return (!this->q_.empty() || isExit_;)
				});
			if (isExit_)
			{
				return nullptr;
			}
			T item = std::move(q_.front());
			std::shared_ptr<T> itemSPtr = std::make_shared<T>(item);
			q_.pop_front();
			return itemSPtr;
		}

		bool Dequeue(T& item)
		{
			std::unique_lock<std::mutex> lock(mtx_);
			cv_.wait(lock, [this]() {
				return (!this->q_.empty() || isExit_);
				});
			if (isExit_)
			{
				return false;
			}
			item = std::move(q_.front());
			q_.pop_front();
			return true;
		}

		bool Empty() const
		{
			std::lock_guard<std::mutex> guard(mtx_);
			return q_.empty();
		}

		void ReleaseQueue()
		{
			std::lock_guard<std::mutex> guard(mtx_);
			isExit_ = true;
		}

		~ThreadSafeQueue()
		{
			ReleaseQueue();
		}

	private:
		Container q_;
		mutable std::mutex mtx_;
		std::condition_variable cv_;
		bool isExit_;
	};
}

#endif	//	_THREAD_SAFE_QUEUE_H_