// 480x320
////// Rectangles =screen.Fill_Rectangle(x1, y1, x2,y2);

#define LCD_CS A3    // Chip Select goes to Analog 3
#define LCD_CD A2    // Command/Data goes to Analog 2
#define LCD_WR A1    // LCD Write goes to Analog 1
#define LCD_RD A0    // LCD Read goes to Analog 0
#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

#include <SPI.h>          // f.k. for Arduino-1.5.2
#include "Adafruit_GFX.h" // Hardware-specific library
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;
//#include <Adafruit_TFTLCD.h>
// Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

// Assign human-readable names to some common 16-bit color values:
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define WHITE 0xFFFF
#if defined(ESP32)
int XP = 27, YP = 4, XM = 15, YM = 14; // most common configuration
#else
// int XP = 6, YP = A1, XM = A2, YM = 7;  //most common configuration
int XP = 7, YP = A2, XM = A1, YM = 6; // next common configuration
// int XP=PB7,XM=PA6,YP=PA7,YM=PB6; //BLUEPILL must have Analog for YP, XM
#endif
#if USE_LOCAL_KBV
#include "TouchScreen_kbv.h" //my hacked version
#define TouchScreen TouchScreen_kbv
#define TSPoint TSPoint_kbv
#else
#include <TouchScreen.h> //Adafruit Library
#endif
TouchScreen ts(XP, YP, XM, YM, 300); // re-initialised after diagnose
TSPoint tp;                          // global point

int pageno;
int ppage;
int width = 320;
int height = 480;

int pinpt = 0; /// previous input time

int draw = 1;
int r1 = -1;
int r2 = -1;
int r3 = -1;
int r4 = -1;
int r5 = -1;
int r6 = -1;
/////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void setup()
{
  Serial.begin(9600);
  uint16_t ID = tft.readID();
  tft.begin(ID);
  tft.setRotation(0);
  tft.fillScreen(BLACK);
  pageno = 1;
  pageno = 8;
  ppage = -1;
}

void loop()
{
  // put your main code here, to run repeatedly:
  tft.fillScreen(BLACK);
  if (pageno == 1)
  {
    pageno = HomeScreen(); // Draw HomeScreen
    ppage = 1;
    // draw=1;
  }
  else if (pageno == 2)
  {
    pageno = AppScreen();
    ppage = 2;
  }
  else if (pageno == 3)
  {
    pageno = shade();
    ppage = 3;
  }
  else if (pageno == 4)
  {
    pageno = settings();
    ppage = 4;
  }
  else if (pageno == 5)
  {
    pageno = notes();
    ppage = 5;
  }
  else if (pageno == 6)
  {
    pageno = clk();
    ppage = 6;
  }

  else if (pageno == 61)
  {
    pageno = timer();
    ppage = 61;
  }
  else if (pageno == 7)
  {
    pageno = ticTacToe();
    ppage = 7;
  }
  else if (pageno == 71)
  {
    pageno = ttch();
    ppage = 71;
  }
  else if (pageno == 72)
  {
    pageno = ttcc();
    ppage = 72;
  }
  else if (pageno == 8)
  {
    pageno = calc();
    ppage = 72;
  }

}
//////////////////////////////////////////////////////////////////////



int drawNumPad(int x, int y)
{
}
void readTouch(void)

{

  tp = ts.getPoint();
  pinMode(YP, OUTPUT); // restore shared pins
  pinMode(XM, OUTPUT);

  // digitalWrite(YP, HIGH);  //because TFT control pins
  // digitalWrite(XM, HIGH);
  //     Serial.println("tp.x=" + String(tp.x) + ", tp.y=" + String(tp.y) + ", tp.z =" + String(tp.z));
}
bool isTouch()
{
  readTouch();
  pinpt = millis();
  bool inpPause = tp.z > 2;
  delay(130);
  return tp.z > 2;
}

////////////////////////////////////////////////////////////////////////////
//   HOMESCREEN
////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
int HomeScreen()
{
  tft.setCursor(30, int(height / 2 - 50));
  tft.setTextColor(WHITE);
  tft.setTextSize(3);
  tft.print("Abhinav's PDA");
  //////////////// LOCK BUTTONS
  tft.fillRect(20, height - 50, 50, 50, BLUE);
  tft.fillRect(130, height - 50, 50, 50, BLUE);
  tft.fillRect(250, height - 50, 50, 50, BLUE);
  r1 = 0;
  r2 = 0;

  //////////////////////////////////////////////
  while (true)
  {
    if (isTouch())
    {
      // Serial.print(tp.x);
      // Serial.print(",");
      // Serial.println(tp.y);
      if (tp.x > 850 && tp.x < 930)
      {
        if (tp.y > 769 && tp.y < 880)
        {
          tft.fillRect(250, height - 50, 50, 50, WHITE);
          r1 = 1;
        }
        if (r1 == 1 && tp.y > 480 && tp.y < 595)
        {
          tft.fillRect(130, height - 50, 50, 50, WHITE);
          r2 = 1;
        }
        if (r2 == 1 && tp.y > 200 && tp.y < 335)
        {
          tft.fillRect(20, height - 50, 50, 50, WHITE);
          return 2;
        }
      }
    }
  }
}
////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////

