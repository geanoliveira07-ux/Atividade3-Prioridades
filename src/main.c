#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/device.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(app_kl25z, LOG_LEVEL_INF);

#define LED_A_NODE DT_ALIAS(led0) // LED Verde onboard
#define LED_B_NODE DT_ALIAS(led2) // LED Vermelho onboard
#define SW0_NODE   DT_ALIAS(sw0)  // Botao / Pino PTA16

static const struct gpio_dt_spec ledA = GPIO_DT_SPEC_GET(LED_A_NODE, gpios);
static const struct gpio_dt_spec ledB = GPIO_DT_SPEC_GET(LED_B_NODE, gpios);
static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET_OR(SW0_NODE, gpios, {0});

static struct gpio_callback button_cb_data;

#define PRIO_THREAD_A 5
#define PRIO_THREAD_B 7
#define TEMPO_A_MS   1500
#define TEMPO_B_MS   1000

/* ISR: Interrupção do Botão */
void button_pressed_isr(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    LOG_INF("-> [ISR ATIVADA] Interrupcao disparada pelo Botao/Jumper!");
}

/* THREAD A - Curta, Alta Prioridade */
void thread_A(void *p1, void *p2, void *p3)
{
    while (1) {
        gpio_pin_set_dt(&ledA, 1);
        for (volatile int i = 0; i < 200000; i++) {}
        gpio_pin_set_dt(&ledA, 0);

        LOG_INF("Thread A rodou");
        k_msleep(TEMPO_A_MS);
    }
}

/* THREAD B - Longa, Baixa Prioridade */
void thread_B(void *p1, void *p2, void *p3)
{
    while (1) {
        gpio_pin_set_dt(&ledB, 1);
        for (volatile int i = 0; i < 5000000; i++) {}
        gpio_pin_set_dt(&ledB, 0);

        LOG_INF("Thread B rodou");
        k_msleep(TEMPO_B_MS);
    }
}

/* Pilha aumentada para 1024 bytes para evitar HardFault */
K_THREAD_DEFINE(a_tid, 1024, thread_A, NULL, NULL, NULL, PRIO_THREAD_A, 0, 0);
K_THREAD_DEFINE(b_tid, 1024, thread_B, NULL, NULL, NULL, PRIO_THREAD_B, 0, 0);

int main(void)
{
    if (!device_is_ready(ledA.port) || !device_is_ready(ledB.port)) {
        return 0;
    }

    gpio_pin_configure_dt(&ledA, GPIO_OUTPUT_INACTIVE);
    gpio_pin_configure_dt(&ledB, GPIO_OUTPUT_INACTIVE);

    /* Configuração do Botão com Pull-Up interno */
    if (device_is_ready(button.port)) {
        gpio_pin_configure_dt(&button, GPIO_INPUT | GPIO_PULL_UP);
        gpio_pin_interrupt_configure_dt(&button, GPIO_INT_EDGE_TO_ACTIVE);
        gpio_init_callback(&button_cb_data, button_pressed_isr, BIT(button.pin));
        gpio_add_callback(button.port, &button_cb_data);
    }

    LOG_INF("=== Sistema Zephyr RTOS Iniciado no PlatformIO ===");

    while (1) {
        k_sleep(K_FOREVER);
    }
    return 0;
}