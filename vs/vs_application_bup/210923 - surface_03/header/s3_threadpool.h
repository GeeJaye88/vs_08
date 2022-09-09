////////////////////////////////////////////////////////////////////////////////

// ---------- s3_threadpool.h ----------
/*!
\file s3_threadpool.cpp
\brief Implementation of a function pool
\author Gareth Edwards
*/

#pragma once


// ---- thread pool test

	#include <string>
	#include <iostream>


	#include <queue>
	#include <functional>
	#include <mutex>
	#include <condition_variable>
	#include <atomic>
	#include <cassert>

	#include <thread>
	#include <vector>


// ---- thread pool test class
	class Function_pool
	{

	private:
		std::queue<std::function<void()>> m_function_queue;
		std::mutex m_lock;
		std::condition_variable m_data_condition;
		std::atomic<bool> m_accept_functions;

	public:

		Function_pool();
		~Function_pool();
		void push(std::function<void()> func);
		void done();
		void infinite_loop_func();
	};


////////////////////////////////////////////////////////////////////////////////


// ---- CDTOR & METHODS ----

	//#include "../header/s3_function_pool.h"


	Function_pool::Function_pool() : m_function_queue(), m_lock(), m_data_condition(), m_accept_functions(true)
	{
		;
	}

	Function_pool::~Function_pool()
	{
		;
	}

	void Function_pool::push(std::function<void()> func)
	{
		std::unique_lock<std::mutex> lock(m_lock);
		m_function_queue.push(func);
		// when we send the notification immediately, the consumer will try to get the lock , so unlock asap
		lock.unlock();
		m_data_condition.notify_one();
	}

	void Function_pool::done()
	{
		std::unique_lock<std::mutex> lock(m_lock);
		m_accept_functions = false;
		lock.unlock();
		// when we send the notification immediately, the consumer will try to get the lock , so unlock asap
		m_data_condition.notify_all();
		//notify all waiting threads.
	}

	void Function_pool::infinite_loop_func()
	{
		std::function<void()> func;
		while (true)
		{
			{
				std::unique_lock<std::mutex> lock(m_lock);
				m_data_condition.wait(lock, [this]() {return !m_function_queue.empty() || !m_accept_functions; });
				if (!m_accept_functions && m_function_queue.empty())
				{
					//lock will be release automatically.
					//finish the thread loop and let it join in the main thread.
					return;
				}
				func = m_function_queue.front();
				m_function_queue.pop();
				//release the lock
			}
			func();
		}
	}


////////////////////////////////////////////////////////////////////////////////


// ---- thread pool test

//   #define THREAD_STUFF


// ---- GLOBAL ----

	#if defined(THREAD_STUFF)

	// ---- global threadpool stuff
		Function_pool func_pool;
		class quit_worker_exception : public std::exception {};

	#endif


////////////////////////////////////////////////////////////////////////////////


// ---- EXAMPLE FUNCTION ----

	#if defined(THREAD_STUFF)

	INT fred = 0;
	CHAR test[512][512];

	void example_function()
	{
		CHAR msg[128];
		CHAR copy[512][512];
		std::mutex mtx;

		mtx.lock();
		memcpy((void *)copy, (void *)test, 512 * 512);
		sprintf(msg, "    thread %d\n", fred++);
		mtx.unlock();

		OutputDebugString(msg);
	}

	#endif


////////////////////////////////////////////////////////////////////////////////


// ---- EXAMPLE USAGE ----

	#if defined(THREAD_STUFF)

	INT Model::SetupGraphics()
	{

		// ---- start thread pool
			OutputDebugString(" ---- starting operation\n");
			int num_threads = std::thread::hardware_concurrency();

			CHAR msg[128];
			sprintf(msg, "---- number of threads %d\n", num_threads);
			OutputDebugString(msg);

			std::vector<std::thread> thread_pool;
			for (int i = 0; i < num_threads; i++)
			{
				thread_pool.push_back(std::thread(&Function_pool::infinite_loop_func, &func_pool));
			}

		// ---- here we should send our functions
			for (int i = 0; i < 50; i++)
			{
				func_pool.push(example_function);
			}
			func_pool.done();
			for (unsigned int i = 0; i < thread_pool.size(); i++)
			{
				thread_pool.at(i).join();
			}


		return result;
	}

	#endif


////////////////////////////////////////////////////////////////////////////////