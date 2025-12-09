# 📚 Guía de Uso y Ejemplos

## Inicialización Rápida

El proyecto está completamente configurado y listo para usar. El archivo `main.cpp` contiene toda la lógica de inicialización.

## Instancias Globales

Todas las instancias de las clases se definen como globales en `main.cpp`:

```cpp
LightController lightController;    // Control de LEDs
PhaseManager phaseManager;          // Gestor de fases
ConfigManager configManager;        // Persistencia EEPROM
MP3Player mp3Player;                // Reproductor MP3
WebServer webServer;                // Servidor web
EffectManager effectManager;        // Efectos especiales
```

## Ejemplos de Uso

### Cambiar el Color Manualmente

```cpp
// Crear un color (RGBW)
Color miColor(255, 100, 50, 0);  // Naranja con rojo
lightController.setColor(miColor);
```

### Interpolar Entre Dos Colores

```cpp
Color inicio(255, 0, 0, 0);      // Rojo
Color fin(0, 255, 0, 0);         // Verde
lightController.setInterpolatedColor(inicio, fin, 0.5f);  // 50% interpolado
```

### Obtener la Fase Actual

```cpp
Fase actual = phaseManager.getCurrentPhase();
Serial.println(getFaseName(actual));
```

### Verificar si Está en Transición

```cpp
if (phaseManager.isInTransition()) {
    Serial.println("Transición en progreso");
}
```

### Cambiar Duración de una Fase

```cpp
// Establecer la fase de día a 30 segundos
phaseManager.setPhaseDuration(Fase::DIA, 30000);

// Guardar en EEPROM
configManager.saveConfiguration();
```

### Cambiar Color de una Fase

```cpp
// Establecer color de amanecer a rojo puro
phaseManager.setPhaseColor(Fase::AMANECER, Color(255, 0, 0, 0));

// Guardar en EEPROM
configManager.saveConfiguration();
```

### Forzar un Rayo Inmediato (para pruebas)

```cpp
effectManager.forceLightning();
```

### Reproducir una Pista MP3

```cpp
mp3Player.playTrack(2);  // Reproducir pista 2 (Día)
```

### Establecer Volumen MP3

```cpp
mp3Player.setVolume(20);  // Volumen 0-30
```

## Flujo de Transición de Fase

El sistema automáticamente:

1. **Detecta** que una fase ha terminado usando `shouldAdvancePhase()`
2. **Inicia transición** llamando a `startTransition()`
3. **Interpola colores** durante `transitionDuration` ms
4. **Cambia de fase** cuando termina la transición
5. **Reproduce audio** de la nueva fase

```cpp
// Esto ocurre automáticamente en loop(), pero puedes hacerlo manualmente:
if (phaseManager.shouldAdvancePhase()) {
    phaseManager.startTransition();
}

// En el siguiente ciclo:
if (phaseManager.isInTransition()) {
    lightController.setColor(phaseManager.getTransitionColor());
    if (phaseManager.updateTransition()) {
        mp3Player.playTrack(getTrackNumber(phaseManager.getCurrentPhase()));
    }
}
```

## Personalización Avanzada

### Agregar un Nuevo Efecto

1. Crear un método en `EffectManager`:

```cpp
void triggerStrobeEffect() {
    for (int i = 0; i < 10; i++) {
        lightController.turnOff();
        delay(100);
        lightController.setColor(phaseManager.getCurrentColor());
        delay(100);
    }
}
```

2. Llamar desde el `loop()` o desde la interfaz web.

### Agregar una Nueva Ruta Web

En `WebServer.h`, agregar en el método `begin()`:

```cpp
server.on("/api/status", HTTP_GET, [this]() {
    String json = "{\"phase\":\"" + String(getFaseName(phaseManager->getCurrentPhase())) + "\"}";
    server.send(200, "application/json", json);
});
```

### Modificar la Interfaz Web

Editar la función `generateConfigurationPage()` en `WebServer.h` para cambiar HTML/CSS.

## Monitoreo vía Puerto Serial

El sistema reporta estado cada segundo:

```
[STATE] Fase: AMANECER | Progreso: 35% | Transición: NO
[STATE] Fase: AMANECER | Progreso: 70% | Transición: NO
[STATE] Fase: DIA | Progreso: 0% | Transición: SI
[PHASE] 🔄 Transición completada a fase: DIA
[EFFECT] ⚡ Rayo detectado con 3 destellos.
```

## Estructura de Datos EEPROM

```
Dirección   | Tamaño | Contenido
------------|--------|-------------------
0-3         | 4 bytes| Duración Amanecer (unsigned long)
4-7         | 4 bytes| Duración Día
8-11        | 4 bytes| Duración Atardecer
12-15       | 4 bytes| Duración Noche
16-19       | 4 bytes| Duración Transición
20-23       | -      | (Reservado)
24-39       | 16 bytes| Color Amanecer (4 uint8_t)
40-55       | 16 bytes| Color Día
56-71       | 16 bytes| Color Atardecer
72-87       | 16 bytes| Color Noche
88-511      | -      | (Disponible para expansión)
```

## Dépuración

### Ver Todos los Mensajes de Depuración

Los mensajes tienen prefijos para facilitar el filtrado:

- `[INIT]` - Inicialización
- `[WIFI]` - Conectividad WiFi
- `[WEB]` - Servidor web
- `[MP3]` - Reproductor MP3
- `[CONFIG]` - Configuración EEPROM
- `[STATE]` - Estado del sistema
- `[PHASE]` - Cambios de fase
- `[EFFECT]` - Efectos especiales

### Habilitar Modo Debug Completo

En `config.h`, agregar:

```cpp
#define DEBUG_VERBOSE 1

// Luego en main.cpp:
#ifdef DEBUG_VERBOSE
    Serial.print("[DEBUG] ");
    Serial.println("mensaje");
#endif
```

## Resetear Configuración

Para resetear a valores por defecto desde código:

```cpp
configManager.resetToDefaults();
```

O desde el puerto serial:

```cpp
// Agregar al setup():
Serial.println("Escriba 'RESET' para resetear configuración");

// En loop():
if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    if (cmd == "RESET") {
        configManager.resetToDefaults();
        Serial.println("Configuración reseteada");
    }
}
```

## Performance y Optimización

### Timings Críticos

- **Loop principal**: ~10ms (1ms delay)
- **Reporte estado**: ~1000ms
- **Actualización web**: ~50ms por request
- **Transición color**: Suave (sin delays bloqueantes)

### Memory Usage

```
RAM utilizado: ~45KB
FLASH utilizado: ~300KB
EEPROM utilizado: ~88 bytes
```

## Troubleshooting

### ¿Por qué cambia el color rojo-verde?

El mapeo de pines está invertido intencionalmente:
```cpp
analogWrite(PIN_ROJO, color.g);   // Pin ROJO <- valor VERDE
analogWrite(PIN_VERDE, color.r);  // Pin VERDE <- valor ROJO
```

Esto se debe a la conexión física. Si lo necesitas cambiar:

```cpp
// En LightController.h, método setColor():
analogWrite(PIN_ROJO, color.r);    // Normal
analogWrite(PIN_VERDE, color.g);   // Normal
```

### La transición es demasiado rápida/lenta

Cambiar en `config.h`:

```cpp
const unsigned long DEFAULT_DURACION_TRANSICION = 15000;  // 15 segundos
```

O desde la interfaz web.

### El MP3 se detiene

Verificar que `enableLoop()` se está ejecutando correctamente. El comando se envía al iniciar.

---

**Última actualización**: Diciembre 2025
