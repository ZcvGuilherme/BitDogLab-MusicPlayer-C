#ifndef NOTAS_H
#define NOTAS_H

// Definição das frequências das notas musicais
#define do 29886
#define do_2 28294
#define re 26795
#define re_2 25386
#define mi 24063

#define fa 22823
#define fa_2 21663
#define sol 20579
#define sol_2 19569
#define la 18630

#define la_2 17758
#define si 16952
#define DO 15963
#define DO_2 14929
#define RE 14097

#define RE_2 13298
#define MI 12503
#define FA 11794
#define FA_2 11157
#define SOL 10529

#define SOL_2 9950
#define LA 9332
#define LA_2 8790
#define SI 8296
#define PAUSE 1


typedef struct pixel_t pixel_t;
typedef pixel_t npLED_t;

struct pixel_t {
    uint8_t G, R, B; // Três valores de 8-bits, cada um, compõem um pixel.
};

typedef struct {
    uint note;
    npLED_t color;
    uint position;
} NoteMapping;

//definindo as cores
// Definindo cores para as notas
#define COR_do     (npLED_t){0, 50, 0}     // Vermelho
#define COR_do_2   (npLED_t){0, 120, 0}   // Vermelho claro
#define COR_re     (npLED_t){0, 180, 0}   // Laranja
#define COR_re_2   (npLED_t){0, 240, 0} // Laranja claro
#define COR_mi     (npLED_t){0, 200, 80}     // Verde

#define COR_fa     (npLED_t){0, 0, 60}     // Azul
#define COR_fa_2   (npLED_t){0, 0, 140} // Azul claro
#define COR_sol    (npLED_t){0, 0, 200}    // Índigo
#define COR_sol_2  (npLED_t){0, 100, 255}   // Roxo
#define COR_la     (npLED_t){0, 180, 220} // Violeta

#define COR_la_2   (npLED_t){40, 0, 80}   // Magenta
#define COR_si     (npLED_t){80, 0, 160}  // Deep pink
#define COR_DO     (npLED_t){120, 0, 220} 
#define COR_DO_2   (npLED_t){180, 100, 255} 
#define COR_RE   (npLED_t){140, 60, 190}   // Laranja mais claro

#define COR_RE_2   (npLED_t){60, 0, 0}
#define COR_MI   (npLED_t){140, 0, 0}
#define COR_FA   (npLED_t){200, 20, 20}
#define COR_FA_2   (npLED_t){255, 50, 50}    // Verde mais claro
#define COR_SOL   (npLED_t){220, 80, 120} 

#define COR_SOL_2   (npLED_t){90, 40, 0} 
#define COR_LA   (npLED_t){160, 80, 0} 
#define COR_LA_2   (npLED_t){220, 140, 0} 
#define COR_SI   (npLED_t){255, 190, 80} 
#define COR_PAUSE  (npLED_t){0, 0, 0}       // LED apagado (pausa)



// Tabela de mapeamento das notas para cores e posições
NoteMapping note_mappings[] = {
    {do, COR_do, 0},       // Nota do mapeada para o LED 0
    {do_2, COR_do_2, 1},   // Nota do_2 mapeada para o LED 1
    {re, COR_re, 2},       // Nota re mapeada para o LED 2
    {re_2, COR_re_2, 3},   // Nota re_2 mapeada para o LED 3
    {mi, COR_mi, 4},       // Nota mi mapeada para o LED 4

    {fa, COR_fa, 5},       // Nota fa mapeada para o LED 5
    {fa_2, COR_fa_2, 6},   // Nota fa_2 mapeada para o LED 6
    {sol, COR_sol, 7},     // Nota sol mapeada para o LED 7
    {sol_2, COR_sol_2, 8}, // Nota sol_2 mapeada para o LED 8
    {la, COR_la, 9},       // Nota la mapeada para o LED 9

    {la_2, COR_la_2, 10},  // Nota la_2 mapeada para o LED 10
    {si, COR_si, 11},      // Nota si mapeada para o LED 11
    {DO, COR_DO, 12},    // Nota DO mapeada para o LED 12
    {DO_2, COR_DO_2, 13},  // Nota DO_2 mapeada para o LED 13
    {RE, COR_RE, 14},    // Nota RE mapeada para o LED 14

    {RE_2, COR_RE_2, 15},  // Nota RE_2 mapeada para o LED 15
    {MI, COR_MI, 16},    // Nota MI mapeada para o LED 16
    {FA, COR_FA, 17},      // Nota FA mapeada para o LED 17
    {FA_2, COR_FA_2, 18},  // Nota FA_2 mapeada para o LED 18
    {SOL, COR_SOL, 19},    // Nota SOL mapeada para o LED 19

    {SOL_2, COR_SOL_2, 20},// Nota SOL_2 mapeada para o LED 20
    {LA, COR_LA, 21},      // Nota LA mapeada para o LED 21
    {LA_2, COR_LA_2, 22},  // Nota LA_2 mapeada para o LED 22
    {SI, COR_SI, 23},      // Nota SI mapeada para o LED 23
    {PAUSE, COR_PAUSE, 24} // Pausa (LED apagado) mapeada para o LED 24
};



