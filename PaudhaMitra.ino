#define BLYNK_TEMPLATE_ID "TMPL3wJ2ib4-A"
#define BLYNK_TEMPLATE_NAME "PaudhaMitra"
#define BLYNK_AUTH_TOKEN "e76cZQ0NkG12bZ_zd0i9o0P8KkwX9ptD"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <math.h>

// --- Bitmap Definitions ---
const unsigned char happyPlant [] PROGMEM = {
  0b00000000, 0b00000000,
  0b00011111, 0b11100000,
  0b00100000, 0b00010000,
  0b01000110, 0b01100000,
  0b01001000, 0b00010000,
  0b10010000, 0b00001000,
  0b10010000, 0b00001000,
  0b10001000, 0b10001000,
  0b10001000, 0b10001000,
  0b10000111, 0b00001000,
  0b01000000, 0b00010000,
  0b01000000, 0b00010000,
  0b00100000, 0b00100000,
  0b00011111, 0b11100000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000
};

const unsigned char thirstyPlant [] PROGMEM = {
  0b00000000, 0b00000000,
  0b00111111, 0b11111100,
  0b01000000, 0b00000010,
  0b10010010, 0b01001001,
  0b10100000, 0b00000101,
  0b10100000, 0b00000101,
  0b10100100, 0b00100101,
  0b10100111, 0b11100101,
  0b10010000, 0b00001001,
  0b10010011, 0b11001001,
  0b01000000, 0b00000010,
  0b01001111, 0b11110010,
  0b00110000, 0b00001100,
  0b00011111, 0b11111000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000
};

const unsigned char sleepyPlant [] PROGMEM = {
  0b00000000, 0b00000000,
  0b00111111, 0b11111100,
  0b01000000, 0b00000010,
  0b10001001, 0b00100001,
  0b10000000, 0b00000001,
  0b10001001, 0b00100001,
  0b10000100, 0b01000001,
  0b10000100, 0b01000001,
  0b10000111, 0b11000001,
  0b10000000, 0b00000001,
  0b01000000, 0b00000010,
  0b01000000, 0b00000010,
  0b00111111, 0b11111100,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000
};

const unsigned char pollutedPlant [] PROGMEM = {
  0b00000000, 0b00000000,
  0b00111111, 0b11111100,
  0b01000000, 0b00000010,
  0b10011100, 0b00111001,
  0b10100010, 0b01000101,
  0b10100000, 0b00000101,
  0b10100000, 0b00000101,
  0b10101111, 0b11110101,
  0b10010101, 0b01010101,
  0b10010101, 0b01010101,
  0b01001111, 0b11110010,
  0b01000000, 0b00000010,
  0b00111111, 0b11111100,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000
};

const unsigned char hotPlant [] PROGMEM = {
  0b00000000, 0b00000000,
  0b00111111, 0b11111100,
  0b01000000, 0b00000010,
  0b10010010, 0b01001001,
  0b10100000, 0b00000101,
  0b10100000, 0b00000101,
  0b10100001, 0b10000101,
  0b10100000, 0b00000101,
  0b10010000, 0b00001001,
  0b10011100, 0b00111001,
  0b01000000, 0b00000010,
  0b01000100, 0b00100010,
  0b00111011, 0b11011100,
  0b00000100, 0b00100000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000
};

const unsigned char sadPlant [] PROGMEM = {
  0b00000000, 0b00000000,
  0b00111111, 0b11111100,
  0b01000000, 0b00000010,
  0b10010010, 0b01001001,
  0b10100000, 0b00000101,
  0b10100000, 0b00000101,
  0b10100000, 0b00000101,
  0b10100100, 0b00100101,
  0b10010011, 0b11001001,
  0b10010000, 0b00001001,
  0b01000000, 0b00000010,
  0b01001111, 0b11110010,
  0b00100000, 0b00000100,
  0b00011111, 0b11111000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000
};

const unsigned char neutralPlant [] PROGMEM = {
  0b00000000, 0b00000000,
  0b00111111, 0b11111100,
  0b01000000, 0b00000010,
  0b10010010, 0b01001001,
  0b10100000, 0b00000101,
  0b10100000, 0b00000101,
  0b10100100, 0b00100101,
  0b10100000, 0b00000101,
  0b10010000, 0b00001001,
  0b10010000, 0b00001001,
  0b01000000, 0b00000010,
  0b01011111, 0b11111010,
  0b00100000, 0b00000100,
  0b00011111, 0b11111000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000
};

char ssid[] = "IOT";
char pass[] = "abcdefgh";

#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define SOILPIN 34
#define LDRPIN 35
#define MQ135PIN 32

String emotion = "";
const unsigned char* currentBitmap = happyPlant;
float RZERO = 76.63;

BlynkTimer timer;

void showEmotionBitmap() {
  display.drawBitmap(96, 0, currentBitmap, 16, 16, WHITE);
}

