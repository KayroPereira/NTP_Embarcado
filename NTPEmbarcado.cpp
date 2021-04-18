#include "NTPEmbarcado.h"

#define DELAY_NTP_GET	5000

long timeReference = 0;

WiFiUDP udp;															//Cria um objeto "UDP".
//NTPClient ntp(udp, "a.st1.ntp.br", -3 * 3600, 60000);					//Cria um objeto "NTP" com as configurações.
NTPClient ntp(udp, "0.br.pool.ntp.org", -3 * 3600, 60000);				//Cria um objeto "NTP" com as configurações.
String hora;

void startNTP(){
	ntp.begin();														//Inicia o NTP.
	ntp.forceUpdate();													//Força o Update.
}

void setup() {

	Serial.begin(57600);

	pinMode(LED_2, OUTPUT);

	WiFi.mode(WIFI_STA);
	WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
	Serial.printf("\n\nConnecting to Wi-Fi");
	while (WiFi.status() != WL_CONNECTED) {
		Serial.print(".");
		delay(300);
	}
	Serial.println();
	Serial.print("Connected with IP: ");
	Serial.println(WiFi.localIP());
	Serial.println();
	WiFi.setAutoReconnect(true);

	startNTP();
}

void updatePin(uint8_t pin, uint8_t val){
	digitalWrite(pin, val);
}

void loop() {

	if(millis() - timeReference > DELAY_NTP_GET){

		timeReference = millis();

		digitalWrite(LED_2, !digitalRead(LED_2));

		if(!ntp.update()){
			Serial.printf("\n\n---------- NTP desconectado ----------\n\n");
			startNTP();
		}else{
			hora = ntp.getFormattedTime();								//Armazena na váriavel HORA, o horario atual.
			Serial.println(hora);										//Printa a hora já formatada no monitor.
		}
	}
}