// Definição das durações das notas
#define SEMIBREVE      2000   // Semibreve (4 tempos)
#define SEMIBREVE_P    3000   // Semibreve pontuada (4 + 2 = 6 tempos)

#define MINIMA         1000   // Mínima (2 tempos)
#define MINIMA_P       1500   // Mínima pontuada (2 + 1 = 3 tempos)

#define SEMINIMA       500    // Semínima (1 tempo)
#define SEMINIMA_P     750    // Semínima pontuada (1 + 0.5 = 1.5 tempos)

#define COLCHEIA       250    // Colcheia (meio tempo)
#define COLCHEIA_P     375    // Colcheia pontuada (0.5 + 0.25 = 0.75 tempos)

#define SEMICOLCHEIA   125    // Semicolcheia (um quarto de meio tempo)
#define SEMICOLCHEIA_P 187    // Semicolcheia pontuada (0.25 + 0.125 = 0.375 tempos)

#define FUSA            62    // Fusa (1/8 de semicolcheia)
#define FUSA_P          93    // Fusa pontuada (1/8 + 1/16 = 3/16 tempos)












const uint star_wars_notes[] = {
    MI, MI, MI, do, sol, DO, MI, DO,
    sol, DO, MI, MI, MI, MI, fa, do,
    sol_2, fa, MI, do, sol, DO, MI, do,
    sol, DO, MI, MI, MI, fa, DO,
    sol_2, fa, MI, DO, si, la, sol, MI,
    MI, SOL, MI, DO, si, la, sol, MI,
    MI, MI, SOL, LA, FA, SOL, MI,
    DO, si, la, sol, MI, SOL, MI, DO,
    si, la, sol, MI, MI, DO, MI, do,
    MI, re, si, do, LA, do, MI, do,
    MI, re, si, do, MI, sol, DO, la,
    fa, MI, MI, SOL, MI, DO, si, la,
    sol, MI, SOL, MI, DO, si, la, sol
};

const uint star_wars_durations[] = {
    SEMINIMA, SEMINIMA, SEMINIMA, SEMINIMA_P, COLCHEIA, MINIMA, SEMINIMA, SEMINIMA_P,
    COLCHEIA, MINIMA, SEMINIMA, SEMINIMA, SEMINIMA, SEMINIMA, SEMINIMA_P, COLCHEIA,
    MINIMA, SEMINIMA, SEMINIMA_P, COLCHEIA, SEMINIMA, SEMINIMA, SEMINIMA, SEMINIMA_P,
    SEMINIMA, SEMINIMA, SEMINIMA, SEMINIMA_P, SEMINIMA, SEMINIMA, SEMINIMA, COLCHEIA,
    SEMINIMA, SEMINIMA, SEMINIMA, SEMINIMA_P, SEMINIMA, SEMINIMA, SEMINIMA, SEMINIMA,
    SEMINIMA_P, COLCHEIA, SEMINIMA, COLCHEIA, SEMINIMA_P, SEMINIMA, SEMINIMA_P, SEMINIMA,
    SEMINIMA_P, SEMINIMA, SEMINIMA_P, SEMINIMA, SEMINIMA, SEMINIMA, SEMINIMA, SEMINIMA
};

const uint megalovania_refrao[] = {
    // Primeira parte
    mi, mi, MI, si, PAUSE, la_2, la, sol, PAUSE, mi, 
    sol, la, PAUSE,

    // Segunda parte
    re, re, MI, si, PAUSE, la_2, la, sol, PAUSE, mi, 
    sol, la, PAUSE,

    // Terceira parte
    do_2, do_2, MI, si, PAUSE, la_2, la, sol, PAUSE, mi, 
    sol, la, PAUSE,

    // Quarta parte
    do, do, MI, si, PAUSE, la_2, la, sol, PAUSE, mi, 
    sol, la, PAUSE
};

const uint megalovania_refrao_durations[] = {
    // Primeira parte
    SEMICOLCHEIA, SEMICOLCHEIA, COLCHEIA, COLCHEIA, SEMICOLCHEIA, 
    COLCHEIA, COLCHEIA, COLCHEIA, FUSA, SEMICOLCHEIA, 
    SEMICOLCHEIA, SEMICOLCHEIA, FUSA,

    // Segunda parte
    SEMICOLCHEIA, SEMICOLCHEIA, COLCHEIA, COLCHEIA, SEMICOLCHEIA, 
    COLCHEIA, COLCHEIA, COLCHEIA, FUSA, SEMICOLCHEIA, 
    SEMICOLCHEIA, SEMICOLCHEIA, FUSA,

    // Terceira parte
    SEMICOLCHEIA, SEMICOLCHEIA, COLCHEIA, COLCHEIA, SEMICOLCHEIA, 
    COLCHEIA, COLCHEIA, COLCHEIA, FUSA, SEMICOLCHEIA, 
    SEMICOLCHEIA, SEMICOLCHEIA, FUSA,

    // Quarta parte
    SEMICOLCHEIA, SEMICOLCHEIA, COLCHEIA, COLCHEIA, SEMICOLCHEIA, 
    COLCHEIA, COLCHEIA, COLCHEIA, FUSA, SEMICOLCHEIA, 
    SEMICOLCHEIA, SEMICOLCHEIA, FUSA
};

#endif // NOTAS_H
