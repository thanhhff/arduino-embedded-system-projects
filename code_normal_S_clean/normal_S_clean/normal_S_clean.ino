const int EnA = 11; // right 
const int EnB = 10; // left
const int InA = 13;
const int InB = 12;
const int InC = 9;
const int InD = 8;

int error;

const int MAX_SPEED = 150;
const int START_SPEED = 100;

int mask;
const int NUM_SENSORS = 5; 
const int sensor_pins[NUM_SENSORS] = {A4, A3, A2, A1, A0};

int getSensor();
void control_motor(int left, int right);
void control_robot(int error);

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

void loop() {
    error = getSensor();
    control_robot(error);
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
    Serial.println(">>>>Error: " + String(error));
//    delay(2000);
    return error;
}

void control_motor(int left, int right) {
    analogWrite(EnA, right);
    analogWrite(EnB, left);
}

void control_robot(int error) {
    switch (error) {
        case -5: control_motor(100, 10);  break;
        case -4: control_motor(120, 20);  break;
        case -3: control_motor(100, 20);  break;
        case -2: control_motor(100, 60);  break;
        case -1: control_motor(120, 100); break;
        case  0: control_motor(140, 140); break;  
        case  1: control_motor(100, 120); break;
        case  2: control_motor(60, 100);  break;
        case  3: control_motor(20, 100);  break;
        case  4: control_motor(20, 120);  break;
        case  5: control_motor(10, 100);  break;
        default: break;
    }
}
