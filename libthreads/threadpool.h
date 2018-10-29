#ifndef _THREADPOOL_H_
#define _THREADPOOL_H_

#include <iostream>
#include <thread>
#include "threadsafequeue.h"
#include <atomic>
#include <vector>

namespace filesafe
{
    template<class T>
    class ThreadPool
    {
    public:
        
        ThreadPool(
            size_t numThreads
        )
        {
			m_killThreads = false;
            for (size_t i = 0; i < numThreads; ++i)
            {
                m_threads.emplace_back(std::thread(&ThreadPool::ThreadFunc, this));
            }
        }

        void KillThreads()
        {
            m_killThreads = true;
            m_q.ReleaseQueue();
        }

        bool AddJob(T job)
        {
            return m_q.Enqueue(job);
        }

        void Join()
        {
            for (auto &thread : m_threads)
            {
                if (thread.joinable())
                {
                    thread.join();
                }
            }

        }

        ~ThreadPool()
        {
            KillThreads();
            Join();
        }

    private:
        void ThreadFunc()
        {
            for (;;)
            {
                T job;                
                if ( !m_q.Dequeue(job) || m_killThreads )
                {
                    break;
                }
                std::thread::id this_id = std::this_thread::get_id();
                job.Execute();
            }
        }

        filesafe::ThreadSafeQueue<T> m_q;
        std::atomic<bool> m_killThreads;
        std::vector<std::thread> m_threads;
    };
}

#endif  //  _THREADPOOL_H_