# 🎮 CONSOLA RETRO - Arduino OLED

Una consola de videojuegos retro completa para Arduino Uno con pantalla OLED y joystick analógico. Incluye 3 juegos clásicos con gráficos pixel art, sistema de puntuación, múltiples dificultades y menú navegable.

![Arduino](https://img.shields.io/badge/Arduino-00979D?style=for-the-badge&logo=Arduino&logoColor=white)
![C++](https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)

## 📹 Demo en Video

[![Ver demo en YouTube](https://img.shields.io/badge/YouTube-FF0000?style=for-the-badge&logo=youtube&logoColor=white)](https://youtube.com/shorts/D7rgpghpT8s)

**Click en el badge para ver la consola en acción** ▶️

---

## 📋 Tabla de Contenidos

- [Características](#-características)
- [Hardware Necesario](#-hardware-necesario)
- [Esquema de Conexión](#-esquema-de-conexión)
- [Instalación](#-instalación)
- [Juegos Incluidos](#-juegos-incluidos)
- [Controles](#-controles)
- [Gestión de Memoria](#-gestión-de-memoria)
- [Estructura del Proyecto](#-estructura-del-proyecto)
- [Personalización](#-personalización)
- [Troubleshooting](#-troubleshooting)
- [Licencia](#-licencia)

---

## ✨ Características

- 🎮 **3 Juegos Retro Completos**: Dodge Box, Space Shooter y Snake
- 📊 **Sistema de Puntuación**: High scores guardados en EEPROM
- 🎯 **Múltiples Dificultades**: Modo Normal y Hard en cada juego
- 🎨 **Gráficos Pixel Art**: Sprites personalizados y animaciones fluidas
- 🎵 **Efectos Visuales**: Explosiones, power-ups y animaciones
- 🔄 **Menú Navegable**: Selección visual de juegos
- ⏸️ **Sistema de Pausa**: Con opciones de continuar o volver al menú
- 💾 **Persistencia**: Los high scores se guardan al apagar
- 🎬 **Animaciones de Inicio**: Countdown 3-2-1-GO en cada juego

---

## 🛠️ Hardware Necesario

### Componentes Requeridos

| Componente | Especificación | Cantidad |
|------------|---------------|----------|
| **Microcontrolador** | Arduino Uno (o compatible) | 1 |
| **Pantalla OLED** | SSD1306 128x64 (I2C) | 1 |
| **Joystick Analógico** | 2 ejes + botón | 1 |
| **Cables Jumper** | Macho-Macho | 8-10 |
| **Protoboard** | Opcional (para prototipado) | 1 |

### Especificaciones Técnicas

- **Voltaje de Operación**: 5V
- **Consumo**: ~100mA
- **Interfaz Pantalla**: I2C (SDA, SCL)
- **Interfaz Joystick**: Analógico + Digital

---

## 🔌 Esquema de Conexión

### Pantalla OLED SSD1306 (I2C)

```
OLED        Arduino Uno
━━━━━━━━    ━━━━━━━━━━━
VCC    ───► 5V
GND    ───► GND
SDA    ───► A4 (SDA)
SCL    ───► A5 (SCL)
```

### Joystick Analógico

```
Joystick    Arduino Uno
━━━━━━━━    ━━━━━━━━━━━
VRx    ───► A0 (Eje X)
VRy    ───► A1 (Eje Y)
SW     ───► D2 (Botón)
+5V    ───► 5V
GND    ───► GND
```

### Diagrama Visual

```
    ┌─────────────────┐
    │   ARDUINO UNO   │
    │                 │
5V ─┤ 5V         VCC ├─ OLED
GND─┤ GND        GND ├─ OLED
    │ A4  ────── SDA ├─ OLED
    │ A5  ────── SCL ├─ OLED
    │                 │
    │ A0  ────── VRx ├─ Joystick
    │ A1  ────── VRy ├─ Joystick
    │ D2  ───────SW  ├─ Joystick
    └─────────────────┘
```

---

## 📥 Instalación

### 1. Instalación de Librerías

Abre el **Arduino IDE** y ve a `Sketch > Include Library > Manage Libraries`

Instala las siguientes librerías:

- **Adafruit GFX Library** (v1.11.0 o superior)
- **Adafruit SSD1306** (v2.5.0 o superior)

### 2. Descarga del Proyecto

Clona o descarga este repositorio:

```bash
git clone https://github.com/tu-usuario/consola-retro-arduino.git
```

O descarga el ZIP desde GitHub.

### 3. Estructura de Archivos

Asegúrate de tener esta estructura:

```
RetroConsole/
├── RetroConsole.ino      # Archivo principal
├── Config.h              # Configuración y control de juegos
├── MenuPrincipal.h       # Sistema de menú
├── DodgeBox.h            # Juego 1: Esquiva obstáculos
├── SpaceShooter.h        # Juego 2: Dispara naves
└── Snake.h               # Juego 3: Come frutas
```

### 4. Configuración de Memoria (IMPORTANTE)

⚠️ **Arduino Uno tiene solo 2KB de RAM**. Por defecto, solo 2 juegos estarán activos.

**Edita `Config.h`** para elegir qué juegos incluir:

```cpp
// ACTIVAR/DESACTIVAR JUEGOS (máximo 2 a la vez)
#define JUEGO_DODGE_BOX        // ✅ Activado
#define JUEGO_SPACE_SHOOTER    // ✅ Activado
//#define JUEGO_SNAKE          // ❌ Desactivado
```

**Recomendación**: Mantén solo 2 juegos activos para evitar problemas de memoria.

### 5. Compilar y Subir

1. Conecta tu Arduino Uno al PC
2. Abre `RetroConsole.ino` en Arduino IDE
3. Selecciona: `Tools > Board > Arduino Uno`
4. Selecciona el puerto correcto en `Tools > Port`
5. Click en **Upload** ⬆️

---

## 🎮 Juegos Incluidos

### 1️⃣ DODGE BOX - Esquiva Obstáculos

**Descripción**: Corre y esquiva cajas que caen del cielo mientras recolectas power-ups.

**Características**:
- ✅ Movimiento izquierda/derecha con animación de correr
- ✅ 3 vidas (corazones)
- ✅ Power-ups: Escudo, Vida Extra, Slow Motion
- ✅ Dificultad progresiva
- ✅ Modos Normal y Hard

**Puntuación**: +1 punto cada frame sobrevivido

---

### 2️⃣ SPACE SHOOTER - Dispara Naves

**Descripción**: Controla una nave espacial y destruye enemigos alienígenas.

**Características**:
- ✅ Movimiento 360° (arriba, abajo, izquierda, derecha)
- ✅ Disparo automático continuo
- ✅ 2 tipos de enemigos diferentes
- ✅ Explosiones animadas
- ✅ Sistema de vidas
- ✅ Modos Normal y Hard

**Puntuación**: +10 puntos por enemigo destruido

---

### 3️⃣ SNAKE - Come Frutas

**Descripción**: El clásico juego de la serpiente que come frutas y crece.

**Características**:
- ✅ 5 tipos de frutas con diferentes valores
  - 🍎 Manzana: 5 pts
  - 🍒 Cereza: 10 pts
  - 🍓 Fresa: 15 pts
  - 🍇 Uva: 20 pts
  - 🍊 Naranja: 25 pts
- ✅ Crecimiento progresivo
- ✅ Velocidad incremental
- ✅ Modos Normal y Hard

**Puntuación**: Variable según el tipo de fruta

---

## 🕹️ Controles

### Menú Principal

| Control | Acción |
|---------|--------|
| Joystick ⬆️⬇️ | Navegar entre juegos |
| Botón | Seleccionar juego |

### En el Juego

#### Dodge Box
| Control | Acción |
|---------|--------|
| Joystick ⬅️➡️ | Mover jugador |
| Botón | Pausar |

#### Space Shooter
| Control | Acción |
|---------|--------|
| Joystick ⬆️⬇️⬅️➡️ | Mover nave (360°) |
| Automático | Disparo |
| Botón | Pausar |

#### Snake
| Control | Acción |
|---------|--------|
| Joystick ⬆️⬇️⬅️➡️ | Cambiar dirección |
| Botón | Pausar |

### Menú de Pausa

| Control | Acción |
|---------|--------|
| Joystick ⬆️⬇️ | Seleccionar opción |
| Botón | Confirmar |

Opciones:
- **Continuar**: Volver al juego
- **Menú Principal**: Salir al menú

---

## 💾 Gestión de Memoria

### Problema de RAM en Arduino Uno

Arduino Uno tiene **solo 2048 bytes (2KB) de RAM**. Con los 3 juegos cargados simultáneamente, la memoria se agota y el sistema se congela.

### Solución: Compilación Condicional

El proyecto incluye un sistema que te permite **activar/desactivar juegos** antes de compilar.

### Cómo Cambiar los Juegos Activos

**1. Abre `Config.h`**

**2. Modifica las directivas `#define`:**

```cpp
// Solo 2 juegos a la vez (RECOMENDADO)
#define JUEGO_DODGE_BOX        // ✅
#define JUEGO_SPACE_SHOOTER    // ✅
//#define JUEGO_SNAKE          // ❌ Comentado = Desactivado
```

**3. Guarda y vuelve a compilar**

### Configuraciones Recomendadas

✅ **Funcionan Bien** (2 juegos):
```cpp
// Opción 1
#define JUEGO_DODGE_BOX
#define JUEGO_SPACE_SHOOTER
//#define JUEGO_SNAKE

// Opción 2
#define JUEGO_DODGE_BOX
//#define JUEGO_SPACE_SHOOTER
#define JUEGO_SNAKE

// Opción 3
//#define JUEGO_DODGE_BOX
#define JUEGO_SPACE_SHOOTER
#define JUEGO_SNAKE
```

❌ **Problemático** (3 juegos):
```cpp
// Se queda sin RAM y se congela
#define JUEGO_DODGE_BOX
#define JUEGO_SPACE_SHOOTER
#define JUEGO_SNAKE
```
```cpp
// Opción 1
#define JUEGO_DODGE_BOX
#define JUEGO_SPACE_SHOOTER
//#define JUEGO_SNAKE

// Opción 2
#define JUEGO_DODGE_BOX
//#define JUEGO_SPACE_SHOOTER
#define JUEGO_SNAKE

// Opción 3
//#define JUEGO_DODGE_BOX
#define JUEGO_SPACE_SHOOTER
#define JUEGO_SNAKE
```

❌ **Problemático** (3 juegos):
```cpp
// Se queda sin RAM y se congela
#define JUEGO_DODGE_BOX
#define JUEGO_SPACE_SHOOTER
#define JUEGO_SNAKE
```

### Uso de Memoria Aproximado

| Componente | RAM Usada |
|------------|-----------|
| Sistema + Display | ~800 bytes |
| Dodge Box | ~400 bytes |
| Space Shooter | ~500 bytes |
| Snake | ~250 bytes |
| **TOTAL (3 juegos)** | **~1950 bytes** |
| **RAM disponible** | **2048 bytes** |

### Alternativa: Arduino Mega

Si quieres **todos los juegos simultáneamente sin limitaciones**, considera usar:

- **Arduino Mega 2560**: 8KB de RAM (4x más)
- El mismo código funciona sin modificaciones
- Todos los juegos cargados a la vez

---

## 📁 Estructura del Proyecto

```
RetroConsole/
│
├── RetroConsole.ino          # Programa principal
│   ├─ setup()               # Inicialización
│   ├─ loop()                # Bucle principal
│   ├─ mostrarBootScreen()   # Animación de inicio
│   └─ Gestión de estados
│
├── Config.h                  # Configuración global
│   ├─ Activación de juegos  # #define switches
│   ├─ Pines de hardware     # A0, A1, D2
│   └─ Direcciones EEPROM    # Almacenamiento
│
├── MenuPrincipal.h          # Sistema de menú
│   ├─ Lista de juegos       # Array dinámico
│   ├─ Iconos 16x16         # Sprites
│   ├─ Navegación           # Joystick
│   └─ Selección            # Botón
│
├── DodgeBox.h               # Juego 1
│   ├─ Sprites del jugador  # 8x8 animados
│   ├─ Sistema de power-ups # Shield, vida, slow-mo
│   ├─ Obstáculos           # Generación aleatoria
│   └─ Colisiones           # Detección
│
├── SpaceShooter.h           # Juego 2
│   ├─ Movimiento 360°      # 4 direcciones
│   ├─ Disparo automático   # Sistema de balas
│   ├─ Enemigos             # 2 tipos
│   └─ Explosiones          # Animaciones
│
└── Snake.h                  # Juego 3
    ├─ Lógica de serpiente  # Arrays optimizados
    ├─ 5 tipos de frutas    # Puntos variables
    ├─ Sistema de grid      # 32x13 celdas
    └─ Colisiones           # Paredes y cuerpo
```

---

## 🎨 Personalización

### Agregar un Nuevo Juego

**1. Crea el archivo del juego** (ej: `MiJuego.h`)

```cpp
#ifndef MI_JUEGO_H
#define MI_JUEGO_H

#include "Config.h"

namespace MiJuego {
  bool volverAlMenu = false;
  
  void iniciar() {
    // Inicialización
  }
  
  void actualizar() {
    // Lógica del juego
  }
}

#endif
```

**2. Agrega el #define en `Config.h`**

```cpp
#define JUEGO_MI_JUEGO
```

**3. Incluye el juego en `RetroConsole.ino`**

```cpp
#ifdef JUEGO_MI_JUEGO
  #include "MiJuego.h"
#endif
```

**4. Agrega el icono en `MenuPrincipal.h`**

```cpp
const unsigned char iconoMiJuego[] PROGMEM = {
  // ... sprite 16x16
};
```

**5. Agrega al array de juegos**

```cpp
#ifdef JUEGO_MI_JUEGO
  {"MI JUEGO", "Descripción!", iconoMiJuego},
#endif
```

**6. Agrega los casos en las funciones**

```cpp
#ifdef JUEGO_MI_JUEGO
case X:
  MiJuego::iniciar();
  break;
#endif
```

### Modificar Dificultad

Edita las constantes en cada archivo de juego:

```cpp
// DodgeBox.h
const int frameDelay = 40;  // Menor = más rápido
float speedGame = 1.5;      // Velocidad inicial

// SpaceShooter.h
const int SHOOT_DELAY = 400;  // ms entre disparos
int enemySpawnDelay = 1500;   // ms entre enemigos

// Snake.h
int gameSpeed = 150;  // ms entre movimientos
```

### Cambiar Colores (Pantallas OLED de Color)

Si tienes una pantalla OLED de color, puedes cambiar:

```cpp
// De:
SSD1306_WHITE

// A:
SSD1306_BLUE
SSD1306_RED
SSD1306_GREEN
// etc.
```

---

## 🐛 Troubleshooting

### Problema: La pantalla no se enciende

**Solución**:
1. Verifica las conexiones I2C (SDA → A4, SCL → A5)
2. Verifica el voltaje (VCC → 5V, GND → GND)
3. Comprueba la dirección I2C:
   ```cpp
   display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // Prueba 0x3D si no funciona
   ```

### Problema: El joystick no responde

**Solución**:
1. Verifica las conexiones (VRx → A0, VRy → A1, SW → D2)
2. Prueba los valores en Serial Monitor:
   ```cpp
   Serial.println(analogRead(A0));  // Debería variar 0-1023
   ```
3. Ajusta los umbrales si es necesario:
   ```cpp
   if (joyX < 300)  // Prueba con 400 o 200
   ```

### Problema: Se congela al compilar

**Solución**:
Demasiados juegos activos. Edita `Config.h` y desactiva un juego:

```cpp
//#define JUEGO_SNAKE  // Comenta uno
```

### Problema: Los high scores no se guardan

**Solución**:
Verifica que las direcciones EEPROM no se superpongan en `Config.h`:

```cpp
const int EEPROM_DODGE_BOX = 0;
const int EEPROM_JUEGO_2 = 10;   // Separados por 10
const int EEPROM_JUEGO_3 = 20;   // Separados por 10
```

### Problema: Parpadeo en la pantalla

**Solución**:
Asegúrate de llamar `display.clearDisplay()` una sola vez por frame:

```cpp
display.clearDisplay();
// ... dibujar todo
display.display();  // Solo una vez al final
```

---

## 📜 Licencia

Este proyecto es de código abierto y está disponible bajo la licencia MIT.

---

## 👨‍💻 Créditos

Proyecto universitario creado con ❤️ para aprender Arduino y desarrollo de videojuegos embebidos.

---

## 🙏 Agradecimientos

- Adafruit por las librerías GFX y SSD1306
- Comunidad Arduino por la documentación y soporte

---
