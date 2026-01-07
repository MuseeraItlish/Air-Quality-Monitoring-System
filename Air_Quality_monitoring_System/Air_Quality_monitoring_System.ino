#include <DHT.h> 
// DHT22 sensor connected to pin 2 
#define DHTTYPE DHT22 // Define DHT sensor type and pin 
#define DHTPIN 2 // DHT22 (AM2302) 
#define MQ135_PIN A2 // MQ135 sensor connected to analog pin A2 
#define MQ7_PIN A3 // MQ7 sensor connected to analog pin A3 
#define GP2Y_PIN A0 // GP2Y dust sensor connected to digital pin A0 
#define BUZZER_PIN 4 // Buzzer connected to pin 4 
#define LED_CO_PIN 6 // Yellow LED for CO level connected to pin 6 
#define LED_AQI_PIN 5 // Red LED for AQI level connected to pin 5 
// Variables for sensor data 
float temperature, humidity; 
float coLevel, aqi, dustDensity; 
DHT dht(DHTPIN, DHTTYPE); 
void setup() 
{ 
Serial.begin(9600); 
dht.begin(); 
// Initializing sensor pins 
pinMode(GP2Y_PIN, INPUT);
pinMode(BUZZER_PIN, OUTPUT); // Set buzzer pin as output 
pinMode(LED_CO_PIN, OUTPUT); // Set LED pin for CO as output 
pinMode(LED_AQI_PIN, OUTPUT); // Set LED pin for AQI as output 
// Debugging setup 
Serial.println("Setup complete. Initializing sensors..."); 
} 
void loop() 
{ 
// Read DHT22 sensor (temperature and humidity) 
humidity = dht.readHumidity(); 
temperature = dht.readTemperature(); // Celsius by default 
// Applying calibration for exact temperature reading 
temperature = temperature - 8.0; // Adjust for the +4°C error 
// Read MQ7 sensor (CO concentration) 
int coRaw = analogRead(MQ7_PIN); // Read analog value (CO concentration) 
coLevel = coRaw * (5000.0 / 1023.0); // Convert raw value to CO level in ppb 
// Read MQ135 sensor (Air quality index) 
int aqiRaw = analogRead(MQ135_PIN); // Read analog value (Air quality) 
aqi = aqiRaw * (10.0 / 1023.0); // Convert value of AQI to ppm
// Read GP2Y dust sensor (dust concentration) 
dustDensity = digitalRead(GP2Y_PIN); // Read dust sensor digital output 
// Print sensor data to Serial Monitor 
Serial.print("Temperature: "); 
Serial.print(temperature); 
Serial.print(" °C, Humidity: "); 
Serial.print(humidity); 
Serial.println(" %"); 
Serial.print("CO Level (MQ7) in ppb: "); 
Serial.println(coLevel); 
Serial.print("AQI (MQ135) in ppm: "); 
Serial.println(aqi); 
Serial.print("Dust Density (GP2Y): "); 
if (dustDensity == HIGH) 
{ 
Serial.println("High Dust Concentration"); 
} 
else 
{ 
Serial.println("Low Dust Concentration"); 
}
// Handle CO level alerts 
if (coLevel > 500) { // If CO level is above threshold 
Serial.println("Warning: High CO concentration!"); 
digitalWrite(LED_CO_PIN, HIGH); // Turn on the yellow CO LED 
beepBuzzer(); // Start the buzzer beeping 
} 
else 
{ 
digitalWrite(LED_CO_PIN, LOW); // Turn off the yellow CO LED 
} 
// Handle AQI alerts 
if (aqi > 4) { // Assuming 4 ppm is a poor air quality threshold 
Serial.println("Warning: Air Quality is Poor!"); 
digitalWrite(LED_AQI_PIN, HIGH); // Turn on the red AQI LED 
beepBuzzer(); // Start the buzzer beeping 
} 
else 
{ 
Serial.println("Air Quality is Good."); 
digitalWrite(LED_AQI_PIN, LOW); // Turn off the red AQI LED 
} 
// Handle dust concentration alerts
if (dustDensity == HIGH) { 
Serial.println("The air is polluted due to high dust concentration!"); 
beepBuzzer(); // Start the buzzer beeping 
} 
// If temperature exceeds 30°C, it's considered high temperature 
if (temperature > 30) 
{ 
Serial.println("Warning: High Temperature!"); 
beepBuzzer(); // Start the buzzer beeping 
} 
// If none of the thresholds are exceeded, turn off the buzzer 
if (coLevel <= 500 && aqi <= 4 && temperature <= 30 && dustDensity == LOW) 
{ 
digitalWrite(BUZZER_PIN, LOW); // Turn off the buzzer 
} 
delay(2000); // Delay between sensor readings 
} 
// Function to make the buzzer beep (start and stop) 
void beepBuzzer() 
{
digitalWrite(BUZZER_PIN, HIGH); // Turn on the buzzer 
delay(500); // Buzzer stays on for 500 ms 
digitalWrite(BUZZER_PIN, LOW); // Turn off the buzzer 
delay(500); // Buzzer stays off for 500 ms 
}


