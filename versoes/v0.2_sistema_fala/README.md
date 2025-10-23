# Salu Robot v0.2 - Sistema de Fala

## Descrição
Segunda versão do Salu Robot com sistema de fala implementado. O robô agora pode "falar" frases com animação de boca sincronizada e balões de diálogo.

## Novidades da v0.2

### 🗣️ Sistema de Fala
- **Balões de diálogo** com texto
- **Animação de boca** sincronizada com a fala
- **10 frases diferentes** pré-programadas
- **Fala aleatória** a cada 6 segundos

### 💬 Frases Implementadas
1. "Olá!"
2. "Como vai?"
3. "Eu sou o Salu!"
4. "Tudo bem?"
5. "Estou feliz!"
6. "Que legal!"
7. "Vamos lá!"
8. "Oba!"
9. "Legal!"
10. "Massa!"

### 🎬 Animação de Boca
- **Boca aberta** (150ms)
- **Boca fechada** (150ms)
- Ciclos sincronizados com duração do texto
- Retorno suave ao sorriso após falar

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

## Instalação

Mesmas instruções da v0.1:
1. Instale as bibliotecas Adafruit GFX e ST7735
2. Faça upload do código `salu_robot_display.ino`
3. O robô iniciará automaticamente com uma apresentação

## Funcionalidades

### Comportamento Automático
- **Piscar**: A cada 4 segundos (quando não está falando)
- **Falar**: A cada 6 segundos (frase aleatória)
- **Apresentação inicial**: "Olá! Eu sou o Salu!" ao ligar

### Estrutura da Fala
```cpp
void falar(String texto) {
  1. Prepara rosto para falar
  2. Mostra balão de diálogo
  3. Anima boca (abrir/fechar)
  4. Mantém texto visível
  5. Limpa balão
  6. Volta ao estado feliz
}
```

## Personalização

### Adicionar Novas Frases
```cpp
String frases[] = {
  "Ola!",
  "Sua nova frase aqui!",
  // ... mais frases
};
int totalFrases = 11; // Atualizar contador
```

### Ajustar Velocidade da Fala
```cpp
int duracaoFala = texto.length() * 150; // Alterar multiplicador
delay(150); // Ajustar delays de abertura/fechamento
```

### Alterar Frequência de Fala
```cpp
if (tempoAtual - tempoUltimaAnimacao > 6000) // Alterar valor em ms
```

## Funções Principais

### Sistema de Fala
- `falar(String texto)` - Faz o robô falar uma frase
- `mostrarBalaoFala(String texto)` - Desenha balão de diálogo
- `limparBalaoFala()` - Remove balão da tela
- `desenharBocaAberta()` - Boca em estado aberto
- `desenharBocaFechada()` - Boca em estado fechado
- `limparAreaBoca()` - Limpa área da boca para redesenhar

### Controle de Estado
- `estaFalando` - Impede outras animações durante a fala
- `contadorFala` - Controla sequência de frases

## Diferenças da v0.1

| Característica | v0.1 | v0.2 |
|---------------|------|------|
| Emoções | 7 diferentes | 3 (simplificado para fala) |
| Sistema de fala | ❌ | ✅ |
| Balões de diálogo | ❌ | ✅ |
| Animação de boca | Estática | Dinâmica |
| Frases | 1 fixa | 10 aleatórias |

## Solução de Problemas

### Texto não aparece no balão
- Verifique se a string não está muito longa (máx 140px)
- Ajuste o tamanho da fonte se necessário

### Animação de boca muito rápida/lenta
- Ajuste os delays em `falar()`
- Modifique o multiplicador de duração

### Balão cortado na tela
- O código já limita automaticamente a largura
- Reduza o tamanho das frases se necessário

### Erro de compilação com acentos
- Todas as variáveis e comentários usam apenas ASCII
- Não use acentos em nomes de variáveis

## Próxima Versão
v0.3 - Integração com sensores e respostas interativas