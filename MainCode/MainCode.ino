#include <Servo.h>
#include <Wire.h>
#include <TimerOne.h>
#include <SoftwareSerial.h>

#define    MPU9250_ADDRESS            0x68
#define    MAG_ADDRESS                0x0C

#define    GYRO_FULL_SCALE_250_DPS    0x00
#define    GYRO_FULL_SCALE_500_DPS    0x08
#define    GYRO_FULL_SCALE_1000_DPS   0x10
#define    GYRO_FULL_SCALE_2000_DPS   0x18

#define    ACC_FULL_SCALE_2_G        0x00
#define    ACC_FULL_SCALE_4_G        0x08
#define    ACC_FULL_SCALE_8_G        0x10
#define    ACC_FULL_SCALE_16_G       0x18

///// ultrasonico 1 eje Y
#define trigPin 13
#define echoPin 12

/// ultrasonicos 2 y 3 eje X
#define trigPin9 5
#define echoPin8 4

#define trigPin7 3
#define echoPin6 2
int callpin = 1;
Servo myservo, myservo1;
/// servo conectado a
int val;
int prevVal;

/// servo1 conectado a

int val1;
int prevVal1;

/// motor1 front
int motorPin = 9;

//motor2 izquierda
int motorPin1 = 6;

/// motor3 derecha
int motorPin2 = 1;


// Lee bytes y comienza el registro para el arreglo DATA
void I2Cread(uint8_t Address, uint8_t Register, uint8_t Nbytes, uint8_t* Data)
{
  // Set register address
  Wire.beginTransmission(Address);
  Wire.write(Register);
  Wire.endTransmission();


  Wire.requestFrom(Address, Nbytes);
  uint8_t index = 0;
  while (Wire.available())
    Data[index++] = Wire.read();
}


// Data addresss registro
void I2CwriteByte(uint8_t Address, uint8_t Register, uint8_t Data)
{
  // Set register address
  Wire.beginTransmission(Address);
  Wire.write(Register);
  Wire.write(Data);
  Wire.endTransmission();
}

// Initial time
long int ti;
volatile bool intFlag = false;

SoftwareSerial SIM900(7, 8); //Rx Tx
byte emerg = 0;


