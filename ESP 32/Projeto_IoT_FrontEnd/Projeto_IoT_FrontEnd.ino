#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#define DHTTYPE DHT11  // Sensor de Temperatura e Humidade 

const char* ssid = "*************";           // SSID da rede wireless
const char* password = "*************";       // senha da rede wireless
const char* mqtt_server = "*************";    // Endereço do Broker MQTT
const int mqtt_port = 1883;                   // Porta utilizada pelo MQTT
const char* mqtt_user = "*************";      // MQTT user
const char* mqtt_password = "*************";  // Senha do user MQTT

const int DHT_Pin = 14;         // (GPIO14) Pino de conexão do sensor DHT11
const int LAMP_Pin = 32;        // (GPIO32) Pino para acionamento da Lâmpada
const int VENT_Pin = 33;        // (GPIO33) Pino para acionamento da Ventilador

unsigned long previousMillis_1 = 0;  // Variável para o temporizador 1
unsigned long previousMillis_2 = 0;  // Variável para o temporizador 2
const long interval_1 = 1000;        // Intervalo temporizador 1 (em milissegundos)
const long interval_2 = 100;         // Intervalo temporizador 2 (em milissegundos)

int vrLamp = 0;         // Variável para armazenar o valor do Slider da Lâmpada
int vrVent = 0;         // Variável para armazenar o valor do Slider do Ventilador
bool lampStatus = LOW;  // Variável para armazenar o status do Botão da Lâmpada
bool ventStatus = LOW;  // Variável para armazenar o valor do Botão do Ventilador
float temperatura = 0;
float humidade = 0;

DHT dht(DHT_Pin, DHTTYPE);
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {

  pinMode(LED_BUILTIN, OUTPUT);

  // configuração dos canais de PWM
  ledcSetup(0, 1000, 10);      // Canal 0 - Lâmpada (ledChannel, freq, resolution)
  ledcAttachPin(LAMP_Pin, 0);  // (pin, ledChannel)
  ledcSetup(1, 1000, 10);      // Canal 1 - Ventilador (ledChannel, freq, resolution)
  ledcAttachPin(VENT_Pin, 1);  // (pin, ledChannel)

  // Configurações iniciais
  dht.begin();
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

// Conexão Wireless
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Conectando-se a ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Conectado a rede Wi-Fi");
  Serial.print("Endereco MAC: ");
  Serial.println(WiFi.macAddress());
  Serial.println("Endereço IP: ");
  Serial.println(WiFi.localIP());
}

// Conexão MQTT
void reconnect() {
  while (!client.connected()) {
    Serial.print("Conectando ao servidor MQTT...");
    if (client.connect("ESP32_02", mqtt_user, mqtt_password)) {
      Serial.println("Conectado ao servidor MQTT");
      client.subscribe("IFSP/IoT/swLamp");
      client.subscribe("IFSP/IoT/swVent");
      client.subscribe("IFSP/IoT/vrLamp");
      client.subscribe("IFSP/IoT/vrVent");
      client.subscribe("IFSP/IoT/request");
    } else {
      Serial.print("Falha ao conectar ao servidor MQTT, rc=");
      Serial.print(client.state());
      Serial.println(" Tentando novamente em 5 segundos");
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long currentMillis = millis();  // Obtém o tempo atual

  if (currentMillis - previousMillis_2 >= interval_2) {    // Temporizador 2 (100 milesegundos)
    previousMillis_2 = currentMillis;                      
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));  // Pisca o Led da Placa
  }

  if (currentMillis - previousMillis_1 >= interval_1) {  // Temporizador 1 (5 segundos)
    previousMillis_1 = currentMillis;                    // Atualiza o tempo da última leitura

    // Realiza a leitura da temperatura e umidade
    temperatura = dht.readTemperature();
    humidade = dht.readHumidity();

    // Verifica se a leitura foi bem-sucedida
    if (isnan(temperatura) || isnan(humidade)) {
      Serial.println("Erro ao ler o sensor DHT11!");
      return;
    }
    // Envia temperatura e humidade via MQTT
    client.publish("IFSP/IoT/return", String(String(temperatura) + "," +  String(humidade)).c_str());
  }

}

