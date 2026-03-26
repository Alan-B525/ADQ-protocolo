# ADR-0001: Radio y estrategia MAC para v1

- Estado: Aceptado
- Fecha: 2026-03-26

## Contexto

Se abandono el camino anterior y se unifico la direccion tecnica del proyecto para una v1 ejecutable en corto plazo.

Condiciones actuales:

- hardware objetivo: nRF52840
- banda: 2.4 GHz
- alcance objetivo: 100 m LOS
- foco: determinismo, confiabilidad y evidencia de campo

## Decision

1. Mantener topologia estrella (nodos -> base).
2. Implementar MAC propietario ESB-like con slots deterministas.
3. Sincronizar nodos por beacon periodico.
4. Usar ACK/NACK + timeout + reintentos acotados.
5. Mantener fragmentacion/reensamble para desacoplar payload ADQ de payload RF.

## Justificacion

- Control total de comportamiento temporal para evitar colisiones.
- Mejor trazabilidad de fallos por secuencia y razones NACK.
- Implementacion portable en C con limites claros para validacion.
- Coherencia con objetivo de 100 m sin sobre-ingenieria de largo alcance.

## Consecuencias

- La integracion RF real nRF52840 es el bloque tecnico prioritario.
- Todas las pruebas de campo se centran en 10/50/100 m.
- No se considera mesh ni otras topologias en v1.
- Toda documentacion debe alinearse a esta decision.
