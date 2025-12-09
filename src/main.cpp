#include <Arduino.h>
#include "LightController.h"
#include "PhaseManager.h"
#include "ConfigManager.h"
#include "MP3Player.h"
#include "WebServer.h"
#include "EffectManager.h"
#include "config.h"

// ============================================================================
// INSTANCIAS GLOBALES
// ============================================================================
LightController lightController;
PhaseManager phaseManager;
ConfigManager configManager;
MP3Player mp3Player;
WebServer webServer;
EffectManager effectManager;

// Estado del sistema
unsigned long lastReportTime = 0;

// ============================================================================
// FUNCIONES DE MONITOREO Y REPORTE
// ============================================================================

/**
 * @brief Reporta el estado actual del sistema
 */
void reportStatus() {
  unsigned long now = millis();
  if (now - lastReportTime < INTERVALO_REPORTE) {
    return;
  }
  
  lastReportTime = now;

  Serial.print("[STATE] Fase: ");
  Serial.print(getFaseName(phaseManager.getCurrentPhase()));
  Serial.print(" | Progreso: ");
  Serial.print((int)(phaseManager.getPhaseProgress() * 100));
  Serial.print("% | Transición: ");
  Serial.println(phaseManager.isInTransition() ? "SÍ" : "NO");
}

/**
 * @brief Imprime información de inicialización
 */
void printBootInfo() {
  Serial.println("\n");
  Serial.println("═══════════════════════════════════════════════════════════════");
  Serial.println("  🌅 Sistema Lumina - Controlador de Iluminación Inteligente");
  Serial.println("═══════════════════════════════════════════════════════════════");
  Serial.print("  Compilado: ");
  Serial.print(__DATE__);
  Serial.print(" ");
  Serial.println(__TIME__);
  Serial.print("  Versión: 2.0 (Refactorizado)");
  Serial.println("\n");
}

// ============================================================================
// SETUP Y LOOP PRINCIPALES
// ============================================================================

/**
 * @brief Configuración inicial del sistema
 */
void setup() {
  // Inicializar comunicación serial
  Serial.begin(SERIAL_BAUD);
  delay(1000);

  // Imprimir información de boot
  printBootInfo();

  // Inicializar el generador de números aleatorios
  randomSeed(ESP.getChipId());

  // Inicializar controlador de luces
  Serial.println("[INIT] Inicializando controlador de luces...");
  lightController.begin();
  lightController.turnOff();

  // Inicializar gestor de fases
  Serial.println("[INIT] Inicializando gestor de fases...");
  phaseManager.begin(Fase::AMANECER);

  // Inicializar reproductor MP3
  Serial.println("[INIT] Inicializando reproductor MP3...");
  mp3Player.begin();

  // Inicializar gestor de configuración
  Serial.println("[INIT] Inicializando gestor de configuración...");
  configManager.begin(&phaseManager);
  configManager.loadConfiguration();

  // Inicializar gestor de efectos
  Serial.println("[INIT] Inicializando gestor de efectos...");
  effectManager.begin(&lightController, &phaseManager);

  // Inicializar servidor web
  Serial.println("[INIT] Inicializando servidor web...");
  webServer.begin(&phaseManager);

  // Reproducir pista de amanecer
  mp3Player.playTrack(getTrackNumber(phaseManager.getCurrentPhase()));

  Serial.println("\n[INIT] ✅ Sistema inicializado correctamente.\n");
  Serial.println("═══════════════════════════════════════════════════════════════\n");
}

/**
 * @brief Bucle principal del sistema
 */
void loop() {
  // Procesar solicitudes del servidor web
  webServer.handleClient();

  // Actualizar efectos especiales
  effectManager.update();

  // Procesar transiciones de fase
  if (phaseManager.isInTransition()) {
    // Aplicar color de transición
    lightController.setColor(phaseManager.getTransitionColor());

    // Comprobar si la transición ha terminado
    if (phaseManager.updateTransition()) {
      // Transición completada, reproducir audio de nueva fase
      mp3Player.playTrack(getTrackNumber(phaseManager.getCurrentPhase()));
      Serial.print("[PHASE] 🔄 Transición completada a fase: ");
      Serial.println(getFaseName(phaseManager.getCurrentPhase()));
    }
  } else {
    // No en transición
    
    // Comprobar si debe cambiar de fase
    if (phaseManager.shouldAdvancePhase()) {
      phaseManager.startTransition();
      Serial.print("[PHASE] ⏱️ Iniciando transición a: ");
      Serial.println(getFaseName(getNextPhase(phaseManager.getCurrentPhase())));
    } else {
      // Mantener color de fase actual (excepto si hay rayos en noche)
      if (phaseManager.getCurrentPhase() != Fase::NOCHE) {
        lightController.setColor(phaseManager.getCurrentColor());
      }
    }
  }

  // Reportar estado periódicamente
  reportStatus();

  // Pequeño delay para evitar saturación
  delay(10);
}
