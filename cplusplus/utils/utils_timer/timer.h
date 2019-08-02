/**
 * 定时器的实现
 * 支持int startTimer(T interval,function action):设置一个定时器，指定间隔interval和回调函数action,返回定时器id
 * 支持void stopTimer(int timerId):删除一个定时器
 * 数据结构:最小堆模型，按照定时器触发的时间排序
 */
#ifndef TIMER_H
#define TIMER_H
#include <iostream>
#include <chrono>
#include <functional>
#include <queue>
#include <vector>
#include <thread>
#include <memory>
#include <mutex>

class Timer 
{
public:
    Timer(double interval, double timeline, std::function<void()> action, bool isRepeat)
    {
        this->m_interval = interval;
        this->m_deadline = interval + timeline;
        this->m_action = action;
        this->m_isRepeat = isRepeat;
    }
    bool operator > (const Timer &timer) const {
        return m_deadline > timer.m_deadline;
    }
public:
    friend class TimerManager;

private:
    unsigned int m_id;              //定时事件的唯一标示id
    double m_interval;              //事件的触发间隔，在重复事件中会用到这个属性
    double m_deadline;              //定时事件的触发时间
    std::function<void()> m_action; //触发的事件
    bool m_isRepeat;                //是否是重复执行事件
};

class TimerManager{
    private:
        //私有的构造函数
        TimerManager(std::chrono::milliseconds tick){
            this->m_timeline = 0;
            this->m_tick = tick;
            this->m_isStart = false;
            this->m_autoIncrementId = 0;
        }

        /**
         * 执行到达期限的定时器
         */
        void loopForExecute();

    public:

        //单例模式
        static TimerManager* getInstance(std::chrono::milliseconds tick){
            static TimerManager timerManager(tick);
            return &timerManager;
        }

        /**
         * 设置定时器
         * @param interval 定时间隔
         * @param action 定时执行的动作
         * @param isRepeat 是否重复执行,默认不重复执行
         * @param id 定时器id
         * @return unsigned int 定时器的id,可以根据这个id执行删除操作
         */
        unsigned int startTimer(double interval,std::function<void()> action,bool isRepeat = false, int id = -1);

        /**
         * 删除定时器
         * @param timerId 定时器id
         *
         */
        void stopTimer(unsigned int timerId);

        /**
         * 同步执行启动定时器
         */
         void syncWorkStart();

         /**
         * 异步执行启动定时器
         */
         void asyncWorkStart();

         void workStop();

    private:
        std::chrono::milliseconds m_tick;
        double m_timeline;     //当前时间线,long double的字节数为12
        bool m_isStart;        //标志当前定时器的启动状态
        unsigned int m_autoIncrementId;
        std::thread m_threadId;
        std::mutex m_mutex;

        std::vector<Timer> m_timer;

};

#endif
