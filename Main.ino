#include <Keypad.h>
#include <LiquidCrystal.h>
#include <math.h>

LiquidCrystal lcd(1, 2, 3, 4, 5, 6);
const byte ROWS = 4; 
const byte COLS = 3; 

char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

char operations[] = {'+', '-', '*', '/', 'r', 's', 'c'};

byte rowPins[ROWS] = { 7, 8, 9, 10 };

byte colPins[COLS] = { 11, 12, 13 }; 

Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

long number1 = 0, number2=0;

double result = 0;

short int state = 1;

char opr = '.';

bool changeState = true;

short int charInt;

char key;

void setup()
{
  lcd.begin(16,2);

  lcd.setCursor(0, 1);
  lcd.print(number1);
}

void loop()
{
  if(changeState){
    switch(state){
      case 1:
        lcd.setCursor(0, 0);
        lcd.print("*:Operation #:CE");
        lcd.setCursor(1, 1);
        lcd.blink();
        break;
      case 2:
        lcd.clear();
        lcd.print("1:+ 2:- 3:* 4:/");
        lcd.setCursor(0,1);
        lcd.print("5:sr 6:sin 7:cos");
        break;
      case 3:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("*:Calculate #:CE");
        lcd.setCursor(0, 1);
        lcd.print(opr);
        lcd.print("~");
        break;
      case 4:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("#:CE");
        lcd.setCursor(0, 1);
        lcd.print("=");
        lcd.print(result);
        break;
    }
    changeState = false;
  }
  key = kpd.getKey();
  charInt = ((int)key)-48;
  if(key){
    if(state == 3)
      lcd.setCursor(2, 1);
    else
      lcd.setCursor(0, 1);
    
    if(charInt <= 10 && charInt >= 0){
      long tmp = 0;
      if(state == 1)
        tmp = number1 = (number1 * 10) + charInt;
      else if(state == 2)
        selectOperation(charInt);
      else if(state == 3)
        tmp = number2 = (number2 * 10) + charInt;
        
      lcd.print(tmp);
      
    }
    else if(key == '*'){
      if(state == 1){
        changeStateFn(2);
      }
      else if(state == 3)
        calc();
      else if(state == 4)
        _clear();
    }
    else if(key == '#'){
      _clear();
    }
  }
}
void selectOperation(short int charIntParam){
  opr = operations[charIntParam-1];
  if(opr == 'r' || opr == 's' || opr == 'c')
    calc();
  else
    changeStateFn(3);
}

void changeStateFn(short int stateParam){
  changeState = true;
  state = stateParam;
}

void calc(){
  switch(opr){
    case 'r':
      result = sqrt(number1);
      break;
    case 's':
      result = sin(number1);
      break;
    case 'c':
      result = cos(number1);
      break;
    case '+':
      result = number1 + number2;
      break;
    case '-':
      result = number1 - number2;
      break;
    case '*':
      result = number1 * number2;
      break;
    case '/':
      if(number1 > 0)
        result = number1 / (double)number2;
      break;
  }
  changeStateFn(4);
}
void _clear(){
  number1 = number2 = result = 0;
  opr = '.';
  
  lcd.clear();
  changeStateFn(1);

  lcd.setCursor(0, 1);
  lcd.print(number1);
}
