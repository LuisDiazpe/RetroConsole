/*
 *  SPACE SHOOTER - Dispara a los enemigos
 */

#ifndef SPACE_SHOOTER_H
#define SPACE_SHOOTER_H

#include "Config.h"

namespace SpaceShooter {
  
  
  // SPRITES
  
  // Nave del jugador
  const unsigned char playerShip[] PROGMEM = {
    0b00011000,
    0b00111100,
    0b01111110,
    0b11111111,
    0b11111111,
    0b01011010,
    0b00100100,
    0b01000010
  };
  
  // Enemigo tipo 1
  const unsigned char enemy1[] PROGMEM = {
    0b00100100,
    0b01011010,
    0b01111110,
    0b11011011,
    0b11111111,
    0b00111100,
    0b01000010,
    0b10100101
  };
  
  // Enemigo tipo 2
  const unsigned char enemy2[] PROGMEM = {
    0b00011000,
    0b00111100,
    0b01111110,
    0b11011011,
    0b11111111,
    0b01011010,
    0b10000001,
    0b01000010
  };
  
  // Explosion
  const unsigned char explosion[] PROGMEM = {
    0b10100101,
    0b01011010,
    0b00111100,
    0b11011011,
    0b11011011,
    0b00111100,
    0b01011010,
    0b10100101
  };
  
  // VARIABLES
  
  // Jugador
  int playerX = 60;
  int playerY = 30;  
  const int playerSize = 8;
  
  // Dificultad
  bool hardMode = false;
  
  // Balas del jugador
  const int MAX_BULLETS = 5;
  int bulletX[MAX_BULLETS];
  int bulletY[MAX_BULLETS];
  bool bulletActive[MAX_BULLETS];
  unsigned long lastShot = 0;
  const int SHOOT_DELAY = 350; // Disparo automatico cada 350ms
  
  // Enemigos
  const int MAX_ENEMIES = 8;
  int enemyX[MAX_ENEMIES];
  int enemyY[MAX_ENEMIES];
  int enemyType[MAX_ENEMIES];
  bool enemyActive[MAX_ENEMIES];
  unsigned long lastEnemySpawn = 0;
  int enemySpawnDelay = 1500;
  
  // Explosiones
  const int MAX_EXPLOSIONS = 3;
  int explosionX[MAX_EXPLOSIONS];
  int explosionY[MAX_EXPLOSIONS];
  int explosionFrame[MAX_EXPLOSIONS];
  bool explosionActive[MAX_EXPLOSIONS];
  
  // Estado del juego
  int score = 0;
  int highScore = 0;
  int lives = 3;
  bool gameOver = false;
  bool gameStarted = false;
  bool paused = false;
  bool volverAlMenu = false;
  int pauseOption = 0; // 0 = Continuar, 1 = Salir al menu
  
  // Countdown
  bool countdownActive = true;
  int countdownNumber = 3;
  unsigned long lastCountdown = 0;
  
  // Timing
  unsigned long lastFrame = 0;
  const int frameDelay = 30;
  unsigned long gameOverTime = 0;
  const unsigned long GAME_OVER_DELAY = 3000;
  
  // Dificultad progresiva
  float gameSpeed = 1.0;
  
  // FUNCIONES
  
  void iniciarBalas() {
    for (int i = 0; i < MAX_BULLETS; i++) {
      bulletActive[i] = false;
      bulletX[i] = 0;
      bulletY[i] = 0;
    }
  }
  
  void iniciarEnemigos() {
    for (int i = 0; i < MAX_ENEMIES; i++) {
      enemyActive[i] = false;
      enemyX[i] = 0;
      enemyY[i] = 0;
      enemyType[i] = 0;
    }
  }
  
  void iniciarExplosiones() {
    for (int i = 0; i < MAX_EXPLOSIONS; i++) {
      explosionActive[i] = false;
      explosionX[i] = 0;
      explosionY[i] = 0;
      explosionFrame[i] = 0;
    }
  }
  
  void crearExplosion(int x, int y) {
    for (int i = 0; i < MAX_EXPLOSIONS; i++) {
      if (!explosionActive[i]) {
        explosionActive[i] = true;
        explosionX[i] = x;
        explosionY[i] = y;
        explosionFrame[i] = 0;
        break;
      }
    }
  }
  
  void dibujarCorazon(int x, int y) {
    display.fillCircle(x + 2, y + 2, 2, SSD1306_WHITE);
    display.fillCircle(x + 6, y + 2, 2, SSD1306_WHITE);
    display.fillTriangle(x, y + 3, x + 8, y + 3, x + 4, y + 8, SSD1306_WHITE);
  }
  
