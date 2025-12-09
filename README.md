# 🌅 Sistema Lumina - Controlador de Iluminación Inteligente

## 📋 Descripción

Sistema refactorizado para control inteligente de iluminación RGB(W) en NodeMCU ESP8266. Simula el ciclo natural del día (Amanecer → Día → Atardecer → Noche) con transiciones suaves, control mediante interfaz web, persistencia de configuración en EEPROM y efectos visuales especiales (rayos aleatorios en la noche).

## ✨ Características Principales

- **Gestión de Fases**: 4 fases del día con duraciones configurables
- **Transiciones Suaves**: Interpolación lineal entre colores
- **Control Web**: Portal cautivo con interfaz responsive
- **Persistencia**: Almacenamiento de configuración en EEPROM
- **Efectos Especiales**: Rayos aleatorios durante la noche
- **Reproducción de Audio**: Control de reproductor MP3 serial
- **Arquitectura Modular**: Clases bien definidas y reutilizables
- **Reporte de Estado**: Monitoreo vía puerto serial

## 🏗️ Arquitectura del Proyecto

```
Luces_Belen/
├── platformio.ini           # Configuración PlatformIO
├── src/
│   └── main.cpp            # Archivo principal
├── include/
│   ├── config.h            # Configuración global
│   ├── Color.h             # Estructura y utilidades de color
│   ├── Fase.h              # Definiciones de fases del día
│   ├── LightController.h   # Controlador de LEDs
│   ├── PhaseManager.h      # Gestor de fases y transiciones
│   ├── ConfigManager.h     # Persistencia en EEPROM
│   ├── MP3Player.h         # Control del reproductor MP3
│   ├── WebServer.h         # Servidor web y portal cautivo
│   └── EffectManager.h     # Efectos especiales
└── README.md               # Este archivo
```

## 🔧 Configuración de Hardware

### Pines ESP8266 (NodeMCU v2)

| Componente | Pin NodeMCU | GPIO | Función |
|-----------|------------|------|---------|
| LED Rojo | D0 | 16 | PWM Red |
| LED Verde | D7 | 13 | PWM Green |
| LED Azul | D1 | 5 | PWM Blue |
| LED Blanco | D2 | 4 | PWM White |
| MP3 RX | D6 | 12 | Serial RX (TX del MP3) |
| MP3 TX | D5 | 14 | Serial TX (RX del MP3) |

## 📦 Dependencias

- `ESP8266WiFi`: Conectividad WiFi
- `ESP8266WebServer`: Servidor web
- `DNSServer`: Portal cautivo DNS
- `SoftwareSerial`: Comunicación serie adicional
- `EEPROM`: Almacenamiento persistente

## 🚀 Compilación y Carga

```bash
pio run                    # Compilar
pio run -t upload          # Cargar firmware
pio run -t monitor         # Monitor serial
pio run -t upload -t monitor  # Todo junto
```

## 💻 Interfaz Web

**Red WiFi**: LuminaConfig  
**Contraseña**: configurar123  
**URL**: http://192.168.4.1

## 📝 Mejoras Principales vs Código Original

### ✅ Refactorización Completada

1. **Arquitectura Modular**
   - Separación de responsabilidades en clases específicas
   - Menor acoplamiento entre componentes
   - Mayor facilidad de mantenimiento y testing

2. **Manejo de Colores Mejorado**
   - Struct `Color` con métodos de interpolación
   - Soporte para RGBW completo
   - Operadores de comparación

3. **Gestor de Fases Robusto**
   - `PhaseManager` centraliza toda la lógica de fases
   - Transiciones automáticas
   - Cálculo de progreso y tiempo transcurrido

4. **Persistencia EEPROM**
   - `ConfigManager` dedicado
   - Validación de datos
   - Posibilidad de resetear a valores por defecto

5. **Control Web Mejorado**
   - Interfaz HTML/CSS más moderna
   - Clase `WebServer` encapsulada
   - Portal cautivo funcional

6. **Efectos Dinámicos**
   - `EffectManager` para rayos y otros efectos
   - Fácil agregar nuevos efectos

7. **Documentación y Logging**
   - Prefijos en mensajes serial para identificar origen
   - Información clara de estado
   - Función de reporte periódico

### 📊 Comparación de Líneas de Código

| Aspecto | Original | Refactorizado | Cambio |
|---------|----------|---------------|--------|
| Líneas principales | ~1000 | ~400 | -60% |
| Headers modularizados | 1 | 9 | +800% |
| Documentación | ~50 | ~500 | +900% |
| Clases definidas | 1 | 7 | +600% |

## 🔄 Flujo de Ejecución

```
SETUP → Inicializar todas las clases
LOOP (cada 10ms)
  ├─ Procesar web requests
  ├─ Actualizar efectos
  ├─ Gestionar transiciones de fase
  ├─ Reportar estado (cada 1s)
  └─ Delay 10ms
```

## 🎨 Colores Predeterminados (RGBW)

- **Amanecer**: (255, 150, 0, 0) - Naranja
- **Día**: (0, 0, 0, 255) - Blanco
- **Atardecer**: (200, 50, 20, 0) - Rojo
- **Noche**: (10, 10, 50, 0) - Azul

## 💡 Ventajas de la Nueva Arquitectura

✨ **Código más limpio y legible**  
✨ **Fácil de extender con nuevas funcionalidades**  
✨ **Mejor manejo de errores**  
✨ **Código documentado con comentarios**  
✨ **Separación de configuración y lógica**  
✨ **Pruebas unitarias más simples**  
✨ **Reutilización de código**  

---

**Versión**: 2.0 Refactorizada  
**Última actualización**: Diciembre 2025
