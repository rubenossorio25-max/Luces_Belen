#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include "PhaseManager.h"
#include "config.h"

/**
 * @class WebServer
 * @brief Servidor web para configuración y control del sistema
 */
class WebServer {
private:
  ESP8266WebServer server;
  DNSServer dnsServer;
  PhaseManager* phaseManager;
  bool initialized;

  /**
   * @brief Genera la página HTML de configuración
   */
  String generateConfigurationPage() const {
    String html = "<!DOCTYPE html><html><head>";
    html += "<meta charset='UTF-8'>";
    html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
    html += "<title>Configuración Lumina</title>";
    
    // CSS mejorado
    html += "<style>";
    html += "* { margin: 0; padding: 0; box-sizing: border-box; }";
    html += "body { font-family: 'Segoe UI', Tahoma, sans-serif; background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); ";
    html += "color: #333; min-height: 100vh; padding: 20px; }";
    html += ".container { max-width: 800px; margin: 0 auto; background: white; border-radius: 10px; ";
    html += "box-shadow: 0 10px 40px rgba(0,0,0,0.3); padding: 30px; }";
    html += "h1 { color: #667eea; border-bottom: 3px solid #667eea; padding-bottom: 15px; margin-bottom: 30px; }";
    html += "h2 { color: #555; font-size: 1.3em; margin-top: 25px; margin-bottom: 15px; border-left: 4px solid #667eea; padding-left: 10px; }";
    html += ".form-group { margin-bottom: 20px; }";
    html += "label { display: block; margin-bottom: 8px; font-weight: 500; color: #555; }";
    html += "input[type=number], input[type=range] { width: 100%; padding: 10px; border: 1px solid #ddd; border-radius: 5px; font-size: 14px; }";
    html += "input[type=range] { padding: 5px 0; }";
    html += "input[type=submit] { background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); color: white; padding: 12px 30px; ";
    html += "border: none; border-radius: 5px; cursor: pointer; font-size: 16px; margin-top: 20px; }";
    html += "input[type=submit]:hover { transform: translateY(-2px); box-shadow: 0 5px 15px rgba(0,0,0,0.2); }";
    html += ".table { width: 100%; border-collapse: collapse; margin-top: 15px; }";
    html += "th, td { border: 1px solid #ddd; padding: 12px; text-align: left; }";
    html += "th { background-color: #f5f5f5; font-weight: 600; }";
    html += "tr:hover { background-color: #f9f9f9; }";
    html += ".slider-container { display: flex; align-items: center; gap: 10px; }";
    html += ".slider-val { min-width: 40px; text-align: right; font-weight: bold; color: #667eea; }";
    html += ".info { background: #e8f4f8; border-left: 4px solid #0288d1; padding: 15px; margin-top: 20px; border-radius: 5px; }";
    html += ".info p { margin: 5px 0; font-size: 14px; }";
    html += "</style>";
    
    html += "<script>";
    html += "function updateValue(slider) { document.getElementById(slider.name + '_val').textContent = slider.value; }";
    html += "</script>";
    html += "</head><body><div class='container'>";
    
    html += "<h1>⚙️ Configuración Lumina</h1>";
    
    html += "<form method='POST' action='/save_config'>";
    
    // Sección de Duraciones
    html += "<h2>⏱️ Duraciones (milisegundos)</h2>";
    html += "<table class='table'>";
    html += "<tr><th>Fase</th><th>Duración (ms)</th></tr>";
    
    html += "<tr><td>Amanecer</td><td><input type='number' name='duracion_amanecer' value='" + 
            String(phaseManager->getPhaseDuration(Fase::AMANECER)) + "' min='1000' required></td></tr>";
    html += "<tr><td>Día</td><td><input type='number' name='duracion_dia' value='" + 
            String(phaseManager->getPhaseDuration(Fase::DIA)) + "' min='1000' required></td></tr>";
    html += "<tr><td>Atardecer</td><td><input type='number' name='duracion_atardecer' value='" + 
            String(phaseManager->getPhaseDuration(Fase::ATARDECER)) + "' min='1000' required></td></tr>";
    html += "<tr><td>Noche</td><td><input type='number' name='duracion_noche' value='" + 
            String(phaseManager->getPhaseDuration(Fase::NOCHE)) + "' min='1000' required></td></tr>";
    html += "<tr><td>Transición</td><td><input type='number' name='duracion_transicion' value='" + 
            String(phaseManager->getTransitionDuration()) + "' min='100' required></td></tr>";
    
    html += "</table>";
    
    // Sección de Colores
    html += "<h2>🎨 Colores RGBW</h2>";
    html += "<p style='font-size: 0.9em; color: #999; margin-bottom: 15px;'>Arrastra los sliders para ajustar cada componente de color (0-255)</p>";
    html += "<table class='table'>";
    html += "<tr><th>Componente</th><th>Amanecer</th><th>Día</th><th>Atardecer</th><th>Noche</th></tr>";
    
    // Función auxiliar para generar inputs de color
    auto colorRow = [this](const char* component, const char* shortName, int index) -> String {
      String row = "<tr><td>" + String(component) + "</td>";
      for (int f = 0; f < 4; f++) {
        Color c = phaseManager->getPhaseColor(static_cast<Fase>(f));
        uint8_t value;
        switch (index) {
          case 0: value = c.r; break;
          case 1: value = c.g; break;
          case 2: value = c.b; break;
          case 3: value = c.w; break;
          default: value = 0;
        }
        const char* phases[] = {"amanecer", "dia", "atardecer", "noche"};
        String name = String(phases[f]) + "_" + String(shortName);
        row += "<td><div class='slider-container'>";
        row += "<input type='range' name='" + name + "' min='0' max='255' value='" + String(value) + "' oninput='updateValue(this)'>";
        row += "<span class='slider-val' id='" + name + "_val'>" + String(value) + "</span>";
        row += "</div></td>";
      }
      row += "</tr>";
      return row;
    };
    
    html += colorRow("Rojo (R)", "r", 0);
    html += colorRow("Verde (G)", "g", 1);
    html += colorRow("Azul (B)", "b", 2);
    html += colorRow("Blanco (W)", "w", 3);
    
    html += "</table>";
    
    html += "<input type='submit' value='💾 Guardar Configuración'></form>";
    
    html += "<div class='info'>";
    html += "<p><strong>Información del Sistema:</strong></p>";
    html += "<p>IP del AP: <code>" + WiFi.softAPIP().toString() + "</code></p>";
    html += "<p>Fase Actual: <strong>" + String(getFaseName(phaseManager->getCurrentPhase())) + "</strong></p>";
    html += "</div>";
    
    html += "</div></body></html>";
    
    return html;
  }

