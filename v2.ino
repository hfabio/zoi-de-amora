#include <Wire.h>
#include <Adafruit_VL53L0X.h>

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

const int motorPin = 2;  // Pino de controle do motor de vibração
const int buzzerPin = 12;  // Pino de controle do buzzer

int maxValueVibration = 192;
float focinho = 8;
float percentageAdvice;
int maxDistance = 50;
float soundTime;

void setup() {
  Serial.begin(9600);

  // Inicializa o sensor VL53L0X
  if (!lox.begin()) {
    Serial.println("Falha ao iniciar o sensor VL53L0X");
    while (1);
  }

  // Configura a taxa de aquisição do sensor
  lox.setMeasurementTimingBudgetMicroSeconds(200000);

  // Configura os pinos de controle como saídas
  pinMode(motorPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
}

int getVibrationValue(float percentage){
  float vibrationValue = maxValueVibration * percentage;
  int returnValue = (int)vibrationValue;
  Serial.println("percentage: " + (String)percentage + ". Vibration value: "+ (String)vibrationValue + ". Return value: " + (String)returnValue);
  return vibrationValue <= 59 ? 0 : returnValue;
}

void loop() {
  VL53L0X_RangingMeasurementData_t measure;

  // Faz a leitura da distância
  lox.rangingTest(&measure);
  double distancia = abs((measure.RangeMilliMeter/10) - focinho);
  percentageAdvice = (distancia / maxDistance);
  soundTime = (200 * percentageAdvice);
  // Imprime a distância lida no canal serial
  Serial.println("Distância: " + String(distancia) + " cm");
  if (measure.RangeStatus == 4){
    return;
  }
  if(distancia < maxDistance && distancia > 5){
    float vibrationSpeed = getVibrationValue(percentageAdvice);
    float buzzerDuration = soundTime;
    analogWrite(motorPin, vibrationSpeed);
    tone(buzzerPin, 5000, buzzerDuration);
    delay(soundTime);
    noTone;
    analogWrite(motorPin, 0);
    Serial.println("Deveria avisar, velocidade de vibração: " + String(vibrationSpeed) + " e duração do bipe " + String(buzzerDuration));
  }
  delay(50);  // Espera 500ms antes de fazer a próxima leitura
}