// Tratamento dos comandos recebidos via MQTT
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String messageTemp;

  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    messageTemp += (char)payload[i];
  }
  Serial.println();

  // Button Lâmpada
  if (String(topic) == "IFSP/IoT/swLamp") {
    if (messageTemp == "ON") {
      lampStatus = HIGH;
      int lamp = map(vrLamp, 0, 100, 0, 1023);
      ledcWrite(0, lamp);  // PWM Lâmpada Ligado (0 - 1023)
      //client.publish("IFSP/IoT/return", "Ligada");
      Serial.println("Lâmpada Ligada");
    } else if (messageTemp == "OFF") {
      lampStatus = LOW;
      ledcWrite(0, 0);  // PWM Lâmpada Desligado (0)
      //client.publish("IFSP/IoT/return", "Desligada");
      Serial.println("Lâmpada Desligada");
    }
  }

  // Slider Lâmpada
  if (String(topic) == "IFSP/IoT/vrLamp") {
    vrLamp = messageTemp.toInt();

    if (vrLamp > 0 && lampStatus == HIGH) {
      int lamp = map(vrLamp, 0, 100, 0, 1023);
      ledcWrite(0, lamp);  // PWM Lâmpada Ligado (1 - 1023)
      //client.publish("IFSP/IoT/return", String(vrLamp).c_str());
      Serial.println("Lâmpada Ligada");
      Serial.print("vrLamp = ");
      Serial.println(vrLamp);
    } else {
      ledcWrite(0, 0);  // PWM Lâmpada Desligado (0)
      //client.publish("IFSP/IoT/return", String(vrLamp).c_str());
      Serial.println("Lâmpada Desligada");
      Serial.print("vrLamp = ");
      Serial.println(vrLamp);
    }
  }

  // Button Ventilador
  if (String(topic) == "IFSP/IoT/swVent") {
    if (messageTemp == "ON") {
      ventStatus = HIGH;
      int vent = map(vrVent, 0, 100, 160, 1023);
      ledcWrite(1, vent);  // PWM Ventilador Ligado (0 - 1023)
      //client.publish("IFSP/IoT/return", "Ligado");
      Serial.println("Ventilador Ligado");
    } else if (messageTemp == "OFF") {
      ventStatus = LOW;
      ledcWrite(1, 0);  // PWM Ventilador Desligado (0)
      //client.publish("IFSP/IoT/return", "Desligado");
      Serial.println("Ventilador Desligado");
    }
  }

  // Slider Ventilador
  if (String(topic) == "IFSP/IoT/vrVent") {
    vrVent = messageTemp.toInt();

    if (vrVent > 0 && ventStatus == HIGH) {
      int vent = map(vrVent, 0, 100, 160, 1023);
      ledcWrite(1, vent);  // PWM Ventilador Ligado (1 - 1023)
      //client.publish("IFSP/IoT/return", String(vrVent).c_str());
      Serial.println("Ventilador Ligado");
      Serial.print("vrVent = ");
      Serial.println(vrVent);
    } else {
      ledcWrite(1, 0);  // PWM Ventilador Desligado (0)
      //client.publish("IFSP/IoT/return", String(vrVent).c_str());
      Serial.println("Ventilador Desligado");
      Serial.print("vrVent = ");
      Serial.println(vrVent);
    }
  }

  // Temperatura e Humidade
  if (String(topic) == "IFSP/IoT/request") {
    Serial.println(messageTemp);
    Serial.print("Temperatura: ");
    Serial.print(temperatura);
    Serial.print(" °C | Umidade: ");
    Serial.print(humidade);
    Serial.println(" %");
  }
}
