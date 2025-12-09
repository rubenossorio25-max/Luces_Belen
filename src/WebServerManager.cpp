#include "WebServerManager.h"

void WebServerManager::begin(ConfigManager* cm, PhaseManager* pm, LightController* lc) {
  configManager = cm;
  phaseManager = pm;
  lightController = lc;

  Serial.println("[WEB] Iniciando servidor web...");

  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(localIP, gateway, subnet);
  WiFi.softAP(ssid, password);

  delay(100);
  Serial.print("[WEB] Punto de acceso creado: ");
  Serial.println(ssid);
  Serial.print("[WEB] IP: ");
  Serial.println(WiFi.softAPIP());
  Serial.print("[WEB] Contrasena: ");
  Serial.println(password);

  webServer = new WebServer(80);

  webServer->on("/", HTTP_GET, [this]() { handleRoot(); });
  webServer->on("/api/config", HTTP_GET, [this]() { handleGetConfig(); });
  webServer->on("/api/config", HTTP_POST, [this]() { handleSetConfig(); });
  webServer->on("/api/status", HTTP_GET, [this]() { handleGetStatus(); });
  webServer->on("/api/phase", HTTP_POST, [this]() { handleSetPhase(); });
  webServer->onNotFound([this]() { handleNotFound(); });

  webServer->begin();
  Serial.println("[WEB] Servidor web iniciado en puerto 80");
}

void WebServerManager::handleClient() {
  if (webServer) {
    webServer->handleClient();
  }
}

void WebServerManager::stop() {
  if (webServer) {
    webServer->stop();
    delete webServer;
    webServer = nullptr;
  }
  WiFi.softAPdisconnect(true);
}

IPAddress WebServerManager::getIP() const {
  return WiFi.softAPIP();
}

bool WebServerManager::isWiFiConnected() const {
  return WiFi.softAPgetStationNum() > 0;
}

void WebServerManager::handleRoot() {
  webServer->send(200, "text/html; charset=utf-8", getWebPage());
}

void WebServerManager::handleGetConfig() {
  DynamicJsonDocument doc(1024);
  
  doc["amanecer"] = phaseManager->getPhaseDuration(Fase::AMANECER);
  doc["dia"] = phaseManager->getPhaseDuration(Fase::DIA);
  doc["atardecer"] = phaseManager->getPhaseDuration(Fase::ATARDECER);
  doc["noche"] = phaseManager->getPhaseDuration(Fase::NOCHE);
  
  String json;
  serializeJson(doc, json);
  
  webServer->send(200, "application/json", json);
}

void WebServerManager::handleSetConfig() {
  if (webServer->hasArg("plain")) {
    String body = webServer->arg("plain");
    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, body);
    
    if (!error) {
      if (doc.containsKey("amanecer")) {
        phaseManager->setPhaseDuration(Fase::AMANECER, doc["amanecer"].as<unsigned long>());
      }
      if (doc.containsKey("dia")) {
        phaseManager->setPhaseDuration(Fase::DIA, doc["dia"].as<unsigned long>());
      }
      if (doc.containsKey("atardecer")) {
        phaseManager->setPhaseDuration(Fase::ATARDECER, doc["atardecer"].as<unsigned long>());
      }
      if (doc.containsKey("noche")) {
        phaseManager->setPhaseDuration(Fase::NOCHE, doc["noche"].as<unsigned long>());
      }
      
      configManager->saveConfiguration();
      
      webServer->send(200, "application/json", "{\"status\":\"ok\"}");
      Serial.println("[WEB] Configuracion actualizada");
    } else {
      webServer->send(400, "application/json", "{\"error\":\"JSON invalido\"}");
    }
  } else {
    webServer->send(400, "application/json", "{\"error\":\"Sin datos\"}");
  }
}

