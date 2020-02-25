#include "esp_log.h"
#include "mqtt_client.h"

#include <mqtt.h>
#include <led.h>

static const char *TAG = "MQTT";

static esp_mqtt_client_handle_t xMqttClient;

static const char *STATUS_TOPIC = "esp/status";
static const char *CONTROL_TOPIC = "esp/control";

static void subscribe_control_topic()
{

  int msg_id = esp_mqtt_client_subscribe(xMqttClient, CONTROL_TOPIC, 1);
  ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);
}

static void publish_current_status()
{
  const char *status = get_current_status();

  int msg_id = esp_mqtt_client_publish(xMqttClient, STATUS_TOPIC, status, 0, 1, 0);
  ESP_LOGI(TAG, "sent current status successful, msg_id=%d", msg_id);
}

static void handle_event(esp_mqtt_event_handle_t event)
{
  if (strcmp(event->topic, CONTROL_TOPIC) == 0)
  {
    if (event->data_len == 6 && strncmp("report", event->data, event->data_len) == 0)
    {
      ESP_LOGI(TAG, "got report request");
      publish_current_status();
    }
    else if (event->data_len == 14 && strncmp("start blinking", event->data, event->data_len) == 0)
    {
      start_blinking();
      publish_current_status();
    }
    else if (event->data_len == 13 && strncmp("stop blinking", event->data, event->data_len) == 0)
    {
      stop_blinking();
      publish_current_status();
    }
  }
}

static esp_err_t mqtt_event_handler(esp_mqtt_event_handle_t event)
{
  // your_context_t *context = event->context;
  switch (event->event_id)
  {
  case MQTT_EVENT_CONNECTED:
    ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");

    subscribe_control_topic();
    publish_current_status();

    break;

  case MQTT_EVENT_DISCONNECTED:
    ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
    break;

  case MQTT_EVENT_SUBSCRIBED:
    ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
    break;

  case MQTT_EVENT_UNSUBSCRIBED:
    ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
    break;

  case MQTT_EVENT_PUBLISHED:
    ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
    break;

  case MQTT_EVENT_DATA:
    ESP_LOGI(TAG, "MQTT_EVENT_DATA");
    printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
    printf("DATA=%.*s\r\n", event->data_len, event->data);
    handle_event(event);
    break;

  case MQTT_EVENT_ERROR:
    ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
    break;

  default:
    ESP_LOGI(TAG, "Other event id:%d", event->event_id);
    break;
  }
  return ESP_OK;
}

void mqtt_app_start(void)
{
  esp_mqtt_client_config_t mqtt_cfg = {
      .uri = CONFIG_BROKER_URL,
      .event_handle = mqtt_event_handler,
      // .user_context = (void *)your_context
      .lwt_topic = STATUS_TOPIC,
      .lwt_msg = "vanished",
  };

  xMqttClient = esp_mqtt_client_init(&mqtt_cfg);
  esp_mqtt_client_start(xMqttClient);
}
