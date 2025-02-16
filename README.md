# BitDogLab MusicPlayer C

 # Descrição
BitDogLab-MusicPlayer-C é um projeto desenvolvido em C para o Raspberry Pi Pico que utiliza um buzzer e LEDs Neopixel para reproduzir músicas com sincronização visual e sonora. O projeto permite tocar músicas enquanto os led's brilham de acordo com as notas que estão sendo tocadas.

 # Requisitos
- Raspberry Pi Pico
- Buzzer Piezoelétrico
- LEDs Neopixel (WS2812B)
- Fonte de alimentação compatível
- Cabos de conexão

 # Configuração do Hardware
- **Buzzer:** Conectado ao GPIO 21
- **LEDs Neopixel:** Conectados ao GPIO 7
- **Alimentação:** 5V para os LEDs e 3.3V para o Raspberry Pi Pico

 # Descrição do Código
 ### Bibliotecas Utilizadas
 - stdio.h
 - pico/stdlib.h
 - hardware/pwm.h
 - hardware/pio.h
 - hardware/clock.h
 - ws2818b.pio.h
 - Notas.h
   
 ### Definição de elementos (Notas.h)
 O arquivo `Notas.h` contém a definição das notas musicais e suas respectivas frequências em Hz. Além disso, são declaradas estruturas como `pixel_t` e `NoteMapping`, responsáveis por armazenar informações sobre a cor (RGB) e a posição das notas.

O arquivo também especifica os tempos de duração das notas, abrangendo desde a Semibreve (2 segundos) até a Fusa (62 ms). Os tempos pontuados (por exemplo, `NOME_DO_TEMPO_P`) representam a duração de uma nota somada à metade do seu valor original.

Para exemplificar o uso dessas definições, o arquivo inclui algumas músicas de demonstração. A estrutura para criar sua própria música é simples:

`const uint nome_da_musica_notes[]`: Este array contém as notas musicais.
`const uint nome_da_musica_durations[]`: Este array define a duração de cada nota correspondente.
Com isso, o usuário pode facilmente customizar suas músicas, definindo notas e durações de forma eficiente.

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
A função `play_note()` é responsável por gerar uma nota musical no buzzer utilizando PWM. O método recebe os seguintes parâmetros:

- `uint pin` : O pino do buzzer conectado à placa.
- `uint16_t wrap`: O valor que define o intervalo de cada nota, representando a frequência correspondente.
Dentro do método, o valor de `wrap` ajusta a frequência do buzzer e ativa os LEDs correspondentes à nota tocada. O ciclo de trabalho do PWM é fixado em 50% para gerar um som audível.
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
A função `play_music()` é a principal responsável por tocar uma sequência de notas musicais. Ela utiliza as funções anteriores para tocar cada nota da música de acordo com sua duração. Os parâmetros dessa função são:

- `notes` : Um array contendo as notas musicais representadas pelos valores PWM.
- `durations` : Um array com os tempos de duração de cada nota.
- `num_notes` : O número total de notas na música.
A função percorre as notas, tocando cada uma no buzzer com a duração apropriada e silenciando o buzzer brevemente entre elas.

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
No método principal, inicializamos os componentes e chamamos a função `play_music()` para tocar uma música previamente definida. Você pode selecionar uma música existente ou criar a sua própria, contanto que siga a estrutura esperada (com arrays de notas e durações). A função `sizeof(megalovania_refrao) / sizeof(megalovania_refrao[0])` é utilizada para calcular o número de notas na música.

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
## Instalação
# 1. **Clone o repositório**
Clone o repositório para seu ambiente local usando Git:
``` git
git clone https://github.com/usuario/repo.git
cd repo
```
# 2. **Instale as dependências**
Este projeto utiliza a biblioteca `pico-sdk` para o Raspberry Pi. Para instalar as dependências siga os seguintes passos:
1. instalar o `pico-sdk` (se necessário)
```bash
cd ~
git clone https://github.com/raspberrypi/pico-sdk.git
cd pico-sdk
git submodule update --init --recursive

export PICO_SDK_PATH=~/pico-sdk

source ~/.bashrc
```

2. Instalar o `CMake` e outras depêndencias
Você precisará do CMake para compilar o código:
``` bash
sudo apt update
sudo apt install cmake gcc-arm-none-eabi libnewlib-arm-none-eabi build-essential
```

# 3. **Compilação**
Com o ambiente configurado e as dependências instaladas, vá para o diretório do projeto e compile o código com o CMake:
```bash
mkdir build
cd build
```

``` bash
cmake ..
```

```bash
make
```

# 4. **Conectar o Hardware**
Conecte o Buzzer e os Leds aos pinos GPIO da sua placa. Certifique-se de que o buzzer está conectado ao pino configurado no código (`BUZZER_PIN`), faça o mesmo com os Led's.


#Créditos
Este código foi feito baseado na documentação da BitDogLab, quaisquer dúvidas, pode pesquisar em: https://github.com/BitDogLab/BitDogLab-C/tree/main

A lógica desse código foi feita baseada em outro código de minha autoria, caso tenha interesse: https://github.com/ZcvGuilherme/BitDogLab-MusicPlayer


