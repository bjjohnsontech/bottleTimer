//Project bottleTimer
// include the AnalogMultiButton library
#include <AnalogMultiButton.h>

#include <TimerOne.h>
 
#include <EEPROMex.h>

// define the pin you want to use
const int BUTTONS_PIN = A0;

// set how many buttons you have connected
const int BUTTONS_TOTAL = 5;

// find out what the value of analogRead is when you press each of your buttons and put them in this array
// you can find this out by putting Serial.println(analogRead(BUTTONS_PIN)); in your loop() and opening the serial monitor to see the values
// make sure they are in order of smallest to largest
const int BUTTONS_VALUES[BUTTONS_TOTAL] = {0, 90, 167, 232, 576};

// you can also define constants for each of your buttons, which makes your code easier to read
// define these in the same order as the numbers in your BUTTONS_VALUES array, so whichever button has the smallest analogRead() number should come first
const int sixtyBtn = 0;
const int ninetyBtn = 1;
const int dnBtn = 2;
const int upBtn = 3;
const int showBtn = 4;

// make an AnalogMultiButton object, pass in the pin, total and values array
AnalogMultiButton buttons(BUTTONS_PIN, BUTTONS_TOTAL, BUTTONS_VALUES);

const int beeperPin = 19;
const int ledPin = 18;

int ledState = 0;
int show = 1;

//the pins of 4-digit 7-segment display attach to pin2-13 respectively

int a = 2;
int b = 3;
int c = 4;
int d = 5;
int e = 6;
int f = 7;
int g = 8;
int p = 9;

int d4 = 10;
int d3 = 11;
int d2 = 12;
int d1 = 13;
/*value = EEPROM.read(address);*/
long total = 200;
long n = 0;// n represents the value displayed on the LED display. For example, when n=0, 0000 is displayed. The maximum value is 9999. 
int del = 5;//Set del as 5; the value is the degree of fine tuning for the clock
int count = 0;//Set count=0. Here count is a count value that increases by 1 every 0.1 second, which means 1 second is counted when the value is 10

void setup()
{
  /*Serial.begin(9600); /* Is this necessary? */
  pinMode(ledPin, OUTPUT);
  pinMode(beeperPin, OUTPUT);
  //set all the pins of the LED display as output
  // move to library with initialization
  pinMode(d1, OUTPUT);
  pinMode(d2, OUTPUT);
  pinMode(d3, OUTPUT);
  pinMode(d4, OUTPUT);
  pinMode(a, OUTPUT);
  pinMode(b, OUTPUT);
  pinMode(c, OUTPUT);
  pinMode(d, OUTPUT);
  pinMode(e, OUTPUT);
  pinMode(f, OUTPUT);
  pinMode(g, OUTPUT);
  pinMode(p, OUTPUT);

  Timer1.initialize(100000); // set a timer of length 100000 microseconds (or 0.1 sec - or 10Hz => the led will blink 5 times, 5 cycles of on-and-off, per second)
  // attach the service routine here

  int check = EEPROM.readInt(0);
  if(check){
    total = check; 
  }
}
/***************************************/ 

void loop() {
  buttons.update();
  if (show){
    displayOn(total);
  } else {
    displayOff();
  }
  if (buttons.onPressAfter(dnBtn, 2000)){
    resetTotal();
  } else if (buttons.onPressAfter(upBtn, 2000)){
    setTotal();
  } else if (buttons.onPress(sixtyBtn)) {
    n = 60;
    Timer1.attachInterrupt( countDown ); 
    while (n>0){
      displayOn(n);
    }
    beep(1, 350);
    ledControl(1);
  } else if (buttons.onPress(ninetyBtn) && ledState == 1) {
    n = 90;
    Timer1.attachInterrupt( countDown ); 
    while (n>0){
      displayOn(n);
    }
    
    ledControl(0);
    beep(2, 200);
    increaseTotal();
  } else if (buttons.onPress(upBtn)) {
    increaseTotal();
  } else if (buttons.onPress(dnBtn)) {
    decreaseTotal();
  } else if (buttons.onPress(showBtn)) {
    toggleShow();
  }/**/
  
}

void ledControl(int state){
  ledState = state;
  if (state == 1){
    digitalWrite(ledPin,HIGH);
  } else {
    digitalWrite(ledPin,LOW);
  }
}

void increaseTotal(){
  total ++;
  EEPROM.updateInt(0, total);
}

void decreaseTotal(){
  total --;
  EEPROM.updateInt(0, total);
}

void resetTotal(){
  if (confirm(ninetyBtn, sixtyBtn)){
    total = 0;
    EEPROM.updateInt(0, total);
  }
}

void setTotal(){
  bool done = 0;
  int store = total + 0;
  while (done == 0){
    buttons.update();
    displayOn(total);
    if(buttons.isPressed(upBtn))
    {
      total += 1;
    }
    if(buttons.isPressed(dnBtn))
    {
      total -= 1;
    }
    if(buttons.onPress(sixtyBtn)){
      done = 1;
      total = store;
    }
    if(buttons.onPress(ninetyBtn)){
      done = 2;
      EEPROM.updateInt(0, total);
    }
    delay(50);
  }
}

