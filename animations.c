#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/adc.h"
#include "pico/bootrom.h"

// arquivo .pio
#include "pio_matrix.pio.h"

#define NUM_PIXELS 25

// pino do led 5x5
#define OUT_PIN 7

#define BUZZER_PIN 28

extern uint32_t matrix_rgb(double b, double r, double g);

// ########################################### ANIMAÇÃO DE UM OLHO #########################################
void animacao_olho(PIO pio, uint sm)
{
    // Definição dos frames iniciais para o olho
    double frames[21][25] = {
        {0, 0, 0, 0, 0,
         0, 0, 1, 0, 0,
         0, 1, 1, 1, 0,
         0, 0, 1, 0, 0,
         0, 0, 0, 0, 0},

        {0, 0, 0, 0, 0,
         0, 1, 0, 0, 0,
         0, 0, 1, 1, 1,
         0, 1, 0, 0, 0,
         0, 0, 0, 0, 0},

        {0, 0, 0, 0, 0,
         0, 0, 1, 0, 0,
         0, 1, 1, 1, 0,
         0, 0, 1, 0, 0,
         0, 0, 0, 0, 0},

        {0, 0, 0, 0, 0,
         0, 0, 0, 1, 0,
         1, 1, 1, 0, 0,
         0, 0, 0, 1, 0,
         0, 0, 0, 0, 0},

        {0, 0, 0, 0, 0,
         0, 0, 1, 0, 0,
         0, 1, 1, 1, 0,
         0, 0, 1, 0, 0,
         0, 0, 0, 0, 0},

        {0, 0, 1, 0, 0,
         0, 1, 1, 1, 0,
         0, 0, 1, 0, 0,
         0, 0, 0, 0, 0,
         0, 0, 0, 0, 0},

        {0, 0, 0, 0, 0,
         0, 0, 1, 0, 0,
         0, 1, 1, 1, 0,
         0, 0, 1, 0, 0,
         0, 0, 0, 0, 0},

        {0, 0, 0, 0, 0,
         0, 0, 0, 0, 0,
         0, 0, 1, 0, 0,
         0, 1, 1, 1, 0,
         0, 0, 1, 0, 0},

        {0, 0, 0, 0, 0,
         0, 0, 1, 0, 0,
         0, 1, 1, 1, 0,
         0, 0, 1, 0, 0,
         0, 0, 0, 0, 0},

        {0, 1, 0, 0, 0,
         0, 0, 1, 1, 1,
         0, 1, 0, 0, 0,
         0, 0, 0, 0, 0,
         0, 0, 0, 0, 0},

        {0, 0, 0, 0, 0,
         0, 0, 1, 0, 0,
         0, 1, 1, 1, 0,
         0, 0, 1, 0, 0,
         0, 0, 0, 0, 0},

        {0, 0, 0, 1, 0,
         1, 1, 1, 0, 0,
         0, 0, 0, 1, 0,
         0, 0, 0, 0, 0,
         0, 0, 0, 0, 0},

        {0, 0, 0, 0, 0,
         0, 0, 1, 0, 0,
         0, 1, 1, 1, 0,
         0, 0, 1, 0, 0,
         0, 0, 0, 0, 0},

        {0, 0, 0, 0, 0,
         0, 0, 0, 0, 0,
         0, 1, 0, 0, 0,
         0, 0, 1, 1, 1,
         0, 1, 0, 0, 0},

        {0, 0, 0, 0, 0,
         0, 0, 1, 0, 0,
         0, 1, 1, 1, 0,
         0, 0, 1, 0, 0,
         0, 0, 0, 0, 0},

        {0, 0, 0, 0, 0,
         0, 0, 0, 0, 0,
         0, 0, 0, 1, 0,
         1, 1, 1, 0, 0,
         0, 0, 0, 1, 0},

        {0, 0, 0, 0, 0,
         0, 0, 1, 0, 0,
         0, 1, 1, 1, 0,
         0, 0, 1, 0, 0,
         0, 0, 0, 0, 0},

        {0, 0, 0, 0, 0,
         0, 0, 1, 0, 0,
         0, 1, 0, 1, 0,
         0, 0, 1, 0, 0,
         0, 0, 0, 0, 0},

        {0, 0, 0, 0, 0,
         0, 0, 0, 0, 0,
         0, 1, 1, 1, 0,
         0, 0, 0, 0, 0,
         0, 0, 0, 0, 0},

        {0, 0, 0, 0, 0,
         0, 0, 1, 0, 0,
         0, 1, 0, 1, 0,
         0, 0, 1, 0, 0,
         0, 0, 0, 0, 0},

        {0, 0, 0, 0, 0,
         0, 0, 1, 0, 0,
         0, 1, 1, 1, 0,
         0, 0, 1, 0, 0,
         0, 0, 0, 0, 0}};

    uint32_t valor_led;

    for (int frame = 0; frame < 21; frame++)
    {
        for (int16_t i = 0; i < NUM_PIXELS; i++)
        {
            valor_led = matrix_rgb(0.0, frames[frame][24 - i], 0.0); // LEDs vermelhos para o olho
            pio_sm_put_blocking(pio, sm, valor_led);
        }
        sleep_ms(400); // Tempo entre os frames
    }
}

