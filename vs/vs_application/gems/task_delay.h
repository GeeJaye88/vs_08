
// ---- complete_after (task-delay.cpp) ----

// compile with: /EHsc
#include <ppltasks.h>
#include <agents.h>
//#include <iostream>

using namespace concurrency;

// Creates a task that completes after the specified delay.
task<void> complete_after(unsigned int timeout)
{

	// ---- task completion event that is set when a timer fires.
	task_completion_event<void> tce;


	// ---- create a non-repeating timer.
	auto fire_once = new timer<int>(timeout, 0, nullptr, false);


	// ---- create a call object that sets the completion event after the timer fires.
	auto callback = new call<int>([tce](int)
	{
		tce.set();
	});


	// ---- connect the timer to the callback and start the timer.
	fire_once->link_target(callback);
	fire_once->start();


	// ---- create a task that completes after the completion event is set.
	task<void> event_set(tce);


	// ---- create a continuation task that cleans up resources and
	//      and return that continuation task.
	return event_set.then([callback, fire_once]()
	{
		delete callback;
		delete fire_once;
	});

}