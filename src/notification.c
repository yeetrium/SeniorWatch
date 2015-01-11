#include "notification.h"
#include "status.h"
#include "face.h"
#include "window_manager.h"
  
notification_key expected_key;

void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message received!");
  Tuple *data = dict_read_first(iterator);
  
  while (data != NULL) {
    // Size of input message
    static char s_buffer[42];
    
    switch (data->key) {
      case UNUSABLE_NOTIFICATION_KEY:
        APP_LOG(APP_LOG_LEVEL_INFO, "UNUSABLE_NOTIFICATION_KEY");
        break;
      case GENERAL_NOTIFICATION_KEY:
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
  }
  
  expected_key = UNUSABLE_NOTIFICATION_KEY;
  
  if(strcmp(response, "") == 0) return;
  
  switch(type) {
    case FACE:
      face_greeting_update(response);
      break;
    case STATUS:
      app_timer_register(2000, status_pop, NULL);
      status_push_update(response);
      break;
    case MENU:
      break;
  }
  
  vibes_short_pulse();
}

void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send message.");
  
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
  }
  
  expected_key = UNUSABLE_NOTIFICATION_KEY;
  
  if(strcmp(response, "") == 0) return;
  
  switch(type) {
    case FACE:
      face_greeting_update(response);
      break;
    case STATUS:
      app_timer_register(2000, status_pop, NULL);
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