// ########################################### CORAÇÃO PULSANDO #########################################

void coracao_batendo(uint32_t valor_led, PIO pio, uint sm, int repeticoes, int delay_ms)
{

    double frames[5][25] = {
        {0.0, 1.0, 0.0, 1.0, 0.0,
         1.0, 0.0, 1.0, 0.0, 1.0,
         1.0, 0.0, 0.0, 0.0, 1.0,
         0.0, 1.0, 0.0, 1.0, 0.0,
         0.0, 0.0, 1.0, 0.0, 0.0},

        {0.0, 1.0, 0.0, 1.0, 0.0,
         1.0, 0.0, 1.0, 0.0, 1.0,
         1.0, 0.0, 0.5, 0.0, 1.0,
         0.0, 1.0, 0.0, 1.0, 0.0,
         0.0, 0.0, 1.0, 0.0, 0.0},

        {0.0, 1.0, 0.0, 1.0, 0.0,
         1.0, 0.0, 1.0, 0.0, 1.0,
         1.0, 0.5, 0.5, 0.5, 1.0,
         0.0, 1.0, 0.0, 1.0, 0.0,
         0.0, 0.0, 1.0, 0.0, 0.0},

        {0.0, 1.0, 0.0, 1.0, 0.0,
         1.0, 0.0, 1.0, 0.0, 1.0,
         1.0, 1.0, 1.0, 1.0, 1.0,
         0.0, 1.0, 0.0, 1.0, 0.0,
         0.0, 0.0, 1.0, 0.0, 0.0},

        {0.0, 1.0, 0.0, 1.0, 0.0,
         1.0, 1.0, 1.0, 1.0, 1.0,
         1.0, 1.0, 1.0, 1.0, 1.0,
         0.0, 1.0, 1.0, 1.0, 0.0,
         0.0, 0.0, 1.0, 0.0, 0.0}};

    for (int rep = 0; rep < repeticoes; rep++)
    {
        for (int f = 0; f < 5; f++)
        {
            for (double brilho = 0.2; brilho <= 1.0; brilho += 0.2) // AUMENTA o brilho
            {
                for (int16_t i = 0; i < NUM_PIXELS; i++)
                {
                    double intensidade = frames[f][24 - i] * brilho;
                    valor_led = matrix_rgb(0, intensidade, 0);
                    pio_sm_put_blocking(pio, sm, valor_led);
                }
                sleep_ms(delay_ms / 2);
            }

            for (double brilho = 1.0; brilho >= 0.2; brilho -= 0.2) // DIMINUI o brilho
            {
                for (int16_t i = 0; i < NUM_PIXELS; i++)
                {
                    double intensidade = frames[f][24 - i] * brilho;
                    valor_led = matrix_rgb(0, intensidade, 0);
                    pio_sm_put_blocking(pio, sm, valor_led);
                }
                sleep_ms(delay_ms / 2);
            }
        }
    }
}


