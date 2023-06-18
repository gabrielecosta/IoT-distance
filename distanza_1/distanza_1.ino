int trigPin = D3;
int echoPin = D2;

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  Serial.println(distance());
  delay(5000);
}

double distance() {
  digitalWrite(trigPin, LOW);
  digitalWrite(trigPin, HIGH);
  delay(1);
  digitalWrite(trigPin, LOW);
  int duration = pulseIn(echoPin, HIGH);
  return duration * 0.03431/2;
}