  /**
   * @brief Manejador para la página raíz (redirige a configuración)
   */
  void handleRoot() {
    server.sendHeader("Location", "/config", true);
    server.send(302, "text/plain", "Redirecting");
  }

  /**
   * @brief Manejador para la página de configuración
   */
  void handleConfigPage() {
    server.send(200, "text/html", generateConfigurationPage());
  }

  /**
   * @brief Manejador para guardar configuración
   */
  void handleSaveConfig() {
    if (!phaseManager) {
      server.send(400, "text/plain", "Error: PhaseManager not initialized");
      return;
    }

    // Procesar duraciones
    if (server.hasArg("duracion_amanecer")) {
      phaseManager->setPhaseDuration(Fase::AMANECER, server.arg("duracion_amanecer").toInt());
    }
    if (server.hasArg("duracion_dia")) {
      phaseManager->setPhaseDuration(Fase::DIA, server.arg("duracion_dia").toInt());
    }
    if (server.hasArg("duracion_atardecer")) {
      phaseManager->setPhaseDuration(Fase::ATARDECER, server.arg("duracion_atardecer").toInt());
    }
    if (server.hasArg("duracion_noche")) {
      phaseManager->setPhaseDuration(Fase::NOCHE, server.arg("duracion_noche").toInt());
    }
    if (server.hasArg("duracion_transicion")) {
      phaseManager->setTransitionDuration(server.arg("duracion_transicion").toInt());
    }

    // Procesar colores (Amanecer)
    if (server.hasArg("amanecer_r") && server.hasArg("amanecer_g") && 
        server.hasArg("amanecer_b") && server.hasArg("amanecer_w")) {
      Color c(server.arg("amanecer_r").toInt(), server.arg("amanecer_g").toInt(),
              server.arg("amanecer_b").toInt(), server.arg("amanecer_w").toInt());
      phaseManager->setPhaseColor(Fase::AMANECER, c);
    }

    // Procesar colores (Día)
    if (server.hasArg("dia_r") && server.hasArg("dia_g") && 
        server.hasArg("dia_b") && server.hasArg("dia_w")) {
      Color c(server.arg("dia_r").toInt(), server.arg("dia_g").toInt(),
              server.arg("dia_b").toInt(), server.arg("dia_w").toInt());
      phaseManager->setPhaseColor(Fase::DIA, c);
    }

    // Procesar colores (Atardecer)
    if (server.hasArg("atardecer_r") && server.hasArg("atardecer_g") && 
        server.hasArg("atardecer_b") && server.hasArg("atardecer_w")) {
      Color c(server.arg("atardecer_r").toInt(), server.arg("atardecer_g").toInt(),
              server.arg("atardecer_b").toInt(), server.arg("atardecer_w").toInt());
      phaseManager->setPhaseColor(Fase::ATARDECER, c);
    }

    // Procesar colores (Noche)
    if (server.hasArg("noche_r") && server.hasArg("noche_g") && 
        server.hasArg("noche_b") && server.hasArg("noche_w")) {
      Color c(server.arg("noche_r").toInt(), server.arg("noche_g").toInt(),
              server.arg("noche_b").toInt(), server.arg("noche_w").toInt());
      phaseManager->setPhaseColor(Fase::NOCHE, c);
    }

    // Responder al cliente
    String response = "<!DOCTYPE html><html><head><meta charset='UTF-8'><title>Guardado</title>";
    response += "<style>body { font-family: sans-serif; text-align: center; padding: 40px; background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); color: white; }";
    response += ".box { background: white; color: #333; padding: 30px; border-radius: 10px; max-width: 500px; margin: 0 auto; }";
    response += "h1 { color: #28a745; } a { color: #667eea; text-decoration: none; font-weight: bold; }</style>";
    response += "</head><body><div class='box'><h1>✅ Configuración Guardada</h1>";
    response += "<p>Los cambios se aplicarán en la siguiente fase.</p>";
    response += "<p><a href='/config'>Volver a la configuración</a></p>";
    response += "</div></body></html>";

    server.send(200, "text/html", response);
    Serial.println("[WEB] Configuración actualizada desde el navegador.");
  }

public:
  WebServer() : server(80), initialized(false), phaseManager(nullptr) {}

