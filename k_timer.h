/**
    name      : k_tree.h
    date      ：2015.09.26
    describe  : 计时器 
    copyright : 你可以自由使用它, 但是不能在不变更版本号的情况下对其作出修改.
    updata    :
        1) 
*/

#ifndef __K_TIMER_H__
    #define  __K_TIMER_H__

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define K_TIMER_RUN      0
#define K_TIMER_PAUSE    1

struct k_timer{
    int status;
    double time_cost;
    clock_t last;
};

/* 开始计时, 时间与青春年华有关, 为嘛青春的英文翻译是 youth ... */
int k_timer_start(struct k_timer *youth);
/* 停止计时, 返回时长(sec) */ 
double k_timer_stop(struct k_timer *youth);
/* 暂停计时 */
int k_timer_pause(struct k_timer *youth);
/* 恢复计时 */
int k_timer_resume(struct k_timer *youth);
/* 打印当前消耗时间 */ 
int k_timer_print(struct k_timer *youth);

/******************************************************************/

int k_timer_start(struct k_timer *youth)
{
    youth -> status = K_TIMER_RUN;
    youth -> time_cost = 0.0;
    youth -> last = clock();
    
    return 0;
}

double k_timer_stop(struct k_timer *youth)
{
    double sec;
    
    if(youth -> status == K_TIMER_RUN){
        sec = (double)(clock() - youth -> last) / CLOCKS_PER_SEC;
        youth -> time_cost += sec;
    }
    
    return youth -> time_cost;
}

int k_timer_pause(struct k_timer *youth)
{
    double sec;
    
    if(youth -> status == K_TIMER_RUN){
        sec = (double)(clock() - youth -> last) / CLOCKS_PER_SEC;
        youth -> time_cost += sec;
        youth -> status = K_TIMER_PAUSE;
    }
    
    return 0;
}

int k_timer_resume(struct k_timer *youth)
{
    if(youth -> status == K_TIMER_PAUSE){
        youth -> status = K_TIMER_RUN;
        youth -> last = clock();
    }
    
    return 0;
}

int k_timer_print(struct k_timer *youth)
{
    printf("time cost: %g Sec.\n", youth -> time_cost);
    
    return 0;
}

#endif
