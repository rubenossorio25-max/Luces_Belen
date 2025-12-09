# 🚀 Compilación e Instalación del Servidor Web

## Requisitos

1. **PlatformIO CLI** instalado
2. **USB drivers para ESP32** (si es necesario)
3. **Arduino IDE** o **VS Code con extensión PlatformIO**

## Pasos de Compilación

### Opción 1: Compilar sin cargar (solo verificar)

```bash
pio run
```

Esta es la opción actual en tu `tasks.json` "Build".

### Opción 2: Compilar y cargar en el ESP32

```bash
pio run --target upload
```

Asegúrate de que el ESP32 esté conectado por USB.

### Opción 3: Limpiar y compilar desde cero

```bash
pio run --target clean
pio run
```

## Verificar la Compilación

Si la compilación es exitosa, verás:

```
...
======================== [SUCCESS] Took X.XX seconds ========================
Environment Status Duration
---------- -------- ---------
esp32doit  SUCCESS  X.XX s
========================= 1 succeeded in X.XX seconds ==========================
```

## Solución de Problemas de Compilación

### Error: "ArduinoJson/ArduinoJson.h: No such file or directory"

**Solución**: Asegúrate de tener la librería ArduinoJson en `lib_deps`:

```ini
lib_deps =
    EEPROM
    ArduinoJson@^6.21.0
```

Luego ejecuta:
```bash
pio lib install
```

### Error: WebServer.h no encontrado

Este es normal si no compilaste la nueva rama. Asegúrate de:
1. Tener los archivos `include/WebServer.h` y `src/WebServer.cpp`
2. Ejecutar `pio run --target clean` seguido de `pio run`

### Error al cargar (Upload)

Si ves errores durante el upload:
1. Verifica que el puerto USB sea correcto
2. Intenta otro cable USB
3. Reinicia el ESP32 y vuelve a intentar
4. Ejecuta:
   ```bash
   pio device list
   ```
   Para verificar que el dispositivo se detecte

## Verificar Compilación Remota

Después de cargar el código:

1. Abre el Monitor Serial:
   ```bash
   pio device monitor --baud 115200
   ```

2. Deberías ver algo como:
   ```
   ═══════════════════════════════════════════════════════════════
     🌅 Sistema Lumina - Controlador de Iluminación Inteligente
   ═══════════════════════════════════════════════════════════════
   [INIT] Inicializando controlador de luces...
   [INIT] Inicializando gestor de fases...
   [INIT] Inicializando reproductor MP3...
   [INIT] Inicializando gestor de configuración...
   [INIT] Inicializando gestor de efectos...
   [INIT] Inicializando servidor web...
   [WEB] Iniciando servidor web...
   [WEB] Punto de acceso creado: Luces_Belen
   [WEB] IP: 192.168.1.100
   [WEB] Contraseña: 12345678
   [WEB] Servidor web iniciado en puerto 80
   ```

3. Busca la red WiFi `Luces_Belen` en tu teléfono
4. Conéctate con la contraseña `12345678`
5. Abre `192.168.1.100` en el navegador

## Archivos Modificados

- ✅ `src/main.cpp` - Agregado WebServerManager
- ✅ `include/WebServer.h` - Nuevo archivo de cabecera
- ✅ `src/WebServer.cpp` - Implementación del servidor
- ✅ `platformio.ini` - Agregada librería ArduinoJson
- ✅ `WEBSERVER_README.md` - Documentación para usuarios

## Próximos Pasos

1. Compila el código: `pio run`
2. Carga en ESP32: `pio run --target upload`
3. Abre el monitor serial para verificar que funciona
4. Conecta desde tu Android a `192.168.1.100`
