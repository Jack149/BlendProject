#include <bitswap.h>
#include <chipsets.h>
#include <color.h>
#include <colorpalettes.h>
#include <colorutils.h>
#include <controller.h>
#include <cpp_compat.h>
#include <dmx.h>
#include <FastLED.h>
#include <fastled_config.h>
#include <fastled_delay.h>
#include <fastled_progmem.h>
#include <fastpin.h>
#include <fastspi.h>
#include <fastspi_bitbang.h>
#include <fastspi_dma.h>
#include <fastspi_nop.h>
#include <fastspi_ref.h>
#include <fastspi_types.h>
#include <hsv2rgb.h>
#include <led_sysdefs.h>
#include <lib8tion.h>
#include <noise.h>
#include <pixelset.h>
#include <pixeltypes.h>
#include <platforms.h>
#include <power_mgt.h>

 #include <LiquidCrystal_I2C.h>
    #include <Keypad.h>
    #include <Wire.h>
  
    #define NUMLED 60
    #define NUM_LEDS 60
    #define FRAMES_PER_SECOND  120
    #define BRIGHTNESS          255//96
    #define FRAMES_PER_SECOND  120
  
  #define DATA_PIN    3
#define BRIGHTNESS  128
#define LED_TYPE    WS2812
#define COLOR_ORDER GBR
#define NUM_LEDS 186

CRGB leds[NUM_LEDS];
  uint8_t max_bright = 255;     // Overall brightness definition. It can be changed on the fly.    
      LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address
    int counter = 0;
    boolean Light = false;
    String KeyCommand = "";
    String ProgCommand = "";
    int intProgCommand = 0;
    const byte ROWS = 4; 
    const byte COLS = 4; 
    char keys[ROWS][COLS] = {
      {'1','2','3','A'},
      {'4','5','6','B'},
      {'7','8','9','C'},
      {'*','0','#','D'}
    };
  // byte rowPins[ROWS] = {2,3,4,5}; //connect to row pinouts 
   // byte colPins[COLS] = {6,7,8,9}; //connect to column pinouts
   
    // byte rowPins[ROWS] = {6,7,8,9}; //connect to row pinouts //Use for BLACK good keypads.
    //byte colPins[COLS] = {2,3,4,5}; //connect to column pinouts
  
   byte rowPins[ROWS] = {8,9,10,11}; //connect to row pinouts //Black Key changed for UNO.
    byte colPins[COLS] = {4,5,6,7}; //connect to column pinouts
    
    Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
    
    const int bSize = 141;
    char code[2];
    char myString[10];
    char codeBuffer[2]; // Stores the code currently being entered.
    int keypressCount=0; // counts the number of keypresses
    int StringLength = ProgCommand.length();
    int ProgNum; //Used to store value of the entry in "B" mode.

      CRGB endclr;
      CRGB midclr; 

    void setup()
    {  //================== // BEGIN SETUP +++++++++===================
//        pinMode(PotPIN, INPUT); //potentiometer SPEED      A1?
//        pinMode (DimPIN, INPUT); // LED pin for DIMMER pot. A0?
//        pinMode(speedLED, OUTPUT);//POT for the speed of patterns
//        pinMode(dimLED, OUTPUT);//dim LED
//        pinMode(PIRpin, INPUT); //Passive Infra Red motion sensor
        Serial.println(" Version 082817 ");
        FastLED.addLeds<NEOPIXEL,3>(leds, NUM_LEDS); //PIN 3 is the LED pin, 60 total num in strip
           Serial.begin(9600); 
           Serial.println(" Version 082817 ");
        lcd.begin(20,4);   // initialize the lcd for 20 chars 4 lines, turn on backlight
     for(int i = 0; i< 3; i++)
  {
    lcd.backlight();
    delay(250);
    lcd.noBacklight();
    delay(250);
   } 

//    -------- Write characters on the display ------------------
//   NOTE: Cursor Position: (CHAR, LINE) start at 0  
    lcd.backlight();
    
    lcd.setCursor (4,1);
    lcd.print("JACKTRONICS");
    delay(2000);
//    lcd.clear();
//    lcd.setCursor(4,0); //Start at character 0 on line 0
//    
//    lcd.print("-= RGB Lamp =-");
//    
//    lcd.setCursor(6,2);
//    lcd.print("Version 2");
//    
//    lcd.setCursor(2,4);
//    lcd.print(" by  Jack Keeney");
//    
//    delay(4000);  
      lcd.noBacklight();
  lcd.clear();
    lcd.setCursor(0,0); //Start at character 0 on line 0 
   
    FastLED.addLeds<NEOPIXEL, 10>(leds, 60); //PIN 10 is the LED pin, 60 total num in strip
    FastLED.setBrightness(150);
//    Twinkle(); //Lets user know POWER ON was sucessful. <<<<<<<<<<<<<<<<<<<    
  } 
//=========================== START LOOP ========================================
    void loop()   {

     counter++;// simple LOOP COUNTER
delay(1);
     int b = 0;
     READ();
     
  //-------------------------------- Second Switch Case  -------------------
   // Serial.println(ProgNum);
    switch (ProgNum) {
      case 01:
         AquaOrange  ();

       break;//------------------------------------------------
   case 2:
            OrangeRed();
            
            
      break;//------------------------------------------------

  case 3:
            VioBlue();
            
            
      break;//------------------------------------------------
          
    case 4:
           RedBlue();
            
            
      break;//------------------------------------------------

    }
    }//============================ END LOOP ==========================================
    
