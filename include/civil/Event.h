#pragma once
#include <queue>

#include "Event.forward"


struct WhenEvent 
{
    time when;
    Event* what;
};

class Event 
{
protected:
    bool abort = false;
public:
    
    void Abort() 
        {abort = true;}
    bool isAborted() const
        {return abort;}
    virtual ChildEvent Execute() = 0; // Execute the event and return a follow-up event.
        // doesn't return WhenEvent simply because WhenEvent.when is absolute not relative
    
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
    time current_time = 0;
    
    void DeleteCurrentEvent() 
    {
        delete present.front();
        present.pop();
    }
public:
    void QueueEvent (WhenEvent const &toque);
    void QueueEvent (Event* toque, time how_long = 0);
    void DoEvent();
    time MoveOn();
    bool isEmpty() const
        {return present.empty() && future.empty();}
    
    EventQueue(EventQueue const &) = delete;
    EventQueue() = default;
};

