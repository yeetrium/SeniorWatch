#include <string.h>
  
#include "notification.h"
#include "status.h"
#include "face.h"
#include "window_manager.h"
#include "menu.h"
#include "input.h"
  
#include "jsmn.h"
  
notification_key expected_key;

void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Message received!");
  Tuple *data = dict_read_first(iterator);
  
  while (data != NULL) {
    // Size of input message
    static char s_buffer[255];
    
    switch (data->key) {
      case UNUSABLE_NOTIFICATION_KEY:
        APP_LOG(APP_LOG_LEVEL_INFO, "UNUSABLE_NOTIFICATION_KEY");
        break;
      case GENERAL_NOTIFICATION_KEY:
        APP_LOG(APP_LOG_LEVEL_INFO, "GENERAL_KEY");
        snprintf(s_buffer, sizeof(s_buffer), "'%s'", data->value->cstring);
      
        status_push_update(s_buffer);
        APP_LOG(APP_LOG_LEVEL_INFO, s_buffer);
      
        // Create GBitmap, then set to created BitmapLayer
        s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_ALERT);
        s_background_layer = bitmap_layer_create(GRect(0, 0, 32, 32));
        bitmap_layer_set_alignment(s_background_layer, GAlignTop);
        bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
        layer_add_child(window_get_root_layer(window_stack_get_top_window()), bitmap_layer_get_layer(s_background_layer));
        
        vibes_short_pulse();
        break;
      case RESPONSE_NOTIFICATION_KEY:
        APP_LOG(APP_LOG_LEVEL_INFO, "RESPONSE_NOTIFICATION_KEY");
      
        // Create GBitmap, then set to created BitmapLayer
        s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_RESPONSE);
        s_background_layer = bitmap_layer_create(GRect(0, 0, 32, 32));
        bitmap_layer_set_alignment(s_background_layer, GAlignTop);
        bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
        layer_add_child(window_get_root_layer(window_stack_get_top_window()), bitmap_layer_get_layer(s_background_layer));
      
        vibes_long_pulse();
        vibes_short_pulse();
        vibes_long_pulse();
        break;
      case PHONE_CALL_KEY:
        APP_LOG(APP_LOG_LEVEL_INFO, "PHONE_CALL_KEY");
      
        // Create GBitmap, then set to created BitmapLayer
        s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_PHONE);
        s_background_layer = bitmap_layer_create(GRect(0, 0, 32, 32));
        bitmap_layer_set_alignment(s_background_layer, GAlignTop);
        bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
        layer_add_child(window_get_root_layer(window_stack_get_top_window()), bitmap_layer_get_layer(s_background_layer));
      
        break;
      case UPDATE_TASK_KEY:
        APP_LOG(APP_LOG_LEVEL_INFO, "UPDATE_TASK_KEY");
        // TODO: Create a 2d array of strings
        // from data->value
        //menu_update()
      
         // Create GBitmap, then set to created BitmapLayer
        s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_TASK);
        s_background_layer = bitmap_layer_create(GRect(0, 0, 32, 32));
        bitmap_layer_set_alignment(s_background_layer, GAlignTop);
        bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
        layer_add_child(window_get_root_layer(window_stack_get_top_window()), bitmap_layer_get_layer(s_background_layer));
      
        jsmn_parser parser;

        jsmn_init(&parser);
      
        jsmntok_t tokens[32];
        jsmnerr_t r;
      
        char *temp = data->value->cstring;
      

        r = jsmn_parse(&parser, temp, tokens, 32);
        char testBuffer[42];
      if(r >= 0) {
          jsmntok_t key = tokens[1];
          unsigned int length = key.end - key.start;
          status_push_update("first");
          snprintf(testBuffer, sizeof(testBuffer), "'%u'", length);
          status_push_update(testBuffer);
          char keyString[length + 1];    
           memcpy(keyString, &temp[key.start], length);
          keyString[length] = '\0';
          //status_push_update(keyString);
        }
        
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