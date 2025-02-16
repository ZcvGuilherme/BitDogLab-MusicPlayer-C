# BitDogLab MusicPlayer C

 ## Descrição
BitDogLab-MusicPlayer-C é um projeto desenvolvido em C para o Raspberry Pi Pico que utiliza um buzzer e LEDs Neopixel para reproduzir músicas com sincronização visual e sonora. O projeto permite tocar músicas enquanto os led's brilham de acordo com as notas que estão sendo tocadas.

 ## Requisitos
- Raspberry Pi Pico
- Buzzer Piezoelétrico
- LEDs Neopixel (WS2812B)
- Fonte de alimentação compatível
- Cabos de conexão

 ## Configuração do Hardware
- **Buzzer:** Conectado ao GPIO 21
- **LEDs Neopixel:** Conectados ao GPIO 7
- **Alimentação:** 5V para os LEDs e 3.3V para o Raspberry Pi Pico

 ## Descrição do Código
 ### Bibliotecas Utilizadas
 - stdio.h
 - pico/stdlib.h
 - hardware/pwm.h
 - hardware/pio.h
 - hardware/clock.h
 - ws2818b.pio.h
 - Notas.h
   
 ### Definição de elementos (Notas.h)
 O arquivo Notas.h define as notas e suas respectivas frequencias (Hz).
 Além disso, ele define estruturas como pixel_t e NoteMapping, responsáveis por levar as informações do RGB e posição das notas.
 Após isso, são definidos os tempos de duração das notas, tendo desde a Semibreve (2 seg) até a Fusa (62 ms), também são definidos os tempos pontuados (NOME_DO_TEMPO_P), que simbolizam o tempo + metade do valor.
 Depois apenas algumas músicas que eu fiz, com o intuito de demonstrar o uso, a estrutura para criar sua música é:
- const uint nome_da_musica_notes[]
- const uint nome_da_musica_durations[]
 Como o nome sugere, o primeiro array contém as notas, e no segundo array vem a duração de cada nota.

### Principais Métodos

#### Utilização dos Leds RGB
O principal método é o npInit, que recebe como parâmetro o pino dos Leds, por padrão fica o 7, e ele tem por responsabilidade fazer a configuração inicial dos leds

``` C
void npInit(uint pin) {


    uint offset = pio_add_program(pio0, &ws2818b_program);
    np_pio = pio0;

    sm = pio_claim_unused_sm(np_pio, false);
    if (sm < 0) {
      np_pio = pio1;
      sm = pio_claim_unused_sm(np_pio, true); 
    }
   

    ws2818b_program_init(np_pio, sm, offset, pin, 800000.f);

    for (uint i = 0; i < LED_COUNT; ++i) {
      leds[i].R = 0;
      leds[i].G = 0;
      leds[i].B = 0;
    }
}
```


npSetLed tem por função ligar o led, recebe como parametros a posição do led e as informações do RGB
``` C
void npSetLED(const uint index, const uint8_t r, const uint8_t g, const uint8_t b) {
    leds[index].R = (uint8_t)(r * 0.1);
    leds[index].G = (uint8_t)(g * 0.1);
    leds[index].B = (uint8_t)(b * 0.1);
}
```


npClear simplesmente limpa a matriz
``` C
void npWrite() {
    // Escreve cada dado de 8-bits dos pixels em sequência no buffer da máquina PIO.
    for (uint i = 0; i < LED_COUNT; ++i) {
      pio_sm_put_blocking(np_pio, sm, leds[i].G);
      pio_sm_put_blocking(np_pio, sm, leds[i].R);
      pio_sm_put_blocking(np_pio, sm, leds[i].B);
    }
}
```










