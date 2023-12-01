
byte rows[] = {6,7,8,9};
const int rowCount = sizeof(rows)/sizeof(rows[0]);

// JP2 and JP3 are outputs
byte cols[] = {2,3,4};
const int colCount = sizeof(cols)/sizeof(cols[0]);

byte keys[colCount][rowCount];


byte LED_rows[] = {13,14,15,16};
const int LED_rowCount = sizeof(LED_rows)/sizeof(LED_rows[0]);

// JP2 and JP3 are outputs
byte LED_cols[] = {10,11,12};
const int LED_colCount = sizeof(LED_cols)/sizeof(LED_cols[0]);

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