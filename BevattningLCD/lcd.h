#define SPEED_CHANGE_FACTOR 50 // how fast speed should change when holding the button. Lower value is faster

#define MENU_MAIN     1
#define MENU_IDLE     2
#define MENU_SELECT   10
#define MENU_EDIT_SENSOR     20
#define MENU_WRITE_FLASH     30

int currentMenu = MENU_IDLE;
int prevMenu = 1;


int selectedFunction = 0;

long redraw;

void initLCD() {
  lcd.begin(16, 2);              // start the library
  lcd.setCursor(0,0);
}

#define MAX_PAGE 1
int page = 0;
int curline = 0;

void menuMain() {
  if (millis()>redraw || btnPressed != 0) {
    redraw = millis()+500;
  }else {
    return;
  }

  if ( btnPressed == btnSELECT) {
    btnPressed = 0;
    if (page == 0 && curline == 0) { // Back
      currentMenu = MENU_IDLE;
    }
    if (page == 0 && curline == 1) { // Edit sensor
      currentMenu = MENU_EDIT_SENSOR;
    }
    if (page == 1 && curline == 0) { // Write flash
      currentMenu = MENU_WRITE_FLASH;
    }
    
    lcd.clear();
    return;
  }
  if ( btnPressed == btnRIGHT) {
    btnPressed = 0;
    
  }
  if ( btnPressed == btnLEFT) {
    btnPressed = 0;
    
  }
  if ( btnPressed == btnUP) {
    btnPressed = 0;
    curline--;
  }
  if ( btnPressed == btnDOWN) {
    btnPressed = 0;
    curline++;
  }
  if (curline >=2) {
    page++;
    curline = 0;
  }
  if (curline <0) {
    page--;
    curline = 1;
  }
  if (page > MAX_PAGE) {
    page = 0;
  }
  if (page < 0) {
    page = MAX_PAGE;
  }
  
  if (page == 0) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("  Back");
    lcd.setCursor(0,1);
    lcd.print("  Edit sensor");
  }
  if (page == 1) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("  Write flash");
    lcd.setCursor(0,1);
    lcd.print("  ");
  }
  
  lcd.setCursor(0,curline);
  lcd.print(">");
  lcd.setCursor(12,0);
  lcd.print("4");

  lcd.setCursor(0,1);
 
  
  
}
void menuIdle() {
  if (millis()>redraw || btnPressed != 0) {
    redraw = millis()+500;
  }else {
    return;
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(sensor1_value);
  lcd.print("%");
  lcd.setCursor(4,0);
  lcd.print("2");
  lcd.setCursor(8,0);
  lcd.print("3");
  lcd.setCursor(12,0);
  lcd.print("4");

  lcd.setCursor(0,1);
  if (pump1_state) {
    lcd.print("On");
  }else {
    lcd.print("Off");
  }

  if ( btnPressed == btnSELECT) {
    btnPressed = 0;
    currentMenu = MENU_MAIN;
    Serial.println("Button select menu");
    lcd.clear();
    return;
  }
  if ( btnPressed == btnRIGHT) {
    btnPressed = 0;
    
  }
  if ( btnPressed == btnLEFT) {
    btnPressed = 0;
    
  }
  if ( btnPressed == btnUP) {
    btnPressed = 0;
  }
  if ( btnPressed == btnDOWN) {
    btnPressed = 0;
    lcd.clear();
    return;
  }
  
  
}

void MenuSelect() {
  
  lcd.setCursor(0,0);
  lcd.setCursor(9,0);
  lcd.print("S:");
  
  lcd.print("    ");
}

void MenuEditSensor() {
  static byte cur = 0;
  if (millis()>redraw || btnPressed != 0) {
    redraw = millis()+500;
  }else {
    return;
  }
  lcd.clear();
  if ( btnPressed == btnSELECT) {
    btnPressed = 0;
    currentMenu = MENU_MAIN;
    
    return;
  }
  if ( btnPressed == btnUP) {
    btnPressed = 0;
    cur--;
  }
  if ( btnPressed == btnDOWN) {
    btnPressed = 0;
    cur++;
  }
  cur = constrain(cur, 0,1);

  if ( btnPressed == btnRIGHT) {
    btnPressed = 0;
    if (cur == 0) {
      SENSOR1_AIR = SENSOR1_AIR +4;
    }
    if (cur == 1) {
      SENSOR1_WATER = SENSOR1_WATER +4;
    }
  }
  if ( btnPressed == btnLEFT) {
    btnPressed = 0;
    if (cur == 0) {
      SENSOR1_AIR = SENSOR1_AIR -4;
    }
    if (cur == 1) {
      SENSOR1_WATER = SENSOR1_WATER -4;
    }
  }

  lcd.setCursor(0,0);
  lcd.print("s1:");
  lcd.print(sensor1_value);
  lcd.print("%");
  lcd.setCursor(8,0);
  lcd.print("dry:");
  lcd.print(SENSOR1_AIR);

  lcd.setCursor(0,1);
  lcd.print(sensorValue);
  lcd.setCursor(8,1);
  lcd.print("wet:");
  lcd.print(SENSOR1_WATER);

  lcd.setCursor(7,cur);
  lcd.print(">");
}

void MenuWriteFlash() {
  if ( btnPressed == btnSELECT) {
    btnPressed = 0;
    currentMenu = MENU_MAIN;
    lcd.clear();
    return;
  }

  lcd.setCursor(0,0);
  lcd.print("Write Flash:");

}



void drawMenu() {

  switch (currentMenu) {
    case MENU_SELECT: {
        MenuSelect();
        break;
    }
    case MENU_MAIN: {
        menuMain();
        break;
    }
    case MENU_IDLE: {
        menuIdle();
        break;
    }
    case MENU_EDIT_SENSOR: {
        MenuEditSensor();
        break;
    }
    case MENU_WRITE_FLASH: {
        MenuWriteFlash();
        break;
    }
     
    default: {
        menuIdle();
        break;
    }
  }

}
