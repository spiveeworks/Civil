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

bool Later (WhenEvent A, WhenEvent B)//for future queue
{
    return (B.time < A.time);
}

struct EventQueue 
{
private:
    typedef priority_queue<WhenEvent, std::vector<WhenEvent>, Later> future_queue_type future;
    typedef queue<Event*> present_queue_type present;
    time current_time;
    
    void DeleteCurrentEvent() 
    {
        delete present.front();
        present.pop();
    }
    
public:
    void QueueEvent (WhenEvent const &toque);
    void QueueEvent (time how_long = 0, Event* toque);
    bool doNext();
} action;




void EventQueue::QueueEvent (WhenEvent const &toque)
{
    if (toque.when <= current_time)
        present.push(toque.what);
    else
        future.push(toque);
}

void EventQueue::QueueEvent (time how_long = 0, Event* toque) //equivalent to QueueEvent({current_time + how_long, toque})
{
    if (how_long <= 0)
        present.push(toque);
    else
        future.push({current_time + how_long, toque});
}

bool EventQueue::doNext() 
{
    if (present.empty())
    {
        if (future.empty()) return false;
        current_time = future.top().when;
        while(future.top().when == current_time)
        {
            present.push(future.top());
            future.pop();
        }
    }
    while(present.front()->IsAborted())
        DeleteCurrentEvent(); //pop away all the aborted events
    present.front()->Execute(&this);
    DeleteCurrentEvent(); //done and dusted
    return true;
}






