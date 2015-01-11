#include "notification.h"
#include "status.h"
#include "face.h"

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
        status_push_update(s_buffer, 0);
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
}

void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
  face_greeting_update("Sending message failed.");
}

void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
  face_greeting_update("Sent message!");
}

void send_connect_event_to_phone(int key, char *message) {
	DictionaryIterator *iter;
	app_message_outbox_begin(&iter);
	
	if(iter == NULL) {
		return;
	}
	
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