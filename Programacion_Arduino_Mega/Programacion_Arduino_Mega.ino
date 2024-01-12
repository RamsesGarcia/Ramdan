#include <SoftwareSerial.h>
#include <AccelStepper.h>
#include <Servo.h>

Servo servo01;
Servo servo02;
Servo servo03;
Servo servo04;
Servo servo05;
Servo servo06;

SoftwareSerial Bluetooth(A8, 38);  // Arduino(RX, TX) - HC-05 Bluetooth (TX, RX)

// Defina los motores paso a paso y los pines que usarán
AccelStepper LeftBackWheel(1, 42, 43);    // (Type:driver, STEP, DIR) - Stepper1
AccelStepper LeftFrontWheel(1, 40, 41);   // Stepper2
AccelStepper RightBackWheel(1, 44, 45);   // Stepper3
AccelStepper RightFrontWheel(1, 46, 47);  // Stepper4

#define led 14

int wheelSpeed = 1500;

int lbw[50], lfw[50], rbw[50], rfw[50];  //Array para almacenar posiciones/pasos

int servo1Pos, servo2Pos, servo3Pos, servo4Pos, servo5Pos, servo6Pos;                          // Posición actual
int servo1PPos, servo2PPos, servo3PPos, servo4PPos, servo5PPos, servo6PPos;                    // Posición anterior
int servo01SP[50], servo02SP[50], servo03SP[50], servo04SP[50], servo05SP[50], servo06SP[50];  // para almacenar posiciones/steps
int speedDelay = 20;
int index = 0;
int dataIn;
int m = 0;

void setup() {
  //Establecer valores de inicialización iniciales para los steppers
  LeftFrontWheel.setMaxSpeed(3000);
  LeftBackWheel.setMaxSpeed(3000);
  RightFrontWheel.setMaxSpeed(3000);
  RightBackWheel.setMaxSpeed(3000);
  pinMode(led, OUTPUT);
  servo01.attach(5);
  servo02.attach(6);
  servo03.attach(7);
  servo04.attach(8);
  servo05.attach(9);
  servo06.attach(10);
  Bluetooth.begin(9600);  //Velocidad de transmisión predeterminada del módulo Bluetooth
  Bluetooth.setTimeout(5);
  delay(20);
  Serial.begin(9600);
  // Mueve el brazo del robot a la posición inicial
  servo1PPos = 90;
  servo01.write(servo1PPos);
  servo2PPos = 100;
  servo02.write(servo2PPos);
  servo3PPos = 120;
  servo03.write(servo3PPos);
  servo4PPos = 95;
  servo04.write(servo4PPos);
  servo5PPos = 60;
  servo05.write(servo5PPos);
  servo6PPos = 110;
  servo06.write(servo6PPos);
}

