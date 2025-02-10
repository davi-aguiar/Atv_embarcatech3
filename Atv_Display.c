// Standard Libraries
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "inc/ssd1306.h"
#include "inc/font.h"
#include "hardware/clocks.h"
#include "hardware/gpio.h"
#include "ws2812.pio.h"
#include "hardware/pio.h"
#include "Matriz.h"

// Definition for the OLED display
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define OLED_ADDRESS 0x3C

// Definition for RGB LEDs
#define LED_GREEN 11
#define LED_BLUE 12

// Button definitions
#define BUTTON_A 5
#define BUTTON_B 6

// LED Matrix definitions
#define NUM_LEDS 25
#define MATRIX_PIN 7

// UART configuration
#define UART_ID uart0
#define BAUD_RATE 115200
#define UART_TX_PIN 0
#define UART_RX_PIN 1

// Function to animate the LED matrix
void update_matrix(int number, PIO pio, uint sm)
{
    if (number >= 1 && number <= 9)
    {
        double *pattern = desenhos[number - 1];              // Get the pattern corresponding to the number
        animacao(pattern, NUM_LEDS, pio, sm, 0.3, 0.0, 0.0); // Animate the pattern
    }
}

// Interrupt function prototype
static void button_pressed(uint gpio, uint32_t events);

// Stores the time of the last event (in microseconds)
static volatile uint32_t last_event_time = 0;

// LED states
bool green_led_state = false;
bool blue_led_state = false;

// Display structure initialization
ssd1306_t display;

// Main function
int main()
{
    stdio_init_all(); // Initialize serial communication

    // Initialize UART with specified baud rate
    uart_init(UART_ID, BAUD_RATE);

    // Configure GPIO pins for UART function
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    // Set UART format (8 data bits, 1 stop bit, no parity)
    uart_set_format(UART_ID, 8, 1, UART_PARITY_NONE);

    // Enable UART FIFO to prevent data loss
    uart_set_fifo_enabled(UART_ID, true);

    PIO pio = pio0; // PIO initialization
    sleep_ms(500);  // Delay to prevent overload

    printf("Initializing...\n");

    bool clock_configured = set_sys_clock_khz(128000, false); // System clock configuration

    /////////////////////////// INITIALIZING I2C ///////////////////////////
    // Initialize OLED display at 400000 Hz
    i2c_init(I2C_PORT, 400 * 1000);

    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C); // Set GPIO pin function to I2C
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C); // Set GPIO pin function to I2C
    gpio_pull_up(I2C_SDA);                     // Pull-up on data pin
    gpio_pull_up(I2C_SCL);                     // Pull-up on clock pin

    ssd1306_init(&display, WIDTH, HEIGHT, false, OLED_ADDRESS, I2C_PORT); // Initialize display
    ssd1306_config(&display);                                             // Configure display
    ssd1306_send_data(&display);                                          // Send data to display

    // Clear the display (starts with all pixels off)
    ssd1306_fill(&display, false);
    ssd1306_send_data(&display);

    // Initialize RGB LEDs
    gpio_init(LED_GREEN);
    gpio_set_dir(LED_GREEN, GPIO_OUT);
    gpio_init(LED_BLUE);
    gpio_set_dir(LED_BLUE, GPIO_OUT);

    // Initialize Button A
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);

    // Initialize Button B
    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);

    // Button interrupt configuration
    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &button_pressed);
    gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_FALL, true, &button_pressed);

    // PIO configuration
    uint offset = pio_add_program(pio, &Atv_Display_program);
    uint sm = pio_claim_unused_sm(pio, true);
    Atv_Display_program_init(pio, sm, offset, MATRIX_PIN, 800000, false);

    // Main loop
    while (true)
    {
        // Check if UART is connected
        if (stdio_usb_connected)
        {
            char received_char;
            printf("Digite um caractere ou um número de 0 a 9: ");
            if (scanf("%c", &received_char) == 1)
            {
                printf("Texto recebido %c\n", received_char);

                ssd1306_fill(&display, false); // Clear display

                if (received_char == 'D' || received_char == 'd' ||
                    received_char == 'L' || received_char == 'l')
                {

                    char display_text[2] = {received_char, '\0'};
                    ssd1306_draw_string(&display, "CARACTERE       RECEBIDO", 20, 30);
                    ssd1306_draw_string(&display, display_text, 45, 50);
                    ssd1306_send_data(&display);

                    if (received_char == 'D' || received_char == 'd')
                    {
                        printf("Desligando Matriz de LEDS...\n");
                        animacao(NULL, 0, pio0, sm, 0.0, 0.0, 0.0);
                    }
                    else if (received_char == 'L' || received_char == 'l')
                    {
                        printf("Ligando Matriz de LEDS...\n");
                        animacao(desenhos[0], 0, pio0, sm, 0.3, 0.0, 0.0);
                    }
                }
                else if (received_char >= '0' && received_char <= '9')
                {
                    int number = received_char - '0';
                    ssd1306_fill(&display, false);
                    char display_text[2] = {received_char, '\0'};
                    ssd1306_draw_string(&display, "MATRIZ ATUALIZADA", 10, 15);
                    ssd1306_draw_string(&display, "NUMERO", 25, 37);
                    ssd1306_draw_string(&display, display_text, 40, 50);
                    ssd1306_send_data(&display);

                    animacao(desenhos[number], 0, pio0, sm, 0.3, 0.0, 0.0);
                }
                else
                {
                    ssd1306_fill(&display, false);
                    char display_text[2] = {received_char, '\0'};
                    ssd1306_draw_string(&display, "CARACTERE       RECEBIDO", 20, 30);
                    ssd1306_draw_string(&display, display_text, 45, 50);
                    ssd1306_send_data(&display);
                }
            }
            sleep_ms(1000); // Delay to prevent overload
        }
    }
}

// Button interrupt handler
void button_pressed(uint gpio, uint32_t events)
{
    uint32_t current_time = to_us_since_boot(get_absolute_time());

    // Check if the time between events is greater than 300ms
    if (current_time - last_event_time > 300000)
    {
        last_event_time = current_time;

        if (gpio == BUTTON_A)
        {
            green_led_state = !green_led_state;   // Toggle LED state
            gpio_put(LED_GREEN, green_led_state); // Update LED state

            printf("Botão A pressionado\n");
            ssd1306_fill(&display, false);
            ssd1306_draw_string(&display, green_led_state ? "LED VERDE: ON" : "LED VERDE: OFF", 10, 30);
            ssd1306_send_data(&display);
        }
        if (gpio == BUTTON_B)
        {
            blue_led_state = !blue_led_state;   // Toggle LED state
            gpio_put(LED_BLUE, blue_led_state); // Update LED state

            printf("Botão B pressionado\n");
            ssd1306_fill(&display, false);
            ssd1306_draw_string(&display, blue_led_state ? "LED AZUL: ON" : "LED AZUL: OFF", 10, 30);
            ssd1306_send_data(&display);
        }
    }
}