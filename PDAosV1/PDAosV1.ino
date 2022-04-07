//480x320
////// Rectangles =screen.Fill_Rectangle(x1, y1, x2,y2);


#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0
#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

#include <SPI.h>          // f.k. for Arduino-1.5.2
#include "Adafruit_GFX.h"// Hardware-specific library
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;
//#include <Adafruit_TFTLCD.h>
//Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

// Assign human-readable names to some common 16-bit color values:
#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define WHITE   0xFFFF
#if defined(ESP32)
int XP = 27, YP = 4, XM = 15, YM = 14;  //most common configuration
#else
//int XP = 6, YP = A1, XM = A2, YM = 7;  //most common configuration
int XP = 7, YP = A2, XM = A1, YM = 6;  //next common configuration
//int XP=PB7,XM=PA6,YP=PA7,YM=PB6; //BLUEPILL must have Analog for YP, XM
#endif
#if USE_LOCAL_KBV
#include "TouchScreen_kbv.h"         //my hacked version
#define TouchScreen TouchScreen_kbv
#define TSPoint     TSPoint_kbv
#else
#include <TouchScreen.h>         //Adafruit Library
#endif
TouchScreen ts(XP, YP, XM, YM, 300);   //re-initialised after diagnose
TSPoint tp;                            //global point


int pageno;
int ppage;
int width=320;
int height=480;
int hour=20;
int minute=45;
long long int pt=0;
bool sec=false;
int pinpt=0; ///previous input time
bool inpPause=false;

int draw=1;
bool isClockUpdating(){
  //if(millis()-pt>=600){
  if(millis()-pt>=60000){
    pt =millis();
    tft.print(minute);
    
    //tft.fillRect(30, int(height/2-50), 300, 50, BLACK);
    
     minute+=1;
    if(minute==60){
      hour+=1;
      minute=0;
    }
    if(hour==24){
      hour=0;
    }
    return true;
  }
  return false;
}

