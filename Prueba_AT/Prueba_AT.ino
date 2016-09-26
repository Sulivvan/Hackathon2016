byte r_pin = A0 ;
int x = 0;
byte brightness = 0;
byte fadeamount = 0;
void setup() {
  pinMode(r_pin, INPUT);
  pinMode(11, OUTPUT);
  Serial.begin(9600);
}
void loop () {
  x = analogRead(r_pin);
  Serial.println(x);
  delay(1000);
  if (x > 270 ) {
    analogWrite(11, 0);
  }
  if (x < 250 ) {
    for(byte i = 5; i<255; i+15){
      analogWrite(11,i);
      
    }
      }
}
