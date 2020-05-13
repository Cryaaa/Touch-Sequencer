
#include <Wire.h>
#include <TTP229.h>

TTP229 ttp229;

void setup()
{
    Wire.begin();
    Serial.begin(9600);

    // define digital pins as output
    for (int i = 2; i < 10; ++i) {
        pinMode(i, OUTPUT);
    }

}

void loop()
{
    // reset pins to low
    for (int i = 2; i < 10; ++i) {
        digitalWrite(i, LOW);
    }

    ttp229.readKeys();

    for (int i = 0; i < 8; ++i) {
        // variable to adress pin
        int pinnumber = i + 2;
        
        if (ttp229.isKeyPress(i)) {
            digitalWrite (pinnumber, HIGH);
    }
    
    delay(1);
}
