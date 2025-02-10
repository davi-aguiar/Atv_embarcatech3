# Projeto: Controle de Matriz de LEDs e Display OLED com Raspberry Pi Pico W

## Descrição

Este projeto utiliza um **Raspberry Pi Pico W** para controlar uma **matriz de LEDs RGB**, um **display OLED via I2C** e interagir com o usuário por meio de **botões físicos** e **comunicação UART**.

### **Principais Funcionalidades:**

- Controle da matriz de LEDs RGB utilizando **PIO (Programável I/O)**.
- Exibição de informações em um **display OLED SSD1306**.
- Leitura de comandos via **UART (serial)** para acionar diferentes animações na matriz e leituras de caracteres.
- Uso de **botões físicos** para alternar LEDs individuais (verde e azul).

## **Hardware Necessário**

- Raspberry Pi Pico W
- Display OLED SSD1306 (com I2C)
- Matriz de LEDs RGB (WS2812)
- Botões (2x) para controle manual
- Resistores de pull-up (caso necessário para os botões)

## **Esquemático de Ligação**

| Componente  | Pino do Pico W    |
| ----------- | ----------------- |
| OLED SDA    | GPIO 14 (I2C SDA) |
| OLED SCL    | GPIO 15 (I2C SCL) |
| Matriz LEDs | GPIO 7            |
| LED Verde   | GPIO 11           |
| LED Azul    | GPIO 12           |
| Botão A     | GPIO 5            |
| Botão B     | GPIO 6            |
| UART TX     | GPIO 0            |
| UART RX     | GPIO 1            |

## **Instalação e Uso**

### **1. Configurar o Ambiente**

1. Instale o SDK do Raspberry Pi Pico se ainda não o tiver configurado ([Guia Oficial](https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf)).
2. Clone este repositório:
   ```sh
   git clone https://github.com/patrickpk7/Atv_Display.git
   cd Atv_Display
   ```
3. Compile o projeto e envie para o Pico (exemplo com CMake e Make):
   ```sh
   mkdir build
   cd build
   cmake ..
   make
   ```
4. Carregue o arquivo `.uf2` gerado para o Raspberry Pi Pico W.

### **2. Uso do Programa**

- **Comandos via UART:**
  - **'D' ou 'd'**: Desliga a matriz de LEDs e aparece no display.
  - **'L' ou 'l'**: Liga a matriz de LEDs e aparece no display.
  - **Caracteres de A-Z e de a-z**: Exibidas no display
  - **Números de 0 a 9**: Atualiza a animação da matriz com um padrão predefinido.
- **Botões Físicos:**
  - **Botão A (GPIO 5)**: Liga/desliga o LED verde.
  - **Botão B (GPIO 6)**: Liga/desliga o LED azul.

## **LINK PARA VÍDEO**
