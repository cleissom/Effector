# Effector

Effector é uma implementação de Processamento de Áudio para a aplicação de efeitos de áudio temporais.

O Projeto utiliza o Kit de Desenvolvimento STM32F4DISCOVERY. Este Kit possui o microcontrolador STM32F407, um microcontrolador com um núcleo 32-bit ARM Cortex-M4. Este é um microcontrolador de alta performance e possui uma unidade de ponto flutuante.



![showcase](/img/showcase.jpg)



## Dependências

Hardware:

* Kit de Desenvolvimento STM32F4DISCOVERY.
* Placa de Som Wolfson Pi.
* Display OLED com controlador SSD1306.
* Encoder Rotativo.

Software:

* IDE Eclipse.



## Funcionalidades

A código aplica os seguintes efeitos:

+ Eco.
+ Vibrato.
+ Flanger.
+ Tremolo.
+ Overdrive.



## Compilando e Executando

Para executar é necessário apenas importar o projeto no software _eclipse._  As configurações necessárias para o eclipse já estão incluídas no arquivo `.cproject`.

### Esquema de Ligação

Esquema de ligação para o display e o encoder rotativo:

|    Conexão    | Pino |
| :-----------: | :--: |
|      SCK      | PB8  |
|      SDA      | PB9  |
|   EncoderA    | PB4  |
|   EncoderB    | PB5  |
| EncoderSwitch | PB7  |