#ifndef CONFIG_H
#define CONFIG_H

// ============================================================================
// CONFIGURACIÓN DE PINES - NodeMCU ESP8266
// ============================================================================
const int PIN_ROJO = 16;    // D0 - GPIO 16
const int PIN_VERDE = 13;   // D7 - GPIO 13
const int PIN_AZUL = 5;     // D1 - GPIO 5
const int PIN_BLANCO = 4;   // D2 - GPIO 4

// Pines para Comunicación MP3
const int MP3_RX_PIN = 12;  // D6 - Conecta al TX del MP3
const int MP3_TX_PIN = 14;  // D5 - Conecta al RX del MP3

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
