#include <Civil/Event.h>

void EventQueue::QueueEvent (Event* toque, time how_long) //equivalent to QueueEvent(WhenEvent{current_time + how_long, toque})
{
    if (!toque)
        return;
    if (how_long <= 0)
        present.push(toque);
    else
        future.push(WhenEvent{current_time + how_long, toque});
}

void EventQueue::DoEvent() 
{
    if (present.empty() && !future.empty())
    //    if (!future.empty() && future.top().when == current_time) // this should always be false
            MoveOn();
    //    else
    //        return false;
    while(present.front()->isAborted())
        DeleteCurrentEvent(); //pop away all the aborted events
    
    auto next = present.front()->Execute(); // execute the current event and 
    QueueEvent(next.second, next.first); // queue the requested next event.
    DeleteCurrentEvent(); //done and dusted
    
    //return true;
}

time EventQueue::MoveOn()
{
    if (!present.empty())
        return 0;
    time past_time = current_time;
    current_time = future.top().when;
    while(!future.empty() && future.top().when == current_time)
    {
        present.push(future.top().what);
        future.pop();
    }
    return current_time - past_time;
}

