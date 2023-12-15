//This is a test program to figure out how the membrane switch module works
//We mapped the output of each button on the membrane switch module to a 4x4 array
//We have added a few features, such as passkey authentication and deleting strings

#include <Keypad.h>
#include <string.h>

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte colPins[ROWS] = {5, 4, 3, 2};
byte rowPins[COLS] = {9, 8, 7, 6};

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

String inputPasskey = "";
String correctPasskey = "1234";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  char button = customKeypad.getKey();

  if (button) {
    if (button == '#'){
      if (inputPasskey.equals(correctPasskey)){
        Serial.println("Correct Passkey");
      }
      else{
        Serial.println("Wrong Input!");
      }
    }
    else if (button == '*'){
      if (inputPasskey.length() != 0){
        inputPasskey.remove(inputPasskey.length()-1);
        Serial.println(inputPasskey);
      }
    }
    else{
      inputPasskey += button;
      Serial.println(inputPasskey);
    }
  }
}
