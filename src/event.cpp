#include <Civil/Event.h>

void EventQueue::QueueEvent (WhenEvent const &toque)
{
    if (toque.when <= current_time)
        present.push(toque.what);
    else
        future.push(toque);
}

void EventQueue::QueueEvent (Event* toque, time how_long) //equivalent to QueueEvent({current_time + how_long, toque})
{
    if (how_long <= 0)
        present.push(toque);
    else
        future.push(WhenEvent{current_time + how_long, toque});
}

bool EventQueue::DoEvent() 
{
    if (present.empty())
        if (!future.empty() && future.top().when == current_time)
            MoveOn();
        else
            return false;
    while(present.front()->IsAborted())
        DeleteCurrentEvent(); //pop away all the aborted events
    present.front()->Execute(*this);
    DeleteCurrentEvent(); //done and dusted
    return true;
}

time EventQueue::MoveOn()
{
    if (!present.empty())
        return 0;
    time past_time = current_time;
    current_time = future.top().when;
    while(future.top().when == current_time)
    {
        present.push(future.top().what);
        future.pop();
    }
    return current_time - past_time;
}
