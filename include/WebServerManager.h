#ifndef WEB_SERVER_MANAGER_H
#define WEB_SERVER_MANAGER_H

#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include "ConfigManager.h"
#include "PhaseManager.h"
#include "LightController.h"

/**
 * @class WebServerManager
 * @brief Gestiona el servidor web para configuración remota
 */
class WebServerManager {
private:
  WebServer* webServer;
  ConfigManager* configManager;
  PhaseManager* phaseManager;
  LightController* lightController;
  
  const char* ssid = "Luces_Belen";
  const char* password = "12345678";
  IPAddress localIP;
  IPAddress gateway;
  IPAddress subnet;

  // Métodos privados para manejar rutas
  void handleRoot();
  void handleGetConfig();
  void handleSetConfig();
  void handleGetStatus();
  void handleSetPhase();
  void handleNotFound();
  
  // Método para generar HTML de la interfaz
  String getWebPage();
  
public:
  WebServerManager() : webServer(nullptr), configManager(nullptr), 
                       phaseManager(nullptr), lightController(nullptr),
                       localIP(192, 168, 1, 100),
                       gateway(192, 168, 1, 1),
                       subnet(255, 255, 255, 0) {}

  /**
   * @brief Inicializa el servidor web
   */
  void begin(ConfigManager* cm, PhaseManager* pm, LightController* lc);

  /**
   * @brief Maneja las solicitudes del cliente
   */
  void handleClient();

  /**
   * @brief Detiene el servidor web
   */
  void stop();

  /**
   * @brief Obtiene la dirección IP del servidor
   */
  IPAddress getIP() const;

  /**
   * @brief Verifica si WiFi está conectado
   */
  bool isWiFiConnected() const;
};

#endif // WEB_SERVER_MANAGER_H
