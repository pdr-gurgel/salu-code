/*
 * Salu Robot v0.3 - Display Definitivo (Estilo Tamagotchi/BMO)
 * Versao definitiva do display com sequencia de animacoes completa
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

// Definicoes dos pinos
#define TFT_CS    10
#define TFT_RST   9
#define TFT_DC    8

// Inicializacao do display
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// Paleta de cores estilo retro/BMO
#define COR_FUNDO       0x2945  // Azul escuro
#define COR_TELA        0x3D8F  // Azul medio (tela interna)
#define COR_CORPO       0xE6F7  // Cinza claro
#define COR_BORDA       0x4A49  // Cinza escuro
#define COR_OLHOS       ST7735_CYAN  // Ciano brilhante
#define COR_BOCA        ST7735_CYAN  // Ciano brilhante
#define COR_TEXTO       ST7735_WHITE
#define COR_DETALHES    0x8C51  // Verde agua
#define COR_BALAO       ST7735_WHITE  // Balao de fala

// Estados de emocao
enum Emocao {
  FELIZ,
  PISCANDO,
  FALANDO,
  AMOR
};

// Variaveis de controle
Emocao emocaoAtual = FELIZ;
unsigned long tempoUltimaAnimacao = 0;
unsigned long tempoUltimoPiscar = 0;
int etapaSequencia = 0;
bool estaFalando = false;
bool sequenciaInicial = true;

void setup() {
  // Inicializacao do display em modo HORIZONTAL
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(1); // Rotacao para modo horizontal (landscape)
  tft.fillScreen(COR_FUNDO);
  
  // Desenhar corpo do robo
  desenharCorpoRobo();
  
  // Desenhar rosto inicial
  desenharRosto(FELIZ);
  
  Serial.begin(9600);
  Serial.println("Salu Robot v0.3 - Display Definitivo iniciado!");
  
  // Iniciar sequencia de apresentacao
  delay(1000);
  executarSequenciaInicial();
}

void loop() {
  unsigned long tempoAtual = millis();
  
  // Piscar aleatoriamente (a cada 4 segundos)
  if (tempoAtual - tempoUltimoPiscar > 4000 && !estaFalando) {
    animacaoPiscar();
    tempoUltimoPiscar = tempoAtual;
  }
  
  // Repetir sequencia a cada 20 segundos
  if (tempoAtual - tempoUltimaAnimacao > 20000 && !estaFalando) {
    executarSequenciaInicial();
    tempoUltimaAnimacao = tempoAtual;
  }
  
  delay(100);
}

void executarSequenciaInicial() {
  // Etapa 1: Apresentacao
  emocaoAtual = FELIZ;
  desenharRosto(FELIZ);
  falar("Ola! Eu sou o Salu!");
  delay(2000);
  
  // Etapa 2: Pergunta com coracao
  emocaoAtual = AMOR;
  desenharRosto(AMOR);
  falar("Como voce esta?");
  delay(1000);
  
  // Voltar ao estado feliz
  emocaoAtual = FELIZ;
  desenharRosto(FELIZ);
  delay(2000);
  
  // Etapa 3: Pergunta sobre temperatura (olhos normais)
  falar("Quer medir a temperatura?");
  delay(2000);
}

void desenharCorpoRobo() {
  // Corpo principal (retangulo arredondado estilo console)
  tft.fillRoundRect(10, 10, 140, 108, 12, COR_CORPO);
  tft.drawRoundRect(10, 10, 140, 108, 12, COR_BORDA);
  tft.drawRoundRect(11, 11, 138, 106, 11, COR_BORDA);
  
  // Tela interna (onde fica o rosto)
  tft.fillRoundRect(20, 20, 120, 88, 8, COR_TELA);
  
  // Detalhes decorativos (botoes/luzes)
  // Botao esquerdo
  tft.fillCircle(30, 115, 3, COR_DETALHES);
  // Botao direito
  tft.fillCircle(130, 115, 3, COR_DETALHES);
  
  // Antena
  tft.fillCircle(80, 8, 4, COR_DETALHES);
  tft.drawLine(80, 8, 80, 12, COR_BORDA);
}

void desenharRosto(Emocao emocao) {
  // Limpar area da tela
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
      
    case FALANDO:
      // Mantem os olhos da emocao atual
      if(emocaoAtual == AMOR) {
        desenharOlhosCoracao();
      } else {
        desenharOlhosFeliz();
      }
      // Boca sera animada durante a fala
      break;
      
    case AMOR:
      desenharOlhosCoracao();
      desenharBocaSorriso();
      break;
  }
}

// ========== FUNCOES DE DESENHO DOS OLHOS ==========

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

void desenharOlhosCoracao() {
  // Olho esquerdo - coracao
  desenharCoracaoOlho(54, 55);
  
  // Olho direito - coracao
  desenharCoracaoOlho(106, 55);
}

void desenharCoracaoOlho(int x, int y) {
  // Desenhar coracao preenchido (cores padrão ciano)
  tft.fillCircle(x-4, y-2, 5, COR_OLHOS);
  tft.fillCircle(x+4, y-2, 5, COR_OLHOS);
  tft.fillTriangle(x-9, y, x, y+10, x+9, y, COR_OLHOS);
}

// ========== FUNCOES DE DESENHO DA BOCA ==========

void desenharBocaSorriso() {
  // Sorriso simples
  tft.drawLine(55, 80, 65, 88, COR_BOCA);
  tft.drawLine(65, 88, 95, 88, COR_BOCA);
  tft.drawLine(95, 88, 105, 80, COR_BOCA);
  
  tft.drawLine(55, 81, 65, 89, COR_BOCA);
  tft.drawLine(95, 89, 105, 81, COR_BOCA);
}

void desenharBocaAberta() {
  // Boca aberta (falando)
  tft.fillRoundRect(65, 78, 30, 20, 8, COR_BOCA);
  tft.fillRoundRect(67, 80, 26, 16, 6, COR_TELA);
}

void desenharBocaFechada() {
  // Boca fechada (momento da fala)
  tft.fillRoundRect(65, 85, 30, 6, 3, COR_BOCA);
}

void limparAreaBoca() {
  // Limpar apenas a area da boca
  tft.fillRect(50, 75, 60, 25, COR_TELA);
}

// ========== SISTEMA DE FALA ==========

void falar(String texto) {
  estaFalando = true;
  Emocao emocaoBackup = emocaoAtual;
  
  // Mostrar balao de fala
  mostrarBalaoFala(texto);
  
  // Animar boca enquanto fala
  int duracaoFala = texto.length() * 150; // 150ms por caractere
  int ciclos = duracaoFala / 300; // Cada ciclo de animacao dura 300ms
  
  for(int i = 0; i < ciclos; i++) {
    // Boca aberta
    limparAreaBoca();
    desenharBocaAberta();
    delay(150);
    
    // Boca fechada
    limparAreaBoca();
    desenharBocaFechada();
    delay(150);
  }
  
  // Voltar ao sorriso
  limparAreaBoca();
  desenharBocaSorriso();
  
  // Manter texto por mais um tempo
  delay(1500);
  
  // Limpar balao de fala
  limparBalaoFala();
  
  estaFalando = false;
  emocaoAtual = emocaoBackup;
}

void mostrarBalaoFala(String texto) {
  // Calcular dimensoes do balao
  int larguraTexto = texto.length() * 6 + 10;
  int alturaBalao = 20;
  int xBalao = 80 - larguraTexto/2;
  int yBalao = 5;
  
  // Limitar largura maxima
  if(larguraTexto > 140) {
    larguraTexto = 140;
    xBalao = 10;
  }
  
  // Desenhar balao de fala
  tft.fillRoundRect(xBalao, yBalao, larguraTexto, alturaBalao, 8, COR_BALAO);
  tft.drawRoundRect(xBalao, yBalao, larguraTexto, alturaBalao, 8, COR_BORDA);
  
  // Desenhar "rabinho" do balao
  tft.fillTriangle(75, yBalao + alturaBalao, 80, yBalao + alturaBalao + 5, 85, yBalao + alturaBalao, COR_BALAO);
  tft.drawLine(75, yBalao + alturaBalao, 80, yBalao + alturaBalao + 5, COR_BORDA);
  tft.drawLine(80, yBalao + alturaBalao + 5, 85, yBalao + alturaBalao, COR_BORDA);
  
  // Escrever texto
  tft.setTextColor(ST7735_BLACK);
  tft.setTextSize(1);
  tft.setCursor(xBalao + 5, yBalao + 7);
  tft.print(texto);
}

void limparBalaoFala() {
  // Limpar area do balao (apenas acima do corpo do robo)
  tft.fillRect(0, 0, 160, 10, COR_FUNDO);
  
  // Redesenhar a parte superior do corpo e antena
  // Redesenhar topo do corpo
  tft.fillRoundRect(10, 10, 140, 20, 12, COR_CORPO);
  tft.drawRoundRect(10, 10, 140, 108, 12, COR_BORDA);
  tft.drawRoundRect(11, 11, 138, 106, 11, COR_BORDA);
  
  // Redesenhar topo da tela interna
  tft.fillRoundRect(20, 20, 120, 10, 8, COR_TELA);
  
  // Redesenhar antena
  tft.fillCircle(80, 8, 4, COR_DETALHES);
  tft.drawLine(80, 8, 80, 12, COR_BORDA);
}

// ========== ANIMACOES ==========

void animacaoPiscar() {
  Emocao emocaoAnterior = emocaoAtual;
  desenharRosto(PISCANDO);
  delay(150);
  desenharRosto(emocaoAnterior);
}

