#pragma once
#include <queue>

typedef signed long time;

class EventQueue;

struct Event 
{
protected:
    bool abort = false;
public:
    void Abort() 
        {abort = true;}
    bool IsAborted() const
        {return abort;}
    virtual void Execute(EventQueue&) = 0;
};

struct WhenEvent 
{
    time when;
    Event* what;
};

struct Later 
{
public:
    bool operator() (WhenEvent A, WhenEvent B)//for future queue
        {return (B.when < A.when);}
};

struct EventQueue 
{
private:
    typedef std::priority_queue<WhenEvent, std::vector<WhenEvent>, Later> future_queue_type;
    future_queue_type future;
    typedef std::queue<Event*> present_queue_type;
    present_queue_type present;
    time current_time;
    
    void DeleteCurrentEvent() 
    {
        delete present.front();
        present.pop();
    }
public:
    void QueueEvent (WhenEvent const &toque);
    void QueueEvent (Event* toque, time how_long = 0);
    bool DoEvent();
    time MoveOn();
    bool Empty()
        {return present.empty() && future.empty();}
};