void setup() {
  pinMode(callpin,INPUT);
  Wire.begin();
  Serial.begin(19200);
  Serial.begin(115200);
  SIM900.begin(19200);
  Serial.println("OK");
  I2CwriteByte(MPU9250_ADDRESS, 29, 0x06);
  I2CwriteByte(MPU9250_ADDRESS, 26, 0x06);


  // Configure gyroscope range
  I2CwriteByte(MPU9250_ADDRESS, 27, GYRO_FULL_SCALE_1000_DPS);
  // Configure accelerometers range
  I2CwriteByte(MPU9250_ADDRESS, 28, ACC_FULL_SCALE_4_G);
  // Set by pass mode for the magnetometers
  I2CwriteByte(MPU9250_ADDRESS, 0x37, 0x02);

  pinMode(13, OUTPUT);
  Timer1.initialize(10000);
  Timer1.attachInterrupt(callback);

  ti = millis();
  myservo.attach(10);
  myservo1.attach(11);
  delay(1000);
  Serial.begin (9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  //// ultrasonico 2 y 3
  pinMode(trigPin9, OUTPUT);
  pinMode(echoPin8, INPUT);

  pinMode(trigPin7, OUTPUT);
  pinMode(echoPin6, INPUT);

  // motor1 front
  pinMode(motorPin, OUTPUT);
  // Motor2 izquierdo
  pinMode(motorPin1, OUTPUT);
  /// Motor3 derecho
  pinMode(motorPin2, OUTPUT);

}
long int cpt = 0;
void EnviarMsg() {
  emerg = digitalRead(callpin);
  if (emerg == 1) {
    SIM900.print("AT+CMGF=1\r"); // Comando AT para mandar mensaje
    delay(100);
    SIM900.println("AT+CMGS=\"6861787464\""); // numero del movil que se desea mandar msg
    delay(100);
    SIM900.println("Me extravie mi posicion es: "); // mensaje a mandar
    delay(100);
    SIM900.println((char)26); //Comando de finalizacion
    delay(100);
    SIM900.println();
    delay(5000); // Tiempo para que se envie el mensaje
    Serial.println("SMS enviado correctamente");
  }
}
void callback()
{
  intFlag = true;
  digitalWrite(13, digitalRead(13) ^ 1);
}
void start() {

  uint8_t Buf[14];
  I2Cread(MPU9250_ADDRESS, 0x3B, 14, Buf);


  // Accelerometer
  int16_t ax = (Buf[0] << 8 | Buf[1]);
  int16_t ay = -(Buf[2] << 8 | Buf[3]);
  int16_t az = Buf[4] << 8 | Buf[5];

  // Gyroscope
  int16_t gx = -(Buf[8] << 8 | Buf[9]);
  int16_t gy = -(Buf[10] << 8 | Buf[11]);
  int16_t gz = Buf[12] << 8 | Buf[13];

  // Display values
  /*
    // Accelerometer
    Serial.print (ax,DEC);
    Serial.print ("\t");
    Serial.print (ay,DEC);
    Serial.print ("\t");
    Serial.print (az,DEC);
    Serial.print ("\t");

    // Gyroscope
    Serial.print (gx,DEC);
    Serial.print ("\t");
    Serial.print (gy,DEC);
    Serial.print ("\t");
    Serial.print (gz,DEC);
    Serial.print ("\t");
  */
  val = map(ay, -7000, 7000, 0, 179);
  if (val != prevVal) {

    myservo.write(val);
    prevVal = val;
  }

  val1 = map(ax, -7000, 7000, 0, 179);
  if (val != prevVal1); {
    myservo1.write(val1);
    prevVal1 = val1;

  }

  //// sensor ultrasonico

  float duration, distance, duration1, distance1, duration2, distance2;
  //// ultrasonico eje Y 1
  digitalWrite(trigPin, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin, HIGH);

  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.1;

  ////// ultrasonico eje X 2 y 3
  ///ultrasico 2 izquierda
  digitalWrite(trigPin9, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin9, HIGH);

  delayMicroseconds(10);
  digitalWrite(trigPin9, LOW);
  duration1 = pulseIn(echoPin8, HIGH);
  distance1 = (duration1 / 2) / 29.1;

  // ultrasonico 3 derecha

  digitalWrite(trigPin7, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin7, HIGH);

  delayMicroseconds(10);
  digitalWrite(trigPin7, LOW);
  duration2 = pulseIn(echoPin6, HIGH);
  distance2 = (duration2 / 2) / 29.1;


  //// Impresion distancia centimetros
  Serial.print("Distancia sensor 1 frontal: ");
  Serial.print(distance);
  Serial.println(" cm");

  Serial.print ( "Distancia sensor 2 izquierda: " );
  Serial.print(distance1);
  Serial.println(" cm");

  Serial.print( "Distancia sensor 3 derecha: ");
  Serial.print(distance2);
  Serial.println(" cm");

  //// Motores
  /// Motor 1 front
  /*int motorVal;
    int motorVal1;
    motorVal = map(distance,0, 400, 0, 255);
    analogWrite(motorPin, motorVal);*/
  if (distance < 200 ) {
    digitalWrite(motorPin, HIGH);

  } else {
    digitalWrite(motorPin, LOW);
  }

  /// Motor 2 izquierdo

  if (distance1 < 200) {
    digitalWrite(motorPin1, HIGH);
  } else {
    digitalWrite(motorPin1, LOW);
  }

  if (distance2 < 200) {
    digitalWrite(motorPin2, HIGH);
  } else {
    digitalWrite(motorPin2, LOW);
  }

  delay(500);
}


void loop() {
  start(); // comienza el sistema
  EnviarMsg();
}
