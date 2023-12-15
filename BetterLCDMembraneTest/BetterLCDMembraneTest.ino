/*
Krishay Iyer
ENES 100
Professor Mellini

Code to run the Elegoo vault
*/

//Importing Libraries
#include <LiquidCrystal.h> // For the Screen
#include <Keypad.h> // For the membrane keypad switch
#include <string.h>
#include <Servo.h> // Manages the servo


//Membrane Keypad Switch Initialization

const byte ROWS = 4;
const byte COLS = 4;

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte colPins[ROWS] = {30, 32, 34, 36};
byte rowPins[COLS] = {22, 24, 26, 28};

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

//LCD Display Initialization
//Referencing Pins
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


//Passkey Initialization
String inputPasskey = "";
String correctPasskey = "696969DD";

bool correct = false;

int numTries = 0;
int numTriesLimit = 5;
int timeOut = 10; //In Seconds

Servo myservo;  // create servo object to control a servo

void clearScreen(){ // This function is redundant :/
  lcd.clear();
}

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Enter Passkey");
  //Starting serial port output (not used)
  Serial.begin(9600);
  //Referencing the servo's control pin
  myservo.attach(9);
  //Setting the servo to its 0 position
  myservo.write(0);
}

void loop() {

  if (numTries >= numTriesLimit){ // Lock the user out if the attempts reach the limit
    correct = false;
    for (int i = 0; i <= timeOut; i++){
      clearScreen();
      lcd.setCursor(0,0);
      lcd.print("Lock Timeout");
      lcd.setCursor(0, 1);
      lcd.print(String(timeOut - i));
      delay(1000);
    }
    numTries = 0;
  }

  else{
    lcd.setCursor(0, 1);
    char button = customKeypad.getKey(); // Retreive button input

    if (button) {
      if (button == '#'){ // Locking/Unlocking Vault
        if (correct){ // Already correct? lock it
          clearScreen();
          lcd.setCursor(0,0);
          Serial.println("Vault Locked");
          lcd.print("Vault Locked");
          inputPasskey="";
          correct = false;
          myservo.write(0);
        }
        else if (inputPasskey.equals(correctPasskey)){ // Check if it is correct, if not already correct
          clearScreen();
          lcd.setCursor(0,0);
          Serial.println("Correct Passkey");
          lcd.print("Correct Passkey");
          lcd.setCursor(0,1);
          Serial.println("Vault Unlocked");
          lcd.print("Vault Unlocked");
          myservo.write(180);
          correct = true;
        }

        else{ // Increase attempt by 1
          Serial.println("Wrong Input!");
          clearScreen();
          lcd.setCursor(0,0);
          numTries++;
          lcd.print("Incorrect Pass " + String(numTries));
        }
      }
      else if (button == '*'){ // Removing last input
        if (inputPasskey.length() != 0){
          inputPasskey.remove(inputPasskey.length()-1);
          // Serial.println(inputPasskey);
          clearScreen();
          lcd.setCursor(0,0);
          lcd.print("Enter Passkey");
          lcd.setCursor(0,1);
          lcd.print(inputPasskey);
        }
      }

      else{ //Adding input to end of string
        inputPasskey += button;
        clearScreen();
        lcd.setCursor(0,0);
        lcd.print("Enter Passkey");
        lcd.setCursor(0,1);
        // Serial.println(inputPasskey);
        lcd.print(inputPasskey);
      }
    }
  }
}