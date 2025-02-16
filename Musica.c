#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2818b.pio.h"

#include "Notas.h" 
// Definição do pino do buzzer
#define BUZZER_PIN 21

// Definição do número de LEDs e pino.
#define LED_COUNT 25
#define LED_PIN 7

typedef struct pixel_t pixel_t;
typedef pixel_t npLED_t;

npLED_t leds[LED_COUNT];

PIO np_pio;
uint sm;

void npInit(uint pin) {

    // Cria programa PIO.
    uint offset = pio_add_program(pio0, &ws2818b_program);
    np_pio = pio0;
   
    // Toma posse de uma máquina PIO.
    sm = pio_claim_unused_sm(np_pio, false);
    if (sm < 0) {
      np_pio = pio1;
      sm = pio_claim_unused_sm(np_pio, true); // Se nenhuma máquina estiver livre, panic!
    }
   
    // Inicia programa na máquina PIO obtida.
    ws2818b_program_init(np_pio, sm, offset, pin, 800000.f);
   
    // Limpa buffer de pixels.
    for (uint i = 0; i < LED_COUNT; ++i) {
      leds[i].R = 0;
      leds[i].G = 0;
      leds[i].B = 0;
    }
}

void npSetLED(const uint index, const uint8_t r, const uint8_t g, const uint8_t b) {
    leds[index].R = (uint8_t)(r * 0.1);
    leds[index].G = (uint8_t)(g * 0.1);
    leds[index].B = (uint8_t)(b * 0.1);
}

void npClear() {
    for (uint i = 0; i < LED_COUNT; ++i){
      npSetLED(i, 0, 0, 0);
    }
    npWrite();

}

void npWrite() {
    // Escreve cada dado de 8-bits dos pixels em sequência no buffer da máquina PIO.
    for (uint i = 0; i < LED_COUNT; ++i) {
      pio_sm_put_blocking(np_pio, sm, leds[i].G);
      pio_sm_put_blocking(np_pio, sm, leds[i].R);
      pio_sm_put_blocking(np_pio, sm, leds[i].B);
    }
}

// Configurações PWM
const float DIVISOR_CLK_PWM = 16.0;      // Divisor de clock do PWM
const uint16_t DEFAULT_WRAP = 5000;      // Valor padrão de wrap para PWM

// Inicializa o PWM no buzzer
void setup_audio() {
    gpio_set_function(BUZZER_PIN, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(BUZZER_PIN);
    pwm_set_clkdiv(slice, DIVISOR_CLK_PWM);
}

// Função para tocar uma nota
void play_note(uint pin, uint16_t wrap) {
    uint slice = pwm_gpio_to_slice_num(pin);
    pwm_set_wrap(slice, wrap);
    pwm_set_gpio_level(pin, wrap / 2); // Duty cycle de 50%
    pwm_set_enabled(slice, true);

    for (int i = 0; i < sizeof(note_mappings) / sizeof(NoteMapping); i++) {
        if (wrap == note_mappings[i].note) {
            npSetLED(note_mappings[i].position, note_mappings[i].color.R, note_mappings[i].color.G, note_mappings[i].color.B);
            npWrite();  // Atualiza os LEDs
            break;
        }
    }
    npClear();
}

// Função para silenciar o buzzer
void play_rest(uint pin) {
    uint slice = pwm_gpio_to_slice_num(pin);
    pwm_set_enabled(slice, false);

}

void play_music(const uint* notes, const uint* durations, size_t num_notes) {
    size_t i = 0;
    while (i < num_notes) {
        if (notes[i] != PAUSE) {
            play_note(BUZZER_PIN, notes[i]);  // Toca a nota
        }
        sleep_ms(durations[i]);  // Espera o tempo correspondente
        play_rest(BUZZER_PIN);  // Desliga o buzzer (PWM)
        sleep_ms(10);  // Pequena pausa entre as notas
        i++;  // Avança para a próxima nota
    }

    // Desliga o buzzer após a música acabar
    play_rest(BUZZER_PIN);
    npClear();
}

// Função principal
int main() {
    stdio_init_all();
    setup_audio();
    npInit(LED_PIN);
    npClear();
    play_music(megalovania_refrao, megalovania_refrao_durations, sizeof(megalovania_refrao) / sizeof(megalovania_refrao[0]));

    npClear();
    return 0;
}
