int total = 0;
int count = 0;
int lastZ = 0;
// Keyboard Matrix Tutorial Example
// baldengineer.com
// CC BY-SA 4.0
 #include <PicoGamepad.h>
 PicoGamepad gamepad;
// JP1 is an input
byte rows[] ={9,10,11,12,13,14,15};
const int rowCount = sizeof(rows)/sizeof(rows[0]);

// JP2 and JP3 are outputs
byte cols[] =  {16,17,18,19,20,21};
const int colCount = sizeof(cols)/sizeof(cols[0]);

int analog1 = 26;
int analog2 = 27;
int analog3 = 28;

byte keys[colCount][rowCount];

void setup() {
	Serial.begin(115200);
  
  pinMode(LED_BUILTIN, OUTPUT);
	for(int x=0; x<rowCount; x++) {
		Serial.print(rows[x]); Serial.println(" as input");
		pinMode(rows[x], INPUT_PULLUP);
	}

	for (int x=0; x<colCount; x++) {
		Serial.print(cols[x]); Serial.println(" as input-pullup");
		pinMode(cols[x], OUTPUT);
	}
  pinMode(analog1, INPUT);
  pinMode(analog2, INPUT);
  pinMode(analog3, INPUT);
		Serial.print("HELLO");
}

void readMatrix() {
	// iterate the columns
	for (int colIndex=0; colIndex < colCount; colIndex++) {
		// col: set to output to low
		byte curCol = cols[colIndex];
		digitalWrite(curCol, LOW);
    delayMicroseconds(5);
		// row: interate through the rows
		for (int rowIndex=0; rowIndex < rowCount; rowIndex++) {
			byte rowCol = rows[rowIndex];
			//pinMode(rowCol, INPUT_PULLUP);
			keys[colIndex][rowIndex] = digitalRead(rowCol);
			//pinMode(rowCol, INPUT);
		}
		// disable the column
		digitalWrite(curCol, HIGH);
    delayMicroseconds(5);
	}
}

void printMatrix() {
	for (int rowIndex=0; rowIndex < rowCount; rowIndex++) {
		if (rowIndex < 10)
			Serial.print(F("0"));
		Serial.print(rowIndex); Serial.print(F(": "));

		for (int colIndex=0; colIndex < colCount; colIndex++) {	
			Serial.print(keys[colIndex][rowIndex]);
			if (colIndex < colCount)
				Serial.print(F(", "));
		}	
		Serial.println("");
	}
	Serial.println("");
}
void pressButtons()
{
  for (int rowIndex=0; rowIndex < rowCount; rowIndex++) {
		for (int colIndex=0; colIndex < colCount; colIndex++) {	
      gamepad.SetButton((rowIndex * sizeof(cols)) + colIndex, !keys[colIndex][rowIndex]);
		}	
	}
}
void readAnalog()
{
  float val = analogRead(analog1);
  val = map(val, 0, 1023, -32767, 32767);
  gamepad.SetX(val);

   val = analogRead(analog2);
  val = map(val, 0, 1023, -32767, 32767);
  gamepad.SetY(val);

  val = analogRead(analog3);
  total += val;
  count += 1;
  if(count >= 20)
  {
    lastZ = map(total / 20, 500, 800, -32767, 32767);
    count = 0;
    total = 0;
  }
  gamepad.SetZ(lastZ);
}
void loop() {
	  readMatrix();
		printMatrix();
    pressButtons();
    readAnalog();
    gamepad.send_update();
    delay(10);
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); 
}
