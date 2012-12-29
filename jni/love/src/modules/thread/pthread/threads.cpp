/**
* Copyright (c) 2006-2012 LOVE Development Team
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented; you must not
*    claim that you wrote the original software. If you use this software
*    in a product, an acknowledgment in the product documentation would be
*    appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
*    misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
**/

#include "threads.h"

namespace love
{
namespace thread
{
	Mutex::Mutex()
	{
		pthread_mutex_init(&mutex, NULL);
	}

	Mutex::~Mutex()
	{
		pthread_mutex_destroy(&mutex);
	}

	void Mutex::lock()
	{
		pthread_mutex_lock(&mutex);
	}

	void Mutex::unlock()
	{
		pthread_mutex_unlock(&mutex);
	}

	void* ThreadBase::thread_runner(void* param)
	{
		ThreadBase* thread = (ThreadBase*)param;
		thread->main();
		return 0;
	}

	ThreadBase::ThreadBase()
		: running(false)
	{
	}

	ThreadBase::~ThreadBase()
	{
		if (running)
		{
			wait();
		}
	}

	bool ThreadBase::start()
	{
		pthread_create(&thread, NULL, thread_runner, this);
		if (thread == NULL)
			return false;
		else
			return (running = true);
	}

	void ThreadBase::wait()
	{
		pthread_join(thread, NULL);
		running = false;
	}

	void ThreadBase::kill()
	{
		pthread_kill(thread, SIGTERM);
		running = false;
	}

	unsigned int ThreadBase::threadId()
	{
		return (unsigned int) pthread_self();
	}

	Semaphore::Semaphore(unsigned int initial_value)
	{
		sem_init(&semaphore, 0, initial_value);
	}

	Semaphore::~Semaphore()
	{
		sem_destroy(&semaphore);
	}

	unsigned int Semaphore::value()
	{
		int sval;
		sem_getvalue(&semaphore, &sval);
		return sval;
	}

	void Semaphore::post()
	{
		sem_post(&semaphore);
	}

	bool Semaphore::wait(int timeout)
	{
		if (timeout < 0)
			return sem_wait(&semaphore) ? false : true;
		else if (timeout == 0)
			return sem_trywait(&semaphore) ? false : true;
		else
		{
			timespec spec;
			spec.tv_sec = timeout/1000;
			spec.tv_nsec = (timeout%1000)*1000000;
			
			int ret = sem_timedwait(&semaphore, &spec);
			return (ret == 0);
		}
	}

	bool Semaphore::tryWait()
	{
		return sem_trywait(&semaphore) ? false : true;
	}

	Conditional::Conditional()
	{
		pthread_cond_init(&cond, NULL);
	}

	Conditional::~Conditional()
	{
		pthread_cond_destroy(&cond);
	}

	void Conditional::signal()
	{
		pthread_cond_signal(&cond);
	}

	void Conditional::broadcast()
	{
		pthread_cond_broadcast(&cond);
	}

	bool Conditional::wait(Mutex* mutex, int timeout)
	{
		if (timeout < 0)
		{
			return !pthread_cond_wait(&cond, &mutex->mutex);
		}
		else
		{
			timespec spec;
			spec.tv_sec = timeout/1000;
			spec.tv_nsec = (timeout%1000)*1000000;
			return (pthread_cond_timedwait(&cond, &mutex->mutex, &spec) == 0);
		}
	}

} // thread
} // love
