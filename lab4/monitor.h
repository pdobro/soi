#ifndef __monitor_h
#define __monitor_h

#include <stdio.h> 
#include <stdlib.h> 

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/types.h> 
#include <sys/stat.h> 
#include <string.h> 
#include <errno.h> 
#include <fcntl.h> 
#include <pthread.h> 
#include <unistd.h>
#include <semaphore.h>

#endif

class Semaphore
{
public:

  Semaphore( int value )
  {
#ifdef _WIN32
	sem = CreateSemaphore( NULL, value, 1, NULL );
#else
     if( sem_init( & sem, 0, value ) != 0 )
       throw "sem_init: failed";
#endif
  }
  ~Semaphore()
  { 
#ifdef _WIN32
	CloseHandle( sem );
#else
	  sem_destroy( & sem ); 
#endif
  }

  void p()
  {
#ifdef _WIN32
	  WaitForSingleObject( sem, INFINITE );
#else
     if( sem_wait( & sem ) != 0 )
       throw "sem_wait: failed";
#endif
  }

  void v()
  {
#ifdef _WIN32
	  ReleaseSemaphore( sem, 1, NULL );
#else
     if( sem_post( & sem ) != 0 )
       throw "sem_post: failed";
#endif
  }
  

private:

#ifdef _WIN32
	HANDLE sem;
#else
	sem_t sem;
#endif
};

class Condition
{
  friend class Monitor;

public:
	Condition() : w( 0 )
	{
		waitingCount = 0;
	}

	void wait()
	{
		w.p();
	}

	bool signal()
	{
		if( waitingCount )
		{
			-- waitingCount;
			w.v();
			return true;
		}//if
		else
			return false;
	}

private:
	Semaphore w;
	int waitingCount; //liczba oczekujacych watkow
};


class Monitor
{
public:
	Monitor() : sem_q1( 1 ), sem_q2( 1 ) {}

	void enter_q1()//kolejka 1 
	{
		sem_q1.p();
	}
	void enter_q2()//kolejka 2
	{
		sem_q2.p();
	}
	void leave_q1()
	{
		sem_q1.v();
	}
	void leave_q2()
	{
		sem_q2.v();
	}

	void wait( Condition & cond, int queue )
	{
		++ cond.waitingCount;
		if (queue == 1)
		leave_q1();
		else
		leave_q2();

		cond.wait();
	}

	void signal( Condition & cond, int queue )
	{
		if( cond.signal() ) {
			if (queue == 1)
			enter_q1();
			else
			enter_q2();

		}
	}


private:
	Semaphore sem_q1, sem_q2;
};

#endif

