

#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <splash.h>

#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 
#define RED 0xF800
#define GREEN 0x07E0

#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
const int MPU_addr=0x68;
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;
long ran;
int num;
int displaycount = 0;
int buttonState=0;
int buttonStateSwitch=0;
int buttonStateUp=0;
int buttonStateDown=0;
const int buttonPin = 12;
const int buttonUp = 8;
const int buttonDown = 7;
const int buttonSwitch = 4;
long life=20;







void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(buttonUp, INPUT_PULLUP);
  pinMode(buttonDown, INPUT_PULLUP);
  pinMode(buttonSwitch, INPUT);

  
  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  delay(2000);
  display.clearDisplay();

  init_acc();
 
  Serial.begin(115200);
  
  randomSeed(analogRead(0));

}

void loop() {
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 14, true);
  while (Wire.available()<14);
  AcX=Wire.read()<<8|Wire.read();
  AcY=Wire.read()<<8|Wire.read();
  AcZ=Wire.read()<<8|Wire.read();
  Tmp=Wire.read()<<8|Wire.read();
  GyX=Wire.read()<<8|Wire.read();
  GyY=Wire.read()<<8|Wire.read();
  GyZ=Wire.read()<<8|Wire.read();

  Serial.print("AcX = "); Serial.println(AcX);
  Serial.print("AcY = "); Serial.println(AcY);
  Serial.print("AcZ = "); Serial.println(AcZ);
  Serial.print("Temp = "); Serial.println(Tmp/340.00+36.53);
  Serial.print("GyX = "); Serial.println(GyX);
  Serial.print("GyY = "); Serial.println(GyY);
  Serial.print("GyZ = "); Serial.println(GyZ);
  delay(100);

  

  displaycount=displaycount+1;
  buttonState=digitalRead(buttonPin);
  buttonStateSwitch=digitalRead(buttonSwitch);
  buttonStateUp=digitalRead(buttonUp);
  buttonStateDown=digitalRead(buttonDown);
  
 

      if (buttonStateSwitch==LOW){
          Serial.print("buttonstateswitch..........."); Serial.println(buttonStateSwitch);
         diceRoll();
        }
      else{
        Serial.print("buttonstateswith............."); Serial.println(buttonStateSwitch);
           counter();}
           
     if (buttonState==LOW){
      display.clearDisplay();
    display.ssd1306_command(SSD1306_DISPLAYON);
    }
  else{
    goodbye();
    display.clearDisplay();
  }
  
  
}

void testfillrect(void) {
  display.clearDisplay();

  for(int16_t i=0; i<display.height()/2; i+=3) {
    // The INVERSE color is used so rectangles alternate white/black
    display.fillRect(i, i, display.width()-i*2, display.height()-i*2, SSD1306_INVERSE);
    display.display(); // Update screen with each newly-drawn rectangle
    delay(1);
  }

  delay(300);
}

void diceRoll(void){
  if (displaycount >= 3){
            ran=random(1,21);
            num=ran;
            
              if((GyY<0)or (GyY>1000)) {
             
              testfillrect();
              display.clearDisplay();
            
              display.setTextSize(6);     
              display.setTextColor(SSD1306_WHITE);
              display.setCursor(30, 10);     
              display.print(num);
              display.display();
              delay(1000);
            }
            else {
              display.clearDisplay();
              display.setTextSize(2);
              display.setTextColor(SSD1306_WHITE);
              display.setCursor(20,20);
              display.println("Tilt to");
              display.setCursor(35,45);
              display.println("Roll!");
              display.display();
            }
display.clearDisplay();
displaycount =0;
}
}

void counter(void){
unsigned long newlife;
unsigned long curState=0;
unsigned long lastState=HIGH;

lastState=digitalRead(buttonUp);

  
  if (buttonStateUp==LOW){
       if (curState!=lastState){
          curState = 1;}
        
        life++;
        lastState=curState;
        }
        else {
          curState=0;}
        
       
      
        
      
            
          
        if (buttonStateDown==LOW){
              if (curState!=lastState){
                 curState=1;}
              else {
                curState=0;
              }
            life--;
            lastState=curState;
            }

       
       
          
         
        
        delay(50);
         
           
          
   display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(50,0);
        display.println("LIFE");
        display.setTextSize(6);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(30,10);
        display.print(life);
        display.display();
        display.clearDisplay();}

    /* if (life<=0);{
      display.clearDisplay();
     display.setTextSize(4);
     display.setTextColor(RED);
     display.setCursor(20,10);
     display.println("YOU");
     display.setTextSize(4);
     display.setTextColor(RED);
     display.setCursor(15,40);
     display.println("LOSE!");
     display.display();
     display.clearDisplay();}
     
}*/

          
       

void goodbye(void){
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(15,30);
    display.println("GOOD-BYE");
    display.display();
    display.clearDisplay();
    delay(1000);
    display.ssd1306_command(SSD1306_DISPLAYOFF);
    
}

void init_acc(void){
   Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission(true);}