  void moverJugador() {
    int joyX = analogRead(Config::pinJoyX);
    int joyY = analogRead(Config::pinJoyY);
    
    // Movimiento en X (horizontal)
    if (joyX < 300) {
      playerX -= 3;
    } else if (joyX > 700) {
      playerX += 3;
    }
    
    // Movimiento en Y (vertical) - MOVIMIENTO 360°
    if (joyY < 300) {
      playerY -= 3;
    } else if (joyY > 700) {
      playerY += 3;
    }
    
    // Limitar movimiento dentro de la pantalla
    playerX = constrain(playerX, 0, SCREEN_WIDTH - playerSize);
    playerY = constrain(playerY, 10, SCREEN_HEIGHT - playerSize);
    
    if (digitalRead(Config::pinJoySW) == LOW) {
      paused = true;
      pauseOption = 0;
      delay(300);
    }
  }
  
  void dispararAutomatico() {
    if (millis() - lastShot < SHOOT_DELAY) return;
    
    //Buscar slot disponible para bala
    for (int i = 0; i < MAX_BULLETS; i++) {
      if (!bulletActive[i]) {
        bulletActive[i] = true;
        bulletX[i] = playerX + 3;
        bulletY[i] = playerY - 2;  //Dispara desde la posicion actual del jugador
        lastShot = millis();
        break;
      }
    }
  }
  
  void moverBalas() {
    for (int i = 0; i < MAX_BULLETS; i++) {
      if (bulletActive[i]) {
        bulletY[i] -= 4;
        if (bulletY[i] < 0) {
          bulletActive[i] = false;
        }
      }
    }
  }
  
  void generarEnemigos() {
    if (millis() - lastEnemySpawn < enemySpawnDelay) return;
    
    for (int i = 0; i < MAX_ENEMIES; i++) {
      if (!enemyActive[i]) {
        enemyActive[i] = true;
        enemyX[i] = random(0, SCREEN_WIDTH - 8);
        enemyY[i] = -8;
        enemyType[i] = random(0, 2);
        lastEnemySpawn = millis();
        
        //Dificultad progresiva
        if (hardMode) {
          enemySpawnDelay = max(300, 1000 - score * 8);  // Hard: mas enemigos, mas rapido
        } else {
          enemySpawnDelay = max(500, 1500 - score * 10);  // Normal
        }
        break;
      }
    }
  }
  
  void moverEnemigos() {
    for (int i = 0; i < MAX_ENEMIES; i++) {
      if (enemyActive[i]) {
        //En modo HARD los enemigos se mueven mas rapido
        float speedMultiplier = hardMode ? 1.8 : 1.5;
        enemyY[i] += speedMultiplier * gameSpeed;
        
        // Si el enemigo llega abajo sin ser destruido se pierde 1 vida por enemigo
        if (enemyY[i] > SCREEN_HEIGHT) {
          enemyActive[i] = false;
          lives--;
          if (lives <= 0) {
            gameOver = true;
            gameOverTime = millis();
            if (score > highScore) {
              highScore = score;
              EEPROM.put(Config::EEPROM_JUEGO_2, highScore);
            }
          }
        }
      }
    }
  }
  
  void actualizarExplosiones() {
    for (int i = 0; i < MAX_EXPLOSIONS; i++) {
      if (explosionActive[i]) {
        explosionFrame[i]++;
        if (explosionFrame[i] > 8) {
          explosionActive[i] = false;
        }
      }
    }
  }
  
  void detectarColisiones() {
    //Balas vs Enemigos
    for (int b = 0; b < MAX_BULLETS; b++) {
      if (!bulletActive[b]) continue;
      
      for (int e = 0; e < MAX_ENEMIES; e++) {
        if (!enemyActive[e]) continue;
        
        if (bulletX[b] >= enemyX[e] && bulletX[b] <= enemyX[e] + 8 &&
            bulletY[b] >= enemyY[e] && bulletY[b] <= enemyY[e] + 8) {
          
          //Colision detectada
          bulletActive[b] = false;
          enemyActive[e] = false;
          crearExplosion(enemyX[e], enemyY[e]);
          score += 10;
          
          //Aumentar velocidad gradualmente
          gameSpeed = 1.0 + (score / 500.0);
        }
      }
    }
  }
  
  void dibujarPantalla() {
    display.clearDisplay();
    
    //Nave del jugador
    display.drawBitmap(playerX, playerY, playerShip, 8, 8, SSD1306_WHITE);
    
    //Balas
    for (int i = 0; i < MAX_BULLETS; i++) {
      if (bulletActive[i]) {
        display.fillRect(bulletX[i], bulletY[i], 2, 4, SSD1306_WHITE);
      }
    }
    
    //Enemigos
    for (int i = 0; i < MAX_ENEMIES; i++) {
      if (enemyActive[i]) {
        const unsigned char* sprite = (enemyType[i] == 0) ? enemy1 : enemy2;
        display.drawBitmap(enemyX[i], enemyY[i], sprite, 8, 8, SSD1306_WHITE);
      }
    }
    
    //Explosiones
    for (int i = 0; i < MAX_EXPLOSIONS; i++) {
      if (explosionActive[i]) {
        display.drawBitmap(explosionX[i], explosionY[i], explosion, 8, 8, SSD1306_WHITE);
      }
    }
    
    //UI - Score
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.print("S:");
    display.print(score);
    
    //Vidas
    for (int i = 0; i < lives; i++) {
      dibujarCorazon(90 + i * 12, 0);
    }
    
    display.display();
  }
  