void WebServerManager::handleGetStatus() {
  DynamicJsonDocument doc(512);
  
  doc["phase"] = (int)phaseManager->getCurrentPhase();
  doc["progress"] = phaseManager->getPhaseProgress() * 100;
  doc["isTransition"] = phaseManager->isInTransition();
  
  String json;
  serializeJson(doc, json);
  
  webServer->send(200, "application/json", json);
}

void WebServerManager::handleSetPhase() {
  if (webServer->hasArg("plain")) {
    String body = webServer->arg("plain");
    DynamicJsonDocument doc(256);
    DeserializationError error = deserializeJson(doc, body);
    
    if (!error && doc.containsKey("phase")) {
      Fase newPhase = (Fase)doc["phase"].as<int>();
      phaseManager->begin(newPhase);
      
      webServer->send(200, "application/json", "{\"status\":\"ok\"}");
      Serial.print("[WEB] Fase cambiada a: ");
      Serial.println((int)newPhase);
    } else {
      webServer->send(400, "application/json", "{\"error\":\"Datos invalidos\"}");
    }
  } else {
    webServer->send(400, "application/json", "{\"error\":\"Sin datos\"}");
  }
}

void WebServerManager::handleNotFound() {
  webServer->send(404, "text/plain", "No encontrado");
}

