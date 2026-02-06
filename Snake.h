/*
 *  SNAKE
 */

#ifndef SNAKE_H
#define SNAKE_H

#include "Config.h"

namespace Snake {
  
  // CONSTANTES
  
  const int GRID_SIZE = 4;
  const int GRID_WIDTH = 32;
  const int GRID_HEIGHT = 13;
  const int MAX_SNAKE_LENGTH = 50;  // reducido para ahorrar memoria ram
  
  //Direcciones
  #define DIR_UP 0
  #define DIR_RIGHT 1
  #define DIR_DOWN 2
  #define DIR_LEFT 3
  
  // VARIABLES
  
  // Snake-Usar bytes en lugar de ints
  byte snakeX[MAX_SNAKE_LENGTH];
  byte snakeY[MAX_SNAKE_LENGTH];
  byte snakeLength = 3;
  byte currentDirection = DIR_RIGHT;
  byte nextDirection = DIR_RIGHT;
  
  //Fruta
  byte fruitX = 10;
  byte fruitY = 7;
  byte fruitType = 0;  // 0-4 tipos
  
  //Estado
  int score = 0;
  int highScore = 0;
  bool gameOver = false;
  bool gameStarted = false;
  bool paused = false;
  bool volverAlMenu = false;
  byte pauseOption = 0;
  bool hardMode = false;
  
  //Timing
  unsigned long lastMove = 0;
  int gameSpeed = 150;
  unsigned long gameOverTime = 0;
  
  //Countdown
  bool countdownActive = true;
  int countdownNumber = 3;  // Cambiado de byte a int
  unsigned long lastCountdown = 0;
  
  // FUNCIONES
  void generarFruta() {
    byte intentos = 0;
    bool valida = false;
    
    while (!valida && intentos < 30) {
      fruitX = random(0, GRID_WIDTH);
      fruitY = random(0, GRID_HEIGHT);
      
      valida = true;
      for (byte i = 0; i < snakeLength; i++) {
        if (snakeX[i] == fruitX && snakeY[i] == fruitY) {
          valida = false;
          break;
        }
      }
      intentos++;
    }
    
    fruitType = random(0, 5);
  }
  
  void iniciarSnake() {
    snakeLength = 3;
    currentDirection = DIR_RIGHT;
    nextDirection = DIR_RIGHT;
    
    snakeX[0] = 16; snakeY[0] = 6;
    snakeX[1] = 15; snakeY[1] = 6;
    snakeX[2] = 14; snakeY[2] = 6;
    
    generarFruta();
  }
  
  void leerControles() {
    int joyX = analogRead(Config::pinJoyX);
    int joyY = analogRead(Config::pinJoyY);
    
    if (joyY < 300 && currentDirection != DIR_DOWN) {
      nextDirection = DIR_UP;
    } else if (joyY > 700 && currentDirection != DIR_UP) {
      nextDirection = DIR_DOWN;
    } else if (joyX < 300 && currentDirection != DIR_RIGHT) {
      nextDirection = DIR_LEFT;
    } else if (joyX > 700 && currentDirection != DIR_LEFT) {
      nextDirection = DIR_RIGHT;
    }
    
    if (digitalRead(Config::pinJoySW) == LOW) {
      paused = true;
      pauseOption = 0;
      delay(300);
    }
  }
  
  void moverSnake() {
    if (millis() - lastMove < gameSpeed) return;
    lastMove = millis();
    
    currentDirection = nextDirection;
    
    byte newX = snakeX[0];
    byte newY = snakeY[0];
    
    if (currentDirection == DIR_UP) newY--;
    else if (currentDirection == DIR_DOWN) newY++;
    else if (currentDirection == DIR_LEFT) newX--;
    else if (currentDirection == DIR_RIGHT) newX++;
    
    //Colision con paredes
    if (newX >= GRID_WIDTH || newY >= GRID_HEIGHT) {
      gameOver = true;
      gameOverTime = millis();
      if (score > highScore) {
        highScore = score;
        EEPROM.put(Config::EEPROM_JUEGO_3, highScore);
      }
      return;
    }
    
    //Colision consigo misma
    for (byte i = 1; i < snakeLength; i++) {
      if (newX == snakeX[i] && newY == snakeY[i]) {
        gameOver = true;
        gameOverTime = millis();
        if (score > highScore) {
          highScore = score;
          EEPROM.put(Config::EEPROM_JUEGO_3, highScore);
        }
        return;
      }
    }
    
    //Comio fruta
    bool comio = (newX == fruitX && newY == fruitY);
    
    if (comio) {
      score += (fruitType + 1) * 5;
      if (snakeLength < MAX_SNAKE_LENGTH) snakeLength++;
      
      gameSpeed = hardMode ? max(50, 150 - score/8) : max(80, 150 - score/12);
      
      generarFruta();
    }
    
    //Mover cuerpo
    for (byte i = snakeLength - 1; i > 0; i--) {
      snakeX[i] = snakeX[i - 1];
      snakeY[i] = snakeY[i - 1];
    }
    
    snakeX[0] = newX;
    snakeY[0] = newY;
  }
  
