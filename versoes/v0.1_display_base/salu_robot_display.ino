/*
 * Salu Robot v0.1 - Display Base (Estilo Tamagotchi/BMO)
 * Robôzinho animado com display ST7735 na HORIZONTAL
 * 
 * Hardware:
 * - Arduino Uno
 * - Display ST7735 (160x128)
 * 
 * Pinagem:
 * - VCC → 5V
 * - GND → GND  
 * - CS → Pino 10
 * - RESET → Pino 9
 * - A0/DC → Pino 8
 * - SDA/MOSI → Pino 11
 * - SCK → Pino 13
 * - LED → 5V
 */

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

// Definições dos pinos
#define TFT_CS    10
#define TFT_RST   9
#define TFT_DC    8

// Inicialização do display
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// Paleta de cores estilo retro/BMO
#define COR_FUNDO       0x2945  // Azul escuro
#define COR_TELA        0x3D8F  // Azul médio (tela interna)
#define COR_CORPO       0xE6F7  // Cinza claro
#define COR_BORDA       0x4A49  // Cinza escuro
#define COR_OLHOS       ST7735_CYAN  // Ciano brilhante
#define COR_BOCA        ST7735_CYAN  // Ciano brilhante
#define COR_TEXTO       ST7735_WHITE
#define COR_DETALHES    0x8C51  // Verde água

// Estados de emoção
enum Emocao {
  FELIZ,
  PISCANDO,
  MUITO_FELIZ,
  SURPRESO,
  DORMINDO,
  PENSANDO,
  AMOR
};

// Variáveis de controle
Emocao emocaoAtual = FELIZ;
unsigned long tempoUltimaAnimacao = 0;
unsigned long tempoUltimoPiscar = 0;
int contadorAnimacao = 0;

void setup() {
  // Inicialização do display em modo HORIZONTAL
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(1); // Rotação para modo horizontal (landscape)
  tft.fillScreen(COR_FUNDO);
  
  // Desenhar corpo do robô
  desenharCorpoRobo();
  
  // Desenhar rosto inicial
  desenharRosto(FELIZ);
  
  Serial.begin(9600);
  Serial.println("Salu Robot v0.1 - Display Base iniciado!");
}

void loop() {
  unsigned long tempoAtual = millis();
  
  // Piscar aleatoriamente (a cada 3-5 segundos)
  if (tempoAtual - tempoUltimaAnimacao > 3500 && emocaoAtual != PISCANDO) {
    animacaoPiscar();
    tempoUltimaAnimacao = tempoAtual;
  }
  
  // Mudar emoção a cada 8 segundos
  if (tempoAtual - tempoUltimaAnimacao > 8000) {
    proximaEmocao();
    tempoUltimaAnimacao = tempoAtual;
  }
  
  delay(100);
}

void desenharCorpoRobo() {
  // Corpo principal (retângulo arredondado estilo console)
  tft.fillRoundRect(10, 10, 140, 108, 12, COR_CORPO);
  tft.drawRoundRect(10, 10, 140, 108, 12, COR_BORDA);
  tft.drawRoundRect(11, 11, 138, 106, 11, COR_BORDA);
  
  // Tela interna (onde fica o rosto)
  tft.fillRoundRect(20, 20, 120, 88, 8, COR_TELA);
  
  // Detalhes decorativos (botões/luzes)
  // Botão esquerdo
  tft.fillCircle(30, 115, 3, COR_DETALHES);
  // Botão direito
  tft.fillCircle(130, 115, 3, COR_DETALHES);
  
  // Antena
  tft.fillCircle(80, 8, 4, COR_DETALHES);
  tft.drawLine(80, 8, 80, 12, COR_BORDA);
}

void desenharRosto(Emocao emocao) {
  // Limpar área da tela
  tft.fillRoundRect(22, 22, 116, 84, 6, COR_TELA);
  
  switch(emocao) {
    case FELIZ:
      desenharOlhosFeliz();
      desenharBocaSorriso();
      break;
      
    case PISCANDO:
      desenharOlhosFechados();
      desenharBocaSorriso();
      break;
      
    case MUITO_FELIZ:
      desenharOlhosMuitoFeliz();
      desenharBocaGrande();
      mostrarTexto("^_^");
      break;
      
    case SURPRESO:
      desenharOlhosSurpreso();
      desenharBocaSurpreso();
      mostrarTexto("!");
      break;
      
    case DORMINDO:
      desenharOlhosDormindo();
      desenharBocaDormindo();
      mostrarTexto("Zzz");
      break;
      
    case PENSANDO:
      desenharOlhosPensando();
      desenharBocaNeutra();
      mostrarTexto("...");
      break;
      
    case AMOR:
      desenharOlhosAmor();
      desenharBocaSorriso();
      mostrarCoracao();
      break;
  }
}

// ========== FUNÇÕES DE DESENHO DOS OLHOS ==========

void desenharOlhosFeliz() {
  // Olho esquerdo
  tft.fillRoundRect(45, 45, 18, 22, 9, COR_OLHOS);
  tft.fillCircle(54, 58, 4, COR_TELA); // Pupila
  
  // Olho direito
  tft.fillRoundRect(97, 45, 18, 22, 9, COR_OLHOS);
  tft.fillCircle(106, 58, 4, COR_TELA); // Pupila
}

void desenharOlhosFechados() {
  // Linhas horizontais (olhos fechados)
  tft.fillRoundRect(45, 55, 18, 4, 2, COR_OLHOS);
  tft.fillRoundRect(97, 55, 18, 4, 2, COR_OLHOS);
}

