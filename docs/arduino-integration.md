# Arduino Integration Guide

## ¿Esto sirve como libreria para dos placas Arduino?

Si. Ahora tienes una libreria Arduino lista en:

- [arduino/ADQProtocol](arduino/ADQProtocol)

Incluye:

- empaquetado Arduino (`library.properties`)
- codec de tramas
- CRC16
- ring buffer
- helper de sincronizacion temporal
- ejemplos de nodo y base

## Instalacion en Arduino IDE

1. Copia la carpeta [arduino/ADQProtocol](arduino/ADQProtocol) dentro de tu carpeta de librerias de Arduino.
   Ejemplo en Windows: `Documentos/Arduino/libraries/ADQProtocol`
2. Reinicia Arduino IDE.
3. Verifica en menu de ejemplos: `File > Examples > ADQProtocol`.

## Flujo minimo con dos placas

1. Placa A (nodo): cargar ejemplo [arduino/ADQProtocol/examples/NodeTxDemo/NodeTxDemo.ino](arduino/ADQProtocol/examples/NodeTxDemo/NodeTxDemo.ino)
2. Placa B (base): cargar ejemplo [arduino/ADQProtocol/examples/BaseRxDemo/BaseRxDemo.ino](arduino/ADQProtocol/examples/BaseRxDemo/BaseRxDemo.ino)
3. Reemplazar transporte Serial por tu modulo RF real (LoRa, FSK, etc.).

## Como implementarlo en tu hardware real

### En nodo

- Leer ADC strain gauge.
- Armar payload en `adq_frame_t`.
- Codificar con `ADQProtocol::encode(...)`.
- Enviar bytes por radio.
- Esperar ACK y reintentar si aplica.

### En basestation

- Recibir bytes desde radio.
- Decodificar con `ADQProtocol::decode(...)`.
- Validar `msg_type`, `seq`, `timestamp`.
- Emitir ACK/NACK al nodo.
- Publicar por USB al PC.

## Recomendacion practica

Para una primera prueba estable con Arduino:

- usa dos placas con suficiente RAM (ESP32, STM32, RP2040, o AVR Mega)
- prueba primero punto a punto a corta distancia
- luego activa ACK/reintentos y finalmente sincronizacion por beacon

## Nota

El ejemplo BaseRxDemo es intencionalmente simple y no incluye un parser de stream robusto. En la siguiente iteracion te puedo implementar un parser por maquina de estados para tramas fragmentadas.