int AppScreen()
{
  tft.fillRect(0, 0, width, 30, WHITE);
  tft.fillRect(0, height - 30, width, 30, RED);
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
  tft.setCursor(9, 9);
  tft.setTextColor(BLACK);
  tft.print("A");
  while (true)
  {
    if (isTouch())
    {
      Serial.print(tp.x);
      Serial.print(",");
      Serial.println(tp.y);
      if (tp.x > 150 && tp.x < 240)
      {
        return 3;
      }
      else if (tp.x > 860 && tp.x < 925)
      {
        return 1;
      }
      else if (tp.x > 287 && tp.x < 378 && tp.y > 227 && tp.y < 366)
      {
        return 4;
      }
      else if (tp.x > 287 && tp.x < 378 && tp.y > 487 && tp.y < 618)
      {
        return 5;
      }
      else if (tp.x > 287 && tp.x < 378 && tp.y > 728 && tp.y < 863)
      {
        return 6;
      }
      else if (tp.x > 460 && tp.x < 571 && tp.y > 320 && tp.y < 449)
      {
        return 7;
      }
      else if (tp.x > 460 && tp.x < 585 && tp.y > 518 && tp.y < 685)
      {
        return 8;
      }
    }
  }
}
////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////

int shade()
{
  tft.fillScreen(WHITE);
  // tft.drawRect(0, 48 + 63, wid, 1, BLACK);
  tft.fillRect(0, height - 40, width, 50, BLACK);
  while (true)
  {

    if (isTouch())
    {
      if (tp.x > 860 && tp.x < 925)
      {
        return ppage;
      }
    }
  }
}
int settings()
{ bool sec = false;
  tft.fillRect(0, 0, width, 30, WHITE);
  tft.fillRect(0, height - 30, width, 30,  RED);
  tft.setCursor(30, 50);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.print("Setting :_");
  // tft.fillRect(28, 90, 50, 50, WHITE);
  tft.setTextColor(BLACK);
  tft.setCursor(9, 9);
  tft.setTextSize(2.5);
  tft.print("A");
  tft.setTextSize(2);
  tft.setTextColor(WHITE);
  tft.setCursor(15, 200);
  tft.print("SECURITY :");
  tft.setCursor(width + 1, height + 1);
  if (sec == true)
  {
    tft.fillRect(width - 90, 200, 90, 50, WHITE);
    tft.fillRect(width - 50, 200, 50, 50, BLUE);
  }
  else
  {
    tft.fillRect(width - 90, 200, 90, 50, WHITE);
    tft.fillRect(width - 90, 200, 50, 50, RED);
  }

  while (true)
  {

    if (isTouch())
    {
      Serial.print(tp.x);
      Serial.print(",");
      Serial.println(tp.y);
      if (tp.x > 150 && tp.x < 230)
      {
        return 3;
      }
      if (tp.x > 860 && tp.x < 925)
      {
        return 2;
      }

      if (tp.x > 460 && tp.x < 552 && tp.x > 460 && tp.x < 552)
      {
        sec = !sec;
        if (sec == true)
        {
          tft.fillRect(width - 90, 200, 90, 50, WHITE);
          tft.fillRect(width - 50, 200, 50, 50, BLUE);
        }
        else
        {
          tft.fillRect(width - 90, 200, 90, 50, WHITE);
          tft.fillRect(width - 90, 200, 50, 50, RED);
        }
      }
    }
  }
}