  /**
   * @brief Inicializa el servidor web
   */
  void begin(PhaseManager* pm) {
    phaseManager = pm;

    // Iniciar Punto de Acceso WiFi
    WiFi.mode(WIFI_AP);
    WiFi.softAP(AP_SSID, AP_PASSWORD);
    IPAddress apIP = WiFi.softAPIP();

    Serial.print("[WIFI] Punto de Acceso creado: ");
    Serial.println(AP_SSID);
    Serial.print("[WIFI] IP del AP: ");
    Serial.println(apIP);

    // Iniciar DNS para portal cautivo
    dnsServer.start(DNS_PORT, "*", apIP);

    // Configurar rutas del servidor
    server.on("/", [this]() { handleRoot(); });
    server.on("/config", [this]() { handleConfigPage(); });
    server.on("/save_config", HTTP_POST, [this]() { handleSaveConfig(); });

    // Rutas de prueba del portal cautivo
    server.on("/generate_204", [this]() { handleRoot(); });
    server.on("/fwlink", [this]() { handleRoot(); });
    server.on("/hotspot-detect.html", [this]() { handleRoot(); });

    // Ruta para requests no encontradas
    server.onNotFound([this]() { handleRoot(); });

    server.begin();
    initialized = true;
    Serial.println("[WEB] Servidor web iniciado en puerto 80.");
  }

  /**
   * @brief Procesa las solicitudes del cliente
   */
  void handleClient() {
    if (initialized) {
      dnsServer.processNextRequest();
      server.handleClient();
    }
  }

  /**
   * @brief Comprueba si el servidor está inicializado
   */
  bool isInitialized() const {
    return initialized;
  }
};

#endif // WEB_SERVER_H