  void pantallaCountdown() {
    display.clearDisplay();
    
    if (millis() - lastCountdown > 1000) {
      countdownNumber--;
      lastCountdown = millis();
      
      if (countdownNumber < 0) {
        countdownActive = false;
        return;
      }
    }
    
    display.setTextSize(4);
    
    if (countdownNumber > 0) {
      //Numeros de cuenta regresiva
      int xPos = (countdownNumber == 1) ? 55 : 50;
      display.setCursor(xPos, 20);
      display.print(countdownNumber);
    } else {
      // GO
      display.setTextSize(3);
      display.setCursor(30, 20);
      display.print("GO!");
    }
    
    display.display();
  }
  
  void menuInicio() {
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(15, 5);
    display.println("SPACE");
    display.setCursor(5, 25);
    display.println("SHOOTER");
    
    display.setTextSize(1);
    display.setCursor(10, 48);
    display.print("MODE: ");
    display.print(hardMode ? "HARD" : "NORMAL");
    
    display.setCursor(25, 58);
    display.print("< SELECT >");
    display.display();
    
    //Seleccionar dificultad con joystick X
    int joyX = analogRead(Config::pinJoyX);
    if (joyX < 300) hardMode = false;
    if (joyX > 700) hardMode = true;
    
    if (digitalRead(Config::pinJoySW) == LOW) {
      gameStarted = true;
      countdownActive = true;
      countdownNumber = 3;
      lastCountdown = millis();
      
      score = 0;
      lives = 3;
      gameSpeed = hardMode ? 1.5 : 1.0;  //Hard mode mas rapido desde el inicio
      gameOver = false;
      paused = false;
      playerX = 60;
      playerY = 30;
      enemySpawnDelay = hardMode ? 1000 : 1500;  //Enemigos mas frecuentes en hard
      
      iniciarBalas();
      iniciarEnemigos();
      iniciarExplosiones();
      
      delay(300);
    }
  }
  
  void pantallaPausa() {
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(20, 5);
    display.println("PAUSA");
    
    display.drawLine(0, 22, 128, 22, SSD1306_WHITE);
    
    // Opcion Continuar
    display.setTextSize(1);
    if (pauseOption == 0) {
      display.fillRect(10, 28, 108, 12, SSD1306_WHITE);
      display.setTextColor(SSD1306_BLACK);
    }
    display.setCursor(25, 30);
    display.print("> CONTINUAR <");
    display.setTextColor(SSD1306_WHITE);
    
    // Opcion Salir al menu
    if (pauseOption == 1) {
      display.fillRect(10, 45, 108, 12, SSD1306_WHITE);
      display.setTextColor(SSD1306_BLACK);
    }
    display.setCursor(20, 47);
    display.print("> MENU PRINCIPAL <");
    display.setTextColor(SSD1306_WHITE);
    
    display.display();
    
    //Navegacion
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
    
    //Seleccion
    if (digitalRead(Config::pinJoySW) == LOW) {
      if (pauseOption == 0) {
        // Continuar jugando
        paused = false;
      } else {
        //Volver al menu principal
        volverAlMenu = true;
        gameStarted = false;
      }
      delay(300);
    }
  }
  
  void pantallaGameOver() {
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(10, 5);
    display.println("GAME");
    display.setCursor(10, 25);
    display.println("OVER");
    
    display.setTextSize(1);
    display.setCursor(5, 45);
    display.print("SCORE: ");
    display.print(score);
    display.setCursor(5, 55);
    display.print("HI: ");
    display.print(highScore);
    display.display();
    
    if (millis() - gameOverTime > GAME_OVER_DELAY) {
      gameOver = false;
      gameStarted = false;
      volverAlMenu = true;
    }
  }
  
  
  // FUNCIONES
  
  void iniciar() {
    EEPROM.get(Config::EEPROM_JUEGO_2, highScore);
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
    
    if (millis() - lastFrame < frameDelay) return;
    lastFrame = millis();
    
    moverJugador();
    dispararAutomatico();
    moverBalas();
    generarEnemigos();
    moverEnemigos();
    detectarColisiones();
    actualizarExplosiones();
    dibujarPantalla();
  }
}

#endif