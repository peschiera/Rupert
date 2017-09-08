#define DEBUG_MODE 0

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
#define pin_S5 A0     //sensor lateral derecha

int S1, S2, S3, S4, S5;   
boolean flag=0;
int KP=5; //valores que ayustan la velocidad dependiendo el error.
int KD=8;
int error_ant=0;

//Espectro de Lectura de Sensores
int Umbral=100;
//int UmbralS2=40;

//Velocidades motores (maximo valor posible 255)
int Vmed1=140;   //Velocidad medio
int Vmed2=120;   //Velocidad medio
int Vatras=100; //Velocidad para atras
void setup() { //se ejecuta una sola vez
  Serial.begin(9600);
  pinMode(pin_BOTON,INPUT_PULLUP);      //los utilizo como un pin digital
  pinMode(pin_LED1, OUTPUT);
  pinMode(pin_LED2, OUTPUT);  
  motoresInit();
}

void loop() {
  if(digitalRead(pin_BOTON)==0) //aprieto el boton y cambio el estado del flag(booleano)
  {
    flag=!flag;   //el flag empieza con valor = 0
    delay(200);
    OnOff(flag);
  }
  moverMotores(Error(Sensores()));
}


void OnOff (boolean state){
  if(state==1){
    digitalWrite(pin_ENABLEM2, 1);   
    digitalWrite(pin_ENABLEM1, 1);
    leds(1,1);}
  else{
    digitalWrite(pin_ENABLEM2, 0);
    digitalWrite(pin_ENABLEM1, 0);
    leds(0,0);}
}

void leds(byte a, byte b){
  digitalWrite(pin_LED1, a);
  digitalWrite(pin_LED2, b);
}

void motoresInit (){
  pinMode(pin_DIRM1, OUTPUT); //adelante con 0, atras con 1
  pinMode(pin_DIRM2, OUTPUT);
  pinMode(pin_ENABLEM1, OUTPUT);
  pinMode(pin_ENABLEM2, OUTPUT);
  digitalWrite(pin_ENABLEM1, 0); //los inicializo con 0 porque se que estan apagados en este valor
  digitalWrite(pin_ENABLEM2, 0);
}

int Sensores() {
  int lectura;
  //Serial.println("lateral derecha");
  S1=analogRead(pin_S1);  //S1=digitalRead(pin_S1);
  //Serial.println(S1); 
  if (S1>Umbral) {
    S1=1; }
  else  {
    if(S1<=Umbral) {
      S1=0; }
    }
      //Si esta sobre negro vale "1" si esta sobre blanco "0"
        
  //Serial.println("centro derecha");
  S2=analogRead(pin_S2);
  //Serial.println(S2);
  if (S2>Umbral) {
    S2=1; }
  else  {
    if(S2<=Umbral) {
      S2=0; }
    }
  
  
  //Serial.println("medio");
  S3=analogRead(pin_S3);
  //Serial.println(S3);
  if (S3>Umbral) {
    S3=1; }
  else  {
    if(S3<=Umbral) {
      S3=0; }
    }
  

  //Serial.println("centro izq");
  S4=analogRead(pin_S4);
  //Serial.println(S4);
  if (S4>Umbral) {
    S4=1; }
  else  {
    if(S4<=Umbral) {
      S4=0; }
    } 

  //Serial.println("lateral izq");
  S5=analogRead(pin_S5);
  //Serial.println(S5);
  if (S5>Umbral) {
    S5=1; }
  else  {
    if(S5<=Umbral) {
      S5=0; }
    }
  

  lectura=S1*10000+S2*1000+S3*100+S4*10+S5;
  if(DEBUG_MODE == 1){
    Serial.println("-------");
    Serial.println(analogRead(pin_S1));
    Serial.println(analogRead(pin_S2));
    Serial.println(analogRead(pin_S3));
    Serial.println(analogRead(pin_S4));
    Serial.println(analogRead(pin_S5));
    Serial.println(lectura);
    Serial.println("-------");
    delay(200);
  }
  return lectura;
  
}

int Error(int lectura){
  //digitalWrite(pin_DIRM1, 0);
  //digitalWrite(pin_DIRM2, 0);
  int error;
  switch(lectura){
    case 11111: //afuera
      error=5;
    break;
    case 11110://lateral der
      error=4;
    break;
    case 11100://L y C der
      error=3;
    break;
    case 11101://centro der
      error=2;
    break;
    case 11001://m y c der
      error=1;
    break;
    case 11011://medio
      error=0;
    break;
    case 10011:// m y c izq
      error=-1;
    break;
    case 10111://centro izq
      error=-2;
    break;
    case 111://c y l izq
      error=-3;
    break;
    case 1111://lateral izq
      error=-4;
    break;
    /*default: 
      error=6;
    break;*/
    }

    return error;
}
void moverMotores(int error){     

  int Pwm = 0;
  Pwm = KP*error + KD*(error - error_ant);
  error_ant=error;
  switch(error){
  case -4:
    //Pwm = KP*error + KD*(error - error_ant);
    digitalWrite(pin_DIRM1, 1);
    digitalWrite(pin_DIRM2, 0); 
    analogWrite(pin_PWMM1,constrain(Vmed1 + Pwm, 0, 255));
    analogWrite(pin_PWMM2,constrain(Vmed2 - Pwm, 0, 255));
  break;
  case 4:
   // Pwm = KP*error + KD*(error - error_ant);
    digitalWrite(pin_DIRM1, 0);
    digitalWrite(pin_DIRM2, 1); 
    analogWrite(pin_PWMM1,constrain(Vmed1 + Pwm, 0, 255));
    analogWrite(pin_PWMM2,constrain(Vmed2 - Pwm, 0, 255));  
  break;
  case 5://atras
    digitalWrite(pin_DIRM1, 1);
    digitalWrite(pin_DIRM2, 1); 
    analogWrite(pin_PWMM1,Vatras);
    analogWrite(pin_PWMM2,Vatras);
  break;
  default:
   // Pwm = KP*error;
    digitalWrite(pin_DIRM1, 0);
    digitalWrite(pin_DIRM2, 0);   
    analogWrite(pin_PWMM1,constrain(Vmed1 + Pwm, 0, 255));
    analogWrite(pin_PWMM2,constrain(Vmed2 - Pwm, 0, 255));
  break;
  }
 
}
  