void desenharOlhosMuitoFeliz() {
  // Olhos em formato de "^" (muito feliz)
  // Esquerdo
  tft.drawLine(45, 60, 54, 50, COR_OLHOS);
  tft.drawLine(54, 50, 63, 60, COR_OLHOS);
  tft.drawLine(46, 61, 54, 52, COR_OLHOS);
  tft.drawLine(54, 52, 62, 61, COR_OLHOS);
  
  // Direito
  tft.drawLine(97, 60, 106, 50, COR_OLHOS);
  tft.drawLine(106, 50, 115, 60, COR_OLHOS);
  tft.drawLine(98, 61, 106, 52, COR_OLHOS);
  tft.drawLine(106, 52, 114, 61, COR_OLHOS);
}

void desenharOlhosSurpreso() {
  // Olhos bem abertos (círculos grandes)
  tft.fillCircle(54, 55, 12, COR_OLHOS);
  tft.fillCircle(54, 55, 6, COR_TELA); // Pupila grande
  
  tft.fillCircle(106, 55, 12, COR_OLHOS);
  tft.fillCircle(106, 55, 6, COR_TELA); // Pupila grande
}

void desenharOlhosDormindo() {
  // Olhos fechados com curva
  for(int i = 0; i < 3; i++) {
    tft.drawLine(45+i, 58, 63-i, 58, COR_OLHOS);
    tft.drawLine(97+i, 58, 115-i, 58, COR_OLHOS);
  }
}

void desenharOlhosPensando() {
  // Olhos olhando para o lado
  tft.fillRoundRect(45, 50, 18, 18, 9, COR_OLHOS);
  tft.fillCircle(58, 59, 4, COR_TELA); // Pupila à direita
  
  tft.fillRoundRect(97, 50, 18, 18, 9, COR_OLHOS);
  tft.fillCircle(110, 59, 4, COR_TELA); // Pupila à direita
}

void desenharOlhosAmor() {
  // Corações nos olhos
  // Esquerdo
  desenharCoracaoPequeno(54, 55);
  // Direito
  desenharCoracaoPequeno(106, 55);
}

// ========== FUNÇÕES DE DESENHO DA BOCA ==========

void desenharBocaSorriso() {
  // Sorriso simples
  tft.drawLine(55, 80, 65, 88, COR_BOCA);
  tft.drawLine(65, 88, 95, 88, COR_BOCA);
  tft.drawLine(95, 88, 105, 80, COR_BOCA);
  
  tft.drawLine(55, 81, 65, 89, COR_BOCA);
  tft.drawLine(95, 89, 105, 81, COR_BOCA);
}

void desenharBocaGrande() {
  // Boca muito feliz (maior)
  tft.fillRoundRect(55, 78, 50, 18, 9, COR_BOCA);
  tft.fillRoundRect(57, 80, 46, 14, 7, COR_TELA);
}

void desenharBocaSurpreso() {
  // Boca "O"
  tft.fillCircle(80, 85, 10, COR_BOCA);
  tft.fillCircle(80, 85, 7, COR_TELA);
}

void desenharBocaDormindo() {
  // Boca pequena
  tft.fillCircle(80, 85, 4, COR_BOCA);
}

void desenharBocaNeutra() {
  // Linha reta
  tft.fillRoundRect(60, 83, 40, 4, 2, COR_BOCA);
}

// ========== FUNÇÕES AUXILIARES ==========

void mostrarTexto(String texto) {
  tft.setTextColor(COR_TEXTO);
  tft.setTextSize(2);
  int largura = texto.length() * 12;
  tft.setCursor(80 - largura/2, 30);
  tft.print(texto);
}

void desenharCoracaoPequeno(int x, int y) {
  // Desenhar coração pequeno
  tft.fillCircle(x-4, y-2, 4, COR_OLHOS);
  tft.fillCircle(x+4, y-2, 4, COR_OLHOS);
  tft.fillTriangle(x-8, y, x, y+8, x+8, y, COR_OLHOS);
}

void mostrarCoracao() {
  // Coração flutuante ao lado
  desenharCoracaoGrande(125, 35);
}

void desenharCoracaoGrande(int x, int y) {
  tft.fillCircle(x-5, y, 6, ST7735_RED);
  tft.fillCircle(x+5, y, 6, ST7735_RED);
  tft.fillTriangle(x-11, y+2, x, y+14, x+11, y+2, ST7735_RED);
}

// ========== ANIMAÇÕES ==========

void animacaoPiscar() {
  Emocao emocaoAnterior = emocaoAtual;
  desenharRosto(PISCANDO);
  delay(150);
  desenharRosto(emocaoAnterior);
}

void proximaEmocao() {
  contadorAnimacao++;
  
  // Sequência de emoções
  switch(contadorAnimacao % 7) {
    case 0:
      emocaoAtual = FELIZ;
      break;
    case 1:
      emocaoAtual = MUITO_FELIZ;
      break;
    case 2:
      emocaoAtual = FELIZ;
      break;
    case 3:
      emocaoAtual = SURPRESO;
      break;
    case 4:
      emocaoAtual = PENSANDO;
      break;
    case 5:
      emocaoAtual = AMOR;
      break;
    case 6:
      emocaoAtual = DORMINDO;
      break;
  }
  
  desenharRosto(emocaoAtual);
  
  // Mensagens especiais
  if (emocaoAtual == MUITO_FELIZ && contadorAnimacao == 1) {
    delay(2000);
    tft.fillRoundRect(22, 22, 116, 84, 6, COR_TELA);
    tft.setTextColor(COR_TEXTO);
    tft.setTextSize(1);
    tft.setCursor(35, 50);
    tft.print("Ola! Sou o");
    tft.setCursor(45, 65);
    tft.print("Salu Bot!");
    delay(3000);
    desenharRosto(FELIZ);
  }
}
