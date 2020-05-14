
#include <Wire.h>
#include <TTP229.h>

TTP229 ttp229;

// position represents the current position in the sequence
volatile int position = 0;

// interrupt_pin 0 is digital pin 2 on the arduino uno
const int interrupt_pin = 0;

// pad_active is false if no pad has been touched since the last interrupt
volatile bool pad_active = false;
// pad_active[position] is true if this pad has been touched since the last interrupt
volatile bool pad_state[8] = {false, false, false, false, false, false, false, false};

// definition direction pin as well as old and new values of it
int direction_pin = 11;
int direction_pin_value = LOW;
int direction_pin_value_old = LOW;

// define direction variable
volatile int direction_variable = 1;

void increase_position(){
    position = (position + direction_variable)%8;
    if (position < 0) {
        position = position + 8;
    }
}

// get's called every clock tick and advances the sequencer based on touchpad state
void clock_trigger(){
    // lets the sequencer advance one step if no pad is activated
    if (!pad_active) {
        for (int x = 0; x < 8; x++) {
            if (x == position) {
                digitalWrite(x+3, HIGH);
            }
            else {
                digitalWrite(x+3, LOW);
            }
        }
        increase_position();
    }
    
    // otherwise some pads have been pressed and the sequencer finds the next step
    // mod 8 from current position to proceed 
    else {
        // here position get's set to the next active pad value
        while (!(pad_state[position])) {
            Serial.println(pad_state[position]);
            increase_position();
            }
        

        // advances the sequence to the just determined step
        for (int x = 0; x < 8; x++) {
            if (x == position) {
                digitalWrite(x+3, HIGH);
            }
            else {
                digitalWrite(x+3, LOW);
            }
        }

        // cleanup
        increase_position();
        pad_active = false;
        for (int i = 0; i<8; i++){
            pad_state[i] = false;
        }
    }
} 


void setup()
{
    Wire.begin();
    Serial.begin(9600);
    
    // define pins 3 - 10 as Output
    for (int i = 3; i < 11; ++i) {
        pinMode(i,OUTPUT);
    }

    // definition interrupt pin
    pinMode(interrupt_pin, INPUT);
    attachInterrupt(interrupt_pin, clock_trigger, RISING);

    pinMode(direction_pin, INPUT);

    

}

void loop()
{
    ttp229.readKeys();

    // sets the active pins to true in pad_state and set_s pad_active flag if a pin gets toggled
    for (int i = 0; i < 8; i++) {
        if (ttp229.isKeyPress(i)){
            pad_state[i] = true;
            pad_active = true;
        }
    }

    // read direction pin state and write into current value
    direction_pin_value = digitalRead(direction_pin);

    // see if rising edge is present and change direction
    if (direction_pin_value == HIGH && direction_pin_value_old == LOW) {
        direction_variable = direction_variable * -1;
    }
    // set old direction to the current
    direction_pin_value_old = direction_pin_value;

    
}
