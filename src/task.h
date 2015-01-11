#include <pebble.h>

void task_alert(void);
void task_check(char *time);
char *task_time_get_row(int row);
char *task_title_get_row(int row);
void task_timer_off(void);
void task_update_row(int row, char *time, char *title);