int notes()
{
  tft.fillRect(0, 0, width, 30, WHITE);
  tft.fillRect(0, height - 30, width, 30,  RED);
  tft.setCursor(30, 50);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.print("Notes :_");
  // tft.fillRect(28, 90, 50, 50, WHITE);
  tft.setTextColor(BLACK);

  tft.print("A");
  tft.setTextSize(2);
  tft.setTextColor(WHITE);
  while (true)
  {
    if (isTouch())
    {
      Serial.print(tp.x);
      Serial.print(",");
      Serial.println(tp.y);
      if (tp.x > 150 && tp.x < 230)
      {
        return 3;
      }
      if (tp.x > 860 && tp.x < 925)
      {
        return 2;
      }
    }
  }
}
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
/////////////////STOP WATCH ////////////////////////////////////////////
int clk()
{ r1 = 0;//hour
  r2 = 0;//min
  r3 = 0;//sec
  r4 = 0; // 0->not running 1-> running
  tft.setTextSize(7);
  tft.setCursor(30, height / 2 - 62);
  tft.setTextColor(WHITE);
  tft.print(String(r1));
  tft.print(":");
  tft.print(String(r2));
  tft.print(":");
  tft.print(String(r3));
  tft.fillRect(0, 0, width, 30, WHITE);
  tft.fillRect(0, height - 30, width, 30,  RED);
  tft.fillRect(width / 2 - 75, height / 2 + 40, 150, 50, WHITE);
  tft.setTextSize(4);
  tft.setTextColor(BLUE);
  tft.setCursor(width / 2 - 57, height / 2 + 50);
  tft.print("Start");
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.setCursor(25, height - 62);
  tft.fillRect(0, height - 80, width / 2, 50, BLUE);
  tft.print("StopWatch");
  tft.setCursor(width / 2 + 45, height - 62);
  tft.print("Timer");
  tft.setCursor(30, 50);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.print("Clock : Stopwatch");
  // tft.fillRect(28, 90, 50, 50, WHITE);
  tft.setTextColor(BLACK);
  tft.print("A");
  tft.setTextSize(2);
  tft.setTextColor(WHITE);
  while (true)
  {
    if (isTouch())
    {
      Serial.print(tp.x);
      Serial.print(",");
      Serial.println(tp.y);
      if (tp.x > 150 && tp.x < 230)
      {
        return 3;
      }
      else if (tp.x > 860 && tp.x < 925)
      {
        return 2;
      }
      else if (tp.x > 790 && tp.x < 860) {
        if (tp.y > 582) {
          return 61;

        }
      }
      else if (tp.x > 600 && tp.x < 700 && tp.y > 419 && tp.y < 766) {

        if (r4 == 0) {
          r4 = 1;
          tft.fillRect(width / 2 - 75, height / 2 + 40, 150, 50, BLUE);
          tft.setTextSize(4);
          tft.setTextColor(WHITE);
          tft.setCursor(width / 2 - 57, height / 2 + 50);
          tft.print("Start");
        }
        else {
          r4 = 0;
          tft.fillRect(width / 2 - 75, height / 2 + 40, 150, 50, WHITE);
          tft.setTextSize(4);
          tft.setTextColor(BLUE);
          tft.setCursor(width / 2 - 57, height / 2 + 50);
          tft.print("Start");
        }


      }

    }


  }
}

int timer() {

  tft.fillRect(0, 0, width, 30, WHITE);
  tft.fillRect(0, height - 30, width, 30,  RED);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.setCursor(25, height - 62);
  tft.fillRect(0, height - 80, width / 2, 50, BLACK);
  tft.print("StopWatch");
  tft.setCursor(width / 2 + 45, height - 62);
  tft.fillRect(width / 2, height - 80, width / 2, 50, BLUE);
  tft.print("Timer");
  tft.setCursor(30, 50);
  tft.print("Clock : TIMER");

  while (true)
  {
    if (isTouch())
    {
      Serial.print(tp.x);
      Serial.print(",");
      Serial.println(tp.y);
      if (tp.x > 150 && tp.x < 230)
      {
        return 3;
      }
      if (tp.x > 860 && tp.x < 925)
      {
        return 2;
      }
      if (tp.x > 790 && tp.x < 860) {
        if (tp.y < 582) {
          return 6;


        }


      }
    }
  }

}



int ticTacToe()
{
  tft.fillRect(0, 0, width, 30, WHITE);
  tft.fillRect(0, height - 30, width, 30,  RED);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.setCursor(30, 50);
  tft.print("Tic Tac Toe : ");
  tft.fillRect(0, 90, width, 50, WHITE);
  tft.setCursor(10, 107);
  tft.setTextColor(BLACK);
  tft.print("1. Human Vs Human ");
  tft.fillRect(0, 165, width, 50, WHITE);
  tft.setCursor(10, 182);
  tft.print("2. Human Vs Computer");

  tft.setTextColor(WHITE);
  while (true)
  {
    if (isTouch())
    {
      Serial.print(tp.x);
      Serial.print(",");
      Serial.println(tp.y);
      if (tp.x > 150 && tp.x < 230)
      {
        return 3;
      }
      if (tp.x > 860 && tp.x < 925)
      {
        return 2;
      }
      if (tp.x > 284 && tp.x < 384) {
        return 71;
      }
      if (tp.x > 405 && tp.x < 500) {
        return 72;

      }
    }
  }
}

