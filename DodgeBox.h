/*
 *  DODGE BOX - Esquiva obstaculos
 */

#ifndef DODGE_BOX_H
#define DODGE_BOX_H

#include "Config.h"

namespace DodgeBox {
  
  
  // SPRITES
  
  const unsigned char playerLeftRun1[] PROGMEM = {
    0b00011000, 0b00111000, 0b01111000, 0b00111000,
    0b00111100, 0b00011000, 0b00101000, 0b01000100
  };
  
  const unsigned char playerLeftRun2[] PROGMEM = {
    0b00011000, 0b00111000, 0b01111000, 0b00111000,
    0b00111100, 0b00011000, 0b00100100, 0b01001000
  };
  
  const unsigned char playerRightRun1[] PROGMEM = {
    0b00011000, 0b00011100, 0b00011110, 0b00011100,
    0b00111100, 0b00011000, 0b00010100, 0b00100010
  };
  
  const unsigned char playerRightRun2[] PROGMEM = {
    0b00011000, 0b00011100, 0b00011110, 0b00011100,
    0b00111100, 0b00011000, 0b00100100, 0b00010010
  };
  
  const unsigned char playerFront[] PROGMEM = {
    0b00011000, 0b00111100, 0b00011000, 0b00111100,
    0b01011010, 0b00011000, 0b00100100, 0b01000010
  };
  
  const unsigned char shieldSprite[] PROGMEM = {
    0b00111100, 0b01111110, 0b11111111, 0b11111111,
    0b11111111, 0b11111111, 0b01111110, 0b00111100
  };
  
  const unsigned char heartSprite[] PROGMEM = {
    0b01100110, 0b11111111, 0b11111111, 0b11111111,
    0b01111110, 0b00111100, 0b00011000, 0b00000000
  };
  
  const unsigned char clockSprite[] PROGMEM = {
    0b00111100, 0b01000010, 0b10011001, 0b10011001,
    0b10000001, 0b10000001, 0b01000010, 0b00111100
  };
  
  
  // VARIABLES
  
  // Jugador
  int playerX = 60;
  const int playerY = 54;
  const int playerSize = 8;
  int animFrame = 0;
  unsigned long lastAnimTime = 0;
  int playerDirection = 0;
  
  // Obstaculos
  const int NUM_OBS = 3;
  int obsX[NUM_OBS];
  int obsY[NUM_OBS];
  const int obsSize = 8;
  
  // Power-ups
  const int NUM_POWERUPS = 3;
  int powerupX[NUM_POWERUPS];
  int powerupY[NUM_POWERUPS];
  int powerupType[NUM_POWERUPS];
  bool powerupActive[NUM_POWERUPS];
  unsigned long powerupSpawnTime[NUM_POWERUPS];
  
  //Estados power-ups
  bool hasShield = false;
  unsigned long shieldTime = 0;
  const unsigned long SHIELD_DURATION = 5000;
  
  bool slowMotion = false;
  unsigned long slowMoTime = 0;
  const unsigned long SLOWMO_DURATION = 4000;
  
  // Estado del juego
  int score = 0;
  int highScore = 0;
  int lives = 3;
  float speedGame = 1.5;
  bool gameOver = false;
  bool gameStarted = false;
  bool paused = false;
  bool hardMode = false;
  bool volverAlMenu = false;
  int pauseOption = 0; // 0 = Continuar, 1 = Salir al menu
  
  // Timing
  unsigned long lastFrame = 0;
  const int frameDelay = 40;
  unsigned long gameOverTime = 0;
  const unsigned long GAME_OVER_DELAY = 3000;
  
  // Explosion
  bool exploding = false;
  int explosionFrame = 0;
  
  
  // FUNCIONES
  
  void iniciarObstaculos() {
    for (int i = 0; i < NUM_OBS; i++) {
      obsX[i] = random(0, SCREEN_WIDTH - obsSize);
      obsY[i] = random(-60, 0);
    }
  }
  
  void iniciarPowerups() {
    for (int i = 0; i < NUM_POWERUPS; i++) {
      powerupX[i] = random(0, SCREEN_WIDTH - 8);
      powerupY[i] = random(-200, -50);
      powerupType[i] = random(0, 3);
      powerupActive[i] = false;
      powerupSpawnTime[i] = millis() + random(3000, 8000);
    }
  }
  
  void dibujarCorazon(int x, int y) {
    display.fillCircle(x + 2, y + 2, 2, SSD1306_WHITE);
    display.fillCircle(x + 6, y + 2, 2, SSD1306_WHITE);
    display.fillTriangle(x, y + 3, x + 8, y + 3, x + 4, y + 8, SSD1306_WHITE);
  }
  
