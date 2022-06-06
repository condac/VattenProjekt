#define SPEED_CHANGE_FACTOR 50 // how fast speed should change when holding the button. Lower value is faster

#define MENU_MAIN     1
#define MENU_IDLE     2
#define MENU_SELECT   10
#define MENU_EDIT_SENSOR     20
#define MENU_EDIT_TARGET     30
#define MENU_WRITE_FLASH     40

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
    if (page == 1 && curline == 0) { // Edit target
      currentMenu = MENU_EDIT_TARGET;
    }
    if (page == 1 && curline == 1) { // Write flash
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
    lcd.print("  Edit Target");
    lcd.setCursor(0,1);
    lcd.print("  Write flash");
  }
  
  lcd.setCursor(0,curline);
  lcd.print(">");

 
  
  
}
void menuIdle() {
  if (millis()>redraw || btnPressed != 0) {
    redraw = millis()+500;
  }else {
    return;
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(fukt[1]);
  lcd.print("%");
  lcd.setCursor(4,0);
  lcd.print(fukt[2]);
  lcd.print("%");
  lcd.setCursor(8,0);
  lcd.print("3");
  lcd.setCursor(12,0);
  lcd.print("4");

  lcd.setCursor(0,1);
  if (failstate[1]) {
    lcd.print("fail");
  }else {
    if (pump_state[1]) {
      lcd.print("On");
    }else {
      lcd.print("Off");
    }
  }
  
  lcd.setCursor(4,1);
  if (failstate[2]) {
    lcd.print("fail");
  }else {
    if (pump_state[2]) {
      lcd.print("On");
    }else {
      lcd.print("Off");
    }
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
  static byte sen = 1;
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
  cur = constrain(cur, 0,2);
  sen = constrain(sen, 1,4);

  if ( btnPressed == btnRIGHT) {
    btnPressed = 0;
    if (cur == 0) {
      sen++;
      sen = constrain(sen, 1,4);
    }
    if (sen == 1) {
      if (cur == 1) {
        SENSOR_AIR[1] = SENSOR_AIR[1] +4;
        SENSOR_AIR[1] = constrain(SENSOR_AIR[1], 0,1023);
      }
      if (cur == 2) {
        SENSOR_WATER[1] = SENSOR_WATER[1] +4;
        SENSOR_WATER[1] = constrain(SENSOR_WATER[1], 0,1023);
      }
    }
    if (sen == 2) {
      if (cur == 1) {
        SENSOR_AIR[2] = SENSOR_AIR[2] +4;
        SENSOR_AIR[2] = constrain(SENSOR_AIR[2], 0,1023);
      }
      if (cur == 2) {
        SENSOR_WATER[2] = SENSOR_WATER[2] +4;
        SENSOR_WATER[2] = constrain(SENSOR_WATER[2], 0,1023);
      }
    }
  }
  if ( btnPressed == btnLEFT) {
    btnPressed = 0;
    if (cur == 0) {
      sen--;
      sen = constrain(sen, 1,4);
    }
    if (sen == 1) {
      if (cur == 1) {
        SENSOR_AIR[1] = SENSOR_AIR[1] -4;
        SENSOR_AIR[1] = constrain(SENSOR_AIR[1], 0,1023);
      }
      if (cur == 2) {
        SENSOR_WATER[1] = SENSOR_WATER[1] -4;
        SENSOR_WATER[1] = constrain(SENSOR_WATER[1], 0,1023);
      }
    }
    if (sen == 2) {
      if (cur == 1) {
        SENSOR_AIR[2] = SENSOR_AIR[2] -4;
        SENSOR_AIR[2] = constrain(SENSOR_AIR[2], 0,1023);
      }
      if (cur == 2) {
        SENSOR_WATER[2] = SENSOR_WATER[2] -4;
        SENSOR_WATER[2] = constrain(SENSOR_WATER[2], 0,1023);
      }
    }
  }

  if (sen == 1) {
    lcd.setCursor(0,0);
    lcd.print("s1:");
    lcd.print(fukt[1]);
    lcd.print("%");
    lcd.setCursor(8,0);
    lcd.print("dry:");
    lcd.print(SENSOR_AIR[1]);
  
    lcd.setCursor(0,1);
    lcd.print(sensor_value[1]);
    lcd.setCursor(8,1);
    lcd.print("wet:");
    lcd.print(SENSOR_WATER[1]);
  }
  if (sen == 2) {
    lcd.setCursor(0,0);
    lcd.print("s2:");
    lcd.print(fukt[2]);
    lcd.print("%");
    lcd.setCursor(8,0);
    lcd.print("dry:");
    lcd.print(SENSOR_AIR[2]);
  
    lcd.setCursor(0,1);
    lcd.print(sensor_value[2]);
    lcd.setCursor(8,1);
    lcd.print("wet:");
    lcd.print(SENSOR_WATER[2]);
  }
  

  if (cur >0) {
    lcd.setCursor(7,cur);
    lcd.print(">");
  }else {
    lcd.setCursor(0,0);
    lcd.print(">");
  }
  
}

void MenuEditTarget() {
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
  if ( btnPressed == btnLEFT) {
    btnPressed = 0;
    cur--;
  }
  if ( btnPressed == btnRIGHT) {
    btnPressed = 0;
    cur++;
  }
  cur = constrain(cur, 0,3);

  if ( btnPressed == btnUP) {
    btnPressed = 0;
    if (cur == 0) {
      PUMP_TRIGGER[1] = PUMP_TRIGGER[1] +1;
      PUMP_TRIGGER[1] = constrain(PUMP_TRIGGER[1], 0,100);
    }
    if (cur == 1) {
      PUMP_TRIGGER[2] = PUMP_TRIGGER[2] +1;
      PUMP_TRIGGER[2] = constrain(PUMP_TRIGGER[2], 0,100);
    }
    
  }
  if ( btnPressed == btnDOWN) {
    btnPressed = 0;
    if (cur == 0) {
      PUMP_TRIGGER[1] = PUMP_TRIGGER[1] -1;
      PUMP_TRIGGER[1] = constrain(PUMP_TRIGGER[1], 0,100);
    }
    if (cur == 1) {
      PUMP_TRIGGER[2] = PUMP_TRIGGER[2] -1;
      PUMP_TRIGGER[2] = constrain(PUMP_TRIGGER[2], 0,100);
    }
    
  }


  lcd.setCursor(1,0);
  lcd.print("1:");
  lcd.print(fukt[1]);
  lcd.setCursor(5,0);
  lcd.print("2:");
  lcd.print(fukt[2]);
  lcd.setCursor(9,0);
  lcd.print("3:");
  lcd.print(fukt[3]);
  lcd.setCursor(13,0);
  lcd.print("4:");
  lcd.print(fukt[4]);

  lcd.setCursor(1,1);
  
  lcd.print(PUMP_TRIGGER[1]);
  lcd.setCursor(5,2);
  lcd.print(PUMP_TRIGGER[2]);
  lcd.setCursor(9,3);
  lcd.print(PUMP_TRIGGER[3]);
  lcd.setCursor(13,4);
  lcd.print(PUMP_TRIGGER[4]);
  

  lcd.setCursor(4*cur,0);
  lcd.print(">");
}

void MenuWriteFlash() {
  static byte cur = 0;
  if (millis()>redraw || btnPressed != 0) {
    redraw = millis()+500;
  }else {
    return;
  }
  lcd.clear();
  if ( btnPressed == btnSELECT) {
    btnPressed = 0;
    if (cur ==1) {
      writeSaveData();
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(" Save complete");
      delay(1000);
    }
    currentMenu = MENU_MAIN;
    
    return;
  }
  if ( btnPressed == btnDOWN) {
    btnPressed = 0;
    cur--;
  }
  if ( btnPressed == btnUP) {
    btnPressed = 0;
    cur++;
  }
  cur = constrain(cur, 0,1);


  lcd.setCursor(1,0);
  lcd.print(" Back");

  lcd.setCursor(1,1);
  lcd.print(" Save");

  lcd.setCursor(0,cur);
  lcd.print(">");
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
    case MENU_EDIT_TARGET: {
        MenuEditTarget();
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
