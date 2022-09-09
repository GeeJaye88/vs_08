// Timer.cpp: implementation of the Timer class.
//
//////////////////////////////////////////////////////////////////////

#include "windows.h"

#include "Timer.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Timer::Timer()
{
	m_bStop=true;
	m_msTimeout=-1;
	m_hThreadDone = NULL;
	m_hThreadDone = CreateEvent(NULL,FALSE, FALSE, NULL);
	//ASSERT (m_hThreadDone);
	SetEvent(m_hThreadDone);

}


Timer::~Timer()
{
	//dont destruct until the thread is done
	DWORD ret=WaitForSingleObject(m_hThreadDone,INFINITE);
	//ASSERT(ret==WAIT_OBJECT_0);
	Sleep(500);
}


void Timer::Tick()
{
	//Will be overriden by subclass

}

DWORD WINAPI Timer::TickerThread(
		LPVOID lpParam
	)
{
	Timer* me = (Timer*)lpParam;
	//ASSERT (me->m_msTimeout!=-1);
	while (!me->m_bStop)
	{
		Sleep (me->GetTimeout());
		me->Tick();
	}
	SetEvent(me->m_hThreadDone);
	return 0;
}


void Timer::StartTicking()
{
	if (m_bStop == false)
		return; ///ignore, it is already ticking...
	m_bStop = false;
	ResetEvent(m_hThreadDone);
	CreateThread(
			NULL,
			0,
			Timer::TickerThread,
			(LPVOID)0,
			0,
			0
		);
}


void Timer::StopTicking()
{
	if (m_bStop==true)
		return; ///ignore, it is not ticking...

	m_bStop=true; //ok make it stop
	WaitForSingleObject(m_hThreadDone,INFINITE); 

}