  void moverJugador() {
    int joyX = analogRead(Config::pinJoyX);
    
    if (joyX < 300) {
      playerX -= 3;
      playerDirection = -1;
    } else if (joyX > 700) {
      playerX += 3;
      playerDirection = 1;
    } else {
      playerDirection = 0;
    }
    
    playerX = constrain(playerX, 0, SCREEN_WIDTH - playerSize);
    
    if (playerDirection != 0 && millis() - lastAnimTime > 150) {
      animFrame = 1 - animFrame;
      lastAnimTime = millis();
    }
    
    if (digitalRead(Config::pinJoySW) == LOW) {
      paused = true;
      pauseOption = 0;
      delay(300);
    }
  }
  
  void moverObstaculos() {
    for (int i = 0; i < NUM_OBS; i++) {
      obsY[i] += speedGame;
      if (obsY[i] > SCREEN_HEIGHT) {
        obsY[i] = random(-20, -5);
        obsX[i] = random(0, SCREEN_WIDTH - obsSize);
      }
    }
  }
  
  void moverPowerups() {
    for (int i = 0; i < NUM_POWERUPS; i++) {
      if (!powerupActive[i]) {
        if (millis() > powerupSpawnTime[i]) {
          powerupActive[i] = true;
          powerupY[i] = -8;
          powerupX[i] = random(0, SCREEN_WIDTH - 8);
          powerupType[i] = random(0, 3);
        }
      } else {
        powerupY[i] += speedGame * 0.7;
        if (powerupY[i] > SCREEN_HEIGHT) {
          powerupActive[i] = false;
          powerupSpawnTime[i] = millis() + random(5000, 12000);
        }
      }
    }
  }
  
  void detectarPowerups() {
    for (int i = 0; i < NUM_POWERUPS; i++) {
      if (powerupActive[i]) {
        if (powerupX[i] < playerX + playerSize &&
            powerupX[i] + 8 > playerX &&
            powerupY[i] < playerY + playerSize &&
            powerupY[i] + 8 > playerY) {
          
          switch(powerupType[i]) {
            case 0: hasShield = true; shieldTime = millis(); break;
            case 1: if (lives < 3) lives++; break;
            case 2: slowMotion = true; slowMoTime = millis(); break;
          }
          
          powerupActive[i] = false;
          powerupSpawnTime[i] = millis() + random(5000, 12000);
        }
      }
    }
  }
  
  void actualizarEstadosPowerups() {
    if (hasShield && millis() - shieldTime > SHIELD_DURATION) hasShield = false;
    if (slowMotion && millis() - slowMoTime > SLOWMO_DURATION) slowMotion = false;
  }
  
  void detectarColisiones() {
    for (int i = 0; i < NUM_OBS; i++) {
      if (obsX[i] < playerX + playerSize &&
          obsX[i] + obsSize > playerX &&
          obsY[i] < playerY + playerSize &&
          obsY[i] + obsSize > playerY) {
        
        if (hasShield) {
          hasShield = false;
          obsY[i] = random(-20, -5);
        } else {
          lives--;
          obsY[i] = random(-20, -5);
          if (lives <= 0) {
            exploding = true;
            explosionFrame = 0;
          }
        }
        return;
      }
    }
  }
  
  void animacionExplosion() {
    display.clearDisplay();
    int size = explosionFrame * 3;
    display.drawRect(playerX - size/2, playerY - size/2, 
                     playerSize + size, playerSize + size, SSD1306_WHITE);
    display.display();
    explosionFrame++;
    
    if (explosionFrame > 6) {
      exploding = false;
      gameOver = true;
      gameOverTime = millis();
      if (score > highScore) {
        highScore = score;
        EEPROM.put(Config::EEPROM_DODGE_BOX, highScore);
      }
    }
  }
  
