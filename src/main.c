#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

// No LED RGB integrado da FRDM-KL25Z:
// led0 = Vermelho (PTB18)
// led1 = Verde (PTB19)
#define LED0_NODE DT_ALIAS(led0)
#define LED1_NODE DT_ALIAS(led1)

static const struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
static const struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET(LED1_NODE, gpios);

int main(void)
{
    // Configura os pinos como saída ativa (inicia o LED ligado)
    gpio_pin_configure_dt(&led0, GPIO_OUTPUT_ACTIVE);
    gpio_pin_configure_dt(&led1, GPIO_OUTPUT_ACTIVE);

    int contador_led0 = 0;
    int contador_led1 = 0;

    // Super-Loop Baremetal
    while (1) {
        k_msleep(50); // Tick base de 50 ms

        contador_led0 += 50;
        contador_led1 += 50;

        // LED 0 (Vermelho) alterna a cada 1000 ms (1 segundo)
        if (contador_led0 >= 1000) {
            gpio_pin_toggle_dt(&led0);
            contador_led0 = 0;
        }

        // LED 1 (Verde) alterna a cada 350 ms (Desafio)
        if (contador_led1 >= 350) {
            gpio_pin_toggle_dt(&led1);
            contador_led1 = 0;
        }
    }

    return 0;
}