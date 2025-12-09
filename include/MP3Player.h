#ifndef MP3_PLAYER_H
#define MP3_PLAYER_H

#include <SoftwareSerial.h>
#include "config.h"

/**
 * @class MP3Player
 * @brief Controlador para reproductor MP3 serial
 */
class MP3Player {
private:
  SoftwareSerial serialPort;
  bool initialized;

  /**
   * @brief Envía un comando raw al reproductor MP3
   */
  void sendCommand(const byte* command, size_t length) {
    if (!initialized) return;
    for (size_t i = 0; i < length; i++) {
      serialPort.write(command[i]);
    }
  }

public:
  MP3Player() : serialPort(MP3_RX_PIN, MP3_TX_PIN), initialized(false) {}

  /**
   * @brief Inicializa el reproductor MP3
   */
  void begin() {
    serialPort.begin(MP3_BAUD);
    initialized = true;
    delay(1000);  // Esperar inicialización del módulo
    
    // Configurar volumen al máximo
    setVolume(30);
    delay(200);
    
    // Habilitar reproducción cíclica
    enableLoop();
    delay(200);
    
    Serial.println("[MP3] Reproductor MP3 inicializado.");
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