// ########################################### ANIMAÇÃO DE TETRIX #########################################
void tetrix(PIO pio, uint sm)
{
    // Definição dos frames para simulação do tetrix
    double frames[18][25] = {
        {0, 0, 0, 0, 0,
         0, 0, 0, 0, 0,
         0, 0, 0, 0, 0,
         0, 0, 0, 0, 0,
         0, 0, 0, 0, 0},

        {0, 0, 1, 0, 0,
         0, 0, 0, 0, 0,
         0, 0, 0, 0, 0,
         0, 0, 0, 0, 0,
         0, 0, 0, 0, 0},

        {0, 0, 1, 0, 0,
         0, 0, 1, 0, 0,
         0, 0, 0, 0, 0,
         0, 0, 0, 0, 0,
         0, 0, 0, 0, 0},

        {0, 0, 1, 0, 0,
         0, 0, 1, 0, 0,
         0, 0, 1, 0, 0,
         0, 0, 0, 0, 0,
         0, 0, 0, 0, 0},

        {0, 0, 0, 0, 0,
         0, 0, 0, 1, 0,
         0, 1, 0, 0, 0,
         0, 0, 0, 1, 0,
         0, 0, 0, 0, 0},

        {0, 0, 0, 0, 0,
         0, 0, 0, 0, 0,
         1, 0, 0, 0, 0,
         0, 0, 0, 0, 1,
         1, 0, 0, 0, 0},

        {0, 0, 1, 1, 0,
         0, 0, 0, 0, 0,
         1, 0, 0, 0, 0,
         0, 0, 0, 0, 1,
         1, 0, 0, 0, 0},

        {0, 0, 0, 0, 0,
         0, 0, 1, 1, 0,
         1, 0, 0, 0, 0,
         0, 0, 0, 0, 1,
         1, 0, 0, 0, 0},

        {0, 0, 0, 0, 0,
         0, 0, 0, 0, 0,
         1, 1, 1, 0, 0,
         0, 0, 0, 0, 1,
         1, 0, 0, 0, 0},

        {0, 0, 0, 0, 0,
         0, 0, 0, 0, 0,
         1, 0, 0, 0, 0,
         0, 0, 1, 1, 1,
         1, 0, 0, 0, 0},

        {0, 0, 0, 0, 0,
         0, 0, 0, 0, 0,
         1, 0, 0, 0, 0,
         0, 0, 0, 0, 1,
         1, 1, 1, 0, 0},

        {0, 0, 1, 1, 0,
         0, 0, 0, 0, 0,
         1, 0, 0, 0, 0,
         0, 0, 0, 0, 1,
         1, 1, 1, 0, 0},

        {0, 0, 1, 1, 0,
         0, 1, 1, 0, 0,
         1, 0, 0, 0, 0,
         0, 0, 0, 0, 1,
         1, 1, 1, 0, 0},

        {0, 0, 0, 0, 0,
         0, 1, 1, 0, 0,
         1, 0, 1, 1, 0,
         0, 0, 0, 0, 1,
         1, 1, 1, 0, 0},

        {0, 0, 0, 0, 0,
         0, 0, 0, 0, 0,
         1, 0, 0, 1, 1,
         1, 1, 0, 0, 1,
         1, 1, 1, 0, 0},

        {0, 0, 0, 0, 0,
         0, 0, 0, 0, 0,
         1, 0, 0, 0, 0,
         1, 1, 0, 0, 1,
         1, 1, 1, 1, 1},

         {0, 0, 0, 0, 0,
         0, 0, 0, 0, 0,
         1, 0, 0, 0, 0,
         1, 1, 0, 0, 1,
         0.5, 0.5, 0.5, 0.5, 0.5},

        {0, 0, 0, 0, 0,
         0, 0, 0, 0, 0,
         1, 0, 0, 0, 0,
         1, 1, 0, 0, 1,
         0, 0, 0, 0, 0}
        };

    uint32_t valor_led;

    for (int frame = 0; frame < 18; frame++)
    {
        for (int16_t i = 0; i < NUM_PIXELS; i++)
        {
            valor_led = matrix_rgb(0.0, 0.0, frames[frame][24 - i]); // LEDs verdes para os blocos
            pio_sm_put_blocking(pio, sm, valor_led);
        }
        sleep_ms(650); // Tempo entre os frames
    }
}
// ########################################### ANIMAÇÃO DE SETA #########################################
void seta(PIO pio, uint sm){
    double frames [10][25] = {    
        {0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0, 0.0},

        {0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0, 0.0,
        0.0, 1.0, 1.0, 1.0, 0.0},

        {0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0, 0.0,
        0.0, 1.0, 1.0, 1.0, 0.0,
        1.0, 0.0, 1.0, 0.0, 1.0},

        {0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0, 0.0,
        0.0, 1.0, 1.0, 1.0, 0.0,
        1.0, 0.0, 1.0, 0.0, 1.0,
        0.0, 0.0, 1.0, 0.0, 0.0},
                       
        {0.0, 0.0, 1.0, 0.0, 0.0,
        0.0, 1.0, 1.0, 1.0, 0.0,
        1.0, 0.0, 1.0, 0.0, 1.0,
        0.0, 0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0, 0.0},

        {0.0, 1.0, 1.0, 1.0, 0.0,
        1.0, 0.0, 1.0, 0.0, 1.0,
        0.0, 0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 0.0},

        {1.0, 0.0, 1.0, 0.0, 1.0,
        0.0, 0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 0.0},

        {0.0, 0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 0.0},

        {0.0, 0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 0.0},

        {0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 0.0}
        };

        uint32_t valor_led;

        for(int16_t rep = 0; rep < 3; rep++){
        for(int16_t frame = 0; frame < 10; frame++){
            for (int16_t i = 0; i < NUM_PIXELS; i++){
            valor_led = matrix_rgb(0.0, 0.0, frames[frame][24-i]);
            pio_sm_put_blocking(pio, sm, valor_led);
            }
            sleep_ms(250);
        }
        }

}
// ########################################### ANIMAÇÃO CARINHA PISCANDO #########################################
void carinha(PIO pio, uint sm){
    double frames [5][25] = {    
         {1, 1, 0.0, 1, 1,
        1, 1, 0.0, 1, 1, 
        0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 0.0,
        1, 1, 1, 1, 1},

        {1, 1, 0.0, 1, 1,
        1, 1, 0.0, 1, 1, 
        0.0, 0.0, 0.0, 0.0, 0.0,
        1, 0.0, 0.0, 0.0, 1,
        0.0, 1, 1, 1, 0.0},

        {0.0, 0.0, 0.0, 1, 1,
        1, 1, 0.0, 1, 1, 
        0.0, 0.0, 0.0, 0.0, 0.0,
        1, 0.0, 0.0, 0.0, 1,
        0.0, 1, 1, 1, 0.0},

        {0.0, 0.0, 0.0, 1, 1,
        1, 1, 0.0, 0.0, 0.0, 
        0.0, 0.0, 0.0, 0.0, 0.0,
        1, 0.0, 0.0, 0.0, 1,
        0.0, 1, 1, 1, 0.0},
                       
        {1, 1, 0.0, 1, 1,
        1, 1, 0.0, 1, 1, 
        0.0, 0.0, 0.0, 0.0, 0.0,
        1, 0.0, 0.0, 0.0, 1,
        0.0, 1, 1, 1, 0.0},
        };

        uint32_t valor_led;

        for(int16_t rep = 0; rep < 2; rep++){
        for(int16_t frame = 0; frame < 5; frame++){
            for (int16_t i = 0; i < NUM_PIXELS; i++){
            valor_led = matrix_rgb(0.0, frames[frame][24-i], 0.0);
            pio_sm_put_blocking(pio, sm, valor_led);
            }
            sleep_ms(500);
        }
        }
}
// ########################################### ANIMAÇÃO COBRA #########################################
void tocar_buzzer(int freq, int duration_ms)
{
    uint32_t delay_us = 1000000 / (2 * freq); // Calcula o tempo de meio ciclo
    uint32_t ciclos = (duration_ms * 1000) / delay_us; // Quantidade de ciclos

    for (uint32_t t = 0; t < ciclos; t++)
    {
        gpio_put(BUZZER_PIN, 1); // Ativa o buzzer
        sleep_us(delay_us);
        gpio_put(BUZZER_PIN, 0); // Desativa o buzzer
        sleep_us(delay_us);
    }
}

