//double prop = 1;
//double scale = 0.7;

// higher
//double prop = 0.9;
//double scale = 0.67;
// slow ver
double prop = 0.6;
double scale = 0.64;

int freq = 2000;
int Kp = floor(120*prop);      
int Ki=0;
int Kd=floor (160*prop);
int pwm1=floor(450*prop);
int pwm2;
//------------------------GPIO-----------------
//https://tapit.vn/chuc-nang-nhap-xuat-tin-hieu-gpio-tren-nodemcu-esp32-dev-kit-va-nhung-luu-y-khi-su-dung/
//int cb5 = 16;
//int cb4 = 5;
//int cb3 = 4;
//int cb2 = 14;
//int cb1 = 12;
 
//---------------
//int dc1a = 15;     //////// a > 0 tien len, b< 0 lui xuong
//int dc1b = 13;     ///////  1 banh tra  i 2 banh phai
//int dc2a = 2; 
//int dc2b = 0;


// new point

int cb1 = 14;
int cb2 = 12;
int cb3 = 13;
int cb4 = 5;
int cb5 = 4;

int dc1a = 16;
//int dc1b = 16;
int dc2a = 15;
//int dc2b = 14;
//--------------
int sgn[5];
//----------------------------------------------
int error = 0;
int preError = 0;
int P, I, D, PIDvalue;
//----------------------------------------------
 void readSgn() {
  sgn[0] = digitalRead(cb1);
  sgn[1] = digitalRead(cb2);
  sgn[2] = digitalRead(cb3);
  sgn[3] = digitalRead(cb4);
  sgn[4] = digitalRead(cb5);
}
void show(){
  String t = "Value: ";
  Serial.println(t + sgn[0] + "  " + sgn[1] + "  " + sgn[2] + "  " + sgn[3] + "  " + sgn[4]);
}
void stop() {
  digitalWrite(dc1a, LOW);
//  digitalWrite(dc1b, LOW);
  digitalWrite(dc2a, LOW);
//  digitalWrite(dc2b, LOW);
}

bool caculateError() {
  readSgn();
  if (((sgn[0] == 0) && (sgn[1] == 0) && (sgn[2] == 0) && (sgn[3] == 0) && (sgn[4] == 1)) ||
      ((sgn[0] == 1) && (sgn[1] == 1) && (sgn[2] == 1) && (sgn[3] == 1) && (sgn[4] == 0))) error = 4;
  
  else if (((sgn[0] == 0) && (sgn[1] == 0) && (sgn[2] == 0) && (sgn[3] == 1) && (sgn[4] == 1))||
           ((sgn[0] == 1) && (sgn[1] == 1) && (sgn[2] == 1) && (sgn[3] == 0) && (sgn[4] == 0))) error = 3;   
  
  else if (((sgn[0] == 0) && (sgn[1] == 0) && (sgn[2] == 0) && (sgn[3] == 1) && (sgn[4] == 0)) ||
           ((sgn[0] == 1) && (sgn[1] == 1) && (sgn[2] == 1) && (sgn[3] == 0) && (sgn[4] == 1))) error = 2;
  
  else if (((sgn[0] == 0) && (sgn[1] == 0) && (sgn[2] == 1) && (sgn[3] == 1) && (sgn[4] == 0))||
           ((sgn[0] == 1) && (sgn[1] == 1) && (sgn[2] == 0) && (sgn[3] == 0) && (sgn[4] == 1))) error = 1;
  
  else if (((sgn[0] == 0) && (sgn[1] == 0) && (sgn[2] == 1) && (sgn[3] == 0) && (sgn[4] == 0))||
           ((sgn[0] == 1) && (sgn[1] == 1) && (sgn[2] == 0) && (sgn[3] == 1) && (sgn[4] == 1))) error = 0;
           
  else if (((sgn[0] == 0) && (sgn[1] == 1) && (sgn[2] == 1) && (sgn[3] == 0) && (sgn[4] == 0))||
           ((sgn[0] == 1) && (sgn[1] == 0) && (sgn[2] == 0) && (sgn[3] == 1) && (sgn[4] == 1))) error = -1;
           
  else if (((sgn[0] == 0) && (sgn[1] == 1) && (sgn[2] == 0) && (sgn[3] == 0) && (sgn[4] == 0))||
           ((sgn[0] == 1) && (sgn[1] == 0) && (sgn[2] == 1) && (sgn[3] == 1) && (sgn[4] == 1))) error = -2;
           
  else if (((sgn[0] == 1) && (sgn[1] == 1) && (sgn[2] == 0) && (sgn[3] == 0) && (sgn[4] == 0))||
           ((sgn[0] == 0) && (sgn[1] == 0) && (sgn[2] == 1) && (sgn[3] == 1) && (sgn[4] == 1))) error = -3;
           
  else if (((sgn[0] == 1) && (sgn[1] == 0) && (sgn[2] == 0) && (sgn[3] == 0) && (sgn[4] == 0))||
           ((sgn[0] == 0) && (sgn[1] == 1) && (sgn[2] == 1) && (sgn[3] == 1) && (sgn[4] == 1))) error = -4;
           
  else {
    return false;
  }
//  Serial.print("gia tri cua các tham so la:");
//  Serial.print(error);
//  Serial.print("\n");
  return true;
}
void calculatePID()
{ 
  if(caculateError()){
    PIDvalue = (Kp * error) + (Kd * (error - preError));;
    preError = error; 
  }
}
void control() {
  calculatePID();
  if(error != 0) {
     analogWrite(dc1a, floor((pwm1 + PIDvalue))/scale);
     analogWrite(dc2a, (pwm1-PIDvalue));
  }else{
     analogWrite(dc1a, floor(pwm1/scale));
     analogWrite(dc2a,pwm1);
  } 
//  analogWrite(dc1a, floor((pwm1 + PIDvalue)/scale));
//  analogWrite(dc2a, (pwm1 - PIDvalue));
//  analogWrite(dc1a, floor(pwm1/scale));
//  analogWrite(dc2a, pwm1);
}

void setup(){
  Serial.begin(9600);
  analogWriteFreq(freq);
  pinMode(cb1, INPUT);
  pinMode(cb2, INPUT);
  pinMode(cb3, INPUT);
  pinMode(cb4, INPUT);
  pinMode(cb5, INPUT);

  pinMode(dc1a, OUTPUT);
//  pinMode(dc1b, OUTPUT);
  pinMode(dc2a, OUTPUT);
//  pinMode(dc2b, OUTPUT); 
  stop();
  delay(2000);
}

void test(){
  int d;
  if (Serial.available() > 0) {
  d = Serial.parseInt(); // read the incoming byte:
  Serial.print(" I received:");
  Serial.println(d);
  if (d) {
    pwm1 = d;
    analogWrite(dc1a, floor(pwm1/0.6));
    analogWrite(dc2a, pwm1);
    }
  }
}

void loop()
{
  readSgn();
//  show(); //  Serial.println("hii");
//  test();
  control();
}
