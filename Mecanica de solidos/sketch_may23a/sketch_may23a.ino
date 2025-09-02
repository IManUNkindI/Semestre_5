// Definir los pines para los botones
const int btnPin1 = 2;
const int btnPin2 = 3;
const int btnPin3 = 4;
const int btnPin4 = 5;
const int btnPin5 = 6;
const int btnPin6 = 7;

// Definir los pines para los motores
const int motor1Pin1 = 8;
const int motor1Pin2 = 9;
const int motor2Pin1 = 10;
const int motor2Pin2 = 11;
const int motor3Pin1 = 12;
const int motor3Pin2 = 13;

void setup() {
  Serial.begin(9600);
  // Configurar los pines de los botones como entradas
  pinMode(btnPin1, INPUT);
  pinMode(btnPin2, INPUT);
  pinMode(btnPin3, INPUT);
  pinMode(btnPin4, INPUT);
  pinMode(btnPin5, INPUT);
  pinMode(btnPin6, INPUT);

  // Configurar los pines de los motores como salidas
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  pinMode(motor3Pin1, OUTPUT);
  pinMode(motor3Pin2, OUTPUT);
}

void loop() {
  // Leer el estado de los botones
  int btnState1 = digitalRead(btnPin1);
  int btnState2 = digitalRead(btnPin2);
  int btnState3 = digitalRead(btnPin3);
  int btnState4 = digitalRead(btnPin4);
  int btnState5 = digitalRead(btnPin5);
  int btnState6 = digitalRead(btnPin6);

  // Controlar la dirección de giro de los motores basado en el estado de los botones
  // Por ejemplo, si el botón 1 está presionado, el motor 1 gira en una dirección determinada
  // Si el botón 2 está presionado, el motor 1 gira en la dirección opuesta, y así sucesivamente

  // Motor 1
  if (btnState1 == HIGH) {
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW);
    Serial.println("1");
  } else if (btnState2 == HIGH) {
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH);
    Serial.println("2");
  } else {
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, LOW);
  }

  // Motor 2
  if (btnState3 == HIGH) {
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW);
    Serial.println("3");
  } else if (btnState4 == HIGH) {
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, HIGH);
    Serial.println("4");
  } else {
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, LOW);
  }

  // Motor 3
  if (btnState5 == HIGH) {
    digitalWrite(motor3Pin1, HIGH);
    digitalWrite(motor3Pin2, LOW);
    Serial.println("5");
  } else if (btnState6 == HIGH) {
    digitalWrite(motor3Pin1, LOW);
    digitalWrite(motor3Pin2, HIGH);
    Serial.println("6");
  } else {
    digitalWrite(motor3Pin1, LOW);
    digitalWrite(motor3Pin2, LOW);
  }
  delay(100);
}