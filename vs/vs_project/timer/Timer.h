// Timer.h: interface for the Timer class.
//
//////////////////////////////////////////////////////////////////////


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Timer  
{
public:
	void StopTicking();
	void StartTicking();
	Timer();
	virtual ~Timer();
	int GetTimeout(){return m_msTimeout;}
	void SetTimeout(int t){m_msTimeout=t;}
protected:
	int m_msTimeout;
	virtual void Tick();
private:
	HANDLE m_hThreadDone;
	bool   m_bStop;
	static DWORD WINAPI TickerThread(LPVOID);
};

