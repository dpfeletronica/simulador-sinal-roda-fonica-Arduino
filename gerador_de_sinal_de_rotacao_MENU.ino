////                                       DPF ELETRÔNICA
////                                    SIMULADOR DE SINAL DE RODA FONIA
////                                    VER 2.0   01/08/2020

///Variaveis de memória
int baixo=8;
int cima=9;
int enter=10;
int direita=11;
int esquerda=15;
int a=0,b=0, dente=0,  rpm=0, sensor=0,  ciclo=0, cont=0, rotacao=0, fonica=0, contfase=0;
int analogPin = A0;
int ndentes=60,nfalha=2, ON=0;
int menu=0, fase1i=0 ,fase1f=0, fase2i=0,fase2f=0,fase3i=0, fase3f=0, fase4i=0, fase4f=0, teclacont=0;
//Carrega a biblioteca LiquidCrystal
#include <LiquidCrystal.h>
//carrega a biblioteca eeprom
#include <EEPROM.h> 
//Define os pinos que serão utilizados para ligação ao display
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
/////////////////////
byte senter[8] = {
 B00011,
 B00011,
 B00011,
 B01011,   /////simbolo do enter
 B11111,
 B11111,
 B01000,
 B00000
};
////////////////////////
void setup(void)
{//////carrega valores da eeprom
fase1i =EEPROM.read(0);
fase1f =EEPROM.read(1);
fase2i =EEPROM.read(2);
fase2f =EEPROM.read(3);
fase3i =EEPROM.read(4);
fase3f =EEPROM.read(5);
fase4i =EEPROM.read(6);
fase4f =EEPROM.read(7);
nfalha =EEPROM.read(8);
ndentes =EEPROM.read(9);
////////////////////////////////////
//define valores na primeira inicialização
if(ndentes==255){
 ndentes=60;
 nfalha=2;
 fase1i=0;
 fase1f=0;
 fase2i=0;
 fase2f=0;
 fase3i=0;
 fase3f=0;
 fase4i=0;
 fase4f=0; 
}
///////////////////////////////
  lcd.createChar(1, senter);
  //Define o número de colunas e linhas do LCD
  lcd.begin(16, 2);
   //Limpa a tela
  lcd.clear();
  //Posiciona o cursor na coluna 3, linha 0;
  lcd.setCursor(0, 0);
  //Envia o texto entre aspas para o LCD
  lcd.print("DPF ELETRONICA");
  lcd.setCursor(0, 1);
  //Envia o texto entre aspas para o LCD
  lcd.print("SIMULADOR ECU");
  delay(2000);
   lcd.clear();
//Serial.begin(9600);
pinMode(baixo, INPUT);
pinMode(cima, INPUT);
pinMode(enter, INPUT);
pinMode(direita, INPUT);
pinMode(esquerda, INPUT);
pinMode(13, OUTPUT);
pinMode(12, OUTPUT);
 // inicializa timer1 
 noInterrupts();           // disabilita todas as interrupções
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;

  //OCR1A = 31250;            // compare match register 16MHz/256/2Hz
  OCR1A = 1;
  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS12);    // 256 prescaler 
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt
  interrupts();             // enable all interrupts


}

ISR(TIMER1_COMPA_vect)          // timer compare interrupt service routine
{
 cont++;
 fonica++;
 if(cont>=31250){
rotacao=rpm;
rpm=0;
  cont=0;
 }
 ciclo++;
 if(ciclo>22){
  ciclo=0;
 }
if(ON==1){/// liga sinal
  if(fonica>sensor){
    fonica=0;
  if(nfalha>0){ 
   if(dente>ndentes-nfalha){  //falha
   a=1;
 }
 }
 if(a==0){
 
 digitalWrite(13, LOW);   
 a=1;
 }
 else{                     
  digitalWrite(13, HIGH);
 a=0;

 if(dente==ndentes){ ////Quatidade dentes da roda
dente=1;
 }
  if(contfase+1==ndentes*2){
contfase=1;
dente=1;//////  
 }
 }
if(b==0){
  b=1;
     rpm++;
 dente++;
 contfase++;
}else{
  b=0;

}
  } 
 
//////////////////////sinal de fase

////1 sinal
if(fase1i>0){
if(contfase==fase1i){//dente inicio
  digitalWrite(12, HIGH);  
}
if(contfase==fase1f){// dente final
  digitalWrite(12, LOW);  
}}
////2 sinal
if(fase2i>0){
if(contfase==fase2i){//dente inicio
  digitalWrite(12, HIGH);  
}
if(contfase==fase2f){// dente final
  digitalWrite(12, LOW);  
}}
////3 sinal
if(fase3i>0){
if(contfase==fase3i){//dente inicio
  digitalWrite(12, HIGH);  
}
if(contfase==fase3f){// dente final
  digitalWrite(12, LOW);  
}}
////4 sinal
if(fase4i>0){
if(contfase==fase4i){//dente inicio
 digitalWrite(12, HIGH);  
}
if(contfase==fase4f){// dente fina
 digitalWrite(12, LOW);  
}}///////////////////////////fim sinal fase*/
}
 }
