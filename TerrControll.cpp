// Do not remove the include below
#include "TerrControll.h"

#define DHTPIN 2
#define DHTTYPE DHT22
#define RELAY_PIN_1 3 // heating relay
#define RELAY_PIN_2 4

byte degree[8] = { B1110, B1010, B1110, B0, B0, B0, B0 };

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
RTC_DS1307 RTC;
int MAX_TEMP = 31;
int MIN_TEMP = 29;

int hourToSwitchON = 8;
int hourToSwitchOFF = 18;
boolean LIGHT_STATE = false;
boolean HEATING_STATE = LOW;
boolean AUTO_MODE = true;

void setup() {
	pinMode(RELAY_PIN_1, OUTPUT);
	switchHeatingOn();
	Serial.begin(115200);
	dht.begin();

	lcd.createChar(0, degree);
	lcd.begin(16, 2);

	pinMode(17, OUTPUT);
	digitalWrite(17, HIGH);

	pinMode(16, OUTPUT);
	digitalWrite(16, LOW);

	Wire.begin();
	RTC.begin();

	RTC.adjust(DateTime(__DATE__, __TIME__));

}

void switchHeatingOff() {
//	Serial.print("OFF");
	HEATING_STATE = HIGH;
	digitalWrite(RELAY_PIN_1, HEATING_STATE);
}

void switchHeatingOn() {
//	Serial.print("ON");
	HEATING_STATE = LOW;
	digitalWrite(RELAY_PIN_1, HEATING_STATE);
}

void sendSerialMessage(float h, float t) {
	Serial.print(h);
	Serial.print(',');
	Serial.print(t);
	Serial.print(',');
	Serial.print(t);
	Serial.print(',');
	Serial.print(AUTO_MODE);
	Serial.print(',');
	Serial.print(LIGHT_STATE);
	Serial.print(',');
	Serial.print(!HEATING_STATE);
}

void loop() {

	DateTime now = RTC.now();

	boolean heatingTime;
	uint8_t hour = now.hour();
	uint8_t minute = now.minute();
	if(hour >= hourToSwitchON && hour < hourToSwitchOFF){
		heatingTime = true;
	} else {
		heatingTime = false;
		switchHeatingOff();
	}
	delay(3000);
	float h = dht.readHumidity();
	float t = dht.readTemperature();
	// check if returns are valid, if they are NaN (not a number) then something went wrong!
	if (isnan(t) || isnan(h)) {
		Serial.println("ERROR");
	} else {
		if (AUTO_MODE && heatingTime) {
			if (t > MAX_TEMP) {
				switchHeatingOff();
			}
			if (t < MIN_TEMP) {
				switchHeatingOn();
			}
		}

		sendSerialMessage(h, t);
		printLCD((int) t, (int) h);
		delay(3000);
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print(hour);
		lcd.print(":");
		lcd.print(minute);
	}
	delay(3000);
}

void printLCD(int temp, int hum) {
	lcd.clear();

	lcd.setCursor(0, 0);
	lcd.print("Humidity:   %");
	lcd.setCursor(10, 0);
	lcd.print(hum);

	lcd.setCursor(0, 1);
	lcd.print("L:    C, R:    C");
	lcd.setCursor(3, 1);
	lcd.print(temp);

	lcd.setCursor(5, 1);
	lcd.write(byte(0));
	lcd.setCursor(14, 1);
	lcd.write(byte(0));


}

