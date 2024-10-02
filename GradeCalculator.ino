#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <stdlib.h>

const byte ROWS = 4;
const byte COLS = 4;
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}};
byte rowPins[ROWS] = {10, 9, 8, 7};
byte colPins[COLS] = {6, 5, 4, 3};
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
char keyPressed;

LiquidCrystal_I2C lcd(0x27,20,4);

int buzzer=11;

int Score(int first,int performance,int target,int fr,int pr,int sr)
{
  int cutline=(target*100-first*35-performance*30-50+34)/35;
  if(cutline>100)
  {
    return(101);
  }
  else if(cutline>0)
  {
    return(cutline);
  }
  else
  {
    return(0);
  }
}

void setup()
{
  pinMode(buzzer, OUTPUT);
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);
}

int step=1;
char select;
int first,performance,target,fr,sr,pr;
int warning=0;
char input[19];
int score;
int digit=0;
int cal=0;
int zerocount=0;
int excutline;
int cutline;

void clear()
{
  lcd.setCursor(0,0);
  lcd.print("                    ");
  lcd.setCursor(0,1);
  lcd.print("                    ");
  lcd.setCursor(0,2);
  lcd.print("                    ");
  lcd.setCursor(0,3);
  lcd.print("                    ");
}

int Print(int first,int performance,int fr,int pr,int sr,int target)
{
  cutline=Score(first,performance,target,fr,pr,sr);
  excutline=Score(first,performance,target+10,fr,pr,sr);
  if(excutline==0)
  {
    lcd.print("X");
  }
  else if(cutline==101)
  {
    if(cal==4)
    {
      lcd.print(100);
      lcd.print("-");
      lcd.print(0);
    }
    else
    {
      lcd.print("You Cant");
    }
  }
  else if(cutline==0)
  {
    if(cal==0)
    {
      lcd.print("100-0");
    }
    else
    {
      lcd.print(excutline-1);
      lcd.print("-");
      lcd.print(cutline);
    }

  }
  else
  {
    if(cal==0)
    {
      lcd.print(100);
      lcd.print("-");
      lcd.print(cutline);
    }
    else if(cal==4)
    {
      lcd.print(excutline-1);
      lcd.print("-");
      lcd.print(0);
    }
    else
    {
      lcd.print(excutline-1);
      lcd.print("-");
      lcd.print(cutline);
    }
  }
  cal+=1;
  return;
}

void WarningBuzzer()
{
  for(int n=0; n<3; n+=1)
  {
    digitalWrite(buzzer, HIGH);
      delay(100);
    digitalWrite(buzzer, LOW);
      delay(100);
    lcd.setCursor(0,0);
  }
}

void ClickBuzzer()
{
  digitalWrite(buzzer, HIGH);
    delay(10);
  digitalWrite(buzzer, LOW);
}

void Warning(int type)
{
  clear();

  lcd.setCursor(0,0);
  lcd.print("--------------------");

  if(type==0)
  {
    lcd.setCursor(2,1);
    lcd.print("Please enter the");
    lcd.setCursor(4,2);
    lcd.print("number shown");
  }
  if(type==1)
  {
    lcd.setCursor(2,1);
    lcd.print("Please enter the");
    lcd.setCursor(3,2);
    lcd.print("natural number");
  }
  else if(type==2)
  {
    lcd.setCursor(2,1);
    lcd.print("Please enter the");
    lcd.setCursor(4,2);
    lcd.print("usual score");
  }

  lcd.setCursor(0,3);
  lcd.print("--------------------");
  
  for(int n=0;n<19;n+=1)
  {
    input[n]=0;
  }
  digit=0;
  warning=0;

  WarningBuzzer();
  delay(400);
  clear();
}