void loop() {
  // Comprobar si hay datos entrantes de bluetooh
  if (Bluetooth.available() > 0) {
    dataIn = Bluetooth.read();  //Lee la informacion

    if (dataIn == 0) {
      m = 0;
    }
    if (dataIn == 1) {
      m = 1;
    }
    if (dataIn == 2) {
      m = 2;
    }
    if (dataIn == 3) {
      m = 3;
    }
    if (dataIn == 4) {
      m = 4;
    }
    if (dataIn == 5) {
      m = 5;
    }
    if (dataIn == 6) {
      m = 6;
    }
    if (dataIn == 7) {
      m = 7;
    }
    if (dataIn == 8) {
      m = 8;
    }
    if (dataIn == 9) {
      m = 9;
    }
    if (dataIn == 10) {
      m = 10;
    }
    if (dataIn == 11) {
      m = 11;
    }
    if (dataIn == 12) {
      m = 12;
    }
    if (dataIn == 14) {
      m = 14;
    }
    if (dataIn == 16) {
      m = 16;
    }
    if (dataIn == 17) {
      m = 17;
    }
    if (dataIn == 18) {
      m = 18;
    }
    if (dataIn == 19) {
      m = 19;
    }
    if (dataIn == 20) {
      m = 20;
    }
    if (dataIn == 21) {
      m = 21;
    }
    if (dataIn == 22) {
      m = 22;
    }
    if (dataIn == 23) {
      m = 23;
    }
    if (dataIn == 24) {
      m = 24;
    }
    if (dataIn == 25) {
      m = 25;
    }
    if (dataIn == 26) {
      m = 26;
    }
    if (dataIn == 27) {
      m = 27;
    }

    //Move las llantas
    if (m == 4) {
      moveSidewaysLeft(); //moverse lateralmente a la izquierda
    }
    if (m == 5) {
      moveSidewaysRight(); //moverse lateralmente a la derecha
    } 
    if (m == 2) {
      moveForward(); //avanza
    }
    if (m == 7) {
      moveBackward(); //retrocede
    }
    if (m == 3) {
      moveRightForward(); //Avanzar a la derecha
    }
    if (m == 1) {
      moveLeftForward(); //Avanzar a la izquierda
    }
    if (m == 8) {
      moveRightBackward(); //Avanzar a la derecha-contrario
    }
    if (m == 6) {
      moveLeftBackward(); //Avanzar a la izquierda-controario
    }
    if (m == 9) {
      rotateLeft(); //Rota a la izquierda
    }
    if (m == 10) {
      rotateRight(); //Rota a la derecha 
    }
    if (m == 0) {
      stopMoving(); //Detiene los movimientos si no se presiona agun boton
    }

    // Velocidad de las ruedas
    if (dataIn > 30 & dataIn < 100) {
      wheelSpeed = dataIn * 20;
    }

    // Para mover el brazo
    // Mueve el servo 1 en posicion positiva
    while (m == 16) {
      if (Bluetooth.available() > 0) {
        m = Bluetooth.read();
      }
      servo01.write(servo1PPos);
      servo1PPos++;
      delay(speedDelay);
    }
    //Mueve servo 1 en posicion negativa
    while (m == 17) {
      if (Bluetooth.available() > 0) {
        m = Bluetooth.read();
      }
      servo01.write(servo1PPos);
      servo1PPos--;
      delay(speedDelay);
    }
    //Mueve servo 2
    while (m == 19) {
      if (Bluetooth.available() > 0) {
        m = Bluetooth.read();
      }
      servo02.write(servo2PPos);
      servo2PPos++;
      delay(speedDelay);
    }
    while (m == 18) {
      if (Bluetooth.available() > 0) {
        m = Bluetooth.read();
      }
      servo02.write(servo2PPos);
      servo2PPos--;
      delay(speedDelay);
    }
    // Mueve servo 3
    while (m == 20) {
      if (Bluetooth.available() > 0) {
        m = Bluetooth.read();
      }
      servo03.write(servo3PPos);
      servo3PPos++;
      delay(speedDelay);
    }
    while (m == 21) {
      if (Bluetooth.available() > 0) {
        m = Bluetooth.read();
      }
      servo03.write(servo3PPos);
      servo3PPos--;
      delay(speedDelay);
    }
    // Mueve servo 4
    while (m == 23) {
      if (Bluetooth.available() > 0) {
        m = Bluetooth.read();
      }
      servo04.write(servo4PPos);
      servo4PPos++;
      delay(speedDelay);
    }
    while (m == 22) {
      if (Bluetooth.available() > 0) {
        m = Bluetooth.read();
      }
      servo04.write(servo4PPos);
      servo4PPos--;
      delay(speedDelay);
    }
    // Mueve servo 5
    while (m == 25) {
      if (Bluetooth.available() > 0) {
        m = Bluetooth.read();
      }
      servo05.write(servo5PPos);
      servo5PPos++;
      delay(speedDelay);
    }
    while (m == 24) {
      if (Bluetooth.available() > 0) {
        m = Bluetooth.read();
      }
      servo05.write(servo5PPos);
      servo5PPos--;
      delay(speedDelay);
    }
    // Mueve servo 6
    while (m == 26) {
      if (Bluetooth.available() > 0) {
        m = Bluetooth.read();
      }
      servo06.write(servo6PPos);
      servo6PPos++;
      delay(speedDelay);
    }
    while (m == 27) {
      if (Bluetooth.available() > 0) {
        m = Bluetooth.read();
      }
      servo06.write(servo6PPos);
      servo6PPos--;
      delay(speedDelay);
    }

    // Si se cambia el control deslizante de velocidad del brazo
    if (dataIn > 101 & dataIn < 250) {
      speedDelay = dataIn / 10;  // Cambiar la velocidad del servo (tiempo de retardo)
    }

    // Si se presiona el botón "GUARDAR"
    if (m == 12) {
      //Si se trata de un guardado inicial, establezca la posición de los pasos en 0
      if (index == 0) {
        LeftBackWheel.setCurrentPosition(0);
        LeftFrontWheel.setCurrentPosition(0);
        RightBackWheel.setCurrentPosition(0);
        RightFrontWheel.setCurrentPosition(0);
      }
      lbw[index] = LeftBackWheel.currentPosition();  // Guarda la posición en la matriz
      lfw[index] = LeftFrontWheel.currentPosition();
      rbw[index] = RightBackWheel.currentPosition();
      rfw[index] = RightFrontWheel.currentPosition();

      servo01SP[index] = servo1PPos;  // Guarda la posición en la matriz
      servo02SP[index] = servo2PPos;
      servo03SP[index] = servo3PPos;
      servo04SP[index] = servo4PPos;
      servo05SP[index] = servo5PPos;
      servo06SP[index] = servo6PPos;
      index++;  //  Aumentar el índice de matriz
      m = 0;
    }

    // Si se presiona el botón "RUN"
    if (m == 14) {
      runSteps();

      // Si se presiona el botón "RESET"
      if (dataIn != 14) {
        stopMoving();
        memset(lbw, 0, sizeof(lbw));  // Borra los datos de la matriz a 0
        memset(lfw, 0, sizeof(lfw));
        memset(rbw, 0, sizeof(rbw));
        memset(rfw, 0, sizeof(rfw));
        memset(servo01SP, 0, sizeof(servo01SP));  // Borra los datos de la matriz a 0
        memset(servo02SP, 0, sizeof(servo02SP));
        memset(servo03SP, 0, sizeof(servo03SP));
        memset(servo04SP, 0, sizeof(servo04SP));
        memset(servo05SP, 0, sizeof(servo05SP));
        memset(servo06SP, 0, sizeof(servo06SP));
        index = 0;  //Índice a 0
      }
    }
  }

  LeftFrontWheel.runSpeed();
  LeftBackWheel.runSpeed();
  RightFrontWheel.runSpeed();
  RightBackWheel.runSpeed();

  // Controla el voltaje de la batería
  int sensorValue = analogRead(A0);
  float voltage = sensorValue * (5.0 / 1023.00) * 3;  // Convierte los valores de lectura de 5v a 12V
  //Serial.println(voltage);
  //Si el voltaje es inferior a 11 V, encienda el LED
  if (voltage < 11) {
    digitalWrite(led, HIGH);
  } else {
    digitalWrite(led, LOW);
  }
}


