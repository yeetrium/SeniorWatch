#include "notification.h"
#include "status.h"
#include "face.h"

void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message received!");
  Tuple *t = dict_read_first(iterator);
  
  while (t != NULL) {
    // Size of input message
    static char s_buffer[42];
    
    snprintf(s_buffer, sizeof(s_buffer), "'%s'", t->value->cstring);
    status_push_update(s_buffer, 0);
    vibes_short_pulse();
    
    t = dict_read_next(iterator);
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

void send_connect_event_to_phone(char *message) {
	DictionaryIterator *iter;
	app_message_outbox_begin(&iter);
	
	if(iter == NULL) {
		return;
	}
	
	dict_write_cstring(iter, 0, message);
	dict_write_end(iter);
	
	app_message_outbox_send();
}

void initNotification(void) {
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
}