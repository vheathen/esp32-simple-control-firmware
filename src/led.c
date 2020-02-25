#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#include "esp_log.h"

#include <led.h>

static const char *TAG = "LED";

static TaskHandle_t xBlinkTaskHandle;

static TickType_t xBlinkPeriodMs = 2000;

static int led_status = HOLD;

const char *get_current_status()
{
  static char *statuses[] = {"hold", "blinking"};
  return statuses[led_status];
}

static void led_on()
{
  gpio_pad_select_gpio(BLINK_GPIO);
  /* Set the GPIO as a push/pull output */
  gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);

  gpio_set_level(BLINK_GPIO, 1);
}

static void led_off()
{
  gpio_pad_select_gpio(BLINK_GPIO);
  /* Set the GPIO as a push/pull output */
  gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);

  gpio_set_level(BLINK_GPIO, 0);
}

static void blink_task(void *pvParameter)
{
  /* Configure the IOMUX register for pad BLINK_GPIO (some pads are
       muxed to GPIO on reset already, but some default to other
       functions and need to be switched to GPIO. Consult the
       Technical Reference for a list of pads and their default
       functions.)
    */
  gpio_pad_select_gpio(BLINK_GPIO);
  /* Set the GPIO as a push/pull output */
  gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
  while (1)
  {
    led_off();
    vTaskDelay(xBlinkPeriodMs / 2 / portTICK_PERIOD_MS);
    /* Blink on (output high) */
    led_on();
    vTaskDelay(xBlinkPeriodMs / 2 / portTICK_PERIOD_MS);
  }
}

void start_blinking()
{
  if (led_status == HOLD)
  {
    xTaskCreate(&blink_task, "blink_task", configMINIMAL_STACK_SIZE, NULL, 5, &xBlinkTaskHandle);
    led_status = BLINKING;

    ESP_LOGI(TAG, "Blinking started");
  }
  else
  {
    ESP_LOGI(TAG, "Already blinking");
  }
}

void stop_blinking()
{
  if (led_status == BLINKING)
  {
    vTaskDelete(xBlinkTaskHandle);
    led_off();

    led_status = HOLD;

    ESP_LOGI(TAG, "Went hold");
  }
  else
  {
    ESP_LOGI(TAG, "Already hold");
  }
}
