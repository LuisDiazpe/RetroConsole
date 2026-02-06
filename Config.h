/*
 * 
 *  CONFIG.H - Configuracion global de la consola
 * 
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <Wire.h>
#include <EEPROM.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


// ACTIVAR O DESACTIVAR JUEGOS
// Comentar con (#define) los juegos que no quieres incluir
// Solo puedes tener ACTIVOS maximo 2 juegos a la vez en Arduino Uno

//#define JUEGO_DODGE_BOX        // Juego 1: Dodge Box
#define JUEGO_SPACE_SHOOTER    // Juego 2: Space Shooter
#define JUEGO_SNAKE            // Juego 3: Snake


// Configuracion de pantalla
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Objeto display global
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Configuracion de hardware
namespace Config {
  const int pinJoyX = A0;
  const int pinJoyY = A1;
  const int pinJoySW = 2;
  
  // Direcciones EEPROM para diferentes juegos
  const int EEPROM_DODGE_BOX = 0;
  const int EEPROM_JUEGO_2 = 10;
  const int EEPROM_JUEGO_3 = 20;
  //...
}

// Estructura para informacion de juegos
struct InfoJuego {
  const char* nombre;
  const char* descripcion;
  const unsigned char* icono;
};

#endif