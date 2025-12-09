#ifndef MP3_PLAYER_H
#define MP3_PLAYER_H

#include "config.h"

/**
 * @class MP3Player
 * @brief Controlador para reproductor MP3 serial
 * Nota: En ESP8266 usamos los pines D5(GPIO14) y D6(GPIO12) 
 * como puertos seriales con bit-banging manual o UART alternativo
 */
class MP3Player {
private:
  bool initialized;
  uint8_t txPin;
  uint8_t rxPin;

  /**
   * @brief Envía un comando raw al reproductor MP3
   * Nota: Esta es una implementación de demostración
   */
  void sendCommand(const byte* command, size_t length) {
    if (!initialized) return;
    // En una implementación real aquí se escribiría a través de comunicación serial
    // Por ahora solo se registra que se intentó enviar
    Serial.print("[MP3] Comando enviado: ");
    for (size_t i = 0; i < length; i++) {
      Serial.print(command[i], HEX);
      Serial.print(" ");
    }
    Serial.println();
  }

public:
  MP3Player() : initialized(false), txPin(14), rxPin(12) {}

  /**
   * @brief Inicializa el reproductor MP3
   * Simulamos comunicación serial escribiendo directamente el byte al puerto
   * Nota: Para una implementación real necesitarías una librería como SoftwareSerial
   * compatible con ESP8266 (como EspSoftwareSerial)
   */
  void begin() {
    // En una implementación real, aquí inicializaríamos la comunicación
    // Por ahora, asumimos que el MP3 está siempre escuchando
    initialized = true;
    delay(1000);
    
    // Configurar volumen al máximo
    setVolume(30);
    delay(200);
    
    // Habilitar reproducción cíclica
    enableLoop();
    delay(200);
    
    Serial.println("[MP3] Reproductor MP3 inicializado (Modo simulado).");
  }

  /**
   * @brief Reproduce una pista específica
   */
  void playTrack(int trackNumber) {
    if (!initialized || trackNumber < 1) return;

    // Comando: 0x7E 0xFF 0x06 0x03 0x00 0x00 [TRACK] 0xEF
    byte playTrack[] = {0x7E, 0xFF, 0x06, 0x03, 0x00, 0x00, (byte)trackNumber, 0xEF};
    sendCommand(playTrack, sizeof(playTrack));
    
    Serial.print("[MP3] Reproduciendo pista: ");
    Serial.println(trackNumber);
    delay(50);
  }

  /**
   * @brief Establece el volumen (0-30)
   */
  void setVolume(uint8_t volume) {
    if (!initialized) return;
    if (volume > 30) volume = 30;

    // Comando: 0x7E 0xFF 0x06 0x06 0x00 0x00 [VOL] 0xEF
    byte setVolume[] = {0x7E, 0xFF, 0x06, 0x06, 0x00, 0x00, volume, 0xEF};
    sendCommand(setVolume, sizeof(setVolume));
    
    Serial.print("[MP3] Volumen establecido a: ");
    Serial.println(volume);
  }

  /**
   * @brief Habilita la reproducción cíclica
   */
  void enableLoop() {
    if (!initialized) return;

    // Comando: 0x7E 0xFF 0x06 0x11 0x00 0x00 0x01 0xEF
    byte setLoop[] = {0x7E, 0xFF, 0x06, 0x11, 0x00, 0x00, 0x01, 0xEF};
    sendCommand(setLoop, sizeof(setLoop));
    
    Serial.println("[MP3] Modo cíclico habilitado.");
  }

  /**
   * @brief Pausa la reproducción
   */
  void pause() {
    if (!initialized) return;

    // Comando: 0x7E 0xFF 0x06 0x0E 0x00 0x00 0x00 0xEF
    byte pauseCmd[] = {0x7E, 0xFF, 0x06, 0x0E, 0x00, 0x00, 0x00, 0xEF};
    sendCommand(pauseCmd, sizeof(pauseCmd));
    
    Serial.println("[MP3] Reproducción pausada.");
  }

  /**
   * @brief Reanuda la reproducción
   */
  void resume() {
    if (!initialized) return;

    // Comando: 0x7E 0xFF 0x06 0x0D 0x00 0x00 0x00 0xEF
    byte resumeCmd[] = {0x7E, 0xFF, 0x06, 0x0D, 0x00, 0x00, 0x00, 0xEF};
    sendCommand(resumeCmd, sizeof(resumeCmd));
    
    Serial.println("[MP3] Reproducción reanudada.");
  }

  /**
   * @brief Detiene la reproducción
   */
  void stop() {
    if (!initialized) return;

    // Comando: 0x7E 0xFF 0x06 0x16 0x00 0x00 0x00 0xEF
    byte stopCmd[] = {0x7E, 0xFF, 0x06, 0x16, 0x00, 0x00, 0x00, 0xEF};
    sendCommand(stopCmd, sizeof(stopCmd));
    
    Serial.println("[MP3] Reproducción detenida.");
  }
};

#endif // MP3_PLAYER_H
