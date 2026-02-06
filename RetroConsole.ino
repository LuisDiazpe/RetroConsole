#include "Config.h"
#include "MenuPrincipal.h"

// Incluir solo los juegos activados en Config.h
#ifdef JUEGO_DODGE_BOX
  #include "DodgeBox.h"
#endif

#ifdef JUEGO_SPACE_SHOOTER
  #include "SpaceShooter.h"
#endif

#ifdef JUEGO_SNAKE
  #include "Snake.h"
#endif

// Estado actual de la consola
enum EstadoConsola {
  ESTADO_BOOT,
  ESTADO_MENU,
  ESTADO_JUGANDO
};

EstadoConsola estadoActual = ESTADO_BOOT;
int juegoSeleccionado = -1;

void setup() {
  // Iniciar hardware
  pinMode(Config::pinJoySW, INPUT_PULLUP);
  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextColor(SSD1306_WHITE);
  display.clearDisplay();
  
  randomSeed(analogRead(A1));
  
  //Animacion de arranque
  mostrarBootScreen();
  
  // Inicializar menu
  MenuPrincipal::iniciar();
  estadoActual = ESTADO_MENU;
}

void loop() {
  switch (estadoActual) {
    case ESTADO_MENU:
      MenuPrincipal::actualizar();
      
      // Si se selecciono un juego
      if (MenuPrincipal::juegoSeleccionado >= 0) {
        juegoSeleccionado = MenuPrincipal::juegoSeleccionado;
        iniciarJuego(juegoSeleccionado);
        estadoActual = ESTADO_JUGANDO;
      }
      break;
      
    case ESTADO_JUGANDO:
      ejecutarJuego(juegoSeleccionado);
      
      //Verificar si se debe volver al menu
      if (debeVolverAlMenu(juegoSeleccionado)) {
        estadoActual = ESTADO_MENU;
        MenuPrincipal::iniciar();
      }
      break;
  }
}

void mostrarBootScreen() {
  display.clearDisplay();
  
  //Logo animado
  for (int i = 0; i < 3; i++) {
    display.clearDisplay();
    display.setTextSize(3);
    display.setCursor(25, 15);
    display.println("R C");
    display.setTextSize(1);
    display.setCursor(15, 45);
    display.println("LOIKO CONSOLE");
    display.display();
    delay(200);
    
    display.clearDisplay();
    display.display();
    delay(200);
  }
  
  //Barra de carga
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(30, 20);
  display.println("LOADING...");
  display.drawRect(14, 40, 100, 8, SSD1306_WHITE);
  
  for (int i = 0; i <= 100; i += 10) {
    display.fillRect(14, 40, i, 8, SSD1306_WHITE);
    display.display();
    delay(50);
  }
  
  delay(500);
}

void iniciarJuego(int idJuego) {
  //Mapear el indice del menu al juego
  int juegoReal = 0;
  
  #ifdef JUEGO_DODGE_BOX
    if (idJuego == juegoReal) { DodgeBox::iniciar(); return; }
    juegoReal++;
  #endif
  
  #ifdef JUEGO_SPACE_SHOOTER
    if (idJuego == juegoReal) { SpaceShooter::iniciar(); return; }
    juegoReal++;
  #endif
  
  #ifdef JUEGO_SNAKE
    if (idJuego == juegoReal) { Snake::iniciar(); return; }
    juegoReal++;
  #endif
}

void ejecutarJuego(int idJuego) {
  //Mapear el indice del menu al juego 
  int juegoReal = 0;
  
  #ifdef JUEGO_DODGE_BOX
    if (idJuego == juegoReal) { DodgeBox::actualizar(); return; }
    juegoReal++;
  #endif
  
  #ifdef JUEGO_SPACE_SHOOTER
    if (idJuego == juegoReal) { SpaceShooter::actualizar(); return; }
    juegoReal++;
  #endif
  
  #ifdef JUEGO_SNAKE
    if (idJuego == juegoReal) { Snake::actualizar(); return; }
    juegoReal++;
  #endif
}

bool debeVolverAlMenu(int idJuego) {
  // Mapear el indice del menu al juego 
  int juegoReal = 0;
  
  #ifdef JUEGO_DODGE_BOX
    if (idJuego == juegoReal) return DodgeBox::volverAlMenu;
    juegoReal++;
  #endif
  
  #ifdef JUEGO_SPACE_SHOOTER
    if (idJuego == juegoReal) return SpaceShooter::volverAlMenu;
    juegoReal++;
  #endif
  
  #ifdef JUEGO_SNAKE
    if (idJuego == juegoReal) return Snake::volverAlMenu;
    juegoReal++;
  #endif
  
  return false;
}