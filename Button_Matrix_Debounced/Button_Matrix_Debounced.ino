const int selectPin1_button = 10; //select bit A
const int selectPin2_button = 11; //select bit B
const int selectPin3_button = 12; //select bit C
const int enablePin_columns = 24; //enable bit

const int selectPin1_LED = 25; //select bit A
const int selectPin2_LED = 26; //select bit B
const int selectPin3_LED = 27; //select bit C

const int selectPin1_rows = 32; //select bit A
const int selectPin2_rows = 31; //select bit B
const int selectPin3_rows = 30; //select bit C
const int enablePin_rows = 29;

const int Button_column8 = 8;
const int LED_column8 = 28;

byte rows[] = {33,34,35,36};

const int rowCount = 4;
const int colCount = 9;
const int LED_rowcount = 4;
const int LED_colcount = 9;


int Previous_Button_State[] = {9,9};
int Current_Button_State[] = {9,9}; 
int Last_Pushed_State[] = {9,9};
int Pressed = 0;

unsigned long previousMillis = 0;
unsigned long interval = 50;
int loop_counter = 0;



void setup() {
  // put your setup code here, to run once:

Serial.begin(9600);
  digitalWrite(enablePin_columns, LOW);
  digitalWrite(enablePin_rows, LOW);

  for(int x=0; x<rowCount; x++) {
		pinMode(rows[x], INPUT_PULLUP);
    Serial.println(rows[x]);
  }

  pinMode(Button_column8, INPUT_PULLUP);

  pinMode(LED_column8, OUTPUT);
  digitalWrite(LED_column8, LOW);


  pinMode(selectPin1_button, OUTPUT);  
  pinMode(selectPin2_button, OUTPUT);  
  pinMode(selectPin3_button, OUTPUT);  
  pinMode(enablePin_columns, OUTPUT);  
  digitalWrite(enablePin_columns, HIGH);

  pinMode(selectPin1_LED, OUTPUT);  
  pinMode(selectPin2_LED, OUTPUT);  
  pinMode(selectPin3_LED, OUTPUT);

  pinMode(selectPin1_rows, OUTPUT);  
  pinMode(selectPin2_rows, OUTPUT);  
  pinMode(selectPin3_rows, OUTPUT);  
  pinMode(enablePin_rows, OUTPUT);
  digitalWrite(enablePin_rows, HIGH);

  selectRow_LED(7);


}
void readMatrix() {
	// iterate the columns
	for (int colIndex=0; colIndex < colCount; colIndex++) {
    

    if (colIndex == 8){

      digitalWrite(enablePin_columns, LOW);

      pinMode(Button_column8, OUTPUT);
      digitalWrite(Button_column8, LOW);
      check_rows(colIndex);

      pinMode(Button_column8, INPUT_PULLUP);
      digitalWrite(enablePin_columns, HIGH);
    }

    else{
      
      selectColumn_button(colIndex);
      check_rows(colIndex);
    } 
	}	
}

void check_rows(int colIndex){
  		// row: interate through the rows
		for (int rowIndex=0; rowIndex < rowCount; rowIndex++) {
			byte rowCol = rows[rowIndex];

      if (digitalRead(rows[rowIndex])==0){

        Current_Button_State[0] = rowIndex;
        Current_Button_State[1] = colIndex;
        Pressed = 1;
        Serial.print("Push");
        Serial.print(rowIndex);
        Serial.println(colIndex);

      }
		}
}

void Button_Pressed(int Current_State[], int Previous_State[]){
  previousMillis = millis();
  LED_On(Current_State[0],Current_State[1]);
  
  if (loop_counter == 3){
    
    if (Current_State[1]==8){
  
      pinMode(Button_column8, OUTPUT);
      digitalWrite(Button_column8, LOW);
      
      if (digitalRead(rows[Current_State[0]])==1){
        
          Pressed = 0;
          Button_Released(Current_State,Previous_State);
      }

      pinMode(Button_column8, INPUT_PULLUP);
    }
    else{

      selectColumn_button(Current_State[1]);

      if (digitalRead(rows[Current_State[0]])==1){
        
        Pressed = 0;
        Button_Released(Current_State,Previous_State);
      }
    }
    loop_counter = 0;
  }
  
  loop_counter+=1;
}

void Button_Released(int Current_State[], int Previous_State[]){

  previousMillis = millis();
  LED_Off(Previous_State[0],Previous_State[1]);
  Current_State[0] = 9;
  Current_State[1] = 9;
}

void LED_On(int Row, int Column){

  if (Column == 8){

    digitalWrite(enablePin_columns, LOW);
    digitalWrite(LED_column8, HIGH);
    selectRow_LED(Row); 
  }

  else{
    digitalWrite(LED_column8, LOW);
    selectColumn_LED(Column);
    selectRow_LED(Row); 
  }
}

void LED_Off(int Row, int Column){
  
  if (Column == 8){
    
    digitalWrite(LED_column8, LOW);
    digitalWrite(enablePin_columns, HIGH);
  }

  selectRow_LED(7);
}

void selectColumn_button(int Column){

  digitalWrite(selectPin1_button, bitRead(Column, 0));
  digitalWrite(selectPin2_button, bitRead(Column, 1));
  digitalWrite(selectPin3_button, bitRead(Column, 2));
}

void selectColumn_LED(int Column){

  digitalWrite(selectPin1_LED, bitRead(Column, 0));
  digitalWrite(selectPin2_LED, bitRead(Column, 1));
  digitalWrite(selectPin3_LED, bitRead(Column, 2));
}

void selectRow_LED(int Row){

  digitalWrite(selectPin1_rows, bitRead(Row, 0));
  digitalWrite(selectPin2_rows, bitRead(Row, 1));
  digitalWrite(selectPin3_rows, bitRead(Row, 2));

}

void loop() {
unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    
    if (Pressed==0){
      readMatrix();
    }

    if (Pressed==1){
      Button_Pressed(Current_Button_State, Previous_Button_State);
    }

    Previous_Button_State[0] = Current_Button_State[0];
    Previous_Button_State[1] = Current_Button_State[1];
  }
}
