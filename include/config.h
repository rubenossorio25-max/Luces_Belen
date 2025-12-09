#ifndef CONFIG_H
#define CONFIG_H

// ============================================================================
// CONFIGURACIÓN DE PINES - ESP32 DOIT DevKit v1
// Pines PWM recomendados en ESP32 para control de LEDs
// ============================================================================
const int PIN_ROJO = 15;    // GPIO 15 - PWM compatible
const int PIN_VERDE = 14;   // GPIO 14 - PWM compatible
const int PIN_AZUL = 13;    // GPIO 13 - PWM compatible
const int PIN_BLANCO = 12;  // GPIO 12 - PWM compatible

// Pines para Comunicación MP3 (Serial2 en ESP32)
// Usando UART1 del ESP32: RX=GPIO16, TX=GPIO17
// Nota: Serial2 en ESP32 por defecto usa estos pines
// const int MP3_RX_PIN = 16;  // No necesario, Serial2 maneja automáticamente
// const int MP3_TX_PIN = 17;  // No necesario, Serial2 maneja automáticamente

// ============================================================================
// CONFIGURACIÓN DE TIEMPOS (milisegundos)
// ============================================================================
const unsigned long DEFAULT_DURACION_AMANECER = 10000;
const unsigned long DEFAULT_DURACION_DIA = 10000;
const unsigned long DEFAULT_DURACION_ATARDECER = 10000;
const unsigned long DEFAULT_DURACION_NOCHE = 10000;
const unsigned long DEFAULT_DURACION_TRANSICION = 15000;

// ============================================================================
// CONFIGURACIÓN DE RAYO ALEATORIO
// ============================================================================
const unsigned long MIN_TIEMPO_ENTRE_RAYOS = 5000;      // 5 segundos
const unsigned long MAX_TIEMPO_ENTRE_RAYOS = 20000;     // 20 segundos
const unsigned long MIN_DURACION_DESTELLO = 10;         // ms
const unsigned long MAX_DURACION_DESTELLO = 50;         // ms
const int MAX_NUM_DESTELLOS = 4;

// ============================================================================
// CONFIGURACIÓN DEL PUNTO DE ACCESO (AP) WIFI
// ============================================================================
const char* AP_SSID = "LuminaConfig";
const char* AP_PASSWORD = "configurar123";
const byte DNS_PORT = 53;

// ============================================================================
// CONFIGURACIÓN SERIAL
// ============================================================================
const unsigned long SERIAL_BAUD = 115200;
const unsigned long MP3_BAUD = 9600;
const long INTERVALO_REPORTE = 1000;  // Reporte cada 1 segundo

// ============================================================================
// CONFIGURACIÓN EEPROM
// ============================================================================
const size_t EEPROM_SIZE = 512;

// Direcciones de EEPROM (bytes)
const int EEPROM_DURACION_AMANECER = 0;
const int EEPROM_DURACION_DIA = 4;
const int EEPROM_DURACION_ATARDECER = 8;
const int EEPROM_DURACION_NOCHE = 12;
const int EEPROM_DURACION_TRANSICION = 16;
const int EEPROM_COLOR_AMANECER = 24;
const int EEPROM_COLOR_DIA = 40;
const int EEPROM_COLOR_ATARDECER = 56;
const int EEPROM_COLOR_NOCHE = 72;

#endif // CONFIG_H