  void dibujarPantalla() {
    display.clearDisplay();
    
    //Jugador
    if (playerDirection == -1) {
      display.drawBitmap(playerX, playerY, 
        animFrame == 0 ? playerLeftRun1 : playerLeftRun2, 8, 8, SSD1306_WHITE);
    } else if (playerDirection == 1) {
      display.drawBitmap(playerX, playerY, 
        animFrame == 0 ? playerRightRun1 : playerRightRun2, 8, 8, SSD1306_WHITE);
    } else {
      display.drawBitmap(playerX, playerY, playerFront, 8, 8, SSD1306_WHITE);
    }
    
    //Escudo
    if (hasShield) {
      display.drawCircle(playerX + 4, playerY + 4, 10, SSD1306_WHITE);
      display.drawCircle(playerX + 4, playerY + 4, 11, SSD1306_WHITE);
    }
    
    //Obstaculos
    for (int i = 0; i < NUM_OBS; i++) {
      display.fillRect(obsX[i], obsY[i], obsSize, obsSize, SSD1306_WHITE);
    }
    
    //Power-ups
    for (int i = 0; i < NUM_POWERUPS; i++) {
      if (powerupActive[i]) {
        const unsigned char* sprite = (powerupType[i] == 0) ? shieldSprite :
                                     (powerupType[i] == 1) ? heartSprite : clockSprite;
        display.drawBitmap(powerupX[i], powerupY[i], sprite, 8, 8, SSD1306_WHITE);
      }
    }
    
    //UI
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.print("S:"); display.print(score);
    
    for (int i = 0; i < lives; i++) {
      dibujarCorazon(90 + i * 12, 0);
    }
    
    if (hasShield) { display.setCursor(0, 56); display.print("SHIELD"); }
    if (slowMotion) { display.setCursor(70, 56); display.print("SLOW"); }
    
    display.display();
  }
  
  void menuInicio() {
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(15, 5);
    display.println("DODGE");
    display.setCursor(20, 25);
    display.println("BOX");
    
    display.setTextSize(1);
    display.setCursor(10, 48);
    display.print("MODE: ");
    display.print(hardMode ? "HARD" : "NORMAL");
    display.display();
    
    int joyX = analogRead(Config::pinJoyX);
    if (joyX < 300) hardMode = false;
    if (joyX > 700) hardMode = true;
    
    if (digitalRead(Config::pinJoySW) == LOW) {
      gameStarted = true;
      score = 0;
      lives = 3;
      speedGame = 1.5;
      exploding = false;
      gameOver = false;
      paused = false;
      playerX = 60;
      hasShield = false;
      slowMotion = false;
      iniciarObstaculos();
      iniciarPowerups();
      delay(300);
    }
  }
  
  void pantallaPausa() {
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(20, 5);
    display.println("PAUSA");
    
    display.drawLine(0, 22, 128, 22, SSD1306_WHITE);
    
    // Opcion 1: Continuar
    display.setTextSize(1);
    if (pauseOption == 0) {
      display.fillRect(10, 28, 108, 12, SSD1306_WHITE);
      display.setTextColor(SSD1306_BLACK);
    }
    display.setCursor(25, 30);
    display.print("> CONTINUAR <");
    display.setTextColor(SSD1306_WHITE);
    
    // Opcion 2: Salir al meno
    if (pauseOption == 1) {
      display.fillRect(10, 45, 108, 12, SSD1306_WHITE);
      display.setTextColor(SSD1306_BLACK);
    }
    display.setCursor(20, 47);
    display.print("> MENU PRINCIPAL <");
    display.setTextColor(SSD1306_WHITE);
    
    display.display();
    
    // Navegacion
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
    
    // Seleccion
    if (digitalRead(Config::pinJoySW) == LOW) {
      if (pauseOption == 0) {
        // Continuar jugando
        paused = false;
      } else {
        // Volver al meno principal
        volverAlMenu = true;
        gameStarted = false;
      }
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
    display.print("HI:"); display.print(highScore);
    display.display();
    
    if (millis() - gameOverTime > GAME_OVER_DELAY) {
      gameOver = false;
      gameStarted = false;
      volverAlMenu = true;
    }
  }
  
 
  // FUNCIONES PUBLICAS
  
  void iniciar() {
    EEPROM.get(Config::EEPROM_DODGE_BOX, highScore);
    volverAlMenu = false;
    gameStarted = false;
    gameOver = false;
  }
  
  void actualizar() {
    if (!gameStarted) {
      menuInicio();
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
    
    if (millis() - lastFrame < frameDelay) return;
    lastFrame = millis();
    
    moverJugador();
    moverObstaculos();
    moverPowerups();
    detectarColisiones();
    detectarPowerups();
    actualizarEstadosPowerups();
    
    if (exploding) {
      animacionExplosion();
    } else {
      dibujarPantalla();
    }
    
    score++;
    float baseSpeed = hardMode ? 2.0 + score * 0.015 : 1.5 + score * 0.01;
    speedGame = slowMotion ? baseSpeed * 0.5 : baseSpeed;
  }
}

#endif