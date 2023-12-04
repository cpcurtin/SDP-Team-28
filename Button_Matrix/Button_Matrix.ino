
// MEASURE MATRIX BUTTONS
#define BUTTON_MEASURE_MATRIX_ROW_1 41
#define BUTTON_MEASURE_MATRIX_ROW_2 13
#define BUTTON_MEASURE_MATRIX_ROW_3 14
#define BUTTON_MEASURE_MATRIX_ROW_4 15
#define BUTTON_MEASURE_MATRIX_COLUMN_1 23
#define BUTTON_MEASURE_MATRIX_COLUMN_2 22
#define BUTTON_MEASURE_MATRIX_COLUMN_3 19
#define BUTTON_MEASURE_MATRIX_COLUMN_4 18
#define BUTTON_MEASURE_MATRIX_COLUMN_5 17
#define BUTTON_MEASURE_MATRIX_COLUMN_6 16

// MEASURE MATRIX LEDS
#define LED_MEASURE_MATRIX_ROW_1 32
#define LED_MEASURE_MATRIX_ROW_2 31
#define LED_MEASURE_MATRIX_ROW_3 33
#define LED_MEASURE_MATRIX_ROW_4 34
#define LED_MEASURE_MATRIX_COLUMN_1 40
#define LED_MEASURE_MATRIX_COLUMN_2 39
#define LED_MEASURE_MATRIX_COLUMN_3 38
#define LED_MEASURE_MATRIX_COLUMN_4 37
#define LED_MEASURE_MATRIX_COLUMN_5 36
#define LED_MEASURE_MATRIX_COLUMN_6 35


byte rows[] = {BUTTON_MEASURE_MATRIX_ROW_1,BUTTON_MEASURE_MATRIX_ROW_2,BUTTON_MEASURE_MATRIX_ROW_3,BUTTON_MEASURE_MATRIX_ROW_4};
const int rowCount = sizeof(rows)/sizeof(rows[0]);

// JP2 and JP3 are outputs
byte cols[] = {BUTTON_MEASURE_MATRIX_COLUMN_1,BUTTON_MEASURE_MATRIX_COLUMN_2,BUTTON_MEASURE_MATRIX_COLUMN_3,BUTTON_MEASURE_MATRIX_COLUMN_4,BUTTON_MEASURE_MATRIX_COLUMN_5,BUTTON_MEASURE_MATRIX_COLUMN_6};
const int colCount = sizeof(cols)/sizeof(cols[0]);

byte keys[colCount][rowCount];



byte LED_rows[] = {LED_MEASURE_MATRIX_ROW_1,LED_MEASURE_MATRIX_ROW_2,LED_MEASURE_MATRIX_ROW_3,LED_MEASURE_MATRIX_ROW_4};
const int LED_rowCount = sizeof(rows)/sizeof(rows[0]);

// JP2 and JP3 are outputs
byte LED_cols[] = {LED_MEASURE_MATRIX_COLUMN_1,LED_MEASURE_MATRIX_COLUMN_2,LED_MEASURE_MATRIX_COLUMN_3,LED_MEASURE_MATRIX_COLUMN_4,LED_MEASURE_MATRIX_COLUMN_5,LED_MEASURE_MATRIX_COLUMN_6};
const int LED_colCount = sizeof(cols)/sizeof(cols[0]);


void setup() {
  // put your setup code here, to run once:
	Serial.begin(9600);

	for(int x=0; x<rowCount; x++) {
		//Serial.print(rows[x]); Serial.println(" as input");
		pinMode(rows[x], INPUT_PULLUP);
    Serial.println(rows[x]);
	}
  //pinMode(6,INPUT_PULLUP);

	for (int x=0; x<colCount; x++) {
	//	Serial.print(cols[x]); Serial.println(" as input-pullup");
		pinMode(cols[x], INPUT_PULLUP);
  

	}

	for(int x=0; x<LED_rowCount; x++) {
		//Serial.print(rows[x]); Serial.println(" as input");
		pinMode(LED_rows[x], OUTPUT);
    digitalWrite(LED_rows[x], HIGH);
	}

	for (int x=0; x<LED_colCount; x++) {
	//	Serial.print(cols[x]); Serial.println(" as input-pullup");
		pinMode(LED_cols[x], OUTPUT);
    digitalWrite(LED_cols[x], LOW);
	}
    
}

void readMatrix() {
	// iterate the columns
	for (int colIndex=0; colIndex < colCount; colIndex++) {
		// col: set to output to low
		byte curCol = cols[colIndex];
		pinMode(curCol, OUTPUT);
		digitalWrite(curCol, LOW);
  

		// row: interate through the rows
		for (int rowIndex=0; rowIndex < rowCount; rowIndex++) {
			byte rowCol = rows[rowIndex];
			//pinMode(rowCol, INPUT_PULLUP);
			keys[colIndex][rowIndex] = digitalRead(rowCol);
		//	pinMode(rowCol, INPUT);
      if (digitalRead(rows[rowIndex])==0){
        Serial.println("Push");
        Serial.print(rowIndex);
        Serial.print(colIndex);
        digitalWrite(LED_cols[colIndex], HIGH);
        digitalWrite(LED_rows[rowIndex], LOW);

      }

      else{
        digitalWrite(LED_cols[colIndex], LOW);
        digitalWrite(LED_rows[rowIndex], HIGH);


      }


		}

		// disable the column
    //delay(1000);
		pinMode(curCol, INPUT_PULLUP);



    
	}
}


void loop() {
  // put your main code here, to run repeatedly:
  readMatrix();
  //Serial.print("Done");
  
}