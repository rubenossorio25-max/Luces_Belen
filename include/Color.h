#ifndef COLOR_H
#define COLOR_H

/**
 * @struct Color
 * @brief Estructura para representar un color RGBW
 */
struct Color {
  uint8_t r;  // Rojo (0-255)
  uint8_t g;  // Verde (0-255)
  uint8_t b;  // Azul (0-255)
  uint8_t w;  // Blanco (0-255)

  /**
   * @brief Constructor por defecto
   */
  Color() : r(0), g(0), b(0), w(0) {}

  /**
   * @brief Constructor con parámetros
   */
  Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t white)
      : r(red), g(green), b(blue), w(white) {}

  /**
   * @brief Interpolación lineal entre dos colores
   * @param target Color destino
   * @param progress Progreso de 0.0 a 1.0
   * @return Color interpolado
   */
  Color interpolate(const Color& target, float progress) const {
    if (progress < 0.0f) progress = 0.0f;
    if (progress > 1.0f) progress = 1.0f;

    return Color(
      (uint8_t)(r + (target.r - r) * progress),
      (uint8_t)(g + (target.g - g) * progress),
      (uint8_t)(b + (target.b - b) * progress),
      (uint8_t)(w + (target.w - w) * progress)
    );
  }

  /**
   * @brief Compara dos colores
   */
  bool operator==(const Color& other) const {
    return r == other.r && g == other.g && b == other.b && w == other.w;
  }

  /**
   * @brief Compara dos colores (diferente)
   */
  bool operator!=(const Color& other) const {
    return !(*this == other);
  }
};

#endif // COLOR_H
