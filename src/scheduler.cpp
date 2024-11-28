#include "../h/scheduler.hpp"

List<Thr> Scheduler::readyCoroutineQueue;

Thr *Scheduler::get()
{
    return readyCoroutineQueue.removeFirst();
}

void Scheduler::put(Thr *ccb)
{
    readyCoroutineQueue.addLast(ccb);
}