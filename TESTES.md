# Relatório de Testes - Pisca LED Baremetal

## Ambiente de Teste
- **Placa:** FRDM-KL25Z
- **Framework:** Zephyr RTOS (via PlatformIO)
- **Abordagem:** Super-Loop Clássico (Baremetal)

## Objetivo e Lógica
O firmware foi desenvolvido para controlar dois canais do LED RGB integrado de forma assíncrona, sem a utilização de Threads do RTOS. A lógica implementa um paralelismo não-preemptivo utilizando um laço infinito (`while(1)`) e uma contagem baseada em "ticks" de 50 ms.

## Comportamento Validado
- **LED 0 (Vermelho):** Configurado para alternar o estado a cada 1000 milissegundos.
- **LED 1 (Verde - Desafio):** Configurado para alternar o estado a cada 350 milissegundos.

## Resultado
- [x] Código estruturado com sucesso utilizando polling de tempo.
- [x] Compilação (`build`) executada sem erros no PlatformIO.