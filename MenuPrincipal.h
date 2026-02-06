/*
 *  MENU PRINCIPAL - Selector de juegos con animaciones
 */

#ifndef MENU_PRINCIPAL_H
#define MENU_PRINCIPAL_H

#include "Config.h"

// Iconos de juegos (16x16)
const unsigned char iconoDodgeBox[] PROGMEM = {
  0b00000000, 0b00000000,
  0b00111111, 0b11111100,
  0b01111111, 0b11111110,
  0b01100000, 0b00000110,
  0b01101100, 0b00110110,
  0b01101100, 0b00110110,
  0b01100000, 0b00000110,
  0b01100000, 0b00000110,
  0b01100000, 0b00000110,
  0b01100000, 0b00000110,
  0b01101111, 0b11110110,
  0b01101111, 0b11110110,
  0b01100000, 0b00000110,
  0b01111111, 0b11111110,
  0b00111111, 0b11111100,
  0b00000000, 0b00000000
};

const unsigned char iconoBloqueado[] PROGMEM = {
  0b00000000, 0b00000000,
  0b00000111, 0b11100000,
  0b00011111, 0b11111000,
  0b00111000, 0b00011100,
  0b01110000, 0b00001110,
  0b01100000, 0b00000110,
  0b11111111, 0b11111111,
  0b11111111, 0b11111111,
  0b11001111, 0b11110011,
  0b11001111, 0b11110011,
  0b11001111, 0b11110011,
  0b11001100, 0b00110011,
  0b11001111, 0b11110011,
  0b11111111, 0b11111111,
  0b11111111, 0b11111111,
  0b00000000, 0b00000000
};

const unsigned char iconoSpaceShooter[] PROGMEM = {
  0b00000001, 0b10000000,
  0b00000011, 0b11000000,
  0b00000111, 0b11100000,
  0b00001111, 0b11110000,
  0b00011111, 0b11111000,
  0b00111111, 0b11111100,
  0b01111111, 0b11111110,
  0b11111111, 0b11111111,
  0b11111111, 0b11111111,
  0b01011111, 0b11111010,
  0b00100111, 0b11100100,
  0b00010011, 0b11001000,
  0b00001001, 0b10010000,
  0b00000100, 0b01100000,
  0b00000010, 0b01000000,
  0b00000001, 0b10000000
};

const unsigned char iconoSnake[] PROGMEM = {
  0b00000000, 0b00000000,
  0b00000000, 0b11000000,
  0b00000001, 0b11100000,
  0b11000011, 0b11110000,
  0b11100111, 0b11111000,
  0b11111111, 0b11111100,
  0b01111111, 0b11111110,
  0b00111111, 0b11111111,
  0b00011111, 0b11101111,
  0b00001111, 0b11001111,
  0b00000111, 0b10001110,
  0b00000011, 0b00001100,
  0b00000001, 0b00001000,
  0b00000000, 0b00010000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000
};

namespace MenuPrincipal {
  //Estado del menu
  int juegoSeleccionado = -1;
  int cursorPos = 0;
  
  //Contar juegos activos
  int totalJuegos = 0
    #ifdef JUEGO_DODGE_BOX
      + 1
    #endif
    #ifdef JUEGO_SPACE_SHOOTER
      + 1
    #endif
    #ifdef JUEGO_SNAKE
      + 1
    #endif
  ;
  
  unsigned long lastAnimTime = 0;
  int animFrame = 0;
  
  //Lista de juegos disponibles (solo los activos)
  InfoJuego juegos[] = {
    #ifdef JUEGO_DODGE_BOX
      {"DODGE BOX", "Esquiva!", iconoDodgeBox},
    #endif
    #ifdef JUEGO_SPACE_SHOOTER
      {"SPACE SHOOTER", "Dispara!", iconoSpaceShooter},
    #endif
    #ifdef JUEGO_SNAKE
      {"SNAKE", "Come frutas!", iconoSnake},
    #endif
  };
  
  void iniciar() {
    juegoSeleccionado = -1;
    cursorPos = 0;
    animFrame = 0;
  }
  
  void dibujarJuego(int index, int y, bool seleccionado) {
    if (index >= totalJuegos) return;
    
    // Marco de seleccion
    if (seleccionado) {
      display.drawRect(2, y - 2, 124, 20, SSD1306_WHITE);
      display.drawRect(3, y - 1, 122, 18, SSD1306_WHITE);
    }
    
    //Icono del juego
    display.drawBitmap(8, y, juegos[index].icono, 16, 16, SSD1306_WHITE);
    
    //Nombre del juego
    display.setTextSize(1);
    display.setCursor(30, y + 1);
    display.print(juegos[index].nombre);
    
    //Descripcion
    display.setTextSize(1);
    display.setCursor(30, y + 10);
    display.print(juegos[index].descripcion);
  }
  
  void dibujarAnimacionPreview() {
    //Animacion simple de preview en la parte superior
    int x = (animFrame % 128);
    display.fillRect(x, 0, 3, 3, SSD1306_WHITE);
    display.fillRect((x + 10) % 128, 2, 2, 2, SSD1306_WHITE);
  }
  
  void actualizar() {
    //Leer joystick
    int joyY = analogRead(Config::pinJoyY);
    bool botonPresionado = digitalRead(Config::pinJoySW) == LOW;
    
    //Navegacion
    static unsigned long lastInput = 0;
    if (millis() - lastInput > 200) {
      if (joyY < 300 && cursorPos > 0) {
        cursorPos--;
        lastInput = millis();
      } else if (joyY > 700 && cursorPos < totalJuegos - 1) {
        cursorPos++;
        lastInput = millis();
      }
    }
    
    //Seleccion
    if (botonPresionado) {
      juegoSeleccionado = cursorPos;
      delay(300);
      return;
    }
    
    //Animacion
    if (millis() - lastAnimTime > 50) {
      animFrame++;
      lastAnimTime = millis();
    }
    
    //Dibujar menu
    display.clearDisplay();
    
    //Titulo
    display.setTextSize(1);
    display.setCursor(20, 2);
    display.print("SELECCIONA JUEGO");
    display.drawLine(0, 12, 128, 12, SSD1306_WHITE);
    
    //Lista de juegos (scroll si hay muchos)
    int startIndex = max(0, cursorPos - 1);
    int endIndex = min(totalJuegos, startIndex + 3);
    
    int yPos = 18;
    for (int i = startIndex; i < endIndex; i++) {
      dibujarJuego(i, yPos, i == cursorPos);
      yPos += 22;
    }
    
    //Indicador de mas juegos
    if (cursorPos < totalJuegos - 1) {
      display.setCursor(60, 60);
      display.print("v");
    }
    if (cursorPos > 0) {
      display.setCursor(60, 14);
      display.print("^");
    }
    
    display.display();
  }
}

#endif