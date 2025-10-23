# Salu Robot v0.3 - Display Definitivo

## Descrição
Versão definitiva do display do Salu Robot com sequência completa de animações e expressões. Esta versão integra todas as funcionalidades anteriores e adiciona novas expressões emocionais para criar uma experiência interativa completa.

## Novidades da v0.3

### 🎬 Sequência de Apresentação Completa
A sequência agora segue um fluxo narrativo específico:

1. **Apresentação** 👋
   - Olhos felizes normais
   - Fala: "Olá! Eu sou o Salu!"
   
2. **Pergunta Carinhosa** ❤️
   - Olhos viram CORAÇÕES
   - Fala: "Como você está?"
   
3. **Oferta de Funcionalidade** 🔥
   - Olhos viram FOGO
   - Fala: "Quer medir a temperatura?"

### 🎨 Novas Expressões

#### Olhos de Coração ❤️
- Corações vermelhos no lugar dos olhos
- Usado quando pergunta sobre o bem-estar do usuário
- Design fofo e acolhedor

#### Olhos de Fogo 🔥
- Chamas coloridas (vermelho, laranja, amarelo)
- Usado quando oferece medir temperatura
- Design dinâmico com gradiente de cores
- Brilho branco no centro

### ⚙️ Comportamento Automático
- **Sequência inicial**: Executa ao ligar
- **Repetição**: A cada 20 segundos
- **Piscar**: A cada 4 segundos (quando não está falando)
- **Transições suaves**: Entre todas as emoções

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

1. Instale as bibliotecas Adafruit GFX e ST7735
2. Faça upload do código `salu_robot_display.ino`
3. O robô iniciará automaticamente com a sequência de apresentação

## Funcionalidades Completas

### Expressões Implementadas
- ✅ **FELIZ** - Olhos normais com sorriso
- ✅ **PISCANDO** - Animação de piscar
- ✅ **FALANDO** - Boca animada durante fala
- ✅ **AMOR** - Olhos de coração ❤️
- ✅ **FOGO** - Olhos de fogo 🔥

### Sistema de Fala
- Balões de diálogo com texto
- Animação de boca sincronizada
- Duração baseada no tamanho do texto
- Limpeza automática após fala

### Animações
- Piscar automático
- Transições entre emoções
- Sequência narrativa completa
- Loop contínuo

## Fluxo da Sequência

```
┌─────────────────────┐
│  1. APRESENTAÇÃO    │
│  "Olá! Eu sou o     │
│   Salu!"            │
│  👀 Olhos Felizes   │
└──────────┬──────────┘
           │
           ↓ (2 segundos)
┌─────────────────────┐
│  2. PERGUNTA        │
│  "Como você está?"  │
│  ❤️ Olhos Coração   │
└──────────┬──────────┘
           │
           ↓ (2.5 segundos)
┌─────────────────────┐
│  3. FUNCIONALIDADE  │
│  "Quer medir a      │
│   temperatura?"     │
│  🔥 Olhos Fogo      │
└──────────┬──────────┘
           │
           ↓ (3 segundos)
           │
           └──→ Volta ao início (após 20s)
```

## Detalhes Técnicos

### Olhos de Coração
```cpp
void desenharCoracaoOlho(int x, int y) {
  - Dois círculos vermelhos (topo)
  - Triângulo vermelho (base)
  - Posicionamento preciso
}
```

### Olhos de Fogo
```cpp
void desenharFogoOlho(int x, int y) {
  - Círculo vermelho (base)
  - Círculo laranja (meio)
  - Triângulo amarelo (chama)
  - Círculo branco (brilho)
}
```

### Controle de Estado
```cpp
Emocao emocaoAtual;  // Mantém estado atual
bool estaFalando;    // Previne conflitos
```

## Personalização

### Alterar Tempo da Sequência
```cpp
delay(2000);  // Após apresentação
delay(2500);  // Após pergunta
delay(3000);  // Após temperatura
```

### Alterar Frequência de Repetição
```cpp
if (tempoAtual - tempoUltimaAnimacao > 20000)  // 20 segundos
```

### Modificar Textos
```cpp
falar("Ola! Eu sou o Salu!");           // Etapa 1
falar("Como voce esta?");               // Etapa 2
falar("Quer medir a temperatura?");     // Etapa 3
```

### Adicionar Novas Etapas
```cpp
void executarSequenciaInicial() {
  // Etapa 1
  // Etapa 2
  // Etapa 3
  // Etapa 4 (nova) ← Adicione aqui
}
```

## Comparação de Versões

| Característica | v0.1 | v0.2 | v0.3 |
|---------------|------|------|------|
| Expressões | 7 | 3 | 5 |
| Sistema de fala | ❌ | ✅ | ✅ |
| Sequência narrativa | ❌ | ❌ | ✅ |
| Olhos de coração | ✅ | ❌ | ✅ |
| Olhos de fogo | ❌ | ❌ | ✅ |
| Animação de boca | ❌ | ✅ | ✅ |
| Balões de diálogo | ❌ | ✅ | ✅ |

## Solução de Problemas

### Sequência não inicia
- Verifique se o display está conectado corretamente
- Aguarde 1 segundo após ligar

### Olhos de fogo não aparecem
- Verifique se a biblioteca suporta ST7735_ORANGE
- Caso não, substitua por ST7735_RED

### Sequência muito rápida/lenta
- Ajuste os delays entre cada etapa
- Modifique o tempo de repetição (20000ms)

## Próximas Versões

### v0.4 - Sensores (Planejada)
- Integração com sensor de temperatura
- Resposta real à pergunta de temperatura
- Sensor de luminosidade

### v1.0 - Versão Completa (Planejada)
- Integração de todas funcionalidades
- Sistema de menu
- Múltiplos modos de operação

## Notas Importantes

- Esta é a **versão definitiva do display**
- Todas as animações estão otimizadas
- Código sem acentos (compatível com Arduino IDE)
- Pronto para integração com sensores
- Base sólida para versões futuras

---

**Versão**: 0.3  
**Data**: 23 de outubro de 2024  
**Status**: ✅ Definitiva do Display
