#ifndef FASE_H
#define FASE_H

/**
 * @enum Fase
 * @brief Enumeración de las fases del día
 */
enum class Fase {
  AMANECER = 0,
  DIA = 1,
  ATARDECER = 2,
  NOCHE = 3
};

/**
 * @brief Convierte un Fase a su nombre en string
 */
inline const char* getFaseName(Fase fase) {
  switch (fase) {
    case Fase::AMANECER:
      return "AMANECER";
    case Fase::DIA:
      return "DIA";
    case Fase::ATARDECER:
      return "ATARDECER";
    case Fase::NOCHE:
      return "NOCHE";
    default:
      return "DESCONOCIDA";
  }
}

/**
 * @brief Obtiene la siguiente fase del ciclo
 */
inline Fase getNextPhase(Fase actual) {
  switch (actual) {
    case Fase::AMANECER:
      return Fase::DIA;
    case Fase::DIA:
      return Fase::ATARDECER;
    case Fase::ATARDECER:
      return Fase::NOCHE;
    case Fase::NOCHE:
      return Fase::AMANECER;
    default:
      return Fase::AMANECER;
  }
}

/**
 * @brief Obtiene el número de pista MP3 para una fase
 */
inline int getTrackNumber(Fase fase) {
  switch (fase) {
    case Fase::AMANECER:
      return 1;
    case Fase::DIA:
      return 2;
    case Fase::ATARDECER:
      return 3;
    case Fase::NOCHE:
      return 4;
    default:
      return 0;
  }
}

#endif // FASE_H