void cobra_animacao(uint32_t valor_led, PIO pio, uint sm, int repeticoes, int delay_ms)
{
    gpio_init(BUZZER_PIN);
    gpio_set_dir(BUZZER_PIN, GPIO_OUT);
    // Cor da cobra
    double cor_cobra[3] = {0.0, 1.0, 0.0}; 
    double cor_fundo[3] = {0.0, 0.0, 0.0}; 

    for (int rep = 0; rep < repeticoes; rep++) 
    {
        // Etapa 1: Cobra "andando" pelos LEDs
        for (int16_t pos = 0; pos < NUM_PIXELS; pos++) 
        {
            for (int16_t i = 0; i < NUM_PIXELS; i++) 
            {
                if (i == pos) // LED atual representando a cobra
                {
                    valor_led = matrix_rgb(cor_cobra[0], cor_cobra[1], cor_cobra[2]);
                }
                else // LEDs de fundo
                {
                    valor_led = matrix_rgb(cor_fundo[0], cor_fundo[1], cor_fundo[2]);
                }
                pio_sm_put_blocking(pio, sm, valor_led);
            }
            // Toca o buzzer durante o movimento da cobra
            tocar_buzzer(440 + (pos * 20), delay_ms / 2); // Frequência variável
            sleep_ms(delay_ms);
        }

        // Etapa 2: Piscar todos os LEDs
        for (int blink = 0; blink < 3; blink++) // Piscar 3 vezes
        {
            for (int16_t i = 0; i < NUM_PIXELS; i++) 
            {
                valor_led = matrix_rgb(1.0, 1.0, 0); // Todos os LEDs brancos
                pio_sm_put_blocking(pio, sm, valor_led);
            }
            // Toca o buzzer durante o piscar
            tocar_buzzer(880, delay_ms); // Frequência fixa
            sleep_ms(delay_ms);

            for (int16_t i = 0; i < NUM_PIXELS; i++) 
            {
                valor_led = matrix_rgb(cor_fundo[0], cor_fundo[1], cor_fundo[2]); // Apagar todos os LEDs
                pio_sm_put_blocking(pio, sm, valor_led);
            }
            sleep_ms(delay_ms);
        }
    }
}