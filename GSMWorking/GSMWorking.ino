/*
Herramientas Tecnologicas Profesionales
www.hetpro-store.com
 
Tutorial Original:
http://tronixstuff.com/2014/01/08/tutorial-arduino-and-sim900-gsm-modules/
 
Comandos AT:
http://probots.co.in/Manuals/SIM900%20GSM%20Modem%20-%20Starter%20Guide.pdf
 
Hoja de datos:
 
Instrucciones para Arduino: Configura la terminal serial a 19200 baudios para que puedas ver los mensajes.
*/
 
#include <SoftwareSerial.h>
SoftwareSerial SIM900(7, 8); // Configura el puerto serial para el SIM900
byte button =3;
char incoming_char=0; //Variable que guarda los caracteres que envia el SIM900
int salir = 0;
void setup()
{
  pinMode(button,INPUT);
SIM900.begin(19200); //Configura velocidad serial para el SIM900
delay(25000); //Retardo para que encuentra a una RED
Serial.begin(19200); //Configura velocidad serial para el Arduino
Serial.println("OK"); //Mensaje OK en el arduino, para saber que todo va bien.
}
 

void mensaje_sms(){

int x = digitalRead(button);
if(x == 1) {//Funcion para mandar mensaje de texto
SIM900.print("AT+CMGF=1\r"); // Comando AT para mandar mensaje
delay(100);
SIM900.println("AT+CMGS=\"6861787464\""); // numero del movil que se desea mandar msg
delay(100);
SIM900.println("Me perdi"); // mensaje a mandar
delay(100);
SIM900.println((char)26); //Comando de finalizacion
delay(100);
SIM900.println();
delay(5000); // Tiempo para que se envie el mensaje
Serial.println("SMS enviado correctamente");
}}
void loop()
{
  //llamar();
  mensaje_sms(); //Envia mensaje

delay(100);
SIM900.println();
delay(30000);
while(1); // Esperate por tiempo indefinido
 
}