int ttch() {
  tft.fillRect(0, 0, width, 30, WHITE);
  tft.fillRect(0, height - 30, width, 30,  RED);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.setCursor(30, 50);
  tft.print("Tic Tac Toe : H Vs H");


  tft.setTextColor(WHITE);
  while (true)
  {
    if (isTouch())
    {
      Serial.print(tp.x);
      Serial.print(",");
      Serial.println(tp.y);
      if (tp.x > 150 && tp.x < 230)
      {
        return 3;
      }
      if (tp.x > 860 && tp.x < 925)
      {
        return 7;
      }
      if (tp.x > 790 && tp.x < 860) {
        if (tp.y < 582) {
          return 6;
        }
      }
    }
  }
}
int ttcc() {
  tft.fillRect(0, 0, width, 30, WHITE);
  tft.fillRect(0, height - 30, width, 30,  RED);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.setCursor(30, 50);
  tft.print("Tic Tac Toe : H Vs C ");

  tft.setTextColor(WHITE);
  while (true)
  {
    if (isTouch())
    {
      Serial.print(tp.x);
      Serial.print(",");
      Serial.println(tp.y);
      if (tp.x > 150 && tp.x < 230)
      {
        return 3;
      }
      if (tp.x > 860 && tp.x < 925)
      {
        return 7;
      }
      if (tp.x > 790 && tp.x < 860) {
        if (tp.y < 582) {
          return 6;
        }
      }
    }
  }
}
int calc()
{
  tft.fillRect(0, 0, width, 30, WHITE);
  tft.fillRect(0, height - 30, width, 30,  RED);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.setCursor(30, 50);
  tft.print("Calculator : ");
  tft.fillRect(0, 90, width, 50, WHITE);
  tft.setCursor(30, 50);
  r3 = 1;
  tft.setTextColor(BLACK);
  for (r1 = 0; r1 < 3; r1 += 1) {
    for (r2 = 0; r2 < 3; r2 += 1) {
      tft.fillRect(width * r1 / 4 + 10, height - 230 + (50 * r2) + 5, width / 4 - 20, 40, WHITE);
      tft.setCursor(width * r1 / 4 + 35, height - 230 + (50 * r2) + 20);
      tft.print(String(r3));
      r3 += 1;
    }
  }
  //tft.setTextColor(WHITE);
  tft.fillRect(width * 0 / 4 + 10, height - 230 + (50 * 3) + 5, width / 2 - 20, 40, WHITE);
  tft.setCursor(width * 0 / 4 + 35, height - 230 + (50 * 3) + 20);
  tft.print("0");
  //tft.setTextColor(BLACK);


  tft.fillRect(width * 2 / 4 + 10, height - 230 + (50 * 3) + 5, width / 4 - 20, 40, BLUE);
  tft.setCursor(width * 2 / 4 + 35, height - 230 + (50 * 3) + 20);
  tft.print("=");


  tft.fillRect(width * r1 / 4 + 10, height - 230 + (50 * 3) + 5, width / 4 - 20, 40, RED);
  tft.setCursor(width * r1 / 4 + 35, height - 230 + (50 * 3) + 20);
  tft.print("/");

  tft.fillRect(width * r1 / 4 + 10, height - 230 + (50 * 2) + 5, width / 4 - 20, 40, RED);
  tft.setCursor(width * r1 / 4 + 35, height - 230 + (50 * 2) + 20);
  tft.print("*");


  tft.fillRect(width * r1 / 4 + 10, height - 230 + (50 * 1) + 5, width / 4 - 20, 40, RED);
  tft.setCursor(width * r1 / 4 + 35, height - 230 + (50 * 1) + 20);
  tft.print("-");

  tft.fillRect(width * r1 / 4 + 10, height - 230 + (50 * 0) + 5, width / 4 - 20, 40, RED);
  tft.setCursor(width * r1 / 4 + 35, height - 230 + (50 * 0) + 20);
  tft.print("+");

  tft.setTextColor(WHITE);
  while (true)
  {
    if (isTouch())
    {
      Serial.print(tp.x);
      Serial.print(",");
      Serial.println(tp.y);
      if (tp.x > 150 && tp.x < 230)
      {
        return 3;
      }
      if (tp.x > 860 && tp.x < 925)
      {
        return 2;
      }


    }
  }
}
