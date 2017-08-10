#define pin_BOTON 9
#define pin_LED1 5
#define pin_LED2 4
#define pin_ENABLEM1 2
#define pin_ENABLEM2 13
#define pin_DIRM1 7   //motor izquierda
#define pin_DIRM2 A5  //motor derecha
#define pin_PWMM1 3
#define pin_PWMM2 10

#define pin_S1 A4     //sensor lateral izquierda
#define pin_S2 A3     //sensor centro izquierda
#define pin_S3 A2     //sensor medio
#define pin_S4 A1     //sensor centro derecha
#define pin_S5 A0 

#define Vel_M1 100
#define Vel_M2 100

void setup() {
  motoresInit();
}

void loop() {
  digitalWrite(pin_ENABLEM2, 1);   
  digitalWrite(pin_ENABLEM1, 1);
  digitalWrite(pin_DIRM1,0);
  digitalWrite(pin_DIRM2,0);
  analogWrite(pin_PWMM1,0);
  analogWrite(pin_PWMM2,0);
  delay(2000);

}

void motoresInit (){
  pinMode(pin_DIRM1, OUTPUT); //adelante con 0, atras con 1
  pinMode(pin_DIRM2, OUTPUT);
  pinMode(pin_ENABLEM1, OUTPUT);
  pinMode(pin_ENABLEM2, OUTPUT);
  digitalWrite(pin_ENABLEM1, Vel_M1);
  digitalWrite(pin_ENABLEM2, Vel_M2);
}
