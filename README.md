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

#### Inicialização dos LEDs
O método `npInit()` configura os LEDs Neopixel no pino especificado. Ele inicializa a máquina de estado programável (PIO) para controlar os LEDs e limpa a matriz de pixels.


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


#### Definição de Cor para um LED
A função `npSetLED()` permite definir a cor de um LED específico na matriz de LEDs. Ela recebe como parâmetros a posição do LED e os valores RGB.

``` C
void npSetLED(const uint index, const uint8_t r, const uint8_t g, const uint8_t b) {
    leds[index].R = (uint8_t)(r * 0.1);
    leds[index].G = (uint8_t)(g * 0.1);
    leds[index].B = (uint8_t)(b * 0.1);
}
```


#### Atualização dos LEDs
A função `npWrite()` envia os dados de cor para os LEDs, garantindo que a atualização seja refletida fisicamente.

```c
void npWrite() {
    for (uint i = 0; i < LED_COUNT; ++i) {
      pio_sm_put_blocking(np_pio, sm, leds[i].G);
      pio_sm_put_blocking(np_pio, sm, leds[i].R);
      pio_sm_put_blocking(np_pio, sm, leds[i].B);
    }
}
```
### Utilização do Buzzer
#### Incialização do Buzzer
O método `setup_audio()` configura o pino do buzzer para funcionar com PWM (modularização por largura de pulso). Ela deve ser chamada no início para inicializar o hardware necessário.
``` C
void setup_audio() {
    gpio_set_function(BUZZER_PIN, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(BUZZER_PIN);
    pwm_set_clkdiv(slice, DIVISOR_CLK_PWM);
}
```

#### Método para tocar um som
Este método toca uma nota musical no buzzer com um ciclo de trabalho de 50% usando PWM, recebe como parâmetros:
- `uint pin` Pino do buzzer conectado
- `uint16_t wrap` Valor que define o intervalo de cada nota. Cada valor representa uma frequencia.

  O método ajusta o buzzer para a frequência correta e ativa os LEDS's correspondentes, dependendo da nota tocada.
``` C
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
```

#### Desligar o Buzzer
O método `play_rest(uint pin)`  é muito importante para garantir o tempo entre as notas e que o buzzer pare de tocar quando necessário, ele recebe como parâmetro apenas o pino do buzzer, com a única função de interromper qualquer som em produção.

``` C
void play_rest(uint pin) {
    uint slice = pwm_gpio_to_slice_num(pin);
    pwm_set_enabled(slice, false);

    npClear();
}
```
#### Tocar uma música
Por fim, o método `play_music` é a junção de todos os métodos anteriores, ele recebe como parâmetro:
- `notes` Um array de notas musicais representadas por valores PWM
- `durations` Um array com a duração de cada nota
- `num_notes` Número total de notas na música

Itera pelas notas e toca cada uma delas no buzzer, utilizando o `play_note`, aguardando a duração apropriada para cada uma. Após cada nota, o buzzer é silenciado brevemente antes de passar para a próxima nota.

``` C
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
```


### Método Principal
O método principal incializa os componentes e utiliza o método `play_music`, nesse momento, você pode selecionar uma música que já foi criada ou que você mesmo criou, contanto que siga a estrutura definida, com o único detalhe de você precisar passar o cálculo do tamanho da música diretamente no método.

``` C
int main() {
    stdio_init_all();
    setup_audio();
    npInit(LED_PIN);
    npClear();
    play_music(megalovania_refrao, megalovania_refrao_durations, sizeof(megalovania_refrao) / sizeof(megalovania_refrao[0]));

    npClear();
    return 0;
}
```






