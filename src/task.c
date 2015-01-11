#include <pebble.h>

#include "notification.h"
#include "status.h"
  
AppTimer *task_timer;

char *task_times[10] = { 
  "12:21PM", "12:22PM",
  "12:24PM", "12:23PM",
  "12:19PM", "12:17PM",
  "12:20PM", "12:18PM",
  "12:08PM", "12:12PM", };
char *task_titles[10] = { 
  "Test1", "Test2", "Test3", "Test4", "Test5",
  "Test6", "Test7", "Test8", "Test9", "Test10" };
bool is_alert = false;

void task_alert() {
  // Possible include?
  APP_LOG(APP_LOG_LEVEL_INFO, "Did not respond");
  char alert[] = "Your client didn't respond to a task.";
  send_connect_event_to_phone(GENERAL_NOTIFICATION_KEY, alert);
  is_alert = true;
  if (task_timer != NULL) {
    APP_LOG(APP_LOG_LEVEL_INFO, "Timer off after timeout");
    app_timer_cancel(task_timer);
    task_timer = NULL;
  }
}

char *task_time_get_row(int row) {
  return task_times[row];
}

char *task_title_get_row(int row) {
  return task_titles[row];
}

void task_timer_off() {
  if (is_alert == false) {
    if (task_timer != NULL) {
      APP_LOG(APP_LOG_LEVEL_INFO, "Timer off");
      app_timer_cancel(task_timer);
      task_timer = NULL;
    }
    
    char alert[] = "Your client has responded to a task.";
    send_connect_event_to_phone(RESPONSE_NOTIFICATION_KEY, alert);
    APP_LOG(APP_LOG_LEVEL_INFO, "Msg sent");
  }
}

void task_update_row(int row, char *time, char *title) {
  strcpy(task_times[row], time);
  strcpy(task_titles[row], title);
}

void task_check(char *time) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Checking tasks");
  is_alert = false;
  
  for (int i = 0; i < 10; ++i) {
    if (strcmp(task_time_get_row(i), time) == 0) {
      APP_LOG(APP_LOG_LEVEL_INFO, "Time for task!");
      // TODO: Push status and/or vibrate
      status_push_update(task_title_get_row(i));
      vibes_long_pulse();
      
      int duration = 10000;
      if (task_timer == NULL) task_timer = app_timer_register(duration, task_alert, NULL);
    }
  }
}