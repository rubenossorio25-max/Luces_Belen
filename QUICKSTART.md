# 🚀 Quick Start Guide

## Instalación Rápida

### 1️⃣ Requisitos Previos

```bash
# Instalar PlatformIO CLI
pip install platformio

# O usar la extensión de VS Code:
# https://platformio.org/install/ide?install=vscode
```

### 2️⃣ Clonar/Descargar el Proyecto

```bash
cd /workspaces/Luces_Belen
```

### 3️⃣ Compilar

```bash
pio run
```

**Esperado**: Sin errores, génesis del `.pio/build/`

### 4️⃣ Cargar en NodeMCU

```bash
# Conectar NodeMCU por USB

# Linux/Mac:
pio run -t upload --upload-port /dev/ttyUSB0

# Windows:
pio run -t upload --upload-port COM3
```

### 5️⃣ Monitor Serial

```bash
pio run -t monitor --monitor-port /dev/ttyUSB0
```

**Esperado**: Ver logs de inicialización

### 6️⃣ Conectar a WiFi

1. Abrir **Redes WiFi** en tu dispositivo
2. Buscar: `LuminaConfig`
3. Contraseña: `configurar123`
4. Ir a: http://192.168.4.1

## ⚡ Inicio Rápido Todo en Uno

```bash
# Compilar + Cargar + Monitor
pio run -t upload -t monitor
```

## 🔧 Configuración de Puertos (si es necesario)

```bash
# Listar puertos disponibles
pio device list

# Si tienes múltiples puertos, especificar en platformio.ini:
# [env:nodemcuv2]
# upload_port = /dev/ttyUSB0
# monitor_port = /dev/ttyUSB0
```

## 📝 Primeros Pasos

### Acceso a la Interfaz Web

1. **Conectarse a WiFi**: LuminaConfig / configurar123
2. **Abrir navegador**: http://192.168.4.1
3. **Configurar**:
   - Ajustar duraciones de fases
   - Cambiar colores RGBW
   - Guardar configuración

### Monitoreo Serial

Verás mensajes como:

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
[WIFI] Punto de Acceso creado: LuminaConfig
[WIFI] IP del AP: 192.168.4.1
[WEB] Servidor web iniciado en puerto 80.
[INIT] ✅ Sistema inicializado correctamente.

[STATE] Fase: AMANECER | Progreso: 0% | Transición: NO
[STATE] Fase: AMANECER | Progreso: 5% | Transición: NO
...
```

## 🐛 Troubleshooting Rápido

### ❌ "Error: Board not found"
```bash
# Verificar que el NodeMCU esté conectado
pio device list

# Reinstalar drivers USB si es necesario
```

### ❌ "Port already in use"
```bash
# Cerrar monitor en otra terminal
# O especificar puerto diferente
pio run -t monitor --monitor-port /dev/ttyUSB1
```

### ❌ "Connection refused"
- Esperar 5 segundos después de cargar
- Verificar que la red WiFi esté activa
- Probar http://192.168.4.1 en navegador

## 📚 Siguientes Pasos

1. **Leer README.md** - Información completa
2. **Leer USAGE.md** - Ejemplos de código
3. **Leer REFACTORING.md** - Entender la arquitectura
4. **Modificar config.h** - Personalizar según necesidad

## 🎯 Tareas Comunes

### Cambiar Duraciones de Fases

**Opción 1: Desde Interfaz Web** ← Recomendado
1. Ir a http://192.168.4.1
2. Cambiar valores en "Duraciones (ms)"
3. Guardar

**Opción 2: Desde Código**
```cpp
// En config.h
const unsigned long DEFAULT_DURACION_AMANECER = 20000;  // 20 segundos
```

### Cambiar Colores

**Opción 1: Desde Interfaz Web** ← Recomendado
1. Ir a http://192.168.4.1
2. Ajustar sliders de color
3. Guardar

**Opción 2: Desde Código**
```cpp
// En config.h (predeterminados)
phaseManager.setPhaseColor(Fase::AMANECER, Color(255, 150, 0, 0));
```

### Agregar Nuevos Efectos

```cpp
// En include/EffectManager.h
void triggerStrobe() {
  for (int i = 0; i < 5; i++) {
    lightController->turnOff();
    delay(100);
    lightController->setColor(phaseManager->getCurrentColor());
    delay(100);
  }
}

// En main.cpp
effectManager.triggerStrobe();
```

## 🎯 Comandos PlatformIO Útiles

```bash
# Compilar
pio run

# Compilar para plataforma específica
pio run -e nodemcuv2

# Limpiar compilación
pio clean

# Cargar solo
pio run -t upload

# Monitor solo
pio run -t monitor

# Tests (si existen)
pio test

# Ver información del proyecto
pio info

# Actualizar librerías
pio lib update
```

## 📞 Support

Si encuentras problemas:

1. **Verificar conexiones físicas**
   - LEDs conectados correctamente
   - MP3 en los pines correctos
   - Alimentación adecuada

2. **Revisar logs serial**
   - Monitorear con `pio run -t monitor`
   - Buscar prefijos `[ERROR]` o `[WARN]`

3. **Verificar configuración**
   - Revisar `config.h`
   - Comprobar valores en EEPROM

4. **Reset hardware**
   - Reiniciar ESP8266
   - Flash completo: `pio run -t erase`

## ✅ Checklist de Verificación

Después de instalar:

- [ ] Compilación sin errores
- [ ] Carga en NodeMCU correcta
- [ ] LEDs se encienden
- [ ] WiFi "LuminaConfig" aparece
- [ ] Acceso a http://192.168.4.1
- [ ] Cambio de colores funciona
- [ ] Guardado en EEPROM funciona
- [ ] Transiciones suaves
- [ ] Audio MP3 suena

## 🎉 ¡Listo!

Tu sistema está funcionando. Ahora puedes:

1. **Personalizar**: Cambiar colores y duraciones
2. **Extender**: Agregar nuevos efectos
3. **Mejorar**: Optimizar para tus necesidades

---

**¿Problemas?** Ver README.md o REFACTORING.md

**¿Preguntas?** Revisar ejemplos en USAGE.md

**¡Disfruta tu Lumina! 🌅**