  void dibujarPantalla() {
    display.clearDisplay();
    
    //Marcador
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.print("S:");
    display.print(score);
    
    display.setCursor(80, 0);
    display.print("L:");
    display.print(snakeLength);
    
    display.drawLine(0, 9, 128, 9, SSD1306_WHITE);
    
    //Fruta
    byte fx = fruitX * GRID_SIZE;
    byte fy = fruitY * GRID_SIZE + 10;
    
    if (fruitType == 0) display.fillRect(fx, fy, GRID_SIZE, GRID_SIZE, SSD1306_WHITE);
    else if (fruitType == 1) display.fillRect(fx+1, fy, GRID_SIZE-2, GRID_SIZE, SSD1306_WHITE);
    else if (fruitType == 2) display.fillRect(fx, fy+1, GRID_SIZE, GRID_SIZE-2, SSD1306_WHITE);
    else if (fruitType == 3) display.fillCircle(fx+2, fy+2, 2, SSD1306_WHITE);
    else display.drawCircle(fx+2, fy+2, 2, SSD1306_WHITE);
    
    //Serpiente
    for (byte i = 0; i < snakeLength; i++) {
      byte sx = snakeX[i] * GRID_SIZE;
      byte sy = snakeY[i] * GRID_SIZE + 10;
      
      if (i == 0) {
        display.fillRect(sx, sy, GRID_SIZE, GRID_SIZE, SSD1306_WHITE);
      } else {
        display.fillRect(sx+1, sy+1, GRID_SIZE-2, GRID_SIZE-2, SSD1306_WHITE);
      }
    }
    
    display.display();
  }
  
  void pantallaCountdown() {
    display.clearDisplay();
    
    if (millis() - lastCountdown > 1000) {
      countdownNumber--;
      lastCountdown = millis();
    }
    
    if (countdownNumber < 0) {
      countdownActive = false;
      return;
    }
    
    display.setTextSize(4);
    if (countdownNumber > 0) {
      display.setCursor(countdownNumber == 1 ? 55 : 50, 20);
      display.print(countdownNumber);
    } else {
      display.setTextSize(3);
      display.setCursor(30, 20);
      display.print("GO!");
    }
    display.display();
  }
  
  void menuInicio() {
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(20, 15);
    display.println("SNAKE");
    
    display.setTextSize(1);
    display.setCursor(10, 45);
    display.print("MODE: ");
    display.print(hardMode ? "HARD" : "NORMAL");
    display.display();
    
    int joyX = analogRead(Config::pinJoyX);
    if (joyX < 300) hardMode = false;
    if (joyX > 700) hardMode = true;
    
    if (digitalRead(Config::pinJoySW) == LOW) {
      gameStarted = true;
      countdownActive = true;
      countdownNumber = 3;
      lastCountdown = millis();
      score = 0;
      gameSpeed = hardMode ? 120 : 150;
      gameOver = false;
      paused = false;
      iniciarSnake();
      delay(300);
    }
  }
  
  void pantallaPausa() {
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(20, 5);
    display.println("PAUSA");
    display.drawLine(0, 22, 128, 22, SSD1306_WHITE);
    
    display.setTextSize(1);
    if (pauseOption == 0) {
      display.fillRect(10, 28, 108, 12, SSD1306_WHITE);
      display.setTextColor(SSD1306_BLACK);
    }
    display.setCursor(25, 30);
    display.print("> CONTINUAR <");
    display.setTextColor(SSD1306_WHITE);
    
    if (pauseOption == 1) {
      display.fillRect(10, 45, 108, 12, SSD1306_WHITE);
      display.setTextColor(SSD1306_BLACK);
    }
    display.setCursor(20, 47);
    display.print("> MENU PRINCIPAL <");
    display.setTextColor(SSD1306_WHITE);
    display.display();
    
    static unsigned long lastInput = 0;
    int joyY = analogRead(Config::pinJoyY);
    
    if (millis() - lastInput > 200) {
      if (joyY < 300 && pauseOption > 0) {
        pauseOption--;
        lastInput = millis();
      } else if (joyY > 700 && pauseOption < 1) {
        pauseOption++;
        lastInput = millis();
      }
    }
    
    if (digitalRead(Config::pinJoySW) == LOW) {
      if (pauseOption == 0) paused = false;
      else { volverAlMenu = true; gameStarted = false; }
      delay(300);
    }
  }
  
  void pantallaGameOver() {
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(10, 10);
    display.println("GAME");
    display.setCursor(10, 30);
    display.println("OVER");
    
    display.setTextSize(1);
    display.setCursor(5, 52);
    display.print("HI: ");
    display.print(highScore);
    display.display();
    
    if (millis() - gameOverTime > 3000) {
      gameOver = false;
      gameStarted = false;
      volverAlMenu = true;
    }
  }
  
  void iniciar() {
    EEPROM.get(Config::EEPROM_JUEGO_3, highScore);
    volverAlMenu = false;
    gameStarted = false;
    gameOver = false;
    countdownActive = true;
  }
  
  void actualizar() {
    if (!gameStarted) {
      menuInicio();
      return;
    }
    
    if (countdownActive) {
      pantallaCountdown();
      return;
    }
    
    if (gameOver) {
      pantallaGameOver();
      return;
    }
    
    if (paused) {
      pantallaPausa();
      return;
    }
    
    leerControles();
    moverSnake();
    dibujarPantalla();
  }
}

#endif