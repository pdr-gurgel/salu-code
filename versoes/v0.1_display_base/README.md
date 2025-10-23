# Salu Robot v0.1 - Display Base

## Descrição
Versão base do Salu Robot com design estilo Tamagotchi/BMO. Display configurado em modo horizontal com múltiplas expressões faciais e animações básicas.

## Componentes Necessários
- Arduino Uno
- Display ST7735 (160x128 pixels)
- Fios de conexão
- Protoboard (opcional)

## Diagrama de Pinagem

```
Arduino Uno    →    ST7735
─────────────────────────────
5V             →    VCC
GND            →    GND
Pino 10        →    CS
Pino 9         →    RESET
Pino 8         →    A0/DC
Pino 11        →    SDA/MOSI
Pino 13        →    SCK
5V             →    LED (backlight)
```

## Instalação das Bibliotecas

1. Abra o Arduino IDE
2. Vá em **Ferramentas** → **Gerenciar Bibliotecas**
3. Procure e instale as seguintes bibliotecas:
   - **Adafruit GFX Library**
   - **Adafruit ST7735 and ST7789 Library**

## Como Fazer Upload

1. Conecte o Arduino Uno ao computador via USB
2. Selecione a placa: **Ferramentas** → **Placa** → **Arduino Uno**
3. Selecione a porta: **Ferramentas** → **Porta** → (sua porta USB)
4. Abra o arquivo `salu_robot_display.ino`
5. Clique em **Upload** (seta para a direita)

## Funcionalidades

### Design
- **Display horizontal** (modo landscape)
- **Corpo estilo console** retro/BMO
- **Tela interna** com fundo azul
- **Detalhes decorativos**: botões e antena

### Expressões Implementadas
- **FELIZ** 😊 - Olhos normais com sorriso
- **PISCANDO** 😌 - Animação de piscar
- **MUITO_FELIZ** 😄 - Olhos em "^" e boca grande
- **SURPRESO** 😮 - Olhos arregalados e boca "O"
- **DORMINDO** 😴 - Olhos fechados com "Zzz"
- **PENSANDO** 🤔 - Olhos olhando pro lado com "..."
- **AMOR** 😍 - Corações nos olhos

### Animações
- Piscar automático a cada 3.5 segundos
- Mudança de emoção a cada 8 segundos
- Sequência cíclica de todas as emoções
- Mensagem de apresentação "Olá! Sou o Salu Bot!"

## Paleta de Cores

```cpp
COR_FUNDO    = Azul escuro (0x2945)
COR_TELA     = Azul médio (0x3D8F)
COR_CORPO    = Cinza claro (0xE6F7)
COR_BORDA    = Cinza escuro (0x4A49)
COR_OLHOS    = Ciano (ST7735_CYAN)
COR_BOCA     = Ciano (ST7735_CYAN)
COR_TEXTO    = Branco (ST7735_WHITE)
COR_DETALHES = Verde água (0x8C51)
```

## Personalização

### Alterar Tempos de Animação
```cpp
// No loop()
if (tempoAtual - tempoUltimaAnimacao > 3500) // Piscar
if (tempoAtual - tempoUltimaAnimacao > 8000) // Mudar emoção
```

### Adicionar Novas Emoções
1. Adicione novo valor no `enum Emocao`
2. Crie funções de desenho dos olhos e boca
3. Adicione case no `switch` da função `desenharRosto()`
4. Atualize a sequência em `proximaEmocao()`

## Próxima Versão
v0.2 - Sistema de Fala com animação de boca e balões de diálogo