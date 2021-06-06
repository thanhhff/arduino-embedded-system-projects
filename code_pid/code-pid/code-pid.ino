const int EnA = 11; // right
const int EnB = 10; // left
const int InA = 13;
const int InB = 12;
const int InC = 9;
const int InD = 8;

int error, previous_error;

const int MAX_SPEED = 140;
const int START_SPEED = 75;

int mask;
const int NUM_SENSORS = 5; 
const int sensor_pins[NUM_SENSORS] = {A4, A3, A2, A1, A0};

float Kp = 6.5;
float Kd = 2.8;
float Ki = 0;

int integral = 0;
int rightSpeedPwm = 0;
int leftSpeedPwm = 0;

int getSensor();
void control_motor(int left, int right);
void control_robot(int error);
float calculate_pid();

void setup() {
    pinMode(InA, OUTPUT);
    pinMode(InB, OUTPUT);
    pinMode(InC, OUTPUT);
    pinMode(InD, OUTPUT);
    digitalWrite(InA, LOW);
    digitalWrite(InB, HIGH);
    digitalWrite(InC, LOW);
    digitalWrite(InD, HIGH);
    
    analogWrite(EnA, START_SPEED);
    analogWrite(EnB, START_SPEED);
    
    for(int i = 0; i < NUM_SENSORS; i++) {
        pinMode(sensor_pins[i], INPUT);
    }
    Serial.begin(9600);
}


int getSensor() {
    mask ^= mask;
    for(int i = 0; i < NUM_SENSORS; i++) {
        mask = mask << 1 | digitalRead(sensor_pins[i]);
    }
    switch (mask) { 
        case 0b11110 : error = -4; break;       // Left deviation
        case 0b11100 : error = -3; break;
        case 0b11101 : error = -2; break;
        case 0b11001 : error = -1; break;
        case 0b11011 : error = 0; break;        // Center 
        case 0b10011 : error = 1; break;        // Right deviation
        case 0b10111 : error = 2; break;
        case 0b00111 : error = 3; break;
        case 0b01111 : error = 4; break;

        case 0b11111 :                          // Left or right deviation
            if(error < 0) error = -5;
            else error = 5;
            break;
        default: break; 
    }
    return error;
}

void control_motor(int left, int right) {
    analogWrite(EnA, right);
    analogWrite(EnB, left);
}


float calculate_pid() {
    integral = integral + error;
    int pid = Kp * error + Kd * (error - previous_error) + Ki * integral;
    previous_error = error;
    return pid;
}


void loop() {
    error = getSensor();
    float pid = calculate_pid();
    
    Serial.println(">>>>Error: " + String(error) + "    >>>>PID: " + String(pid));
    
    int leftSpeedPwm = START_SPEED - pid; 
    int rightSpeedPwm = START_SPEED + pid;
    
    leftSpeedPwm = constrain(leftSpeedPwm, 0, MAX_SPEED);
    rightSpeedPwm = constrain(rightSpeedPwm, 0, MAX_SPEED);
    
    Serial.println(">>>>>>" + String(leftSpeedPwm) + "====" + String(rightSpeedPwm) + "<<<<<<<");
    control_motor(leftSpeedPwm, rightSpeedPwm);
//    delay(1000);
}
