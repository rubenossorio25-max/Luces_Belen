#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <EEPROM.h>
#include "PhaseManager.h"
#include "config.h"

/**
 * @class ConfigManager
 * @brief Gestiona la persistencia de configuración en EEPROM
 */
class ConfigManager {
private:
  PhaseManager* phaseManager;
  bool initialized;
  bool configLoaded;

public:
  ConfigManager() : phaseManager(nullptr), initialized(false), configLoaded(false) {}

  /**
   * @brief Inicializa el gestor de configuración
   */
  void begin(PhaseManager* pm) {
    phaseManager = pm;
    EEPROM.begin(EEPROM_SIZE);
    initialized = true;
  }

  /**
   * @brief Carga la configuración desde EEPROM
   */
  void loadConfiguration() {
    if (!initialized || !phaseManager) return;

    // Verificar si EEPROM contiene datos válidos
    uint8_t checkByte = EEPROM.read(0);
    if (checkByte == 0xFF) {
      Serial.println("[CONFIG] EEPROM vacía. Usando valores predeterminados.");
      saveConfiguration();  // Guardar valores por defecto
      configLoaded = false;
      return;
    }

    // Cargar duraciones
    unsigned long duration;
    
    EEPROM.get(EEPROM_DURACION_AMANECER, duration);
    if (duration >= 1000) phaseManager->setPhaseDuration(Fase::AMANECER, duration);
    
    EEPROM.get(EEPROM_DURACION_DIA, duration);
    if (duration >= 1000) phaseManager->setPhaseDuration(Fase::DIA, duration);
    
    EEPROM.get(EEPROM_DURACION_ATARDECER, duration);
    if (duration >= 1000) phaseManager->setPhaseDuration(Fase::ATARDECER, duration);
    
    EEPROM.get(EEPROM_DURACION_NOCHE, duration);
    if (duration >= 1000) phaseManager->setPhaseDuration(Fase::NOCHE, duration);
    
    EEPROM.get(EEPROM_DURACION_TRANSICION, duration);
    if (duration >= 100) phaseManager->setTransitionDuration(duration);

    // Cargar colores
    Color color;
    
    EEPROM.get(EEPROM_COLOR_AMANECER, color);
    phaseManager->setPhaseColor(Fase::AMANECER, color);
    
    EEPROM.get(EEPROM_COLOR_DIA, color);
    phaseManager->setPhaseColor(Fase::DIA, color);
    
    EEPROM.get(EEPROM_COLOR_ATARDECER, color);
    phaseManager->setPhaseColor(Fase::ATARDECER, color);
    
    EEPROM.get(EEPROM_COLOR_NOCHE, color);
    phaseManager->setPhaseColor(Fase::NOCHE, color);

    configLoaded = true;
    Serial.println("[CONFIG] Configuración cargada desde EEPROM.");
  }

  /**
   * @brief Guarda la configuración en EEPROM
   */
  void saveConfiguration() {
    if (!initialized || !phaseManager) return;

    // Guardar duraciones
    EEPROM.put(EEPROM_DURACION_AMANECER, phaseManager->getPhaseDuration(Fase::AMANECER));
    EEPROM.put(EEPROM_DURACION_DIA, phaseManager->getPhaseDuration(Fase::DIA));
    EEPROM.put(EEPROM_DURACION_ATARDECER, phaseManager->getPhaseDuration(Fase::ATARDECER));
    EEPROM.put(EEPROM_DURACION_NOCHE, phaseManager->getPhaseDuration(Fase::NOCHE));
    EEPROM.put(EEPROM_DURACION_TRANSICION, phaseManager->getTransitionDuration());

    // Guardar colores
    EEPROM.put(EEPROM_COLOR_AMANECER, phaseManager->getPhaseColor(Fase::AMANECER));
    EEPROM.put(EEPROM_COLOR_DIA, phaseManager->getPhaseColor(Fase::DIA));
    EEPROM.put(EEPROM_COLOR_ATARDECER, phaseManager->getPhaseColor(Fase::ATARDECER));
    EEPROM.put(EEPROM_COLOR_NOCHE, phaseManager->getPhaseColor(Fase::NOCHE));

    EEPROM.commit();
    Serial.println("[CONFIG] Configuración guardada en EEPROM.");
  }

  /**
   * @brief Verifica si la configuración fue cargada correctamente
   */
  bool isConfigLoaded() const {
    return configLoaded;
  }

  /**
   * @brief Restablece la configuración a los valores por defecto
   */
  void resetToDefaults() {
    if (!phaseManager) return;

    phaseManager->setPhaseDuration(Fase::AMANECER, DEFAULT_DURACION_AMANECER);
    phaseManager->setPhaseDuration(Fase::DIA, DEFAULT_DURACION_DIA);
    phaseManager->setPhaseDuration(Fase::ATARDECER, DEFAULT_DURACION_ATARDECER);
    phaseManager->setPhaseDuration(Fase::NOCHE, DEFAULT_DURACION_NOCHE);
    phaseManager->setTransitionDuration(DEFAULT_DURACION_TRANSICION);

    phaseManager->setPhaseColor(Fase::AMANECER, Color(255, 150, 0, 0));
    phaseManager->setPhaseColor(Fase::DIA, Color(0, 0, 0, 255));
    phaseManager->setPhaseColor(Fase::ATARDECER, Color(200, 50, 20, 0));
    phaseManager->setPhaseColor(Fase::NOCHE, Color(10, 10, 50, 0));

    saveConfiguration();
    Serial.println("[CONFIG] Configuración restablecida a valores por defecto.");
  }
};

#endif // CONFIG_MANAGER_H