String WebServerManager::getWebPage() {
  String html = "";
  html += "<!DOCTYPE html>";
  html += "<html lang=\"es\"><head>";
  html += "<meta charset=\"UTF-8\">";
  html += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";
  html += "<title>Luces Belen</title>";
  html += "<style>";
  html += "* { margin:0; padding:0; box-sizing:border-box; }";
  html += "body { font-family: 'Segoe UI', sans-serif; background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); min-height: 100vh; display: flex; align-items: center; justify-content: center; padding: 10px; }";
  html += ".container { background: white; border-radius: 20px; box-shadow: 0 20px 60px rgba(0,0,0,0.3); padding: 30px; max-width: 500px; width: 100%; }";
  html += "h1 { color: #333; margin-bottom: 10px; font-size: 28px; text-align: center; }";
  html += ".subtitle { color: #999; text-align: center; margin-bottom: 30px; font-size: 14px; }";
  html += ".status-box { background: #f5f5f5; border-radius: 12px; padding: 20px; margin-bottom: 30px; border-left: 4px solid #667eea; }";
  html += ".status-item { display: flex; justify-content: space-between; align-items: center; margin-bottom: 12px; font-size: 14px; }";
  html += ".status-item:last-child { margin-bottom: 0; }";
  html += ".status-label { color: #666; font-weight: 500; }";
  html += ".status-value { color: #667eea; font-weight: bold; font-size: 16px; }";
  html += ".progress-bar { width: 100%; height: 6px; background: #ddd; border-radius: 3px; margin-top: 10px; overflow: hidden; }";
  html += ".progress-fill { height: 100%; background: linear-gradient(90deg, #667eea, #764ba2); transition: width 0.3s ease; }";
  html += ".section-title { color: #333; font-size: 16px; font-weight: bold; margin-bottom: 15px; margin-top: 25px; }";
  html += ".phase-buttons { display: grid; grid-template-columns: 1fr 1fr; gap: 10px; margin-bottom: 30px; }";
  html += ".phase-btn { padding: 15px; border: none; border-radius: 10px; font-size: 12px; font-weight: bold; cursor: pointer; transition: all 0.3s ease; background: #f0f0f0; color: #333; border: 2px solid transparent; }";
  html += ".phase-btn:active { transform: scale(0.98); }";
  html += ".phase-btn.amanecer { border-color: #FFB347; background: #FFF4E6; color: #FF9500; }";
  html += ".phase-btn.dia { border-color: #FFD700; background: #FFFACD; color: #DAA520; }";
  html += ".phase-btn.atardecer { border-color: #FF6347; background: #FFE4E1; color: #FF4500; }";
  html += ".phase-btn.noche { border-color: #4169E1; background: #E6F0FF; color: #1E40AF; }";
  html += ".config-section { background: #f9f9f9; border-radius: 12px; padding: 20px; margin-bottom: 20px; border: 1px solid #eee; }";
  html += ".config-item { margin-bottom: 20px; }";
  html += ".config-item:last-child { margin-bottom: 0; }";
  html += ".config-label { display: block; color: #666; font-size: 13px; margin-bottom: 8px; font-weight: 500; }";
  html += ".input-group { display: flex; align-items: center; gap: 10px; }";
  html += ".config-input { flex: 1; padding: 10px 12px; border: 1px solid #ddd; border-radius: 8px; font-size: 14px; font-family: monospace; }";
  html += ".config-input:focus { outline: none; border-color: #667eea; box-shadow: 0 0 0 3px rgba(102, 126, 234, 0.1); }";
  html += ".unit { color: #999; font-size: 12px; min-width: 30px; }";
  html += ".button-group { display: grid; grid-template-columns: 1fr 1fr; gap: 12px; margin-top: 30px; }";
  html += "button { padding: 14px; border: none; border-radius: 10px; font-size: 14px; font-weight: bold; cursor: pointer; transition: all 0.3s ease; }";
  html += ".btn-save { background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); color: white; }";
  html += ".btn-save:hover { box-shadow: 0 8px 20px rgba(102, 126, 234, 0.3); }";
  html += ".btn-save:active { transform: scale(0.98); }";
  html += ".btn-reset { background: #f0f0f0; color: #333; }";
  html += ".btn-reset:hover { background: #e0e0e0; }";
  html += ".message { display: none; padding: 12px; border-radius: 8px; margin-bottom: 15px; font-size: 13px; text-align: center; font-weight: 500; }";
  html += ".message.success { background: #C6EFCE; color: #0B6623; display: block; }";
  html += ".message.error { background: #FFC7CE; color: #9C0006; display: block; }";
  html += ".loading { display: none; text-align: center; padding: 15px; color: #667eea; font-size: 12px; }";
  html += "@media (max-width: 480px) { .container { padding: 20px; } h1 { font-size: 24px; } .phase-buttons { grid-template-columns: 1fr; } }";
  html += "</style></head><body>";
  html += "<div class=\"container\">";
  html += "<h1>Luces Belen</h1>";
  html += "<p class=\"subtitle\">Sistema de Control de Iluminacion</p>";
  html += "<div id=\"msg\" class=\"message\"></div>";
  html += "<div id=\"loading\" class=\"loading\">Actualizando...</div>";
  html += "<div class=\"status-box\">";
  html += "<div class=\"status-item\"><span class=\"status-label\">Fase:</span><span class=\"status-value\" id=\"phase\">Cargando...</span></div>";
  html += "<div class=\"status-item\"><span class=\"status-label\">Progreso:</span><span class=\"status-value\" id=\"prog\">0%</span></div>";
  html += "<div class=\"progress-bar\"><div class=\"progress-fill\" id=\"pf\" style=\"width: 0%\"></div></div>";
  html += "<div class=\"status-item\" style=\"margin-top: 12px;\"><span class=\"status-label\">Transicion:</span><span class=\"status-value\" id=\"trans\">No</span></div>";
  html += "</div>";
  html += "<div class=\"section-title\">Cambiar Fase</div>";
  html += "<div class=\"phase-buttons\">";
  html += "<button class=\"phase-btn amanecer\" onclick=\"setPhase(0)\">Amanecer</button>";
  html += "<button class=\"phase-btn dia\" onclick=\"setPhase(1)\">Dia</button>";
  html += "<button class=\"phase-btn atardecer\" onclick=\"setPhase(2)\">Atardecer</button>";
  html += "<button class=\"phase-btn noche\" onclick=\"setPhase(3)\">Noche</button>";
  html += "</div>";
  html += "<div class=\"section-title\">Configuracion</div>";
  html += "<div class=\"config-section\">";
  html += "<div class=\"config-item\"><label class=\"config-label\">Amanecer (ms)</label>";
  html += "<div class=\"input-group\"><input type=\"number\" id=\"a\" class=\"config-input\" value=\"1800000\" min=\"60000\" step=\"60000\"><span class=\"unit\">ms</span></div></div>";
  html += "<div class=\"config-item\"><label class=\"config-label\">Dia (ms)</label>";
  html += "<div class=\"input-group\"><input type=\"number\" id=\"d\" class=\"config-input\" value=\"3600000\" min=\"60000\" step=\"60000\"><span class=\"unit\">ms</span></div></div>";
  html += "<div class=\"config-item\"><label class=\"config-label\">Atardecer (ms)</label>";
  html += "<div class=\"input-group\"><input type=\"number\" id=\"at\" class=\"config-input\" value=\"1800000\" min=\"60000\" step=\"60000\"><span class=\"unit\">ms</span></div></div>";
  html += "<div class=\"config-item\"><label class=\"config-label\">Noche (ms)</label>";
  html += "<div class=\"input-group\"><input type=\"number\" id=\"n\" class=\"config-input\" value=\"3600000\" min=\"60000\" step=\"60000\"><span class=\"unit\">ms</span></div></div>";
  html += "</div>";
  html += "<div class=\"button-group\">";
  html += "<button class=\"btn-save\" onclick=\"save()\">Guardar</button>";
  html += "<button class=\"btn-reset\" onclick=\"load()\">Recargar</button>";
  html += "</div>";
  html += "</div>";
  html += "<script>";
  html += "const pn=['Amanecer','Dia','Atardecer','Noche'];";
  html += "function upd(){fetch('/api/status').then(r=>r.json()).then(d=>{";
  html += "document.getElementById('phase').textContent=pn[d.phase];";
  html += "document.getElementById('prog').textContent=Math.round(d.progress)+'%';";
  html += "document.getElementById('pf').style.width=d.progress+'%';";
  html += "document.getElementById('trans').textContent=d.isTransition?'Si':'No';";
  html += "}).catch(e=>console.log('E'));}";
  html += "function load(){sl(1);fetch('/api/config').then(r=>r.json()).then(d=>{";
  html += "document.getElementById('a').value=d.amanecer;";
  html += "document.getElementById('d').value=d.dia;";
  html += "document.getElementById('at').value=d.atardecer;";
  html += "document.getElementById('n').value=d.noche;sl(0);";
  html += "}).catch(e=>{msg('Error al cargar','error');sl(0);});}";
  html += "function save(){const c={amanecer:parseInt(document.getElementById('a').value),dia:parseInt(document.getElementById('d').value),atardecer:parseInt(document.getElementById('at').value),noche:parseInt(document.getElementById('n').value)};";
  html += "sl(1);fetch('/api/config',{method:'POST',headers:{'Content-Type':'application/json'},body:JSON.stringify(c)}).then(r=>r.json()).then(d=>{";
  html += "msg(d.status==='ok'?'Guardado correctamente':'Error al guardar',d.status==='ok'?'success':'error');sl(0);";
  html += "}).catch(e=>{msg('Error','error');sl(0);});}";
  html += "function setPhase(p){sl(1);fetch('/api/phase',{method:'POST',headers:{'Content-Type':'application/json'},body:JSON.stringify({phase:p})}).then(r=>r.json()).then(d=>{";
  html += "if(d.status==='ok'){upd();}else{msg('Error','error');}sl(0);}).catch(e=>{msg('Error','error');sl(0);});}";
  html += "function msg(t,ty){const m=document.getElementById('msg');m.textContent=t;m.className='message '+ty;setTimeout(()=>m.className='message',3000);}";
  html += "function sl(s){document.getElementById('loading').style.display=s?'block':'none';}";
  html += "window.onload=function(){load();upd();setInterval(upd,2000);};";
  html += "</script></body></html>";
  
  return html;
}
