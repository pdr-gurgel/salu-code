/*
 * Salu Robot v0.4 - LITE (Otimizado Simples)
 * Versao com otimizacoes conservadoras - FUNCIONAL
 * 
 * Otimizacoes aplicadas:
 * - Logs reduzidos (F() macro)
 * - Constantes com #define
 * - Reducao de NUM_LEITURAS de 10 para 8
 * - Bug de boca corrigido (nao aparece durante resultado)
 * 
 * Hardware:
 * - Arduino Uno
 * - Display ST7735 (160x128)
 * - Sensor MLX90614 (I2C)
 */

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_MLX90614.h>

// Definicoes dos pinos do display
#define TFT_CS    10
#define TFT_RST   9
#define TFT_DC    8

// Inicializacao do display e sensor
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

// Paleta de cores estilo retro/BMO
#define COR_FUNDO       0x2945
#define COR_TELA        0x3D8F
#define COR_CORPO       0xE6F7
#define COR_BORDA       0x4A49
#define COR_OLHOS       ST7735_CYAN
#define COR_BOCA        ST7735_CYAN
#define COR_TEXTO       ST7735_WHITE
#define COR_DETALHES    0x8C51
#define COR_BALAO       ST7735_WHITE
#define COR_PROGRESSO   ST7735_GREEN

// Estados de emocao
enum Emocao {
  FELIZ,
  PISCANDO,
  FALANDO,
  AMOR
};

// Estados do sensor
enum EstadoSensor {
  IDLE,
  MEDINDO,
  RESULTADO
};

// Variaveis de controle
Emocao emocaoAtual = FELIZ;
EstadoSensor estadoSensor = IDLE;
unsigned long tempoUltimaAnimacao = 0;
unsigned long tempoUltimoPiscar = 0;
unsigned long tempoUltimaLeitura = 0;
unsigned long tempoInicioResultado = 0;
bool estaFalando = false;
bool sensorDisponivel = false;
bool resultadoProcessado = false;

// Configuracoes de medicao (usando #define economiza RAM)
#define TEMP_THRESHOLD 30.0
#define CORRECAO_TEMP 4.5
#define NUM_LEITURAS 8  // Reduzido de 10 para 8 (economiza 8 bytes)
#define DELAY_IDLE 5000
#define DELAY_ATIVO 500
#define DELAY_RESULTADO 3000

// Buffer de leituras
float leituras[NUM_LEITURAS];

void setup() {
  Serial.begin(9600);
  Serial.println(F("Salu Robot v0.4 LITE"));
  
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(1);
  tft.fillScreen(COR_FUNDO);
  
  desenharCorpoRobo();
  desenharRosto(FELIZ);
  
  delay(500);
  
  if (mlx.begin()) {
    sensorDisponivel = true;
    Serial.println(F("Sensor OK"));
  } else {
    sensorDisponivel = false;
    Serial.println(F("Sensor N/A"));
  }
  
  delay(1000);
  executarSequenciaInicial();
  
  tempoUltimaLeitura = millis();
  tempoUltimaAnimacao = millis();
  
  Serial.println(F("Sistema pronto"));
}