void drawclock(int x,int y, int w, int h,int fts,int col){
    tft.setTextSize(fts);
    if(col==0){
    tft.setTextColor(BLACK);
    tft.fillRect(x, y, w, h, WHITE);
    }
    else{
      tft.setTextColor(WHITE);
    tft.fillRect(x, y, w, h, BLACK);
    }
    tft.setCursor(x, y);
    
    String hr=String(hour);
    tft.print(String(hour));
    tft.print(":");
    tft.print(String(minute));
    tft.setTextColor(WHITE);
  
  

int drawNumPad(int x,int y){
  
}
void readTouch(void)

{ 
  
    tp = ts.getPoint();
    pinMode(YP, OUTPUT);      //restore shared pins
    pinMode(XM, OUTPUT);
    
    //digitalWrite(YP, HIGH);  //because TFT control pins
    //digitalWrite(XM, HIGH);
    //    Serial.println("tp.x=" + String(tp.x) + ", tp.y=" + String(tp.y) + ", tp.z =" + String(tp.z));
}
bool isTouch(){
  if(inpPause==true){
    if(millis()-pinpt>=600){
      inpPause=false;
    }
}
if(inpPause==false){
  readTouch();
  
  pinpt=millis();
    inpPause=tp.z>5;
    return tp.z>5;
}
  return false;
}
void setup() {
  Serial.begin(9600);
    uint16_t ID = tft.readID();  
    tft.begin(ID);
  tft.setRotation(0);
  tft.fillScreen(BLACK);
  pageno=41;
  ppage=-1;
}

void loop() {
  // put your main code here, to run repeatedly:
  tft.fillScreen(BLACK);
  if(pageno==1){
    pageno=HomeScreen();//Draw HomeScreen
    ppage=1;
    //draw=1;
  }
  else if(pageno==2){
    pageno=AppScreen();
    ppage=2;
  }
  else if(pageno==3){
    pageno=shade();
    ppage=3;
    }
   else if(pageno==4){
    pageno=settings();
    ppage=4;
    }
    else if(pageno==5){
    pageno=ticTacToe();
    ppage=5;
    }
    else if(pageno==6){
    pageno=clk();
    ppage=6;
    }
    else if(pageno==41){
    pageno=dateAT();
    ppage=41;
    }

}
int HomeScreen(){
  tft.setCursor(30, int(height/2-50));
  tft.setTextColor(WHITE);
  tft.setTextSize(5);
  tft.print(hour);
  tft.print(":");
  tft.print(minute);
  //////////////// LOCK BUTTONS 
  tft.fillRect(20, height-50, 50, 50, RED);
  tft.fillRect(130, height-50, 50, 50, RED);
  tft.fillRect(250, height-50, 50, 50, RED);
  int l1=0;
  int l2=0;
  
  //////////////////////////////////////////////
  while(true){
   if(isTouch()){
    //Serial.print(tp.x);
    //Serial.print(",");
   //Serial.println(tp.y);
   if(tp.x>850 && tp.x<930){
    if(tp.y>769 && tp.y<880 ){
      tft.fillRect(250, height-50, 50, 50, WHITE)
      ;l1=1;}
    if(l1==1 && tp.y>480 && tp.y<595 ){
    tft.fillRect(130, height-50, 50, 50, WHITE);l2=1;}
    if(l2==1 && tp.y>200 && tp.y<335 ){
    tft.fillRect(20, height-50, 50, 50, WHITE);return 2;}
   }
   }
  
   
   
   hsDraw();
  }
  
}
void hsDraw(){
  
  //60000
  ///slide to unlock thingy
  //tft.fillRect(20, height-50, 300, 50, WHITE);
  
  //Serial.println(millis()-pt);
  //////CLOCK////////////
  if( isClockUpdating()){
   
  drawclock(30, int(height/2-50),250,60,5,1);
}}


int AppScreen(){
  
  tft.fillRect(0, 0, width, 30, WHITE);
  tft.fillRect(0,height-30, width, 30, WHITE);
  tft.setCursor(30, 50);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.print("App Drawer :_");
  tft.fillRect(28, 90, 50, 50, WHITE);
  tft.fillRect(135, 90, 50, 50, WHITE);
  tft.fillRect(241, 90, 50, 50, WHITE);
  tft.fillRect(28, 200, 50, 50, WHITE);
  tft.fillRect(135, 200, 50, 50, WHITE);
  tft.fillRect(241, 200, 50, 50, WHITE);
  tft.setTextColor(BLACK);
  tft.setCursor(width-75, 9);
  tft.print(hour);
  tft.print(":");
  tft.print(minute);
  tft.setCursor(9, 9);
  tft.setTextSize(2);
  tft.print("A");
  tft.setTextSize(2);
  tft.setTextColor(WHITE);
  while(true){
   if( isClockUpdating()){
    drawclock(width-85, 9,85,21,2,0);
  tft.setTextColor(WHITE);
  }
   if(isTouch()){
    Serial.print(tp.x);
    Serial.print(",");
   Serial.println(tp.y);
   if(tp.x>150 &&tp.x<240){
    return 3;
   }
   else if(tp.x>860 &&tp.x<925){
    return 1;
   }
   else if(tp.x>287 && tp.x<378 && tp.y>227 &&tp.y<366){
    return 4;
   }
   else if(tp.x>287 && tp.x<378 && tp.y>487 &&tp.y<618){
    return 5;
   }
   else if(tp.x>287 && tp.x<378 && tp.y>728 &&tp.y<863){
    return 6;
   }
   }
  }
}
int shade(){
  tft.fillScreen(WHITE);
  //tft.drawRect(0, 48 + 63, wid, 1, BLACK);
  tft.fillRect(0,height-40, width, 50, BLACK);
  while(true){
    if( isClockUpdating()){
    drawclock(width-85, 9,85,21,2,0);
  tft.setTextColor(WHITE);
  }
    if(isTouch()){
   if(tp.x>860 &&tp.x<925){
    return ppage;
   }
  }
  }
}
int settings(){
   tft.fillRect(0, 0, width, 30, WHITE);
  tft.fillRect(0,height-30, width, 30, WHITE);
  tft.setCursor(30, 50);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.print("Setting :_");
  //tft.fillRect(28, 90, 50, 50, WHITE);
  tft.setTextColor(BLACK);
  tft.setCursor(width-85, 9);
  tft.print(hour);
  tft.print(":");
  tft.print(minute);
  tft.setCursor(9, 9);
  tft.setTextSize(2.5);
  tft.print("A");
  tft.setTextSize(2);
  tft.setTextColor(WHITE);
  tft.setCursor(15, 120);
  tft.print("DATE AND TIME :");
  tft.setCursor(15, 200);
  tft.print("SECURITY :");
  tft.setCursor(width+1, height+1);
  if (sec==true){
  tft.fillRect(width -90,200, 90, 50, WHITE);
  tft.fillRect(width -50,200, 50, 50, BLUE);}
  else{
    tft.fillRect(width -90,200, 90, 50, WHITE);
  tft.fillRect(width -90,200, 50, 50, RED);
  }
  
  while(true){
   if( isClockUpdating()){
    drawclock(width-85, 9,85,21,2,0);
  tft.setTextColor(WHITE);
  }
   if(isTouch()){
    Serial.print(tp.x);
    Serial.print(",");
   Serial.println(tp.y);
   if(tp.x>150 &&tp.x<230){
    return 3;
   }
   if(tp.x>860 &&tp.x<925){
    return 2;
   }
   if(tp.x>310 &&tp.x<410 && tp.x>280 &&tp.x<905){
    return 41;
   }
   if(tp.x>460 &&tp.x<552 && tp.x>460 &&tp.x<552){
    sec=!sec;
    if (sec==true){
  tft.fillRect(width -90,200, 90, 50, WHITE);
  tft.fillRect(width -50,200, 50, 50, BLUE);
  }
  else{
    tft.fillRect(width -90,200, 90, 50, WHITE);
  tft.fillRect(width -90,200, 50, 50, RED);
  }
   }
   }
  }
  }
int dateAT(){
   tft.fillRect(0, 0, width, 30, WHITE);
  tft.fillRect(0,height-30, width, 30, WHITE);
  tft.setCursor(30, 50);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.print("DATE AND TIME :_");
  //tft.fillRect(28, 90, 50, 50, WHITE);
  

  ////////////////////////////////////////
  //// HOUR 
  tft.setCursor(15, 125);
  tft.setTextSize(3);
  tft.print("Hour");
  tft.setCursor(100, 120);
  tft.setTextSize(5);
  tft.print("<");
  tft.fillRect(150,125, 70, 30, BLACK);
  tft.setCursor(150, 125);
  tft.print(String(hour));
  tft.setCursor(250, 120);
  tft.print(">");
  tft.setTextSize(2);
  
  ////////////////////////////////////////////
  ///////////////  MINUTE
  tft.setCursor(15, 205);
  tft.setTextSize(3);
  tft.print("Min");
   tft.setCursor(100, 200);
  tft.setTextSize(5);
  tft.print("<");
  tft.fillRect(150,205, 70, 30, BLACK);
  tft.setCursor(150, 200);
  tft.print(String(minute));
  tft.setCursor(250, 200);
  tft.print(">");
  tft.setTextSize(2);
  ///////////////////////////////////////////////
  tft.setTextColor(BLACK);
  tft.setCursor(width-75, 9);
  tft.print(hour);
  tft.print(":");
  tft.print(minute);
  tft.setCursor(9, 9);
  tft.setTextSize(2.5);
  tft.print("A");
  tft.setTextSize(2);
  tft.setTextColor(WHITE);
  while(true){
   if( isClockUpdating()){
    drawclock(width-85, 9,85,21,2,0);
  tft.setTextColor(WHITE);
  }
   if(isTouch()){
    Serial.print(tp.x);
    Serial.print(",");
   Serial.println(tp.y);
   if(tp.x>150 &&tp.x<230){
    return 3;
   }
   if(tp.x>860 &&tp.x<925){
    return 4;
   }
   //////////////////////////////////////////
   ///dec minute
   //353,478
   //423,552
  //if(tp.x>353 &&tp.x<478 && tp.y>423 &&tp.y<552){
  //if(tp.x>353 &&tp.x<418 && tp.y>774 &&tp.y<865){
   if(tp.x>450 &&tp.x<578  && tp.y>400 &&tp.y<644){
    minute-=1;
    if(minute==-1){
      minute=59;
      hour-=1;
      if(hour==-1){
        hour=23;
      }
    }
    tft.setTextSize(5);
tft.setTextColor(WHITE);
    tft.fillRect(150,95, 70, 70, BLACK);
  tft.setCursor(150, 125);
  tft.print(String(hour));
  tft.fillRect(150,185, 70, 60, BLACK);
  tft.setCursor(150, 200);
  tft.print(String(minute));
  tft.setTextSize(2);
   }
   
   if(tp.x>443 &&tp.x<560 && tp.y>654 &&tp.y<900){
    minute+=1;
    if(minute==60){
      minute=0;
      hour+=1;
      if(hour==24){
        hour=0;
      }
    }
    tft.setTextSize(5);
    tft.setTextColor(WHITE);
     tft.fillRect(150,95, 70, 70, BLACK);
  tft.setCursor(150, 125);
  tft.print(String(hour));
  tft.fillRect(150,185, 70, 60, BLACK);
  tft.setCursor(150, 200);
  tft.print(String(minute));
  tft.setTextSize(2);
   }
   
   }
  }
}
  
int notes(){
   tft.fillRect(0, 0, width, 30, WHITE);
  tft.fillRect(0,height-30, width, 30, WHITE);
  tft.setCursor(30, 50);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.print("Notes :_");
  //tft.fillRect(28, 90, 50, 50, WHITE);
  tft.setTextColor(BLACK);
  tft.setCursor(width-75, 9);
  tft.print(hour);
  tft.print(":");
  tft.print(minute);
  tft.setCursor(9, 9);
  tft.setTextSize(2.5);
  tft.print("A");
  tft.setTextSize(2);
  tft.setTextColor(WHITE);
  while(true){
   if( isClockUpdating()){
    drawclock(width-85, 9,85,21,2,0);
  tft.setTextColor(WHITE);
  }
   if(isTouch()){
    Serial.print(tp.x);
    Serial.print(",");
   Serial.println(tp.y);
   if(tp.x>150 &&tp.x<230){
    return 3;
   }
   if(tp.x>860 &&tp.x<925){
    return 2;
   }
   }
  }
  }
int clk(){
   tft.fillRect(0, 0, width, 30, WHITE);
  tft.fillRect(0,height-30, width, 30, WHITE);
  tft.setCursor(30, 50);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.print("Clock :_");
  //tft.fillRect(28, 90, 50, 50, WHITE);
  tft.setTextColor(BLACK);
  tft.setCursor(width-75, 9);
  tft.print(hour);
  tft.print(":");
  tft.print(minute);
  tft.setCursor(9, 9);
  tft.setTextSize(2.5);
  tft.print("A");
  tft.setTextSize(2);
  tft.setTextColor(WHITE);
  while(true){
   if( isClockUpdating()){
    drawclock(width-85, 9,85,21,2,0);
  tft.setTextColor(WHITE);
  }
   if(isTouch()){
    Serial.print(tp.x);
    Serial.print(",");
   Serial.println(tp.y);
   if(tp.x>150 &&tp.x<230){
    return 3;
   }
   if(tp.x>860 &&tp.x<925){
    return 2;
   }
   }
  }
  }
int ticTacToe(){
  
}
  return 2; 
}
