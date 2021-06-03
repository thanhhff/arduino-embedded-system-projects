const int EnA = 11; // right 
const int EnB = 10; // left
const int InA = 13;
const int InB = 12;
const int InCright = 9;
const int InD = 8;

int error, is_left_direction;

const int MAX_SPEED = 255;
const int START_SPEED = 150;

int mask;
const int NUM_SENSORS = 5; 
const int sensor_pins[NUM_SENSORS] = {A0, A1, A2, A3, A4};

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
}

void loop() {
    error = getSensor();
    control_robot(error);
}

int getSensor() {
    mask ^= mask;
    for(int i = 0; i < NUM_SENSORS; i++) {
        mask |= digitalRead(sensor_pins[i]) << i;
    }
    error = 0;
    switch (mask) { 
        // Giữa line
        case 0b11011: error = 0; break;                             
        // Lệch hoàn toàn 
        case 0b11111: error = 5; break;
        
        case 0b10000: break;
        
        // Gõ vuông sang phải                         
        case 0b11000: is_left_direction = 1; error = 7; break;  
        // Lệch trái cấp 1
        case 0b11001: is_left_direction = 1; error = 1; break;
        // Lệch trái cấp 2
        case 0b11101: is_left_direction = 1; error = 2; break;
        // Lệch trái cấp 3
        case 0b11100: is_left_direction = 1; error = 3; break;
        // Lệch trái cấp 4
        case 0b11110: is_left_direction = 1; error = 4; break;
        // Lệch phải cấp 1
        
        case 0b10011: is_left_direction = 0; error = -1; break;
        // Lệch phải cấp 2
        case 0b10111: is_left_direction = 0; error = -2; break;
        // Lệch phải cấp 3
        case 0b00111: is_left_direction = 0; error = -3; break;
        // Lệch phải cấp 4
        case 0b01111: is_left_direction = 0; error = -4; break;
        
        //
        case 0b00001: break;
        
        //Gõ vuông sang trái 
        case 0b00011: is_left_direction = 0; error = 6; break;
    }
    return error;
}

void control_motor(int left, int right) {
    analogWrite(EnA, right);
    analogWrite(EnB, left);
}

void control_robot(int error) {
    switch (error) {
        case -4: control_motor(25, 150);  break;
        case -3: control_motor(25, 100);  break;
        case -2: control_motor(50, 100);  break;
        case -1: control_motor(100, 150); break;
        case  0: control_motor(150, 150); break;  
        case  1: control_motor(150, 100); break;
        case  2: control_motor(100, 50);  break;
        case  3: control_motor(100, 25);  break;
        case  4: control_motor(150, 25);  break;
        case  6: control_motor(0, 80);    break;
        case  7: control_motor(80, 0);    break;
        case  5:                                  
            if(is_left_direction == 0)
                control_motor(0, 80); 
            else
                control_motor(80, 0);    
            break;   
        default: break;
    }
}
