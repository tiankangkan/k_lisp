#include "k_timer.h"

int main()
{
    struct k_timer timer;
    
    k_timer_start(&timer);
    getchar();
    k_timer_pause(&timer);
    getchar();
    k_timer_resume(&timer);
    k_timer_stop(&timer);
    k_timer_print(&timer);
    
    system("pause");
    
    return 0;
}
