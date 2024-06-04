#include <LiquidCrystal.h>

#define PIN_TRIG 3
#define PIN_ECHO 2
#define PIR_PIN 1 
#define LED_PIN 5

bool spotOccupied = false; // Flag to track occupancy
int pirState = LOW;  
LiquidCrystal lcd(12, 11, 10, 9, 8, 7);

void setup() {
    Serial.begin(115200);
    pinMode(PIN_TRIG, OUTPUT);
    pinMode(PIN_ECHO, INPUT);
    pinMode(PIR_PIN, INPUT);
    pinMode(LED_PIN, OUTPUT);
    lcd.begin(16, 2);
}

void loop() {
    // Measure distance
    int distanceCm = distance() / 58; 
    
    // Read PIR state
    int newPirState = digitalRead(PIR_PIN);

    // Logic for parking bay behavior
    if (distanceCm <= 5) { 
        digitalWrite(LED_PIN, LOW); // Turn LED off if close
        lcd.clear();
        
        if (newPirState == HIGH && pirState == LOW) {
            // PIR activated while close -> Log in
            lcd.print("Logged In"); 
            Serial.println("User Logged In"); 
            pirState = HIGH; // Update PIR state
            spotOccupied = true; // Mark spot as occupied
        } 
        
        if (spotOccupied) {
            lcd.print("Spot Used"); 
        } else {
            lcd.print("Sign In");
        }        
    } else {
        digitalWrite(LED_PIN, HIGH); // Turn LED on if far
        lcd.clear();

        if (spotOccupied) {
            // User has left, mark spot as free
            lcd.print("Spot Free");
            spotOccupied = false;
        } else {
            lcd.print("Spot Free"); 
        }
        
        pirState = LOW; // Reset PIR state when far away
    }
    
    delay(500); // Adjust delay for responsiveness
}

// Function to measure distance using ultrasonic sensor (unchanged)
int distance() {
    digitalWrite(PIN_TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(PIN_TRIG, LOW);
    return pulseIn(PIN_ECHO, HIGH);
}