bool confirm(int confirm, int cancel){
  while (true){
    displayOn(8899);
    buttons.update();
    if (buttons.onPress(confirm)){
      return true;
    }
    if (buttons.onPress(cancel)){
      return false;
    }
  }
}

void countDown()
{
  // Toggle LED
  count ++;
  if(count == 10)
  {
    count = 0;
    n--;
  }
}

void toggleShow(){
  if (show == 1){
    show = 0;
  } else {
    show = 1;
  }
}

void beep(int beeps, int len){
  int go = beeps;
  while (go > 0){
    digitalWrite(beeperPin,HIGH); //set PIN8 is HIGH , about 5V
    delay(len); //delay 1000ms, 1000ms = 1s
    
    digitalWrite(beeperPin,LOW); //set PIN8 is LOW, 0V
    delay(300); //delay 1000ms, 1000ms = 1s
    go --;
  }
}

void displayOff(){
  clearLEDs();
}
void displayOn(long num)
{
  //clearLEDs();//clear the 7-segment display screen
  pickDigit(0);//Light up 7-segment display d1
  pickNumber((num/1000));// get the value of thousand
  delay(del);//delay 5ms

  //clearLEDs();//clear the 7-segment display screen
  pickDigit(1);//Light up 7-segment display d2
  pickNumber((num%1000)/100);// get the value of hundred
  delay(del);//delay 5ms

  //clearLEDs();//clear the 7-segment display screen
  pickDigit(2);//Light up 7-segment display d3
  pickNumber(num%100/10);//get the value of ten
  delay(del);//delay 5ms

  //clearLEDs();//clear the 7-segment display screen
  pickDigit(3);//Light up 7-segment display d4
  pickNumber(num%10);//Get the value of single digit
  delay(del);//delay 5ms
}
/**************************************/ 
void pickDigit(int x) //light up a 7-segment display
{
  //The 7-segment LED display is a common-cathode one. So also use digitalWrite to set d1 as high and the LED will go out
  digitalWrite(d1, HIGH);
  digitalWrite(d2, HIGH);
  digitalWrite(d3, HIGH);
  digitalWrite(d4, HIGH);

  switch(x)
  {
    case 0: 
    digitalWrite(d1, LOW);//Light d1 up 
    break;
    case 1: 
    digitalWrite(d2, LOW); //Light d2 up 
    break;
    case 2: 
    digitalWrite(d3, LOW); //Light d3 up 
    break;
    default: 
    digitalWrite(d4, LOW); //Light d4 up 
    break;
  }
}
//The function is to control the 7-segment LED display to display numbers. Here x is the number to be displayed. It is an integer from 0 to 9 
void pickNumber(int x)
{
  switch(x)
  {
    default: 
      zero(); 
      break;
    case 1: 
      one(); 
      break;
    case 2: 
      two(); 
      break;
    case 3: 
      three(); 
      break;
    case 4: 
      four(); 
      break;
    case 5: 
      five(); 
      break;
    case 6: 
      six(); 
      break;
    case 7: 
      seven(); 
      break;
    case 8: 
      eight(); 
      break;
    case 9: 
      nine(); 
      break;
  }
} 
void clearLEDs() //clear the 7-segment display screen
{
  digitalWrite(a, LOW);
  digitalWrite(b, LOW);
  digitalWrite(c, LOW);
  digitalWrite(d, LOW);
  digitalWrite(e, LOW);
  digitalWrite(f, LOW);
  digitalWrite(g, LOW);
  digitalWrite(p, LOW);
}

void zero() //the 7-segment led display 0
{
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(g, LOW);
}

void one() //the 7-segment led display 1
{
  digitalWrite(a, LOW);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, LOW);
  digitalWrite(e, LOW);
  digitalWrite(f, LOW);
  digitalWrite(g, LOW);
}

void two() //the 7-segment led display 2
{
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, LOW);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, LOW);
  digitalWrite(g, HIGH);
}

void three() //the 7-segment led display 3
{
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, LOW);
  digitalWrite(f, LOW);
  digitalWrite(g, HIGH);
}

void four() //the 7-segment led display 4
{
  digitalWrite(a, LOW);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, LOW);
  digitalWrite(e, LOW);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
}

void five() //the 7-segment led display 5
{
  digitalWrite(a, HIGH);
  digitalWrite(b, LOW);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, LOW);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
}

void six() //the 7-segment led display 6
{
  digitalWrite(a, HIGH);
  digitalWrite(b, LOW);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
}

void seven() //the 7-segment led display 7
{
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, LOW);
  digitalWrite(e, LOW);
  digitalWrite(f, LOW);
  digitalWrite(g, LOW);
}

void eight() //the 7-segment led display 8
{
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
}

void nine() //the 7-segment led display 9
{
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, LOW);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
}

void decimal(){
  digitalWrite(p, HIGH);
}
/*******************************************/
void add()
{
  // Toggle LED
  count ++;
  if(count == 10)
  {
    count = 0;
    n++;
    if(n == 10000)
    {
      n = 0;
    }
  }
}
