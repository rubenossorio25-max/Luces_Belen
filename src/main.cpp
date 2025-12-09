#include <Arduino.h>
#include "LightController.h"
#include "PhaseManager.h"
#include "ConfigManager.h"
#include "MP3Player.h"
#include "EffectManager.h"
#include "config.h"
// Librerías WiFi y WebServer para ESP32
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>

// Configuración AP y WebServer
#define AP_SSID "LucesBelenConfig"
#define AP_PASSWORD "12345678"
#define DNS_PORT 53
AsyncWebServer server(80);
DNSServer dnsServer;

// ============================================================================
// INSTANCIAS GLOBALES
// ============================================================================
LightController lightController;
PhaseManager phaseManager;
ConfigManager configManager;
MP3Player mp3Player;
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
// FUNCIONES DE CONFIGURACIÓN WEB
// ============================================================================

String htmlPage() {
  return "<!DOCTYPE html><html><head><meta charset='UTF-8'><title>Configuración Luces Belén</title>"
         "<meta name='viewport' content='width=device-width,initial-scale=1'>"
         "<style>body{font-family:sans-serif;background:#222;color:#fff;padding:2em;}input,button{margin:0.5em;}</style></head><body>"
         "<h2>Configuración Luces Belén</h2>"
         "<form action='/config' method='POST'>"
         "Color: <input type='color' name='color' value='#ffffff'><br>"
         "Tiempo (ms): <input type='number' name='tiempo' min='100' max='60000' value='1000'><br>"
         "<button type='submit'>Guardar</button>"
         "</form>"
         "<form action='/reset' method='POST'><button type='submit' style='background:red;color:white;'>Resetear</button></form>"
         "</body></html>";
}

void setupWebServer() {
  // Página principal
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", htmlPage());
  });

  // Configuración
  server.on("/config", HTTP_POST, [](AsyncWebServerRequest *request){
    String color = "#ffffff";
    String tiempo = "1000";
    if (request->hasParam("color", true)) color = request->getParam("color", true)->value();
    if (request->hasParam("tiempo", true)) tiempo = request->getParam("tiempo", true)->value();
    // Aquí puedes guardar los valores en la configuración
    // configManager.setColor(color); configManager.setTime(tiempo.toInt());
    request->send(200, "text/html", "<h3>Configuración guardada</h3><a href='/'>Volver</a>");
  });

  // Reset
  server.on("/reset", HTTP_POST, [](AsyncWebServerRequest *request){
    // Aquí puedes hacer el reset de configuración
    // configManager.reset();
    request->send(200, "text/html", "<h3>¡Sistema reseteado!</h3><a href='/'>Volver</a>");
  });

  server.onNotFound([](AsyncWebServerRequest *request){
    request->redirect("/");
  });

  server.begin();
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
  // En ESP32 usamos el MAC address convertido a uint32
  randomSeed((uint32_t)ESP.getEfuseMac());

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

    // ===================== MODO AP Y CAPTIVE PORTAL =====================
    Serial.println("[WIFI] Iniciando Access Point...");
    WiFi.softAP(AP_SSID, AP_PASSWORD);
    delay(500);
    IPAddress myIP = WiFi.softAPIP();
    Serial.print("[WIFI] AP IP: ");
    Serial.println(myIP);

    // Iniciar DNS para captive portal
    dnsServer.start(DNS_PORT, "*", myIP);

    // Iniciar servidor web
    setupWebServer();
    Serial.println("[WEB] Servidor web iniciado. Conéctate a la red 'LucesBelenConfig' y abre cualquier página para configurar.");
  // Inicializar gestor de efectos
  Serial.println("[INIT] Inicializando gestor de efectos...");
  effectManager.begin(&lightController, &phaseManager);

  // Reproducir pista de amanecer
  mp3Player.playTrack(getTrackNumber(phaseManager.getCurrentPhase()));

  Serial.println("\n[INIT] ✅ Sistema inicializado correctamente.\n");
  Serial.println("═══════════════════════════════════════════════════════════════\n");
}

/**
 * @brief Bucle principal del sistema
 */
void loop() {
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

    // Mantener DNS para captive portal
    dnsServer.processNextRequest();
}