void cycle() {
  fill_gradient_RGB(leds, 0, endclr, NUM_LEDS/2, midclr);
  fill_gradient_RGB(leds, NUM_LEDS/2+1, midclr, NUM_LEDS, endclr);
  
  }
  
     //---------------------------------------------------------------------------
 void READ() { //----------------- Read the Keypad
  
   char key = keypad.getKey();

  if (key != NO_KEY && key != 'A' && key != 'B' && key != 'C' && key != 'D'){
       
    keypressCount ++;
    KeyCommand += String(key);
    lcd.setCursor(9, 1);
    lcd.print(KeyCommand);
  }
  
  switch (key)  {
        case 'A':// <<<<<<<<<<<<<<<<<<<<<<<<<<< TURN ON/OFF LIGHT.
    if (key == 'A') {
      if (Light == true){
              Light = false;          
           }
          else {
              Light = true;            
          }
          
              if(Light == true)
          {  lcd.backlight();
          }
          else
          { lcd.noBacklight();
          }
           break;
  }
    case 'B': // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< PATTERN enter KEY
           MODE();
            break;
     case 'C':// Display info on LCD
           DisplayInfo();
             break;
     case 'D':

             break;
      case '*':// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<  CLEAR KEY
        lcd.clear();
        KeyCommand = "";
        keypressCount = 0;
        ProgNum = 00;
        lcd.noBlink();
        for (int x = 0; x < NUMLED; x++) {
            leds[x] = CRGB::Black; 
              FastLED.show();
              // digitalWrite(LockLEDpin, LOW);// turn OFF BLUE LED      
          }  //---------------
        break;   

   case '#'://  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<  ENTER KEY
        key = '0';
        ProgCommand == KeyCommand;
        StringLength == ProgCommand.length();
        ProgCommand.remove(StringLength);
        
        lcd.clear();
        lcd.setCursor(4,1);
        lcd.print("EXECUTING ");
        KeyCommand.remove(2);      
        ProgCommand = KeyCommand;
        lcd.print(KeyCommand);//print the current running pattern number.
        lcd.noBlink();
        ProgNum = ProgCommand.toInt();       
      delay(2000);       
        
        break;
    }   
 }
 //===========================END of READ Sub ===============================

void MODE()  {
  char Pkey = keypad.getKey();    
    lcd.clear();
      lcd.setCursor(4,0); //Start at character 4 on line 0
      lcd.print("Enter Prog# ");
      delay(20);
      lcd.setCursor(9,1);
      lcd.blink();
       KeyCommand = ""; // Clear the Command buffer
        ProgCommand = "";// Clear the Command buffer
    if (Pkey != NO_KEY && Pkey != 'A' && Pkey != 'B' && Pkey != 'C' && Pkey != 'D' && Pkey != '#'){
        
        KeyCommand += String(Pkey);
      
      ProgCommand += String(Pkey);
      intProgCommand = ProgCommand.toInt();
      lcd.write(intProgCommand);
    } 
  }
 void DisplayInfo()   {// This is displayed with the "C" key.
      lcd.backlight();
    
    lcd.setCursor (4,1);
    lcd.print("JACKTRONICS");
    delay(2000);
    lcd.clear();
    lcd.setCursor(3,1); //Start at character 0 on line 0
    
    lcd.print("-= RGB Lamp =-");
    
    lcd.setCursor(5,2);
    lcd.print("Version 2");
    
    lcd.setCursor(1,4);
    lcd.print(" by  Jack Keeney");
    
     lcd.setCursor (4,0);
    
    lcd.print("JACKTRONICS");
    //lcd.scrollDisplayLeft();
    delay(4000);  
      lcd.noBacklight();
  //lcd.clear();
    lcd.setCursor(0,0); //Start at character 0 on line 0     
}//-------------------------End of Dispaly sub ----------------------------

    void AquaOrange() {//  
       
      uint8_t speed = beatsin8(6,0,255);
  endclr = blend(CRGB::Aqua, CRGB::Orange, speed);
  midclr = blend(CRGB::Orange, CRGB::Aqua, speed);
  cycle();
  FastLED.show();
 }//------------------------------------------------------------------------- 

void OrangeRed() {//  
      uint8_t speed = beatsin8(6,0,255);
  endclr = blend(CRGB::Red, CRGB::Orange, speed);
  midclr = blend(CRGB::Orange, CRGB::Red, speed);
  cycle();
  FastLED.show();
      
      
 }//------------------------------------------------------------------------- 
  
 void VioBlue() {//  
      uint8_t speed = beatsin8(6,0,255);
  endclr = blend(CRGB::Blue, CRGB::Purple, speed);
  midclr = blend(CRGB::Purple, CRGB::Blue, speed);
  cycle();
  FastLED.show();
 }//--------------------------------------------------------------------------
 
 void RedBlue() {//  
     uint8_t speed = beatsin8(6,0,255);
  endclr = blend(CRGB::Aqua, CRGB::Red, speed);
  midclr = blend(CRGB::Red, CRGB::Aqua, speed);
  cycle();
  FastLED.show();
 }//--------------------------------------------------------------------------

      
    

