#include <Arduino.h>

#define GND_PIN 6
#define OUT_PIN 7


auto printer = [](void) {
	Serial.println("heatbeat");
};

void setup()
{	
	Serial.begin(115200);
	pinMode(GND_PIN, OUTPUT);
	digitalWrite(GND_PIN, LOW);

	pinMode(OUT_PIN, OUTPUT);
	digitalWrite(OUT_PIN, LOW);
}

void loop()
{
	delay(500);
	digitalWrite(OUT_PIN, HIGH);
	delay(500);
	digitalWrite(OUT_PIN, LOW);
	printer();	
}