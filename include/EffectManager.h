#ifndef EFFECT_MANAGER_H
#define EFFECT_MANAGER_H

#include "LightController.h"
#include "PhaseManager.h"
#include "Fase.h"
#include "config.h"

/**
 * @class EffectManager
 * @brief Gestiona los efectos especiales como rayos aleatorios
 */
class EffectManager {
private:
  LightController* lightController;
  PhaseManager* phaseManager;
  unsigned long lastLightningTime;
  unsigned long nextLightningWait;
  bool initialized;

public:
  EffectManager() 
      : lightController(nullptr), phaseManager(nullptr), 
        lastLightningTime(0), nextLightningWait(0), initialized(false) {}

  /**
   * @brief Inicializa el gestor de efectos
   */
  void begin(LightController* lc, PhaseManager* pm) {
    lightController = lc;
    phaseManager = pm;
    initialized = true;
    resetLightningTimer();
  }

  /**
   * @brief Reinicia el temporizador de rayos
   */
  void resetLightningTimer() {
    lastLightningTime = millis();
    nextLightningWait = random(MIN_TIEMPO_ENTRE_RAYOS, MAX_TIEMPO_ENTRE_RAYOS);
  }

  /**
   * @brief Actualiza y ejecuta los efectos
   */
  void update() {
    if (!initialized || !lightController || !phaseManager) return;

    // Solo ejecutar rayos durante la noche
    if (phaseManager->getCurrentPhase() != Fase::NOCHE) {
      return;
    }

    unsigned long now = millis();

    // Comprobar si es hora de un nuevo rayo
    if (now - lastLightningTime >= nextLightningWait) {
      triggerLightning();
      resetLightningTimer();
    }
  }

  /**
   * @brief Activa un efecto de rayo
   */
  void triggerLightning() {
    if (!lightController) return;

    // Determinar número de destellos aleatorio
    int numFlashes = random(1, MAX_NUM_DESTELLOS + 1);
    
    Serial.print("[EFFECT] ⚡ Rayo detectado con ");
    Serial.print(numFlashes);
    Serial.println(" destellos.");

    Color nightColor = phaseManager->getCurrentColor();

    // Ejecutar destellos
    for (int i = 0; i < numFlashes; i++) {
      // Destello blanco
      lightController->setColor(Color(0, 0, 0, 255));
      delay(random(MIN_DURACION_DESTELLO, MAX_DURACION_DESTELLO));

      // Volver al color de noche
      lightController->setColor(nightColor);
      delay(random(20, 100));
    }

    Serial.print("[EFFECT] Próximo rayo en: ");
    Serial.print(nextLightningWait / 1000);
    Serial.println(" segundos.");
  }

  /**
   * @brief Obtiene el tiempo hasta el próximo rayo
   */
  unsigned long getTimeToNextLightning() const {
    if (phaseManager->getCurrentPhase() != Fase::NOCHE) {
      return 0;
    }
    
    unsigned long elapsed = millis() - lastLightningTime;
    if (elapsed >= nextLightningWait) {
      return 0;  // Ya debería haber ocurrido
    }
    return nextLightningWait - elapsed;
  }

  /**
   * @brief Fuerza un rayo inmediato (útil para pruebas)
   */
  void forceLightning() {
    triggerLightning();
    resetLightningTimer();
  }
};

#endif // EFFECT_MANAGER_H
