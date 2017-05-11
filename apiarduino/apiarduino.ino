#include <SPI.h>
#include <Ethernet.h>
#include <ArduinoJson.h>

// SETA OS PINOS DOS LEDS E SENSORES REFERENTES A CADA VAGA
const int VRD_VAGA1 = 1; //PINO LED VERDE VAGA 1
const int VRM_VAGA1 = 2; //PINO LED VERMELHO VAGA 1
const int SNR_VAGA1 = 3; //PINO SENSOR VAGA 1
const int VRD_VAGA2 = 4; //PINO LED VERDE VAGA 2
const int VRM_VAGA2 = 5; //PINO LED VERMELHO VAGA 2
const int SNR_VAGA2 = 6; //PINO SENSOR VAGA 2

int VAGA1; // PINO PARA GUARDAR VALOR OCUPADO
int VAGA2; // PINO PARA GUARDAR VALOR OCUPADO

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(192, 168, 0, 177);
EthernetServer server(80);

bool readRequest(EthernetClient& client) {
  bool currentLineIsBlank = true;
  while (client.connected()) {
    if (client.available()) {
      char c = client.read();
      if (c == '\n' && currentLineIsBlank) {
        return true;
      } else if (c == '\n') {
        currentLineIsBlank = true;
      } else if (c != '\r') {
        currentLineIsBlank = false;
      }
    }
  }
  return false;
}

// METODO RETORNA OBJETO DO TIPO JsonArray 
JsonArray& prepareResponse(JsonBuffer& jsonBuffer) {
  // CRIA O ARRAY JSON
  JsonArray& root = jsonBuffer.createArray();
  //CRIA OBJETO E INSERE NO ARRAY
  JsonObject& vaga1 = root.createNestedObject();
  vaga1["id"] = 1;
  vaga1["status"] = digitalRead(VRM_VAGA1);
  //CRIA OBJETO E INSERE NO ARRAY
  JsonObject& vaga2 = root.createNestedObject();
  vaga2["id"] = 2;
  vaga2["status"] = digitalRead(VRM_VAGA2);
  //RETORNA ARRAY PREENCHIDO
  return root;
}

// RECEBE O CLIENTE E O JSON PARA CRIAR O HTTP RESPONSE
void writeResponse(EthernetClient& client, JsonArray& json) {
  // SETA CABECALHO HTTP PARA RESPONDER AS REQUISICOES
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: application/json");
  client.println("Connection: close");
  client.println();
  //HTTP RESPONSE PARA O REQUEST DO CLIENTE
  json.prettyPrintTo(client);
}

void setup() {
  // SETA OS PINMODE
  pinMode(VRD_VAGA1, OUTPUT);
  pinMode(VRM_VAGA1, OUTPUT);
  pinMode(SNR_VAGA1, INPUT);
  pinMode(VRD_VAGA2, OUTPUT);
  pinMode(VRM_VAGA2, OUTPUT);
  pinMode(SNR_VAGA2, INPUT);

  ledStart();

  // INICIA COMUNICACAO SERIAL
  Serial.begin(9600);
  // FICA AGUARDANDO O INICIO DA COMUNICACAO SERIAL
  // UTILIZAR ESSA FUNCAO SOMENTE PARA TESTES ENQUANTO TESTES
  while (!Serial);
  Serial.println("Serial pronto");
  // INICIA CONFIGURACAO DE REDE POR DHCP
  if (Ethernet.begin(mac) == 0) {
    Serial.println("erro dhcp");
    // CASO FALHE EM BUSCAR DHCP SETA O IP MANUALMENTE
    Ethernet.begin(mac, ip);
  }

  // PRINTA ENDERECO IP
  Serial.print("Endereço IP: ");
  Serial.println(Ethernet.localIP());
  delay(1000);
  // START SERVIDOR WEB
  server.begin();
}

void loop() {
  //FICA ESCUTANDO E QUANDO TEM UM CLIENTE RESPONDE O JSON
  EthernetClient client = server.available();
  if (client) {
    bool success = readRequest(client);
    if (success) {
      StaticJsonBuffer<500> jsonBuffer;
      JsonArray& json = prepareResponse(jsonBuffer);
      writeResponse(client, json);
    }
    delay(1);
    client.stop();
  }

  // LE OS VALORES DOS SENSORES E ATRIBUI A VARIAVEL
  VAGA1 = digitalRead(SNR_VAGA1);
  VAGA2 = digitalRead(SNR_VAGA2);

  //CASO ALTERE OS SENSORES ALTERA O VALOR DOS LEDS
  if (VAGA1 == HIGH) // CASO A VAGA ESTEJA OCUPADA ACENDE LED VERMELHO E APAGA VERDE
  { 
    digitalWrite(VRM_VAGA1, HIGH);
    digitalWrite(VRD_VAGA1, LOW);
  }
  else  // CASO A VAGA ESTEJA LIVRE ACENDE LED VERDE E APAGA VERMELHO
  {
    digitalWrite(VRM_VAGA1, LOW);
    digitalWrite(VRD_VAGA1, HIGH);
  }

  if (VAGA2 == HIGH) // CASO A VAGA ESTEJA OCUPADA ACENDE LED VERMELHO E APAGA VERDE
  { 
    digitalWrite(VRM_VAGA2, HIGH);
    digitalWrite(VRD_VAGA2, LOW);
  }
  else  // CASO A VAGA ESTEJA LIVRE ACENDE LED VERDE E APAGA VERMELHO
  {
    digitalWrite(VRM_VAGA2, LOW);
    digitalWrite(VRD_VAGA2, HIGH);
  }
} 


// INICIA OS VALORES DOS LEDS 
void ledStart(){
  digitalWrite(VRM_VAGA1, LOW);
  digitalWrite(VRD_VAGA1, HIGH);
  digitalWrite(VRM_VAGA2, LOW);
  digitalWrite(VRD_VAGA2, HIGH);
}
