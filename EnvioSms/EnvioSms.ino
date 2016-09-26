/*
 SMS sender

 This sketch, for the Arduino GSM shield,sends an SMS message
 you enter in the serial monitor. Connect your Arduino with the
 GSM shield and SIM card, open the serial monitor, and wait for
 the "READY" message to appear in the monitor. Next, type a
 message to send and press "return". Make sure the serial
 monitor is set to send a newline when you press return.

 Circuit:
 * GSM shield
 * SIM card that can send SMS

 created 25 Feb 2012
 by Tom Igoe

 This example is in the public domain.

 http://www.arduino.cc/en/Tutorial/GSMExamplesSendSMS

 */

// Include the GSM library
#include <GSM.h>

#define PINNUMBER ""// si el celular llega a tener PIN

// Inicializa instancias de la libreria
GSM gsmAccess;
GSM_SMS sms;
char ID [20] = {6,8,6,1,7,4,7,8,6,4}; // aqui el numero de emergencia
void setup() {
  // Inicializa comunicacion serial
  Serial.begin(9600);
  while (!Serial) {
    ; // 
  }

  Serial.println("SMS Remitente"); // quien envia el sms

  // connection state
  boolean notConnected = true;

  // Start GSM shield
  // If your SIM has PIN, pass it as a parameter of begin() in quotes
  while (notConnected) {
    if (gsmAccess.begin(PINNUMBER) == GSM_READY) {
      notConnected = false;
    } else {
      Serial.println("Not connected");
      delay(1000);
    }
  }

  Serial.println("GSM inicializado");
}

void loop() {
  // Lee el numero pregrabado ya sea emergencia o familiar
 // readSerial(ID); // envia el mensaje a la persona de confianza
   //o envia mensaje de emergencia a policia
  Serial.println(ID); // muestra el numero

  // texto sms
  char mensaje [8] = "Me perdi"; // mensaje para la persona de emergencia

  // envia el mensaje sms
  sms.beginSMS(ID); // ingresar aqui la var ID
  sms.print(mensaje);
  sms.endSMS();
  Serial.println("\nEnviado!\n");

}