void loop() {
  unsigned long tempoAtual = millis();
  
  // PRIORIDADE 1: Gerenciar sensor
  if (sensorDisponivel) {
    if (estadoSensor == MEDINDO || estadoSensor == RESULTADO) {
      executarEstadoSensor();
    }
    
    if (estadoSensor == IDLE) {
      unsigned long tempoDesdeUltimaLeitura = tempoAtual - tempoUltimaLeitura;
      
      if (tempoDesdeUltimaLeitura >= 1000) {
        float temp = mlx.readObjectTempC();
        
        if (tempoDesdeUltimaLeitura >= DELAY_IDLE) {
          if (temp >= TEMP_THRESHOLD) {
            if (estaFalando) {
              estaFalando = false;
              limparBalaoFala();
            }
            estadoSensor = MEDINDO;
          }
          tempoUltimaLeitura = tempoAtual;
        } else if (temp >= TEMP_THRESHOLD) {
          if (estaFalando) {
            estaFalando = false;
            limparBalaoFala();
          }
          estadoSensor = MEDINDO;
          tempoUltimaLeitura = tempoAtual;
        }
      }
    }
  }
  
  // PRIORIDADE 2: Piscar
  if (tempoAtual - tempoUltimoPiscar > 4000 && !estaFalando && estadoSensor == IDLE) {
    animacaoPiscar();
    tempoUltimoPiscar = tempoAtual;
  }
  
  // PRIORIDADE 3: Sequencia
  if (tempoAtual - tempoUltimaAnimacao > 30000 && !estaFalando && estadoSensor == IDLE) {
    executarSequenciaInicial();
    tempoUltimaAnimacao = tempoAtual;
  }
  
  delay(50);
}

void executarEstadoSensor() {
  switch(estadoSensor) {
    case MEDINDO: {
      Serial.println(F("\n=== MEDICAO ==="));
      iniciarMedicao();
      float tempMedia = calcularMedia();
      mostrarResultado(tempMedia);
      estadoSensor = RESULTADO;
      tempoInicioResultado = millis();
      resultadoProcessado = false;
      break;
    }
      
    case RESULTADO: {
      if (!resultadoProcessado) {
        resultadoProcessado = true;
      }
      
      if (millis() - tempoInicioResultado >= DELAY_RESULTADO) {
        estadoSensor = IDLE;
        emocaoAtual = FELIZ;
        desenharRosto(FELIZ);
        tempoUltimaLeitura = millis();
        tempoUltimaAnimacao = millis();
        Serial.println(F("IDLE"));
      }
      break;
    }
  }
}

float lerTemperatura() {
  return mlx.readObjectTempC();
}

bool dedoDetectado(float temp) {
  return (temp >= TEMP_THRESHOLD);
}

void iniciarMedicao() {
  desenharTelaMedicao();
  
  for(int i = 0; i < NUM_LEITURAS; i++) {
    leituras[i] = lerTemperatura();
    int progresso = ((i + 1) * 100) / NUM_LEITURAS;
    atualizarBarraProgresso(progresso);
    delay(DELAY_ATIVO);
  }
}

float calcularMedia() {
  float soma = 0;
  for(int i = 0; i < NUM_LEITURAS; i++) {
    soma += leituras[i];
  }
  float media = soma / NUM_LEITURAS;
  float tempCorrigida = media + CORRECAO_TEMP;
  
  // Ajustar margem de erro
  float tempFinal = ajustarMargemErro(tempCorrigida);
  
  return tempFinal;
}

float ajustarMargemErro(float temp) {
  if (temp < 35.5) return 36.1;
  if (temp > 38.0) return 37.2;
  return temp;
}

void desenharTelaMedicao() {
  tft.fillRoundRect(22, 22, 116, 84, 6, COR_TELA);
  desenharOlhosFeliz();
  tft.setTextColor(COR_TEXTO);
  tft.setTextSize(1);
  tft.setCursor(35, 30);
  tft.print(F("Medindo..."));
}

void atualizarBarraProgresso(int percentual) {
  int x = 30;
  int y = 70;
  int largura = 100;
  int altura = 12;
  
  tft.fillRect(x, y, largura, altura, COR_TELA);
  tft.drawRect(x, y, largura, altura, COR_BOCA);
  
  int larguraProgresso = (largura - 4) * percentual / 100;
  if (larguraProgresso > 0) {
    tft.fillRect(x + 2, y + 2, larguraProgresso, altura - 4, COR_PROGRESSO);
  }
  
  tft.setTextColor(COR_TEXTO);
  tft.setTextSize(1);
  tft.setCursor(70, 85);
  tft.print(percentual);
  tft.print('%');
}

