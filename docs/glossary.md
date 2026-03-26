# Glossary

Glosario operativo del enfoque vigente ADQ v1.

## Terminos clave

- Nodo: dispositivo remoto que adquiere datos y transmite.
- Base: dispositivo central que sincroniza, agenda slots y valida tramas.
- Host: PC que recibe, guarda y analiza resultados.
- nRF52840: SoC objetivo de radio para v1.
- ESB-like: enfoque propietario ligero de enlace/MAC en 2.4 GHz.
- Beacon: mensaje periodico de sincronizacion y control de slots.
- Slot: ventana temporal asignada para transmitir.
- Guard time: margen de seguridad temporal dentro del slot.
- ACK: confirmacion de recepcion correcta.
- NACK: rechazo con motivo para disparar reintento.
- Retry budget: cantidad maxima de reintentos permitidos.
- Recovery: estado del nodo para recobrar sincronizacion/estabilidad.
- Seq: contador de secuencia de tramas.
- CRC16: verificacion de integridad de trama.
- Fragmentacion: division de una trama grande en fragmentos RF.
- Reensamble: reconstruccion de la trama original desde fragmentos.
- Parser FSM: maquina de estados que consume stream y reconstruye tramas.
- PER: packet error rate.
- LOS: line of sight.

## Regla rapida

Si un termino no aparece aqui, agregarlo junto con su definicion corta en el mismo cambio donde se introduce.
