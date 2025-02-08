#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

//#include <TimeLib.h>

const int sensorPinTemp = A0;
const int sensorPinLight = A1;
const int actuatorPinBuzzer = 8;

int temperatures[300];
int lights[300];

int time = 0;

int min = 99;
int max = -99;

int currentDay;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(sensorPinTemp, INPUT);
  pinMode(sensorPinLight, INPUT);

  String input = Serial.readStringUntil('\n');
  int hour, minute, second, day, month, year;
  sscanf(input.c_str(), "%d:%d:%d %d/%d/%d", &hour, &minute, &second, &day, &month, &year);
  currentDay = day;
}

void loop() {
  if (time == 300){
    time = 0;
  }

  String input = Serial.readStringUntil('\n');
  int hour, minute, second, day, month, year;
  sscanf(input.c_str(), "%d:%d:%d %d/%d/%d", &hour, &minute, &second, &day, &month, &year);

  int sensorValTemp = analogRead(sensorPinTemp);
  float voltageTemp = (sensorValTemp / 1024.0) * 5.0;
  int temp = (voltageTemp - 0.5) * 100;
  temperatures[time] = temp;

  int sensorValLight = analogRead(sensorPinLight);
  float voltageLight = sensorValLight / 4;
  int light = voltageLight;
  lights[time] = light;

  if ((time < 10) || (time == 30) || ((time % 60) == 0)){
    char tempToPrint[17] = "Temperature: ";
    char tempAvgStr[3] = "";
    int tempAvg = average(temperatures, time);
    sprintf(tempAvgStr, "%d", tempAvg);
    strcat(tempToPrint, tempAvgStr);
    lcd.clear();
    lcd.print(tempToPrint);

    char lightToPrint[17] = "Brightness: ";
    char lightAvgStr[4] = "";
    int lightAvg = average(lights, time);
    sprintf(lightAvgStr, "%d", lightAvg);
    strcat(lightToPrint, lightAvgStr);
    lcd.setCursor(0, 1);
    lcd.print(lightToPrint);

    if (time >= 60){
      if (lightAvg < 10){
        tone(actuatorPinBuzzer, 2000, 59000);
      }

      if (tempAvg < min){
        min = tempAvg;
      }
      if (tempAvg > max){
        max = tempAvg;
      }

      if (day != currentDay){
        Serial.print("Minimum = ");
        Serial.print(min);
        Serial.print(", Maximum = ");
        Serial.println(max);

        currentDay = day;
        min = 99;
        max = -99;
      }
    }
  }

  delay(1000);
  time++;
}

int average(int arr[], int time){
  int entries = 300;
  int sum = 0;
  if ((time + 1) < entries){
    entries = time + 1;
  }

  for (int i = 0; i < entries; i++){
    sum += arr[i];
  }

  int avg = sum / entries;
  return avg;
}