void loop(void)
{
sensor = analogRead(A0)/10;
menu1();
}

void menu1 (){
if(menu==0){
 lcd.setCursor(1, 0);
 lcd.write(1);
 lcd.setCursor(4, 0);
 lcd.print(ndentes);
 lcd.print(" - ");
 lcd.print(nfalha);
  lcd.setCursor(1, 1);
 lcd.print(">> PARA MENU");
 if(digitalRead(enter)== 1){  ////seleciona o menu de entrada, simular o 60-2 ou escolhe menu
 ON=1;
 menu=1;
 dente=0;
  contfase=0;
   delay(200);
   lcd.clear();
 }
  if(digitalRead(direita)== 1){
  menu=2;
    delay(200);
   lcd.clear();
  }
}/////////////////
if(menu==1){
 lcd.setCursor(0, 0);
 lcd.print("SIMULANDO ");
  lcd.print(ndentes);
   lcd.print("-");
    lcd.print(nfalha);
     lcd.setCursor(0, 1);  //simula principal 60-2
 lcd.print("ROTACAO " );
 //lcd.setCursor(9, 0);
 lcd.print(rotacao);
 lcd.print("   ");
 // lcd.setCursor(1, 1);
 //lcd.print("");
 if(digitalRead(enter)== 1){
 ON=0;
 menu=0;
   delay(200);
   lcd.clear();
}///////////////////////
}
if(menu==2){
 lcd.setCursor(0, 0);
 lcd.write(1);
 lcd.print("  CRIAR SINAL");
  lcd.setCursor(0, 1);
 lcd.print("<< VOLTAR");
 if(digitalRead(enter)==1){
  menu=3;
  lcd.clear();
  delay(200); 
 }
 if(digitalRead(esquerda)==1){
  menu=0;
  lcd.clear();
  delay(200);
 }
}/////////////////////
if(menu==3){
 lcd.setCursor(0, 0);
 lcd.print("NUMERO DE DENTES"); 
 lcd.setCursor(5, 1);
 lcd.print("<< ");
 lcd.print(ndentes);
 lcd.print(" >>");
 if(digitalRead(esquerda)==1){
  ndentes--;
  delay(200);
 }
 if(digitalRead(direita)==1){                                         
  ndentes++;
  delay(200);
 }
 if(digitalRead(enter)==1){
 menu=4;
 delay(200);
 lcd.clear();
 }}/////////////////////////////
 if(menu==4){
   lcd.setCursor(0, 0);
 lcd.print("DENTES DE FALHA"); 
 lcd.setCursor(5, 1);
 lcd.print("<< ");
 lcd.print(nfalha);
 lcd.print(" >>");
 if(digitalRead(esquerda)==1){
  nfalha--;
  delay(200);
 }
  if(digitalRead(direita)==1){
  nfalha++;
  delay(200);
 }
 if(digitalRead(enter)==1){
menu=5;
delay(200);
lcd.clear();

 }///////
}
if(menu==5){
   lcd.setCursor(0, 0);
 lcd.print("FASE 1 INICIO"); 
 lcd.setCursor(5, 1);
 lcd.print("<< ");
 lcd.print(fase1i);
 lcd.print(" >>");
 if(digitalRead(esquerda)==1){
  fase1i--;
  delay(200);
 }
  if(digitalRead(direita)==1){
  fase1i++;
  delay(200);
 }
  if(digitalRead(enter)==1){
menu=6;
delay(200);
lcd.clear();
fase1f=fase1i;
 }///////
}
if(menu==6){
   lcd.setCursor(0, 0);
 lcd.print("FASE 1 FIM"); 
 lcd.setCursor(5, 1);
 lcd.print("<< ");
 lcd.print(fase1f);
 lcd.print(" >>");
 if(digitalRead(esquerda)==1){
  fase1f--;
  delay(200);
 }
  if(digitalRead(direita)==1){
  fase1f++;
  delay(200);
 }
if(digitalRead(enter)==1){
menu=7;
delay(200);
lcd.clear();
//fase2i=fase1f;
 }///////
 }
 if(menu==7){
   lcd.setCursor(0, 0);
 lcd.print("FASE 2 INICIO"); 
 lcd.setCursor(5, 1);
 lcd.print("<< ");
 lcd.print(fase2i);
 lcd.print(" >>");
 if(digitalRead(esquerda)==1){
  fase2i--;
  delay(200);
 }
  if(digitalRead(direita)==1){
  fase2i++;
  delay(200);
 }
  if(digitalRead(enter)==1){
menu=8;
delay(200);
lcd.clear();
fase2f=fase2i;
 }///////
}
if(menu==8){
   lcd.setCursor(0, 0);
 lcd.print("FASE 2 FIM"); 
 lcd.setCursor(5, 1);
 lcd.print("<< ");
 lcd.print(fase2f);
 lcd.print(" >>");
 if(digitalRead(esquerda)==1){
  fase2f--;
  delay(200);
 }
  if(digitalRead(direita)==1){
  fase2f++;
  delay(200);
  
 }
if(digitalRead(enter)==1){
menu=9;
delay(200);
lcd.clear();
//fase3i=fase2f;
 }///////
 }
  if(menu==9){
   lcd.setCursor(0, 0);
 lcd.print("FASE 3 INICIO"); 
 lcd.setCursor(5, 1);
 lcd.print("<< ");
 lcd.print(fase3i);
 lcd.print(" >>");
 if(digitalRead(esquerda)==1){
  fase3i--;
  delay(200);
 }
  if(digitalRead(direita)==1){
  fase3i++;
  delay(200);
 }
  if(digitalRead(enter)==1){
menu=10;
delay(200);
lcd.clear();
fase3f=fase3i;

 }///////
}
if(menu==10){
   lcd.setCursor(0, 0);
 lcd.print("FASE 3 FIM"); 
 lcd.setCursor(5, 1);
 lcd.print("<< ");
 lcd.print(fase3f);
 lcd.print(" >>");
 if(digitalRead(esquerda)==1){
  fase3f--;
  delay(200);
 }
  if(digitalRead(direita)==1){
  fase3f++;
  delay(200);
 }
if(digitalRead(enter)==1){
menu=11;
delay(200);
lcd.clear();
//fase4i=fase3f;
 }///////
 }
   if(menu==11){
   lcd.setCursor(0, 0);
 lcd.print("FASE 4 INICIO"); 
 lcd.setCursor(5, 1);
 lcd.print("<< ");
 lcd.print(fase4i);
 lcd.print(" >>");
 if(digitalRead(esquerda)==1){
  fase4i--;
  delay(200);
 }
  if(digitalRead(direita)==1){
  fase4i++;
  delay(200);
 }
  if(digitalRead(enter)==1){
menu=12;
delay(200);
lcd.clear();
fase4f=fase4i;
 }///////
}
if(menu==12){
   lcd.setCursor(0, 0);
 lcd.print("FASE 4 FIM"); 
 lcd.setCursor(5, 1);
 lcd.print("<< ");
 lcd.print(fase4f);
 lcd.print(" >>");
 if(digitalRead(esquerda)==1){
  fase4f--;
  delay(200);
 }
  if(digitalRead(direita)==1){
  fase4f++;
  delay(200);
 }
if(digitalRead(enter)==1){
menu=1;
delay(200);
lcd.clear();
ON=1;
dente=0;
contfase=0;
////grava valores na eeprom
EEPROM.write(0,fase1i );
EEPROM.write(1,fase1f );
EEPROM.write(2,fase2i );
EEPROM.write(3,fase2f );
EEPROM.write(4,fase3i );
EEPROM.write(5,fase3f );
EEPROM.write(6,fase4i );
EEPROM.write(7,fase4f );
EEPROM.write(8,nfalha );
EEPROM.write(9,ndentes );
 }///////////////////////////
 }////reseta programa
if(digitalRead(cima)==1){
teclacont++;
if(teclacont>200){
 ndentes=60;
 nfalha=2;
 fase1i=0;
 fase1f=0;
 fase2i=0;
 fase2f=0;
 fase3i=0;
 fase3f=0;
 fase4i=0;
 fase4f=0; 
lcd.clear();
lcd.setCursor(0, 0);
 lcd.print("MEMORIA APAGADA");
 delay(2000);
 teclacont=0;
lcd.clear(); 
  }
}
}


//////////////