void moveForward() {
  LeftFrontWheel.setSpeed(wheelSpeed);
  LeftBackWheel.setSpeed(wheelSpeed);
  RightFrontWheel.setSpeed(wheelSpeed);
  RightBackWheel.setSpeed(wheelSpeed);
}
void moveBackward() {
  LeftFrontWheel.setSpeed(-wheelSpeed);
  LeftBackWheel.setSpeed(-wheelSpeed);
  RightFrontWheel.setSpeed(-wheelSpeed);
  RightBackWheel.setSpeed(-wheelSpeed);
}
void moveSidewaysRight() {
  LeftFrontWheel.setSpeed(wheelSpeed);
  LeftBackWheel.setSpeed(-wheelSpeed);
  RightFrontWheel.setSpeed(-wheelSpeed);
  RightBackWheel.setSpeed(wheelSpeed);
}
void moveSidewaysLeft() {
  LeftFrontWheel.setSpeed(-wheelSpeed);
  LeftBackWheel.setSpeed(wheelSpeed);
  RightFrontWheel.setSpeed(wheelSpeed);
  RightBackWheel.setSpeed(-wheelSpeed);
}
void rotateLeft() {
  LeftFrontWheel.setSpeed(-wheelSpeed);
  LeftBackWheel.setSpeed(-wheelSpeed);
  RightFrontWheel.setSpeed(wheelSpeed);
  RightBackWheel.setSpeed(wheelSpeed);
}
void rotateRight() {
  LeftFrontWheel.setSpeed(wheelSpeed);
  LeftBackWheel.setSpeed(wheelSpeed);
  RightFrontWheel.setSpeed(-wheelSpeed);
  RightBackWheel.setSpeed(-wheelSpeed);
}
void moveRightForward() {
  LeftFrontWheel.setSpeed(wheelSpeed);
  LeftBackWheel.setSpeed(0);
  RightFrontWheel.setSpeed(0);
  RightBackWheel.setSpeed(wheelSpeed);
}
void moveRightBackward() {
  LeftFrontWheel.setSpeed(0);
  LeftBackWheel.setSpeed(-wheelSpeed);
  RightFrontWheel.setSpeed(-wheelSpeed);
  RightBackWheel.setSpeed(0);
}
void moveLeftForward() {
  LeftFrontWheel.setSpeed(0);
  LeftBackWheel.setSpeed(wheelSpeed);
  RightFrontWheel.setSpeed(wheelSpeed);
  RightBackWheel.setSpeed(0);
}
void moveLeftBackward() {
  LeftFrontWheel.setSpeed(-wheelSpeed);
  LeftBackWheel.setSpeed(0);
  RightFrontWheel.setSpeed(0);
  RightBackWheel.setSpeed(-wheelSpeed);
}
void stopMoving() {
  LeftFrontWheel.setSpeed(0);
  LeftBackWheel.setSpeed(0);
  RightFrontWheel.setSpeed(0);
  RightBackWheel.setSpeed(0);
}

