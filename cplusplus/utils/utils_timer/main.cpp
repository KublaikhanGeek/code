#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <string>
#include <functional>
#include <thread>
#include <memory>
#include <fstream>
#include "timer.h"

std::chrono::milliseconds tick(1000);       //1000毫秒作为一个周期
TimerManager* timer = TimerManager::getInstance(tick);
int g_id = 0;
void myprint(std::string msg){
    //std::ofstream of("timer.txt", std::ios::app);
    //std::thread::id this_id = std::this_thread::get_id();
    auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    //of << "From Thread " << this_id << "at time " << std::put_time(std::localtime(&t), "%Y-%m-%d %H.%M.%S") << ":" << msg << std::endl;
    std::cout << "From Thread " << std::this_thread::get_id() << "at time " << std::put_time(std::localtime(&t), "%Y-%m-%d %H.%M.%S") << ":" << msg << std::endl;
//    if (g_id == 2) {
 //       std::cout << ">>>>>> circle <<<<<<" << std::endl;
  //  }
    ++g_id;

    if (g_id == 5) {
        std::cout << "stop!!!!" << std::endl;
        // 错误的调用方式不能在计时器线程中调用
//        timer->workStop();
    }

}

int main(){
    std::function<void()> f1 = std::bind(myprint,"第一个加入,10tick后执行");
    std::function<void()> f2 = std::bind(myprint,"第二个加入，被删除不执行");
    std::function<void()> f3 = std::bind(myprint,"第三个加入，每5tick重复执行");
    std::function<void()> f4 = std::bind(myprint,"第四个加入，5tick后执行");
    std::function<void()> f5 = std::bind(myprint,"第五个加入，5tick后执行");
    std::function<void()> f6 = std::bind(myprint,"第六个加入，5tick后执行");
    std::function<void()> f7 = std::bind(myprint,"第七个加入，5tick后执行");
    std::function<void()> f8 = std::bind(myprint,"第八个加入，5tick后执行");
    std::function<void()> f9 = std::bind(myprint,"第九个加入，5tick后执行");
    std::function<void()> f10 = std::bind(myprint,"第十个加入，5tick后执行");
    std::function<void()> f11 = std::bind(myprint,"第十一个加入，15tick后执行");
    std::function<void()> f12 = std::bind(myprint,"第十二个在执行后加入，20tick+5s后执行");

  //  timer->startTimer(10,f1);
//    int id = timer->startTimer(11,f2);
    g_id = timer->startTimer(5,f3,true);
//    timer->startTimer(5,f4);
//    timer->startTimer(5,f5);
//    timer->startTimer(5,f6);   
//    timer->startTimer(5,f7);
//    timer->startTimer(5,f8);
//    timer->startTimer(5,f9);
//    timer->startTimer(5,f10);
//    timer->startTimer(15,f11);

 //   timer->stopTimer(id);

    myprint("线程开始启动,每tick是2秒");

    //异步执行，程序退出后计时器也会终止，因此在下面使用while循环保证程序不会退出
    timer->asyncWorkStart();
    //timer->syncStart();


    //休眠5秒钟
    //std::this_thread::sleep_for(std::chrono::seconds(5));   
    //应该在大概20*tick+5秒后执行,
    //TODO 执行后加入的定时器不对
   // timer->startTimer(20,f12);
    //std::this_thread::sleep_for(std::chrono::seconds(50));   
    //std::cout << "stop timer id: " << g_id << std::endl;
    //timer->stopTimer(g_id);


    getchar();

    return 0;
}
