//int NUM_SENSORS = 5;
int MAX_MOTOR_SPEED = 240;

// PIN của motor 
const int RIGHT_MOTOR_PIN1 = 2;
const int RIGHT_MOTOR_PIN2 = 3;

const int LEFT_MOTOR_PIN1 = 4;
const int LEFT_MOTOR_PIN2 = 5;

const int PWD_MOTOR_PIN1 = 6;
const int PWD_MOTOR_PIN2 = 7;

// PIN của IR Sensor
int IR_SENSOR_PINS[5] = {A4, A3, A2, A1, A0};

// Hằng số Kp, Ki, Kd dùng trong điều khiển theo thuật toán PID
// Sẽ điều chỉnh trong quá trình thực hành 
float Kp = 0.07, Ki = 0, Kd = 0.8; 
// Các giá trị liên quan đến tính toán theo thuật toán PID
float integral = 0;
float error = 0, preError = 0;
float PIDValue = 0;

// Mảng lưu giá trị đọc được từ cảm biến 
unsigned int sensorValues[5];

// Tốc độ ban đầu của motor
int initSpeed = 100;

void getIRSensorData(void);
void calculate_pid(void);
void motor_control(void);

void setup() {
 
 pinMode(PWD_MOTOR_PIN1, OUTPUT);
 pinMode(PWD_MOTOR_PIN2, OUTPUT);
 
 // Set pin cho motor
 pinMode(LEFT_MOTOR_PIN1, OUTPUT);
 pinMode(LEFT_MOTOR_PIN2,OUTPUT);
 pinMode(RIGHT_MOTOR_PIN1, OUTPUT); 
 pinMode(RIGHT_MOTOR_PIN1, OUTPUT); 
 
 // Set pin cho IR sensor
 for(int i = 0; i < 5; i++) {
    pinMode(IR_SENSOR_PINS[i], INPUT);
 }

 Serial.begin(9600);
}

void loop() {
    getIRSensorData();
    calculatePID();

    int leftSpeed = constrain(initSpeed - PIDValue, 0, MAX_MOTOR_SPEED);
    int rightSpeed = constrain(initSpeed + PIDValue, 0, MAX_MOTOR_SPEED);
//    Serial.print("Left speed" + leftSpeed);
    
    controlMotor(leftSpeed, rightSpeed);
}

void getIRSensorData() {
  for(int i = 0; i < 5; i++) {
    sensorValues[i] = digitalRead(IR_SENSOR_PINS[i]);
//    Serial.print(sensorValues[i]);
  }
  
  if((sensorValues[0]==0)&&(sensorValues[1]==0)&&(sensorValues[2]==0)&&(sensorValues[4]==0)&&(sensorValues[4]==1))
  error=4;
  else if((sensorValues[0]==0)&&(sensorValues[1]==0)&&(sensorValues[2]==0)&&(sensorValues[4]==1)&&(sensorValues[4]==1))
  error=3;
  else if((sensorValues[0]==0)&&(sensorValues[1]==0)&&(sensorValues[2]==0)&&(sensorValues[4]==1)&&(sensorValues[4]==0))
  error=2;
  else if((sensorValues[0]==0)&&(sensorValues[1]==0)&&(sensorValues[2]==1)&&(sensorValues[4]==1)&&(sensorValues[4]==0))
  error=1;
  else if((sensorValues[0]==0)&&(sensorValues[1]==0)&&(sensorValues[2]==1)&&(sensorValues[4]==0)&&(sensorValues[4]==0))
  error=0;
  else if((sensorValues[0]==0)&&(sensorValues[1]==1)&&(sensorValues[2]==1)&&(sensorValues[4]==0)&&(sensorValues[4]==0))
  error=-1;
  else if((sensorValues[0]==0)&&(sensorValues[1]==1)&&(sensorValues[2]==0)&&(sensorValues[4]==0)&&(sensorValues[4]==0))
  error=-2;
  else if((sensorValues[0]==1)&&(sensorValues[1]==1)&&(sensorValues[2]==0)&&(sensorValues[4]==0)&&(sensorValues[4]==0))
  error=-3;
  else if((sensorValues[0]==1)&&(sensorValues[1]==0)&&(sensorValues[2]==0)&&(sensorValues[4]==0)&&(sensorValues[4]==0))
  error=-4;
  else if((sensorValues[0]==0)&&(sensorValues[1]==0)&&(sensorValues[2]==0)&&(sensorValues[4]==0)&&(sensorValues[4]==0))
    if(error==-4) error=-5;
    else error=5;

}

void calculatePID() {
    integral = integral + error;
    PIDValue = Kp * error + Kd * (error - preError) + Ki * integral;
    preError = error;
}

void controlMotor(int leftSpeed, int rightSpeed){
    if (leftSpeed <= 0) {
      digitalWrite(LEFT_MOTOR_PIN1, 0);
      digitalWrite(LEFT_MOTOR_PIN2, 1);
    } else {
      digitalWrite(LEFT_MOTOR_PIN1, 1);
      digitalWrite(LEFT_MOTOR_PIN2, 0);
    }
  
    if (rightSpeed <= 0) {
      digitalWrite(RIGHT_MOTOR_PIN1, 0);
      digitalWrite(RIGHT_MOTOR_PIN2, 1);
    } else {
      digitalWrite(RIGHT_MOTOR_PIN1, 1);
      digitalWrite(RIGHT_MOTOR_PIN2, 0);
    }  
    analogWrite(PWD_MOTOR_PIN1, abs(leftSpeed)); 
    analogWrite(PWD_MOTOR_PIN2, abs(rightSpeed));  
}
