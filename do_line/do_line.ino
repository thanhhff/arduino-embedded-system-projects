const int EnA = 11; // right
const int EnB = 10; // left
const int InA = 13;
const int InB = 12;
const int InC = 9;
const int InD = 8;

int error, previous_error, is_left_direction;

const int MAX_SPEED = 150;
int mask;
const int NUM_SENSORS = 5; 
const int sensor_pins[NUM_SENSORS] = {A1, A2, A3, A4, A5};

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
    
    analogWrite(EnA, MAX_SPEED);
    analogWrite(EnB, MAX_SPEED);
    
    for(int i = 0; i < NUM_SENSORS; i++) {
        pinMode(sensor_pins[i], INPUT);
    }
}

void loop() {
    previous_error = error;
    error = getSensor();
    if (previous_error != 5) {
      control_robot(error);
    }
}

int getSensor() {
    mask ^= mask;
    for(int i = 0; i < NUM_SENSORS; i++) {
        mask |= digitalRead(sensor_pins[i]) << i;
    }
    error = 0;
    switch (mask) { 
        case 0b00000: error = 10; break;
        case 0b11011: error = 0; break;                             
        case 0b11111: error = 5; break;
        
        case 0b11110: is_left_direction = 0; error = -4; break;
        case 0b11100: is_left_direction = 0; error = -3; break;
        case 0b11101: is_left_direction = 0; error = -2; break;
        case 0b11001: is_left_direction = 0; error = -1; break;
        case 0b10011: is_left_direction = 1; error = 1; break;
        case 0b10111: is_left_direction = 1; error = 2; break;
        case 0b00111: is_left_direction = 1; error = 3; break;
        case 0b01111: is_left_direction = 1; error = 4; break;
        
    }
    return error;
}

void control_motor(int left, int right) {
    analogWrite(EnA, right);
    analogWrite(EnB, left);
}

void control_robot(int error) {
    switch (error) {
        case -4: control_motor(50, 110);  break;
        case -3: control_motor(50, 75);  break;
        case -2: control_motor(25, 75);  break;
        case -1: control_motor(25, 50); break;
        case  1: control_motor(50, 25); break;
        case  2: control_motor(75, 25);  break;
        case  3: control_motor(75, 50);  break;
        case  4: control_motor(110, 50);  break;
//        case 10: control_motor(0, 0);     break;
        case  5: 
            if(is_left_direction == 0)
                control_motor(0, 125); 
            else
                control_motor(125, 0);    
            break;   
        case  0:
                control_motor(130, 130);  
            break; 
        default: break;
    }
}
