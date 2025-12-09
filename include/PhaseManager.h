#ifndef PHASE_MANAGER_H
#define PHASE_MANAGER_H

#include "Fase.h"
#include "Color.h"
#include "config.h"

/**
 * @class PhaseManager
 * @brief Gestiona las fases del día y sus colores asociados
 */
class PhaseManager {
private:
  Fase currentPhase;
  unsigned long phaseDuration[4];  // Duraciones para cada fase
  Color phaseColor[4];             // Colores para cada fase
  unsigned long phaseStartTime;
  unsigned long transitionStartTime;
  bool inTransition;
  Fase nextPhase;
  Color transitionStartColor;
  Color transitionEndColor;
  unsigned long transitionDuration;

public:
  PhaseManager()
      : currentPhase(Fase::AMANECER),
        phaseStartTime(0),
        transitionStartTime(0),
        inTransition(false),
        transitionDuration(DEFAULT_DURACION_TRANSICION) {
    // Inicializar duraciones por defecto
    phaseDuration[0] = DEFAULT_DURACION_AMANECER;
    phaseDuration[1] = DEFAULT_DURACION_DIA;
    phaseDuration[2] = DEFAULT_DURACION_ATARDECER;
    phaseDuration[3] = DEFAULT_DURACION_NOCHE;

    // Inicializar colores por defecto
    phaseColor[0] = Color(255, 150, 0, 0);    // Amanecer: Naranja
    phaseColor[1] = Color(0, 0, 0, 255);      // Día: Blanco
    phaseColor[2] = Color(200, 50, 20, 0);    // Atardecer: Rojo
    phaseColor[3] = Color(10, 10, 50, 0);     // Noche: Azul profundo
  }

  /**
   * @brief Inicializa el gestor de fases
   */
  void begin(Fase startPhase = Fase::AMANECER) {
    currentPhase = startPhase;
    phaseStartTime = millis();
  }

  /**
   * @brief Obtiene la fase actual
   */
  Fase getCurrentPhase() const {
    return currentPhase;
  }

  /**
   * @brief Obtiene el color de una fase específica
   */
  Color getPhaseColor(Fase fase) const {
    return phaseColor[static_cast<int>(fase)];
  }

  /**
   * @brief Obtiene el color de la fase actual
   */
  Color getCurrentColor() const {
    return phaseColor[static_cast<int>(currentPhase)];
  }

  /**
   * @brief Obtiene la duración de una fase específica
   */
  unsigned long getPhaseDuration(Fase fase) const {
    return phaseDuration[static_cast<int>(fase)];
  }

  /**
   * @brief Obtiene la duración de la fase actual
   */
  unsigned long getCurrentPhaseDuration() const {
    return getPhaseDuration(currentPhase);
  }

  /**
   * @brief Establece la duración de una fase
   */
  void setPhaseDuration(Fase fase, unsigned long duration) {
    if (duration >= 1000) {  // Mínimo 1 segundo
      phaseDuration[static_cast<int>(fase)] = duration;
    }
  }

  /**
   * @brief Establece el color de una fase
   */
  void setPhaseColor(Fase fase, const Color& color) {
    phaseColor[static_cast<int>(fase)] = color;
  }

  /**
   * @brief Obtiene la duración de la transición
   */
  unsigned long getTransitionDuration() const {
    return transitionDuration;
  }

  /**
   * @brief Establece la duración de la transición
   */
  void setTransitionDuration(unsigned long duration) {
    if (duration >= 100) {  // Mínimo 100ms
      transitionDuration = duration;
    }
  }

  /**
   * @brief Comprueba si está en transición
   */
  bool isInTransition() const {
    return inTransition;
  }

  /**
   * @brief Inicia una transición a la siguiente fase
   */
  void startTransition() {
    if (inTransition) return;  // Ya en transición
    
    inTransition = true;
    transitionStartTime = millis();
    nextPhase = getNextPhase(currentPhase);
    transitionStartColor = getCurrentColor();
    transitionEndColor = getPhaseColor(nextPhase);
  }

  /**
   * @brief Actualiza la transición
   * @return true si la transición ha terminado
   */
  bool updateTransition() {
    if (!inTransition) return false;

    unsigned long elapsed = millis() - transitionStartTime;
    if (elapsed >= transitionDuration) {
      inTransition = false;
      currentPhase = nextPhase;
      phaseStartTime = millis();
      return true;  // Transición completada
    }
    return false;
  }

  /**
   * @brief Obtiene el color de transición actual
   */
  Color getTransitionColor() const {
    if (!inTransition) return getCurrentColor();

    unsigned long elapsed = millis() - transitionStartTime;
    float progress = (float)elapsed / transitionDuration;
    return transitionStartColor.interpolate(transitionEndColor, progress);
  }

  /**
   * @brief Comprueba si debe cambiar de fase
   * @return true si debe avanzar a la siguiente fase
   */
  bool shouldAdvancePhase() const {
    if (inTransition) return false;
    return (millis() - phaseStartTime) >= getCurrentPhaseDuration();
  }

  /**
   * @brief Avanza a la siguiente fase sin transición
   */
  void advancePhase() {
    currentPhase = getNextPhase(currentPhase);
    phaseStartTime = millis();
  }

  /**
   * @brief Obtiene el tiempo transcurrido en la fase actual
   */
  unsigned long getElapsedPhaseTime() const {
    return millis() - phaseStartTime;
  }

  /**
   * @brief Obtiene el progreso de la fase actual (0.0 a 1.0)
   */
  float getPhaseProgress() const {
    unsigned long duration = getCurrentPhaseDuration();
    if (duration == 0) return 1.0f;
    return (float)getElapsedPhaseTime() / duration;
  }
};

#endif // PHASE_MANAGER_H
