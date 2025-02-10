#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include "hardware/gpio.h"
#include "ws2812.pio.h"
#include "hardware/pio.h"
#define NUM_LEDS 25
#define PIN 7

// Definição dos desenhos na matriz de LEDs

double num_0[NUM_LEDS] =

    {0.0, 0.2, 0.2, 0.2, 0.0,
     0.2, 0.0, 0.0, 0.0, 0.2,
     0.2, 0.0, 0.0, 0.0, 0.2,
     0.2, 0.0, 0.0, 0.0, 0.2,
     0.0, 0.2, 0.2, 0.2, 0.0};

double num_1[NUM_LEDS] =

    {0.2, 0.2, 0.2, 0.2, 0.2,
     0.0, 0.0, 0.2, 0.0, 0.0,
     0.0, 0.0, 0.2, 0.0, 0.2,
     0.0, 0.2, 0.2, 0.0, 0.0,
     0.0, 0.0, 0.2, 0.0, 0.0};

double num_2[NUM_LEDS] =

    {0.2, 0.2, 0.2, 0.2, 0.2,
     0.2, 0.0, 0.0, 0.0, 0.0,
     0.2, 0.2, 0.2, 0.2, 0.2,
     0.0, 0.0, 0.0, 0.0, 0.2,
     0.2, 0.2, 0.2, 0.2, 0.2};

double num_3[NUM_LEDS] =

    {
        0.0,
        0.2,
        0.2,
        0.2,
        0.0,
        0.0,
        0.0,
        0.0,
        0.2,
        0.0,
        0.0,
        0.2,
        0.2,
        0.2,
        0.0,
        0.0,
        0.0,
        0.0,
        0.2,
        0.0,
        0.0,
        0.2,
        0.2,
        0.2,
        0.0,
};

double num_4[NUM_LEDS] =

    {0.0, 0.2, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 0.2, 0.0,
     0.0, 0.2, 0.2, 0.2, 0.0,
     0.0, 0.2, 0.0, 0.2, 0.0,
     0.0, 0.2, 0.0, 0.2, 0.0};

double num_5[NUM_LEDS] =

    {0.2, 0.2, 0.2, 0.2, 0.2,
     0.0, 0.0, 0.0, 0.0, 0.2,
     0.2, 0.2, 0.2, 0.2, 0.2,
     0.2, 0.0, 0.0, 0.0, 0.0,
     0.2, 0.2, 0.2, 0.2, 0.2};

double num_6[NUM_LEDS] =

    {0.2, 0.2, 0.2, 0.2, 0.2,
     0.2, 0.0, 0.0, 0.0, 0.2,
     0.2, 0.2, 0.2, 0.2, 0.2,
     0.2, 0.0, 0.0, 0.0, 0.0,
     0.2, 0.2, 0.2, 0.2, 0.2};

double num_7[NUM_LEDS] =

    {0.0, 0.2, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 0.2, 0.0,
     0.2, 0.2, 0.2, 0.0, 0.0,
     0.0, 0.0, 0.0, 0.2, 0.0,
     0.0, 0.2, 0.2, 0.2, 0.0};

double num_8[NUM_LEDS] =

    {0.0, 0.2, 0.2, 0.2, 0.0,
     0.0, 0.2, 0.0, 0.2, 0.0,
     0.0, 0.2, 0.2, 0.2, 0.0,
     0.0, 0.2, 0.0, 0.2, 0.0,
     0.0, 0.2, 0.2, 0.2, 0.0};

double num_9[NUM_LEDS] =

    {0.0, 0.2, 0.2, 0.2, 0.0,
     0.0, 0.0, 0.0, 0.2, 0.0,
     0.0, 0.2, 0.2, 0.2, 0.0,
     0.0, 0.2, 0.0, 0.2, 0.0,
     0.0, 0.2, 0.2, 0.2, 0.0};

// Variável para armazenar o desenho atual que pode mudar a qualquer momento
volatile uint8_t desenho_atual = 0;
// Vetor de desenhos para serem exibidos na matriz de LEDs
double *desenhos[] = {num_0, num_1, num_2, num_3, num_4, num_5, num_6, num_7, num_8, num_9};

// Definição da intensidade de cada cor na matriz de LEDs
double matrix_rgb(double r, double g, double b)
{
  unsigned char R, G, B;
  R = r * 255;
  G = g * 255;
  B = b * 255;
  return (G << 24) | (R << 16) | (B << 8);
}

// Rotina para acionar a matriz de LEDs - WS2812B
void animacao(double *desenho, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b)
{
  for (int16_t i = 0; i < NUM_LEDS; i++)
  {
    valor_led = matrix_rgb(desenho[i] * r, desenho[i] * g, desenho[i] * b);
    pio_sm_put_blocking(pio, sm, valor_led);
  }
}