void loop()
{
  char key = customKeypad.getKey();

  if(step==1)
  {
    lcd.setCursor(1,0);
    lcd.print("This is Seunggyu's");
    lcd.setCursor(2,1);
    lcd.print("Grade Calculator");
    lcd.setCursor(0,2);
    lcd.print("--------------------");
    lcd.setCursor(1,3);
    lcd.print("Press '*' to START");

    if(key)
    {
      step+=1;
      ClickBuzzer();
    }
  }

  else if(step==2)
  {
    step+=1;
      clear();
  }

  else if(step==3)
  {
    lcd.setCursor(0,0);
    lcd.print("Select a ratio of");
    lcd.setCursor(0,1);
    lcd.print("1st:performance:2nd");
    lcd.setCursor(4,2);
    lcd.print("1. 35:30:35");
    lcd.setCursor(4,3);
    lcd.print("2. 30:30:40");
    

    if(key)
    {
      select=(key);
      ClickBuzzer();
      if(select=='1')
      {
        fr=35;
        pr=30;
        sr=35;
        step+=1;
      }
      else if(select=='2')
      {
        fr=30;
        pr=30;
        sr=40;
        step+=1;
      }
      else
      {
        Warning(0);
      }
    }
  }

  else if(step==4)
  {
      clear();
      step+=1;
  }

  else if(step==5||step==7)
  {
    lcd.setCursor(0,0);
    lcd.print("Please enter your");
    lcd.setCursor(0,1);
    if(step==5)
    {
      lcd.print("score of first exam");
    }
    else if(step==7)
    {
      lcd.print("score of performance");
    }
    lcd.setCursor(0,2);
    lcd.print("and press '*' button");
    lcd.setCursor(0,3);
    lcd.print(">");
    
    lcd.setCursor(1,3);
    if(key) {
      ClickBuzzer();

      if(key!='*')
      {
        input[digit]=key;
        //strcat(input, &key);
        digit+=1;

        if(key=='A'||key=='B'||key=='C'||key=='D'||key=='#')
        {
          warning+=1;
        }
      }
      else if(key=='*')
      {
        score=atoi(input);
        if(warning!=0)
        {
          Warning(1);
        }
        else if(digit>3||digit==0||score>100)
        {
          Warning(2);
        }
        else
        {
          score=atoi(input);
          if(step==5)
          {
            first=score;
          }
          else if(step==7)
          {
            performance=score;
          }
          step+=1;
        }
      }
      lcd.print(input);
    }
  }

  else if(step==6||step==8)
  {
    for(int n=0;n<19;n+=1)
    {
      input[n]=0;
    }
    digit=0;
    
    clear();
    step+=1;
  }

  else if(step==9)
  {
    lcd.setCursor(0,0);
    lcd.print("You must achieve the");
    lcd.setCursor(0,1);
    lcd.print("indicated score to");
    lcd.setCursor(0,2);
    lcd.print("receive the grade");
    lcd.setCursor(0,3);
    lcd.print("Press '*' see result");

    if(key)
    {
      ClickBuzzer();
      step+=1;
    }
  }

  else if(step==10)
  {
    clear();
    step+=1;
  }

  else if(step==11)
  {
    lcd.setCursor(0,0);
    lcd.print("A:");
    Print(first,performance,fr,pr,sr,90);

    lcd.setCursor(10,0);
    lcd.print("B:");
    Print(first,performance,fr,pr,sr,80);

    lcd.setCursor(0,1);
    lcd.print("C:");
    Print(first,performance,fr,pr,sr,70);

    lcd.setCursor(10,1);
    lcd.print("D:");
    Print(first,performance,fr,pr,sr,60);

    lcd.setCursor(0,2);
    lcd.print("E:");
    Print(first,performance,fr,pr,sr,50);

    lcd.setCursor(0,3);
    lcd.print("Press '*' to RESTART");

    cal=0;
    step+=1;
  }

  else if(step==12)
  {
    if(key)
    {
      ClickBuzzer();
      step+=1;
    }
  }

  else if(step==13)
  {
    clear();
    step=3;
  }
}
