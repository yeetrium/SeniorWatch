#include <pebble.h>

void init_animations(void) {
  Layer *layer = text_layer_get_layer(text_time_layer);

  up_animation = property_animation_create_layer_frame(layer, &LOWER_TO_RECT, &UPPER_TO_RECT);
  animation_set_duration((Animation*) up_animation, ANIMATION_DURATION_IN_MS);
  animation_set_curve((Animation*) up_animation, AnimationCurveEaseOut);

  down_animation = property_animation_create_layer_frame(layer, &UPPER_TO_RECT, &LOWER_TO_RECT);
  animation_set_duration((Animation*) down_animation, ANIMATION_DURATION_IN_MS);
  animation_set_curve((Animation*) down_animation, AnimationCurveEaseOut);
}