// Función personalizada para modo automático - ejecuta los pasos guardados
void runSteps() {
  while (dataIn != 13) {                    // Ejecute los pasos una y otra vez hasta que se presione el botón "RESET"
    for (int i = 0; i <= index - 2; i++) {  // Ejecutar todos los pasos (index)
      if (Bluetooth.available() > 0) {      // Comprobar si hay datos incompatibles
        dataIn = Bluetooth.read();
        if (dataIn == 15) {       // Si se presiona el botón "PAUSA"
          while (dataIn != 14) {  // Espera hasta que se vuelva a presionar "RUN"
            if (Bluetooth.available() > 0) {
              dataIn = Bluetooth.read();
              if (dataIn == 13) {
                break;
              }
            }
          }
        }
        // Si se cambia el control deslizante de velocidad
        if (dataIn > 100 & dataIn < 150) {
          speedDelay = dataIn / 10;  //Cambiar la velocidad del servo (tiempo de retardo)
        }
        // Velocidad de las ruedas 
        if (dataIn > 30 & dataIn < 100) {
          wheelSpeed = dataIn * 10;
          dataIn = 14;
        }
      }
      LeftFrontWheel.moveTo(lfw[i]);
      LeftFrontWheel.setSpeed(wheelSpeed);
      LeftBackWheel.moveTo(lbw[i]);
      LeftBackWheel.setSpeed(wheelSpeed);
      RightFrontWheel.moveTo(rfw[i]);
      RightFrontWheel.setSpeed(wheelSpeed);
      RightBackWheel.moveTo(rbw[i]);
      RightBackWheel.setSpeed(wheelSpeed);

      while (LeftBackWheel.currentPosition() != lbw[i] & LeftFrontWheel.currentPosition() != lfw[i] & RightFrontWheel.currentPosition() != rfw[i] & RightBackWheel.currentPosition() != rbw[i]) {
        LeftFrontWheel.runSpeedToPosition();
        LeftBackWheel.runSpeedToPosition();
        RightFrontWheel.runSpeedToPosition();
        RightBackWheel.runSpeedToPosition();
      }
      // Servo 1
      if (servo01SP[i] == servo01SP[i + 1]) {
      }
      if (servo01SP[i] > servo01SP[i + 1]) {
        for (int j = servo01SP[i]; j >= servo01SP[i + 1]; j--) {
          servo01.write(j);
          delay(speedDelay);
        }
      }
      if (servo01SP[i] < servo01SP[i + 1]) {
        for (int j = servo01SP[i]; j <= servo01SP[i + 1]; j++) {
          servo01.write(j);
          delay(speedDelay);
        }
      }

      // Servo 2
      if (servo02SP[i] == servo02SP[i + 1]) {
      }
      if (servo02SP[i] > servo02SP[i + 1]) {
        for (int j = servo02SP[i]; j >= servo02SP[i + 1]; j--) {
          servo02.write(j);
          delay(speedDelay);
        }
      }
      if (servo02SP[i] < servo02SP[i + 1]) {
        for (int j = servo02SP[i]; j <= servo02SP[i + 1]; j++) {
          servo02.write(j);
          delay(speedDelay);
        }
      }

      // Servo 3
      if (servo03SP[i] == servo03SP[i + 1]) {
      }
      if (servo03SP[i] > servo03SP[i + 1]) {
        for (int j = servo03SP[i]; j >= servo03SP[i + 1]; j--) {
          servo03.write(j);
          delay(speedDelay);
        }
      }
      if (servo03SP[i] < servo03SP[i + 1]) {
        for (int j = servo03SP[i]; j <= servo03SP[i + 1]; j++) {
          servo03.write(j);
          delay(speedDelay);
        }
      }

      // Servo 4
      if (servo04SP[i] == servo04SP[i + 1]) {
      }
      if (servo04SP[i] > servo04SP[i + 1]) {
        for (int j = servo04SP[i]; j >= servo04SP[i + 1]; j--) {
          servo04.write(j);
          delay(speedDelay);
        }
      }
      if (servo04SP[i] < servo04SP[i + 1]) {
        for (int j = servo04SP[i]; j <= servo04SP[i + 1]; j++) {
          servo04.write(j);
          delay(speedDelay);
        }
      }

      // Servo 5
      if (servo05SP[i] == servo05SP[i + 1]) {
      }
      if (servo05SP[i] > servo05SP[i + 1]) {
        for (int j = servo05SP[i]; j >= servo05SP[i + 1]; j--) {
          servo05.write(j);
          delay(speedDelay);
        }
      }
      if (servo05SP[i] < servo05SP[i + 1]) {
        for (int j = servo05SP[i]; j <= servo05SP[i + 1]; j++) {
          servo05.write(j);
          delay(speedDelay);
        }
      }

      // Servo 6
      if (servo06SP[i] == servo06SP[i + 1]) {
      }
      if (servo06SP[i] > servo06SP[i + 1]) {
        for (int j = servo06SP[i]; j >= servo06SP[i + 1]; j--) {
          servo06.write(j);
          delay(speedDelay);
        }
      }
      if (servo06SP[i] < servo06SP[i + 1]) {
        for (int j = servo06SP[i]; j <= servo06SP[i + 1]; j++) {
          servo06.write(j);
          delay(speedDelay);
        }
      }
    }
  }
}