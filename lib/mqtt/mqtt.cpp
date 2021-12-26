#include "mqtt.h"

// TODO: generate a class

IPAddress mqtt_server(MQTT_HOST_OCT_1,MQTT_HOST_OCT_2,
                      MQTT_HOST_OCT_3,MQTT_HOST_OCT_4);
WiFiClient esp_wifi_client;
PubSubClient mqtt_client(mqtt_server, MQTT_PORT, callback, esp_wifi_client);

IPAddress staticIP(192,168,0,90);
IPAddress gateway(192,168,0,1);
IPAddress subnet(255,255,255,0);

WiFiState state;

int8_t wifi_connect(){
    int init_time = 0;
    int wifi_time = 0;
    // Test resumeFromShutdown()
    //    REF: https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/examples/WiFiShutdown/WiFiShutdown.ino

    init_time = millis();

    unsigned long wifi_connection_timeout_ms = 10000;
    int8_t wifi_connection_result = 0;
    // F0:81:75:2E:BC:2E
    uint8_t bssid[6] = {240, 129, 117, 46, 188, 46};

    char ssid[] = WIFI_SSID;
    char pass[] = WIFI_PASS;

    // Connect to Wifi
    WiFi.config(staticIP, gateway, subnet); // TODO: config if configuration provided
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, pass, 6, bssid, true);
    wifi_connection_result = WiFi.waitForConnectResult(wifi_connection_timeout_ms);

    wifi_time = millis();

    DEBUG_SERIAL_PRINT("Local IP address: ");
    DEBUG_SERIAL_PRINTLN(WiFi.localIP());

    DEBUG_SERIAL_PRINT("channel: ");
    DEBUG_SERIAL_PRINTLN(WiFi.channel()); // 6

    DEBUG_SERIAL_PRINT("BSSID: ");
    DEBUG_SERIAL_PRINTLN(WiFi.BSSIDstr()); // F0:81:75:2E:BC:2E

    DEBUG_SERIAL_PRINT("millis to connect wifi = ");
    DEBUG_SERIAL_PRINTLN(wifi_time-init_time);

    return wifi_connection_result;
}

void wifi_reconnect(){
    // Reboot board if WIFI is not connected
    // TODO: manage reconnection softly
    if (WiFi.status() != WL_CONNECTED) {
        DEBUG_SERIAL_PRINTLN("Restarting device...");
        delay(10000);
        ESP.restart();
    }
}

void mqtt_reconnect(){
    int init_time = 0;
    int mqtt_time = 0;

    char mqtt_user[] = MQTT_USER;
    char mqtt_pass[] = MQTT_PASS;
    char mqtt_clientid[] = MQTT_CLIENT_ID;
    bool connect_ret_val = false;

    while (!mqtt_client.connected()) {
        DEBUG_SERIAL_PRINT("Attempting MQTT connection...");
        DEBUG_SERIAL_PRINT(" as clientid ");
        DEBUG_SERIAL_PRINT(mqtt_clientid);
        DEBUG_SERIAL_PRINT("...");

        if ( strcmp(mqtt_user, "") && strcmp(mqtt_pass, "") ){
            DEBUG_SERIAL_PRINT(" as user ");
            DEBUG_SERIAL_PRINT(mqtt_user);
            DEBUG_SERIAL_PRINT("...");
            connect_ret_val = mqtt_client.connect(mqtt_clientid, mqtt_user, mqtt_pass);
        }
        else {
            DEBUG_SERIAL_PRINT(" anonymous...");
            connect_ret_val = mqtt_client.connect(mqtt_clientid);
        }
        

        if (connect_ret_val) {
            DEBUG_SERIAL_PRINTLN(" connected");
            // Default: QOS 0
            mqtt_client.subscribe("+/+/node/+/command/#");
            mqtt_client.subscribe("+/+/global/#");
        }
        else
        {
            DEBUG_SERIAL_PRINT(" failed, rc=");
            DEBUG_SERIAL_PRINT(mqtt_client.state());
            DEBUG_SERIAL_PRINTLN(" try again...");
            delay(MQTT_RETRY_MS);
        }
    }

    mqtt_time = millis();

    DEBUG_SERIAL_PRINT("millis to connect mqtt = ");
    DEBUG_SERIAL_PRINTLN(mqtt_time-init_time);
}


void mqtt_loop(){
    mqtt_client.loop();
}


