#include <algorithm>
#include "timer.h"

unsigned int TimerManager::startTimer(double interval,std::function<void()> action,bool isRepeat, int id){
    Timer event(interval,this->m_timeline,action,isRepeat);
    event.m_id = (id == -1) ? this->m_autoIncrementId++ : id;
    std::unique_lock<std::mutex> lock(m_mutex);
    m_timer.push_back(event);
    std::push_heap(m_timer.begin(), m_timer.end(), std::greater<Timer>());

    return event.m_id;
}

void TimerManager::stopTimer(unsigned int timerId){
    std::unique_lock<std::mutex> lock(m_mutex);
    for (auto it = m_timer.begin(); it != m_timer.end();) {
        if (it->m_id == timerId) {
            it = m_timer.erase(it);
            break;
        } else {
            ++it;
        }
    }
    std::make_heap(m_timer.begin(), m_timer.end(), std::greater<Timer>());
}

void TimerManager::loopForExecute(){
    std::unique_lock<std::mutex> lock(m_mutex, std::defer_lock);
    lock.lock();
    if (!m_timer.empty()) {
        std::pop_heap(m_timer.begin(), m_timer.end(), std::greater<Timer>());
        Timer *top = &(this->m_timer.back());
        while (top->m_deadline <= this->m_timeline)
        {
            //如果已经到了执行的时间,新开一个子线程执行任务
            std::cout << "timer id: " << top->m_id << std::endl;
            std::thread t(top->m_action);
            t.detach(); //子线程分离
            //top->m_action();

            //从堆中删除
            Timer newnode(*top);
            this->m_timer.pop_back();

            if (newnode.m_isRepeat)
            {
                //如果是重复事件,则重新添加
                lock.unlock();
                this->startTimer(newnode.m_interval, newnode.m_action, newnode.m_isRepeat, newnode.m_id);
                lock.lock();
            }

            if (m_timer.empty()) {
                break;
            }
            std::pop_heap(m_timer.begin(), m_timer.end(), std::greater<Timer>());
            top = &(this->m_timer.back());
        }
        // 重新堆
        std::make_heap(m_timer.begin(), m_timer.end(), std::greater<Timer>());
    }
    lock.unlock();
    //执行一次后等待一个周期
    std::this_thread::sleep_for(this->m_tick);
    //周期增1
    this->m_timeline++;
    std::cout << "+++++++ circle: " << this->m_timeline << std::endl;
}

void TimerManager::asyncWorkStart()
{
    if (!this->m_isStart)
    {
        m_threadId = std::thread(&TimerManager::syncWorkStart, this);
    }
}

void TimerManager::syncWorkStart(){
    if(!this->m_isStart){
        this->m_isStart = true;
        while(this->m_isStart) {
            this->loopForExecute();
        }
    }
}

void TimerManager::workStop()
{
    this->m_isStart = false;
    if (m_threadId.joinable()) {
        m_threadId.join();
    }
}
