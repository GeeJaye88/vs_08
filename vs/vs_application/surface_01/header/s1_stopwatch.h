////////////////////////////////////////////////////////////////////////////////

// ---------- vs_gfx_stopwatch.h ----------
/*!
\file vs_gfx_stopwatch.h
\brief Implementation of the 
\author Gareth Edwards
*/

#pragma once

#include <ctime>

namespace vs_system
{

	class Stopwatch
	{

	public:
		explicit Stopwatch(bool start_immediately = false);
		void Start(bool reset = false);
		void Stop();
		unsigned long Elapsed() const;

	private:
		std::clock_t start, stop;
		bool running;

	};


	Stopwatch::Stopwatch(bool start_immediately)
		: start(0), stop(0), running(false)
	{
		if ( start_immediately )
		{
			Start(true);
		}
	}


	void Stopwatch::Start(bool reset)
	{
		if ( !running )
		{
			if ( reset )
			{
				start = std::clock();
			}
			running = true;
		}
	}


	void Stopwatch::Stop()
	{
		if ( running )
		{
			stop = std::clock();
			running = false;
		}
	}


	unsigned long Stopwatch::Elapsed() const
	{
		return (running ? std::clock() : stop) - start;
	}

}


////////////////////////////////////////////////////////////////////////////////