void mqtt_publish_message(char *section, char *payload, boolean retain){

    char mqtt_topic_msg[100] = "";
    char device_name[] = MQTT_CLIENT_ID;

    // Build topic
    strcat(mqtt_topic_msg, MQTT_APP_NAME);  strcat(mqtt_topic_msg, "/");
    strcat(mqtt_topic_msg, MQTT_APP_ID);    strcat(mqtt_topic_msg, "/");
    strcat(mqtt_topic_msg, "node");         strcat(mqtt_topic_msg, "/");
    strcat(mqtt_topic_msg, device_name);    strcat(mqtt_topic_msg, "/");
    strcat(mqtt_topic_msg, "data");         strcat(mqtt_topic_msg, "/");
    strcat(mqtt_topic_msg, section);

    if (mqtt_client.publish(mqtt_topic_msg, payload, retain)) {
        DEBUG_SERIAL_PRINTLN("MQTT message published");
        DEBUG_SERIAL_PRINT("- topic: ");
        DEBUG_SERIAL_PRINTLN(mqtt_topic_msg);
        DEBUG_SERIAL_PRINT("- payload: ");
        DEBUG_SERIAL_PRINTLN(payload);
    }
    else {
        DEBUG_SERIAL_PRINTLN("MQTT message publish failed");
    }

}


// Method to be adapted for each application
void callback(char *topic, byte *payload, unsigned int length) {

    char mqtt_topic_node_msg[100] = "";
    char mqtt_topic_global_msg[100] = "";
    char device_name[] = MQTT_CLIENT_ID;

    char mqtt_topic_node_led_control_msg[100] = "";
    char mqtt_topic_global_node_led_msg[100] = "";

    // Build topics
    strcat(mqtt_topic_node_msg, MQTT_APP_NAME); strcat(mqtt_topic_node_msg, "/");
    strcat(mqtt_topic_node_msg, MQTT_APP_ID);   strcat(mqtt_topic_node_msg, "/");
    strcat(mqtt_topic_node_msg, "node");        strcat(mqtt_topic_node_msg, "/");
    strcat(mqtt_topic_node_msg, device_name);   strcat(mqtt_topic_node_msg, "/");
    strcat(mqtt_topic_node_msg, "command");     strcat(mqtt_topic_node_msg, "/");

    strcpy(mqtt_topic_node_led_control_msg, mqtt_topic_node_msg);
    strcat(mqtt_topic_node_led_control_msg, "led_control");

    strcat(mqtt_topic_global_msg, MQTT_APP_NAME); strcat(mqtt_topic_node_msg, "/");
    strcat(mqtt_topic_global_msg, MQTT_APP_ID);   strcat(mqtt_topic_node_msg, "/");
    strcat(mqtt_topic_global_msg, "global");        strcat(mqtt_topic_node_msg, "/");
    strcat(mqtt_topic_global_msg, "command");     strcat(mqtt_topic_node_msg, "/");

    strcpy(mqtt_topic_global_node_led_msg, mqtt_topic_global_msg);
    strcat(mqtt_topic_global_node_led_msg, "led_control");    

    char message_buff[100] = "";

    DEBUG_SERIAL_PRINTLN("Message arrived");
    DEBUG_SERIAL_PRINT("- topic: ");
    DEBUG_SERIAL_PRINTLN(topic);
    DEBUG_SERIAL_PRINT("- payload_length: ");
    DEBUG_SERIAL_PRINTLN(length);
    DEBUG_SERIAL_PRINT("- payload: ");

    unsigned int i;
    for (i = 0; i < length; i++) {
        message_buff[i] = (char) payload[i];
    }
    message_buff[i] = '\0';
    DEBUG_SERIAL_PRINTLN(message_buff);

    if (!strcmp(topic, mqtt_topic_node_led_control_msg) ||
        !strcmp(topic, mqtt_topic_global_node_led_msg) ) { 
        if (!strcmp(message_buff, "1")) {
            digitalWrite(LED_BUILTIN, LOW); // PIN LOW will switch ON the LED
            DEBUG_SERIAL_PRINTLN("- led_write: 0");
        }
        if (!strcmp(message_buff, "0")) {
            digitalWrite(LED_BUILTIN, HIGH); // PIN HIGH will switch OFF the LED
            DEBUG_SERIAL_PRINTLN("- led_write: 1");
        }

        DEBUG_SERIAL_PRINT("- led_read: ");
        DEBUG_SERIAL_PRINTLN(digitalRead(LED_BUILTIN));
    }

}

