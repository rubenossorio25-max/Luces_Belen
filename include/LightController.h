#ifndef LIGHT_CONTROLLER_H
#define LIGHT_CONTROLLER_H

#include "Color.h"
#include "config.h"

/**
 * @class LightController
 * @brief Controlador de iluminación RGBW
 * Maneja la salida PWM a los pines de LED
 */
class LightController {
private:
  Color currentColor;
  bool initialized;

public:
  LightController() : initialized(false) {}

  /**
   * @brief Inicializa los pines de control de LEDs
   */
  void begin() {
    pinMode(PIN_ROJO, OUTPUT);
    pinMode(PIN_VERDE, OUTPUT);
    pinMode(PIN_AZUL, OUTPUT);
    pinMode(PIN_BLANCO, OUTPUT);
    initialized = true;
    setColor(Color(0, 0, 0, 0));
  }

  /**
   * @brief Establece el color actual
   * Nota: PIN_ROJO recibe valor de Verde y PIN_VERDE recibe valor de Rojo
   */
  void setColor(const Color& color) {
    if (!initialized) return;
    currentColor = color;
    
    // Mapeo invertido: PIN_ROJO <- color.g, PIN_VERDE <- color.r
    analogWrite(PIN_ROJO, color.g);
    analogWrite(PIN_VERDE, color.r);
    analogWrite(PIN_AZUL, color.b);
    analogWrite(PIN_BLANCO, color.w);
  }

  /**
   * @brief Obtiene el color actual
   */
  const Color& getColor() const {
    return currentColor;
  }

  /**
   * @brief Establece un color interpolado entre dos colores
   * @param start Color inicial
   * @param end Color final
   * @param progress Progreso de 0.0 a 1.0
   */
  void setInterpolatedColor(const Color& start, const Color& end, float progress) {
    setColor(start.interpolate(end, progress));
  }

  /**
   * @brief Apaga todos los LEDs
   */
  void turnOff() {
    setColor(Color(0, 0, 0, 0));
  }
};

#endif // LIGHT_CONTROLLER_H