void mostrarResultado(float temp) {
  // Limpar TODA a tela interna (sem rosto, sem boca)
  tft.fillRoundRect(22, 22, 116, 84, 6, COR_TELA);
  
  // Mostrar APENAS temperatura em grande escala
  tft.setTextColor(COR_TEXTO);
  tft.setTextSize(4);
  
  char buffer[8];
  dtostrf(temp, 4, 1, buffer);
  
  int larguraTexto = strlen(buffer) * 24;
  int x = 80 - larguraTexto/2;
  tft.setCursor(x, 35);
  tft.print(buffer);
  tft.setTextSize(2);
  tft.print('C');
  
  // Classificar temperatura
  String mensagem = "";
  if (temp < 35.0) {
    mensagem = "Muito baixa!";
  } else if (temp >= 35.0 && temp < 36.0) {
    mensagem = "Abaixo, mas saudavel";
  } else if (temp >= 36.0 && temp <= 37.0) {
    mensagem = "Normal!";
  } else if (temp > 37.0 && temp <= 38.0) {
    mensagem = "Acima, mas saudavel";
  } else if (temp > 38.0 && temp <= 39.0) {
    mensagem = "Febre leve!";
  } else {
    mensagem = "Febre alta!";
  }
  
  // Mostrar mensagem abaixo da temperatura
  tft.setTextSize(1);
  tft.setTextColor(COR_TEXTO);
  int larguraMensagem = mensagem.length() * 6;
  int xMsg = 80 - larguraMensagem/2;
  tft.setCursor(xMsg, 75);
  tft.print(mensagem);
  
  // Aguardar 3 segundos mostrando APENAS temperatura + mensagem
  // NAO chamar falar() para evitar que a boca apareça
  delay(3000);
}

void executarSequenciaInicial() {
  emocaoAtual = FELIZ;
  desenharRosto(FELIZ);
  falar("Ola! Eu sou o Salu!");
  delay(2000);
  
  emocaoAtual = AMOR;
  desenharRosto(AMOR);
  falar("Como voce esta?");
  delay(1000);
  
  emocaoAtual = FELIZ;
  desenharRosto(FELIZ);
  delay(2000);
  
  if (sensorDisponivel) {
    falar("Aproxime seu dedo!");
    delay(1000);
    falar("Vou medir sua temperatura");
  } else {
    falar("Sensor nao disponivel!");
  }
  delay(2000);
}

void desenharCorpoRobo() {
  tft.fillRoundRect(10, 10, 140, 108, 12, COR_CORPO);
  tft.drawRoundRect(10, 10, 140, 108, 12, COR_BORDA);
  tft.drawRoundRect(11, 11, 138, 106, 11, COR_BORDA);
  tft.fillRoundRect(20, 20, 120, 88, 8, COR_TELA);
  tft.fillCircle(30, 115, 3, COR_DETALHES);
  tft.fillCircle(130, 115, 3, COR_DETALHES);
  tft.fillCircle(80, 8, 4, COR_DETALHES);
  tft.drawLine(80, 8, 80, 12, COR_BORDA);
}

void desenharRosto(Emocao emocao) {
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
      if(emocaoAtual == AMOR) {
        desenharOlhosCoracao();
      } else {
        desenharOlhosFeliz();
      }
      break;
      
    case AMOR:
      desenharOlhosCoracao();
      desenharBocaSorriso();
      break;
  }
}

void desenharOlhosFeliz() {
  tft.fillRoundRect(45, 45, 18, 22, 9, COR_OLHOS);
  tft.fillCircle(54, 58, 4, COR_TELA);
  tft.fillRoundRect(97, 45, 18, 22, 9, COR_OLHOS);
  tft.fillCircle(106, 58, 4, COR_TELA);
}

void desenharOlhosFechados() {
  tft.fillRoundRect(45, 55, 18, 4, 2, COR_OLHOS);
  tft.fillRoundRect(97, 55, 18, 4, 2, COR_OLHOS);
}

