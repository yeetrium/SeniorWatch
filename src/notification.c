#include "notification.h"
#include "status.h"
#include "face.h"
#include "window_manager.h"
#include "menu.h"
#include "task.h"
  
notification_key expected_key;

void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Message received!");
  Tuple *data = dict_read_first(iterator);
  
  while (data != NULL) {
    // Size of input message
    static char s_buffer[42];
    
    switch (data->key) {
      case UNUSABLE_NOTIFICATION_KEY:
        APP_LOG(APP_LOG_LEVEL_INFO, "UNUSABLE_NOTIFICATION_KEY");
        break;
      case GENERAL_NOTIFICATION_KEY:
        APP_LOG(APP_LOG_LEVEL_INFO, "GENERAL_KEY");
        snprintf(s_buffer, sizeof(s_buffer), "'%s'", data->value->cstring);
        status_push_update(s_buffer);
        vibes_short_pulse();
        break;
      case RESPONSE_NOTIFICATION_KEY:
        APP_LOG(APP_LOG_LEVEL_INFO, "RESPONSE_NOTIFICATION_KEY");
        vibes_long_pulse();
        vibes_short_pulse();
        vibes_long_pulse();
        break;
      case PHONE_CALL_KEY:
        APP_LOG(APP_LOG_LEVEL_INFO, "PHONE_CALL_KEY");
        break;
      case UPDATE_TASK_KEY:
        APP_LOG(APP_LOG_LEVEL_INFO, "UPDATE_TASK_KEY");
        //int counter = 0;
        char title[42];
        char subtitle[42];
        data = dict_read_next(iterator);
        
        /*
        
        for(int i=0; *string)
        
        */
      
        //while (data != NULL) {
          //strcpy(title, data->value->cstring);
          memcpy(title, data->value->cstring, strlen(data->value->cstring) + 1);
          APP_LOG(APP_LOG_LEVEL_INFO, "Converted first string");
          //APP_LOG(APP_LOG_LEVEL_INFO, data->value->cstring);
          //snprintf(title, sizeof(title), "'%s'", data->value->cstring);
          data = dict_read_next(iterator);
          //strcpy(subtitle, data->value->cstring);
          memcpy(subtitle, data->value->cstring, strlen(data->value->cstring) + 1);
          APP_LOG(APP_LOG_LEVEL_INFO, "Converted second string");
          //APP_LOG(APP_LOG_LEVEL_INFO, data->value->cstring);
          //snprintf(subtitle, sizeof(subtitle), "'%s'", data->value->cstring);
          //menu_update_row(0, title2, subtitle2);
          //APP_LOG(APP_LOG_LEVEL_INFO, title);
          //APP_LOG(APP_LOG_LEVEL_INFO, subtitle);
          task_update_row(1, title, subtitle);
          APP_LOG(APP_LOG_LEVEL_INFO, "Updated");
          //++counter;
          
          //data = dict_read_next(iterator);
          //if (data == NULL) return;
        //}
        //break;
        return;
    }
      
    data = dict_read_next(iterator);
  }
  
  expected_key = UNUSABLE_NOTIFICATION_KEY;
}

void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
  
  expected_key = UNUSABLE_NOTIFICATION_KEY;
}

void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
  
  window_type type = window_manager_get_top_type();
  char *response = "";
  
  switch (expected_key) {
    case UNUSABLE_NOTIFICATION_KEY:
      APP_LOG(APP_LOG_LEVEL_INFO, "UNUSABLE_NOTIFICATION_KEY");
      break;
    case GENERAL_NOTIFICATION_KEY:
      response = "Sending message failed.";
      break;
    case RESPONSE_NOTIFICATION_KEY:
      APP_LOG(APP_LOG_LEVEL_INFO, "RESPONSE_NOTIFICATION_KEY");
      response = "Response failed to send.";
      break;
    case PHONE_CALL_KEY:
      APP_LOG(APP_LOG_LEVEL_INFO, "PHONE_CALL_KEY");
      response = "Phone call request failed.";
      break;
    case UPDATE_TASK_KEY:
      APP_LOG(APP_LOG_LEVEL_INFO, "UPDATE_TASK_KEY");
      break;
  }
  
  expected_key = UNUSABLE_NOTIFICATION_KEY;
  
  if(strcmp(response, "") == 0) return;
  
  switch(type) {
    case FACE:
      face_greeting_update(response);
      break;
    case STATUS:
      status_push_update(response);
      break;
    case MENU:
      break;
  }
  
  vibes_short_pulse();
}

void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send message.");
  
  window_type type = window_manager_get_top_type();
  char *response = "";
  
  switch (expected_key) {
    case UNUSABLE_NOTIFICATION_KEY:
      APP_LOG(APP_LOG_LEVEL_INFO, "UNUSABLE_NOTIFICATION_KEY");
      break;
    case GENERAL_NOTIFICATION_KEY:
      response = "Sent message!";
      break;
    case RESPONSE_NOTIFICATION_KEY:
      APP_LOG(APP_LOG_LEVEL_INFO, "RESPONSE_NOTIFICATION_KEY");
      response = "Got the response!";
      break;
    case PHONE_CALL_KEY:
      APP_LOG(APP_LOG_LEVEL_INFO, "PHONE_CALL_KEY");
      response = "Phone call request sent!";
      break;
    case UPDATE_TASK_KEY:
      APP_LOG(APP_LOG_LEVEL_INFO, "UPDATE_TASK_KEY");
      break;
  }
  
  expected_key = UNUSABLE_NOTIFICATION_KEY;
  
  if(strcmp(response, "") == 0) return;
  
  switch(type) {
    case FACE:
      face_greeting_update(response);
      break;
    case STATUS:
      status_push_update(response);
      break;
    case MENU:
      break;
  }
}

void send_connect_event_to_phone(int key, char *message) {
	DictionaryIterator *iter;
	app_message_outbox_begin(&iter);
	
	if(iter == NULL) {
		return;
	}
  
  expected_key = key;
	
	dict_write_cstring(iter, key, message);
	dict_write_end(iter);
	
	app_message_outbox_send();
}

void initNotification(void) {
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
}