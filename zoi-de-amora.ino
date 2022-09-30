int echo = 11;
int trigger = 12;
int buzzer = 5;

int analogic = 6;

int maxValueVibration = 192;

float soundSpeed = 0.034;
float duration;
float distance;
float focinho = 8;
float result;
float percentageAdvice;
int maxDistance = 50;
float soundTime;

void setup() {
  // put your setup code here, to run once:
  pinMode(echo, INPUT);
  pinMode(buzzer, OUTPUT);
  Serial.begin(9600);
}

int getVibrationValue(float percentage){
  float vibrationValue = maxValueVibration * percentage;
  int returnValue = (int)vibrationValue;
  Serial.println("percentage: " + (String)percentage + ". Vibration value: "+ (String)vibrationValue + ". Return value: " + (String)returnValue);
  return vibrationValue <= 59 ? 0 : returnValue;
}

void loop() {
  // put your main code here, to run repeatedly:

  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);
  delayMicroseconds(10);

  duration = pulseIn(echo, HIGH) / 2;
  distance = duration * soundSpeed;
  result = (distance - focinho);
  percentageAdvice = 1 - (result / maxDistance);
  soundTime = (200 * percentageAdvice);

  if(result >= 2 && result < (maxDistance)){
    analogWrite(analogic, getVibrationValue(percentageAdvice));
    tone(buzzer, 5000, soundTime);
    delay(soundTime);
    noTone;
    analogWrite(analogic, 0);
  }
  Serial.println("Distância: " + (String)distance + " cm. Distância resultante: " + (String)result + " cm. Sound time: " + (String)soundTime + ". Percentage advice: " + (String)percentageAdvice);
  delay(200);
}
