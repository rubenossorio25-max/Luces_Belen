# 📱 Configuración Remota por WiFi

## Características

El sistema ahora incluye un servidor web integrado en el ESP32 que permite configurar todas las opciones del programa desde un teléfono Android a través de WiFi.

### Funcionalidades Disponibles

✅ **Vista de Estado en Tiempo Real**
- Fase actual (Amanecer, Día, Atardecer, Noche)
- Progreso de la fase actual
- Indicador de transición

✅ **Control de Fases**
- Cambiar manualmente entre fases
- Botones rápidos para cada fase

✅ **Configuración Remota**
- Ajustar duración de cada fase
- Guardar configuración en la memoria del ESP32
- Recargar configuración desde el dispositivo

## Cómo Conectarte

### Paso 1: Enciende el ESP32
El sistema creará un punto de acceso WiFi (AP) automáticamente cuando inicie.

### Paso 2: Busca la Red WiFi
En tu teléfono Android:
- Ve a **Configuración > WiFi**
- Busca la red: **`Luces_Belen`**
- La contraseña es: **`12345678`**

### Paso 3: Accede a la Interfaz Web
- Abre un navegador (Chrome, Firefox, Safari)
- Ingresa en la barra de dirección: **`192.168.1.100`**
- ¡Listo! Ya puedes configurar el sistema

## Interfaz Web

### 📊 Panel de Estado
Muestra en tiempo real:
- **Fase Actual**: La fase de iluminación activa
- **Progreso**: Barra de progreso con porcentaje
- **En Transición**: Indicador de transición entre fases

### 🎨 Control de Fases
Botones para cambiar instantáneamente entre:
- 🌅 Amanecer
- ☀️ Día
- 🌅 Atardecer
- 🌙 Noche

### ⚙️ Configuración
Modifica las duraciones de cada fase:
- **Duración Amanecer**: Tiempo que tarda la fase amanecer (en milisegundos)
- **Duración Día**: Tiempo que tarda la fase día
- **Duración Atardecer**: Tiempo que tarda la fase atardecer
- **Duración Noche**: Tiempo que tarda la fase noche

**Nota**: 1000 ms = 1 segundo

### Botones de Acción
- **💾 Guardar**: Guarda la configuración en la EEPROM
- **↻ Recargar**: Carga la configuración guardada

## Ejemplos de Configuración

### Ciclo Rápido (para pruebas)
```
Amanecer: 300000 ms (5 minutos)
Día: 600000 ms (10 minutos)
Atardecer: 300000 ms (5 minutos)
Noche: 600000 ms (10 minutos)
```

### Ciclo Normal (recomendado)
```
Amanecer: 1800000 ms (30 minutos)
Día: 3600000 ms (1 hora)
Atardecer: 1800000 ms (30 minutos)
Noche: 3600000 ms (1 hora)
```

### Ciclo Extendido
```
Amanecer: 3600000 ms (1 hora)
Día: 7200000 ms (2 horas)
Atardecer: 3600000 ms (1 hora)
Noche: 7200000 ms (2 horas)
```

## Solución de Problemas

### No encuentro la red WiFi
- Verifica que el ESP32 esté encendido
- Reinicia el ESP32
- Comprueba los logs por serial (velocidad 115200 baud)

### No puedo conectarme a 192.168.1.100
- Verifica que estés en la red `Luces_Belen`
- Intenta actualizar la página (Ctrl+R o F5)
- Prueba con otra aplicación navegadora

### Los cambios no se guardan
- Asegúrate de presionar el botón **💾 Guardar**
- Espera a ver el mensaje "Configuración guardada correctamente"
- Reinicia el ESP32 para verificar que se guardó

### La interfaz se ve extraña
- Prueba con otro navegador
- Borra el caché del navegador (Ctrl+Shift+Delete)
- Recarga la página completamente (Ctrl+Shift+R)

## Especificaciones Técnicas

- **Protocolo**: HTTP REST API
- **Puerto**: 80
- **SSID**: `Luces_Belen`
- **Contraseña**: `12345678`
- **IP del servidor**: `192.168.1.100`
- **Máximo de clientes**: 4

## API REST

Si quieres integrar con otras aplicaciones:

### GET /api/status
Obtiene el estado actual del sistema.

**Respuesta:**
```json
{
  "phase": 0,
  "progress": 45.5,
  "isTransition": false
}
```

### GET /api/config
Obtiene la configuración actual.

**Respuesta:**
```json
{
  "amanecer": 1800000,
  "dia": 3600000,
  "atardecer": 1800000,
  "noche": 3600000
}
```

### POST /api/config
Actualiza la configuración.

**Request:**
```json
{
  "amanecer": 1800000,
  "dia": 3600000,
  "atardecer": 1800000,
  "noche": 3600000
}
```

### POST /api/phase
Cambia la fase actual.

**Request:**
```json
{
  "phase": 0
}
```

Valores de fase:
- `0`: Amanecer
- `1`: Día
- `2`: Atardecer
- `3`: Noche

## Seguridad

⚠️ **Nota Importante**: 
- Este servidor no tiene autenticación
- Solo use en redes privadas de confianza
- No exponga el dispositivo a internet sin protección
- La contraseña WiFi es débil (cambiar en producción)

## Mejoras Futuras

Posibles mejoras:
- [ ] Autenticación por contraseña
- [ ] Conexión a WiFi existente (en lugar de crear AP)
- [ ] Guardado de múltiples presets de configuración
- [ ] Historial de cambios
- [ ] Control de brillo individual
- [ ] Control de música desde web
- [ ] Estadísticas de consumo