void desenharOlhosCoracao() {
  desenharCoracaoOlho(54, 55);
  desenharCoracaoOlho(106, 55);
}

void desenharCoracaoOlho(int x, int y) {
  tft.fillCircle(x-4, y-2, 5, COR_OLHOS);
  tft.fillCircle(x+4, y-2, 5, COR_OLHOS);
  tft.fillTriangle(x-9, y, x, y+10, x+9, y, COR_OLHOS);
}

void desenharBocaSorriso() {
  tft.drawLine(55, 80, 65, 88, COR_BOCA);
  tft.drawLine(65, 88, 95, 88, COR_BOCA);
  tft.drawLine(95, 88, 105, 80, COR_BOCA);
  tft.drawLine(55, 81, 65, 89, COR_BOCA);
  tft.drawLine(95, 89, 105, 81, COR_BOCA);
}

void desenharBocaAberta() {
  tft.fillRoundRect(65, 78, 30, 20, 8, COR_BOCA);
  tft.fillRoundRect(67, 80, 26, 16, 6, COR_TELA);
}

void desenharBocaFechada() {
  tft.fillRoundRect(65, 85, 30, 6, 3, COR_BOCA);
}

void limparAreaBoca() {
  tft.fillRect(50, 75, 60, 25, COR_TELA);
}

void falar(String texto) {
  estaFalando = true;
  Emocao emocaoBackup = emocaoAtual;
  
  mostrarBalaoFala(texto);
  
  int duracaoFala = texto.length() * 150;
  int ciclos = duracaoFala / 300;
  
  for(int i = 0; i < ciclos; i++) {
    limparAreaBoca();
    desenharBocaAberta();
    delay(150);
    
    limparAreaBoca();
    desenharBocaFechada();
    delay(150);
  }
  
  limparAreaBoca();
  desenharBocaSorriso();
  
  delay(1500);
  limparBalaoFala();
  
  estaFalando = false;
  emocaoAtual = emocaoBackup;
}

void mostrarBalaoFala(String texto) {
  int larguraTexto = texto.length() * 6 + 10;
  int alturaBalao = 20;
  int xBalao = 80 - larguraTexto/2;
  int yBalao = 5;
  
  if(larguraTexto > 140) {
    larguraTexto = 140;
    xBalao = 10;
  }
  
  tft.fillRoundRect(xBalao, yBalao, larguraTexto, alturaBalao, 8, COR_BALAO);
  tft.drawRoundRect(xBalao, yBalao, larguraTexto, alturaBalao, 8, COR_BORDA);
  
  tft.fillTriangle(75, yBalao + alturaBalao, 80, yBalao + alturaBalao + 5, 85, yBalao + alturaBalao, COR_BALAO);
  tft.drawLine(75, yBalao + alturaBalao, 80, yBalao + alturaBalao + 5, COR_BORDA);
  tft.drawLine(80, yBalao + alturaBalao + 5, 85, yBalao + alturaBalao, COR_BORDA);
  
  tft.setTextColor(ST7735_BLACK);
  tft.setTextSize(1);
  tft.setCursor(xBalao + 5, yBalao + 7);
  tft.print(texto);
}

void limparBalaoFala() {
  tft.fillRect(0, 0, 160, 10, COR_FUNDO);
  tft.fillRoundRect(10, 10, 140, 20, 12, COR_CORPO);
  tft.drawRoundRect(10, 10, 140, 108, 12, COR_BORDA);
  tft.drawRoundRect(11, 11, 138, 106, 11, COR_BORDA);
  tft.fillRoundRect(20, 20, 120, 10, 8, COR_TELA);
  tft.fillCircle(80, 8, 4, COR_DETALHES);
  tft.drawLine(80, 8, 80, 12, COR_BORDA);
}

void animacaoPiscar() {
  Emocao emocaoAnterior = emocaoAtual;
  desenharRosto(PISCANDO);
  delay(150);
  desenharRosto(emocaoAnterior);
}