int calculateAQI(float ppm) {
  float C_low, C_high;
  int I_low, I_high;

  if (ppm <= 400) {
    C_low = 0;     C_high = 400;
    I_low = 0;     I_high = 50;
  } else if (ppm <= 1000) {
    C_low = 401;   C_high = 1000;
    I_low = 51;    I_high = 100;
  } else if (ppm <= 2000) {
    C_low = 1001;  C_high = 2000;
    I_low = 101;   I_high = 150;
  } else if (ppm <= 3000) {
    C_low = 2001;  C_high = 3000;
    I_low = 151;   I_high = 200;
  } else if (ppm <= 4000) {
    C_low = 3001;  C_high = 4000;
    I_low = 201;   I_high = 300;
  } else {
    C_low = 4001;  C_high = 5000;
    I_low = 301;   I_high = 500;
  }

  return round(((ppm - C_low) * (I_high - I_low)) / (C_high - C_low) + I_low);
}



void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Booting...");

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED allocation failed!");
    while (true);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 10);
  display.println("PaudhaMittra");
  display.setCursor(0, 20);
  display.println("Connecting...");
  display.display();

  dht.begin();

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("WiFi connected!");
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  display.clearDisplay();
  display.setCursor(0, 10);
  display.println("PaudhaMittra");
  display.setCursor(0, 20);
  display.println("Connected!");
  display.display();
  delay(2000);

  timer.setInterval(5000L, sendSensorData);
  Serial.println("Setup complete.");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    Blynk.run();
  } else {
    Serial.println("WiFi disconnected, attempting to reconnect...");
    WiFi.reconnect();
    delay(5000);
  }
  timer.run();
}

void sendSensorData() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected, skipping sensor data transmission");
    return;
  }

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  int soilValue = analogRead(SOILPIN);
  int soilPercent = map(soilValue, 0, 4095, 100, 0);

  int ldrValue = analogRead(LDRPIN);
  int mqValue = analogRead(MQ135PIN);

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("DHT read failed!");
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("DHT Sensor Error!");
    display.display();
    return;
  }

  float voltage = mqValue * (3.3 / 4095.0);
  float Rs = (3.3 - voltage) * 10.0 / voltage;
  if (Rs <= 0) Rs = 0.1;
  float ratio = Rs / RZERO;
  float ppm = 116.6020682 * pow(ratio, -2.769034857);
  ppm = constrain(ppm, 0, 5000);  // Clamp to range
  int aqi = calculateAQI(ppm);   // Calculate AQI


  if (soilValue > 3500) {
  emotion = "Thirsty";
  currentBitmap = thirstyPlant;
} else if (temperature > 35) {
  emotion = "Hot";
  currentBitmap = hotPlant;
} else if (temperature < 10) {
  emotion = "Cold";
  currentBitmap = sleepyPlant;  // You can replace with coldPlant if created
} else if (ldrValue < 500 && ppm > 1500) {
  emotion = "Sad";
  currentBitmap = pollutedPlant;  // Replace with sadPlant bitmap if available
} else if (ldrValue < 1000) {
  emotion = "Sleepy";
  currentBitmap = sleepyPlant;
} else if (ppm > 1500) {
  emotion = "Stressed";
  currentBitmap = pollutedPlant;
} else if (abs(temperature - 25) < 2 && humidity > 45 && soilValue > 1200 && soilValue < 3000) {
  emotion = "Neutral";
  currentBitmap = happyPlant;  // You can assign a neutralPlant bitmap here
} else {
  emotion = "Happy";
  currentBitmap = happyPlant;
}


  Serial.println("--- PaudhaMittra Readings ---");
  Serial.printf("Temperature: %.2f C\n", temperature);
  Serial.printf("Humidity: %.2f %%\n", humidity);
  Serial.printf("Soil Moisture: %d %%\n", soilPercent);
  Serial.printf("Light Intensity: %d\n", ldrValue);
  Serial.printf("Air Quality (ppm): %.2f | AQI: %d\n", ppm, aqi);
  Serial.printf("Emotion: %s\n", emotion.c_str());
  Serial.println("-----------------------------");

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.printf("Temp: %.1f C\n", temperature);
  display.printf("Hum : %.1f %%\n", humidity);
  display.printf("Soil: %d %%\n", soilPercent);
  display.printf("Light: %d\n", ldrValue);
  display.printf("AQI : %d\n", aqi);
  display.setCursor(96, 0);
  display.drawBitmap(96, 16, currentBitmap, 16, 16, WHITE);
  display.display();
  showEmotionBitmap();

  if (Blynk.connected()) {
    Blynk.virtualWrite(V0, humidity);
    Blynk.virtualWrite(V1, temperature);
    Blynk.virtualWrite(V2, soilPercent);
    Blynk.virtualWrite(V3, ldrValue);
    //Blynk.virtualWrite(V4, (int)ppm);
    Blynk.virtualWrite(V5, emotion);
    Blynk.virtualWrite(V4, aqi);  

  } else {
    Serial.println("Blynk not connected, skipping data transmission");
  }
}