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

#include <common/config.h>
#include <common/delay.h>

#include <sys/time.h>
#include <unistd.h>
#include "Timer.h"

namespace love
{
namespace timer
{
namespace android
{
	Timer::Timer()
		: fps(0), fpsUpdateFrequency(1), frames(0), dt(0)
	{
		gettimeofday(&currTime, NULL);
		gettimeofday(&time_init, NULL);
		gettimeofday(&prevFpsUpdate, NULL);
	}

	Timer::~Timer()
	{
	}

	const char * Timer::getName() const
	{
		return "love.timer.android";
	}

	double Timer::diff(const timeval &val1, const timeval &val2) const
	{
		double res;
		res  = val1.tv_sec + val1.tv_usec/1000000.0;
		res -= val2.tv_sec + val2.tv_usec/1000000.0;
		return res;
	}

	void Timer::step()
	{
		// Frames rendered
		frames++;

		// "Current" time is previous time by now.
		prevTime.tv_sec = currTime.tv_sec;
		prevTime.tv_usec = currTime.tv_usec;

		// Get ticks from SDL
		gettimeofday(&currTime, NULL);

		// Convert to number of seconds
		dt = diff(currTime, prevTime)/1000.0;

		double timeSinceLast = diff(currTime, prevFpsUpdate)/1000.0;
		// Update FPS?
		if (timeSinceLast > fpsUpdateFrequency)
		{
			fps = int((frames/timeSinceLast) + 0.5);
			prevFpsUpdate.tv_sec = currTime.tv_sec;
			prevFpsUpdate.tv_usec = currTime.tv_usec;
			frames = 0;
		}
	}

	void Timer::sleep(double seconds)
	{
		if (seconds > 0)
			usleep(seconds*1000000);
	}

	double Timer::getDelta() const
	{
		return dt;
	}

	int Timer::getFPS() const
	{
		return fps;
	}

	double Timer::getTime() const
	{
		timeval t;
		gettimeofday(&t, NULL);
		return diff(t, time_init);
	}

	double Timer::getMicroTime() const
	{
		timeval t;
		gettimeofday(&t, NULL);
		return t.tv_sec + t.tv_usec/1000000.0;
	}

} // sdl
} // timer
} // love
