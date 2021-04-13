/*
  Analog input, analog output, serial output

 Reads an analog input pin, maps the result to a range from 0 to 255
 and uses the result to set the pulsewidth modulation (PWM) of an output pin.
 Also prints the results to the serial monitor.

 The circuit:
 * potentiometer connected to analog pin 0.
   Center pin of the potentiometer goes to the analog pin.
   side pins of the potentiometer go to +5V and ground
 * LED connected from digital pin 9 to ground

 created 29 Dec. 2008
 modified 9 Apr 2012
 by Tom Igoe

 This example code is in the public domain.

 */

// These constants won't change.  They're used to give names
// to the pins used:
const int EnA = 11;  // Analog input pin that the potentiometer is attached to
const int EnB = 10; // Analog output pin that the LED is attached to
const int InA = 13;
const int InB = 12;
const int InC = 9;
const int InD = 8;

const int Pin_ss1 = A4; // Analog output pin that the LED is attached to
const int Pin_ss2 = A3;
const int Pin_ss3 = A2;
const int Pin_ss4 = A1;
const int Pin_ss5 = A0;

int sensorValue = 0;        // value read from the pot
int outputValue = 0;        // value output to the PWM (analog out)

int IN_line,In_line_last,mode;
int ss1,ss2,ss3,ss4,ss5;

void setup() {
  // initialize serial communications at 9600 bps:
  pinMode(InA, OUTPUT);
  pinMode(InB, OUTPUT);
  pinMode(InC, OUTPUT);
  pinMode(InD, OUTPUT);
  pinMode(13, OUTPUT);
  digitalWrite(InA, LOW);
  digitalWrite(InB, HIGH);
  digitalWrite(InC, LOW);
  digitalWrite(InD, HIGH);
  digitalWrite(13, LOW);
  analogWrite(EnA,255);
  analogWrite(EnB,255);
  pinMode(Pin_ss1, INPUT);//Set chân cảm biến 1 là input
  pinMode(Pin_ss2, INPUT);//Set chân cảm biến 2 là input
  pinMode(Pin_ss3, INPUT);//Set chân cảm biến 3 là input
  pinMode(Pin_ss4, INPUT);//Set chân cảm biến 4 là input
  pinMode(Pin_ss5, INPUT);//Set chân cảm biến 4 là input
  //Serial.begin(9600);
}

void loop() {
  In_line_last = IN_line;
  IN_line = In_SenSor();
  if(IN_line!=In_line_last)
  {
    if(In_line_last!=5)
    {
      robotMover(IN_line);
    } 
  }
}
void robotMover (int line)
{
  switch (line)
  {
    case -4:// sang trai
    {
      analogWrite(EnA,150);//banh phai
      analogWrite(EnB,25);//banh trai
      //delay(200);
      break;
    }
    case -3://sang trai
    {
      analogWrite(EnA,100);//banh phai
      analogWrite(EnB,25);//banh trai
      //delay(50);
      break;
    }
    case -2:// sang trai
    {
      analogWrite(EnA,100);//banh phai
      analogWrite(EnB,50);//banh trai
      //delay(30);
      break;
    };
    case -1:// sang trai
    {
      analogWrite(EnA,150);//banh phai
      analogWrite(EnB,100);//banh trai
      //delay(10);
      break;
    }
    case 0:// quay phải
    {
      analogWrite(EnA,150);//banh phai
      analogWrite(EnB,150);//banh trai
      break;
    }
    case 1:// rẽ phai
    {
      analogWrite(EnA,100);//banh phai
      analogWrite(EnB,150);//banh trai
      //delay(10);
      break;
    };
    case 2:// rẽ phai
    {
      analogWrite(EnA,50);//banh phai
      analogWrite(EnB,100);//banh trai
     // delay(30);
      break;
    }
    case 3:// rẽ phai
    {
      analogWrite(EnA,25);//banh phai
      analogWrite(EnB,100);//banh trai
      //delay(50);
      break;
    }
    case 4:// rẽ phai
    {
      analogWrite(EnA,25);//banh phai
      analogWrite(EnB,150);//banh tra
     // delay(200);
      break;
    }
    case 5:// rẽ phải
    {
      Serial.print(mode);
      if(mode==0)
      {
        analogWrite(EnA,80);//banh phai
        analogWrite(EnB,0);//banh tra
      }
      else
      {
        analogWrite(EnA,0);//banh phai
        analogWrite(EnB,80);//banh tra
      }
      break;
    }
    case 6:{
      analogWrite(EnA,80);//banh phai
      analogWrite(EnB,0);//banh tra
      //delay(1000);
      break;
    }
    case 7:{
      analogWrite(EnA,0);//banh phai
      analogWrite(EnB,80);//banh tra
      //delay(1000);
      break;
    }
  }
}
int In_SenSor()
{
  ss1 = digitalRead(Pin_ss1);
  ss2 = digitalRead(Pin_ss2);
  ss3 = digitalRead(Pin_ss3);
  ss4 = digitalRead(Pin_ss4);
  ss5 = digitalRead(Pin_ss5);
  
  if((ss1==1)&&(ss2==1)&&(ss3==0)&&(ss4==1)&&(ss5==1))//giua line
  {
    return 0; 
  }
  else if((ss1==1)&&(ss2==1)&&(ss3==0)&&(ss4==0)&&(ss5==1))//lech trai cap 1
  {
    mode=1;
    return 1;
  }
  else if((ss1==1)&&(ss2==1)&&(ss3==1)&&(ss4==0)&&(ss5==1))//lech trai cap 2
  {
    mode=1;
    return 2;
  }
  else if((ss1==1)&&(ss2==1)&&(ss3==1)&&(ss4==0)&&(ss5==0))//lech trai cap 3
  {
    mode=1;
    return 3;
  }
  else if((ss1==1)&&(ss2==1)&&(ss3==1)&&(ss4==1)&&(ss5==0))//lech trai cap 4////////////////
  {
    mode=1;
    return 4;
  }
  else if((ss1==1)&&(ss2==0)&&(ss3==0)&&(ss4==1)&&(ss5==1))//lech phai cap 1
  {
    mode=0;
    return -1;
  }
  else if((ss1==1)&&(ss2==0)&&(ss3==1)&&(ss4==1)&&(ss5==1))//lech phai cap 2
  {
    mode=0;
    return -2;
  }
  else if((ss1==0)&&(ss2==0)&&(ss3==1)&&(ss4==1)&&(ss5==1))//lech phai cap 3
  {
    mode=0;
    return -3;
  }
  else if((ss1==0)&&(ss2==1)&&(ss3==1)&&(ss4==1)&&(ss5==1))//lech phai cap 4
  {
    mode=0;
    return -4;
  }
  else if((ss1==1)&&(ss2==1)&&(ss3==1)&&(ss4==1)&&(ss5==1))//lech hoàn toàn
  {
    return 5;
  }
  else if((ss1==0)&&(ss2==0)&&(ss3==0)&&(ss5==1))//góc vuong sang trai
  {
    mode=0;
    return 6;
  }
  else if((ss1==1)&&(ss3==0)&&(ss4==0)&&(ss5==0))//góc vuong sang phai
  {
    mode=1;
    return 7;
  }
  
  
}
