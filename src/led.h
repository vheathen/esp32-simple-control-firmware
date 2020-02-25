#ifndef _LED_H_
#define _LED_H_

#define BLINK_GPIO 2 // CONFIG_BLINK_GPIO

#define HOLD 0
#define BLINKING 1

const char *get_current_status();

void start_blinking();
void stop_blinking();

#endif /* _LED_H_ */