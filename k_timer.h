/**
    name      : k_tree.h
    date      ��2015.09.26
    describe  : ��ʱ�� 
    copyright : ���������ʹ����, ���ǲ����ڲ�����汾�ŵ�����¶��������޸�.
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

/* ��ʼ��ʱ, ʱ�����ഺ�껪�й�, Ϊ���ഺ��Ӣ�ķ����� youth ... */
int k_timer_start(struct k_timer *youth);
/* ֹͣ��ʱ, ����ʱ��(sec) */ 
double k_timer_stop(struct k_timer *youth);
/* ��ͣ��ʱ */
int k_timer_pause(struct k_timer *youth);
/* �ָ���ʱ */
int k_timer_resume(struct k_timer *youth);
/* ��ӡ��ǰ����ʱ�� */ 
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
