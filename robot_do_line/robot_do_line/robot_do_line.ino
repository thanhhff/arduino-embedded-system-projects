const int EnA = 11; // right
const int EnB = 10; // left
const int InA = 13;
const int InB = 12;
const int InC = 9;
const int InD = 8;

int error, previous_error;

const int MAX_SPEED = 80;
int mask;
const int NUM_SENSORS = 5; 
const int sensor_pins[NUM_SENSORS] = {A4, A3, A2, A1, A0};

float Kp = 12;
float Kd = 0.8;
float Ki = 0;

int integral = 0;
int rightSpeedPwm = 0;
int leftSpeedPwm = 0;

void getSensor();
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
    
    analogWrite(EnA, MAX_SPEED);
    analogWrite(EnB, MAX_SPEED);
    
    for(int i = 0; i < NUM_SENSORS; i++) {
        pinMode(sensor_pins[i], INPUT);
    }

    Serial.begin(9600);
}

void loop() {
    getSensor();
    float pid = calculate_pid();
    Serial.println(">>>>Error: " + String(error) + "    >>>>PID: " + String(pid));
    int leftSpeedPwm = MAX_SPEED + pid; 
    int rightSpeedPwm = MAX_SPEED - pid;
    
    leftSpeedPwm = constrain(leftSpeedPwm, 0, 255);
    rightSpeedPwm = constrain(rightSpeedPwm, 0, 255);
    Serial.println(">>>>>>" + String(leftSpeedPwm) + "====" + String(rightSpeedPwm) + "<<<<<<<");

    control_motor(leftSpeedPwm, rightSpeedPwm);
}

void getSensor() {
    mask ^= mask;
    for(int i = 0; i < NUM_SENSORS; i++) {
        mask |= digitalRead(sensor_pins[i]) << i;
    }

    switch (mask) { 
        case 0b00001 : error = -4; break;       // Left deviation
        case 0b00011 : error = -3; break;
        case 0b00010 : error = -2; break;
        case 0b00110 : error = -1; break;

        case 0b00100 : error = 0; break;        // Center 

        case 0b01100 : error = 1; break;        // Right deviation
        case 0b01000 : error = 2; break;
        case 0b11000 : error = 3; break;
        case 0b10000 : error = 4; break;

        case 0b00000 :                          // Left or right deviation
            if(error < 0) error = -5;
            else error = 5;
            break;
        default: break; 
    }
}

void control_motor(int left, int right) {
    analogWrite(EnA, right);
    analogWrite(EnB, left);
}

// void control_robot(int error) {
//     switch (error) {
//         case -4: control_motor(25, 120);  break;
//         case -3: control_motor(25, 100);  break;
//         case -2: control_motor(50, 100);  break;
//         case -1: control_motor(100, 150); break;
//         case  0: control_motor(150, 150); break;
//         case  1: control_motor(150, 100); break;
//         case  2: control_motor(100, 50);  break;
//         case  3: control_motor(100, 25);  break;
//         case  4: control_motor(150, 25);  break;
//         case  6: control_motor(0, 80);    break;
//         case  7: control_motor(80, 0); break;
//         case  5: 
//             if(is_left_direction == 0)
//                 control_motor(0, 80); 
//             else
//                 control_motor(80, 0);    
//             break;   
//         default: break;
//     }
// }


float calculate_pid() {
    integral = integral + error;
    int pid = Kp * error + Kd * (error - previous_error) + Ki * integral;
    previous_error = error;
    return pid;
}
