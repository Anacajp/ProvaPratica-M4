#include <WiFi.h>
#include <HTTPClient.h>

#define led_verde 41 // Pino utilizado para controle do led verde
#define led_vermelho 40 // Pino utilizado para controle do led vermelho
#define led_amarelo 9 // Pino utiliza do para controle do led amarelo

const int buttonPin = 18;  // Declara o número do botão
int buttonState = 0;  // Variável para leitura do status do botão

const int ldrPin = 4;  // Declara o número relacioado sensor de luz
int threshold=600; // Limite de iluminação para o sensor

void setup() {

  // Configuração inicial dos pinos para controle dos leds como OUTPUTs (saídas) do ESP32
  pinMode(led_verde,OUTPUT);
  pinMode(led_vermelho,OUTPUT);
  pinMode(led_amarelo,OUTPUT);

  // Inicialização das entradas
  pinMode(buttonPin, INPUT); // Inicialização do botão como um input

  digitalWrite(led_verde, LOW);
  digitalWrite(led_vermelho, LOW);
  digitalWrite(led_amarelo, LOW);

  Serial.begin(9600); // Configuração para debug por interface serial entre ESP e computador com baud rate de 9600

  WiFi.begin("Wokwi-GUEST", ""); // Conexão à rede WiFi aberta com SSID Wokwi-GUEST

  // Define respostas em formato de output para caso de falha e de conexão  
  while (WiFi.status() != WL_CONNECT_FAILED) { 
    delay(100);
    Serial.print(".");
  }
  Serial.println("Conectado ao WiFi com sucesso!"); // Considerando que saiu do loop acima, o ESP32 agora está conectado ao WiFi (outra opção é colocar este comando dentro do if abaixo)

  // Verifica estado do botão
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {
    Serial.println("Botão pressionado!");
  } else {
    Serial.println("Botão não pressionado!");
  }

  if(WiFi.status() == WL_CONNECTED){ // Se o ESP32 estiver conectado à Internet
    HTTPClient http;

    String serverPath = "http://www.google.com.br/"; // Endpoint da requisição HTTP

    http.begin(serverPath.c_str());

    int httpResponseCode = http.GET(); // Código do Resultado da Requisição HTTP

    // Define respostas para a requisição HTTP em caso de erro e de conexão
    if (httpResponseCode>0) { 
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      String payload = http.getString();
      Serial.println(payload);
      }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
      }
      http.end();
    }

  else {
    Serial.println("WiFi Disconnected"); // caso não ocorra erro nem conexão, o output avisa que está desconectado
  }
}

// Condição necessária de luz para acionar o sensor 
void loop() {
  int ldrstatus=analogRead(ldrPin);

  if(ldrstatus<=threshold){ // caso a luminosidade esteja muito baixa
    Serial.print("its dark turn on led"); // mensagem recebida nessa situação 
    Serial.println(ldrstatus);
    digitalWrite(led_amarelo, HIGH); // liga o led amarelo 
    delay(1000); // espera com o led ligado por 1 segundo 
    digitalWrite(led_amarelo, LOW); // desliga o led amarelo

  }else{ // caso a luminosidade esteja como esperada (alta ou normal)
    Serial.print("its bright turn off light"); // mensagem recebida nessa situação 
    Serial.println(ldrstatus);

    digitalWrite(led_verde, HIGH); // liga o led verde
    delay(3000); // espera 3 segundos
    digitalWrite(led_verde, LOW); //desliga o led verde
    digitalWrite(led_amarelo, HIGH); // liga o led amarelo
    delay(2000);  // espera por 2 segundos
    digitalWrite(led_amarelo, LOW); // desliga o led amarelo
    digitalWrite(led_vermelho, HIGH); // liga o led vermelho
    delay(5000); // espera por 5 segundos
    digitalWrite(led_vermelho, LOW); // desliga o led vermelho
  }

  if(led_vermelho == HIGH && buttonPin == HIGH){ // condição para caso o vermelho esteja ligado e o botão for apertado
    delay(1000);
    digitalWrite(led_verde, HIGH);// Liga o led verde
    delay(3000);
    digitalWrite(led_verde, LOW);   // Desliga o led
  }

}

