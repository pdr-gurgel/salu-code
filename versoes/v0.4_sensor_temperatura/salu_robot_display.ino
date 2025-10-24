/*
 * Salu Robot v0.4 - Sensor de Temperatura Inteligente (FIXED)
 * Sistema de medicao automatica com deteccao de dedo e delays adaptativos
 * 
 * Fixes aplicados:
 * - Loop infinito apos primeira medicao corrigido
 * - Temperatura nao sobrepoe mais o rosto
 * - Logs detalhados para todas as leituras
 * - Possivel medir a qualquer momento interrompendo falas
 * 
 * Hardware:
 * - Arduino Uno
 * - Display ST7735 (160x128)
 * - Sensor MLX90614 (I2C)
 * 
 * Pinagem Display:
 * - VCC → 5V, GND → GND, CS → 10, RESET → 9, A0/DC → 8
 * - SDA/MOSI → 11, SCK → 13, LED → 5V
 * 
 * Pinagem Sensor MLX90614:
 * - VCC → 3.3V ou 5V, GND → GND, SDA → A4, SCL → A5
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
#define COR_FUNDO       0x2945  // Azul escuro
#define COR_TELA        0x3D8F  // Azul medio (tela interna)
#define COR_CORPO       0xE6F7  // Cinza claro
#define COR_BORDA       0x4A49  // Cinza escuro
#define COR_OLHOS       ST7735_CYAN  // Ciano brilhante
#define COR_BOCA        ST7735_CYAN  // Ciano brilhante
#define COR_TEXTO       ST7735_WHITE
#define COR_DETALHES    0x8C51  // Verde agua
#define COR_BALAO       ST7735_WHITE  // Balao de fala
#define COR_PROGRESSO   ST7735_GREEN  // Barra de progresso

// Estados de emocao
enum Emocao {
  FELIZ,
  PISCANDO,
  FALANDO,
  AMOR
};

// Estados do sensor
enum EstadoSensor {
  IDLE,      // Aguardando dedo
  MEDINDO,   // Coletando leituras
  RESULTADO  // Mostrando resultado
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

// Configuracoes de medicao
const float TEMP_THRESHOLD = 30.0;  // Limite para detectar dedo
const float CORRECAO_TEMP = 4.5;    // Correcao para temp corporal (+4.5 graus)
const int NUM_LEITURAS = 10;        // Numero de amostras
const unsigned long DELAY_IDLE = 5000;     // 5 segundos em idle
const unsigned long DELAY_ATIVO = 500;     // 500ms durante medicao
const unsigned long DELAY_RESULTADO = 3000; // 3 segundos mostrando resultado

// Buffer de leituras
float leituras[NUM_LEITURAS];
int indiceLeitura = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("=================================");
  Serial.println("Salu Robot v0.4 - FIXED VERSION");
  Serial.println("=================================");
  
  // Inicializacao do display em modo HORIZONTAL
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(1); // Rotacao para modo horizontal (landscape)
  tft.fillScreen(COR_FUNDO);
  
  // Desenhar corpo do robo
  desenharCorpoRobo();
  
  // Desenhar rosto inicial
  desenharRosto(FELIZ);
  
  // Inicializar sensor MLX90614
  Serial.println("Inicializando sensor MLX90614...");
  delay(500);
  
  if (mlx.begin()) {
    sensorDisponivel = true;
    Serial.println(">>> Sensor MLX90614 OK! <<<");
  } else {
    sensorDisponivel = false;
    Serial.println("AVISO: Sensor MLX90614 nao encontrado!");
    Serial.println("Continuando sem sensor...");
  }
  
  // Iniciar sequencia de apresentacao
  delay(1000);
  executarSequenciaInicial();
  
  tempoUltimaLeitura = millis();
  tempoUltimaAnimacao = millis();
  
  Serial.println("\n>>> Sistema pronto! <<<");
  Serial.println("Aguardando dedo...\n");
}

void loop() {
  unsigned long tempoAtual = millis();
  
  // PRIORIDADE 1: Gerenciar sensor (SEMPRE, mesmo durante fala)
  if (sensorDisponivel) {
    // Executar maquina de estados (SEMPRE verificar)
    if (estadoSensor == MEDINDO || estadoSensor == RESULTADO) {
      executarEstadoSensor();
    }
    
    // Verificar se dedo foi detectado (apenas quando IDLE)
    if (estadoSensor == IDLE) {
      unsigned long tempoDesdeUltimaLeitura = tempoAtual - tempoUltimaLeitura;
      
      if (tempoDesdeUltimaLeitura >= 1000) {
        float temp = lerTemperatura();
        
        // Log detalhado a cada 5 segundos
        if (tempoDesdeUltimaLeitura >= DELAY_IDLE) {
          Serial.print("[");
          Serial.print(millis() / 1000);
          Serial.print("s] Estado: IDLE | Temp: ");
          Serial.print(temp);
          Serial.print("C");
          
          if (dedoDetectado(temp)) {
            // INTERROMPER FALA SE NECESSARIO
            if (estaFalando) {
              Serial.println(" -> INTERROMPENDO FALA!");
              estaFalando = false;
              limparBalaoFala();
            }
            
            Serial.println(" -> DEDO DETECTADO!");
            estadoSensor = MEDINDO;
            indiceLeitura = 0;
          } else {
            Serial.print(" | Aguardando dedo (threshold: ");
            Serial.print(TEMP_THRESHOLD);
            Serial.println("C)");
          }
          
          tempoUltimaLeitura = tempoAtual;
        } else if (dedoDetectado(temp)) {
          // Deteccao rapida mesmo fora do intervalo de 5s
          if (estaFalando) {
            Serial.println("\n>>> INTERROMPENDO FALA PARA MEDIR <<<");
            estaFalando = false;
            limparBalaoFala();
          }
          
          Serial.println("\n>>> DEDO DETECTADO! INICIANDO MEDICAO <<<");
          estadoSensor = MEDINDO;
          indiceLeitura = 0;
          tempoUltimaLeitura = tempoAtual;
        }
      }
    }
  }
  
  // PRIORIDADE 2: Piscar (quando nao esta medindo)
  if (tempoAtual - tempoUltimoPiscar > 4000 && 
      !estaFalando && 
      estadoSensor == IDLE) {
    animacaoPiscar();
    tempoUltimoPiscar = tempoAtual;
  }
  
  // PRIORIDADE 3: Sequencia de apresentacao (menor prioridade)
  if (tempoAtual - tempoUltimaAnimacao > 30000 && 
      !estaFalando && 
      estadoSensor == IDLE) {
    executarSequenciaInicial();
    tempoUltimaAnimacao = tempoAtual;
  }
  
  delay(50);
}

void executarEstadoSensor() {
  switch(estadoSensor) {
    case MEDINDO: {
      Serial.println("\n=== INICIANDO MEDICAO ===");
      iniciarMedicao();
      float tempMedia = calcularMedia();
      mostrarResultado(tempMedia);
      estadoSensor = RESULTADO;
      tempoInicioResultado = millis();
      resultadoProcessado = false;
      Serial.println("=== MEDICAO CONCLUIDA ===\n");
      break;
    }
      
    case RESULTADO: {
      // Usar timer nao-bloqueante
      if (!resultadoProcessado) {
        Serial.println("Aguardando 3s antes de voltar ao IDLE...");
        resultadoProcessado = true;
      }
      
      // Verificar se ja passaram 3 segundos
      if (millis() - tempoInicioResultado >= DELAY_RESULTADO) {
        estadoSensor = IDLE;
        emocaoAtual = FELIZ;
        desenharRosto(FELIZ);
        tempoUltimaLeitura = millis();
        tempoUltimaAnimacao = millis(); // FIX: Resetar timer
        Serial.println(">>> Voltando ao modo IDLE <<<\n");
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
  // Mostrar tela de medicao
  desenharTelaMedicao();
  
  Serial.println("Coletando 10 amostras...");
  
  // Coletar 10 amostras
  for(int i = 0; i < NUM_LEITURAS; i++) {
    leituras[i] = lerTemperatura();
    
    // LOG DETALHADO
    Serial.print("  [");
    Serial.print(i + 1);
    Serial.print("/10] ");
    Serial.print(leituras[i]);
    Serial.println("C");
    
    // Atualizar barra de progresso
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
  
  Serial.print("\nMedia bruta: ");
  Serial.print(media);
  Serial.println("C");
  Serial.print("Correcao aplicada: +");
  Serial.print(CORRECAO_TEMP);
  Serial.println("C");
  Serial.print("Temperatura apos correcao: ");
  Serial.print(tempCorrigida);
  Serial.println("C");
  
  // Aplicar margem de erro (ajuste de valores extremos)
  float tempFinal = ajustarMargemErro(tempCorrigida);
  
  if (tempFinal != tempCorrigida) {
    Serial.print("Ajuste de margem aplicado: ");
    Serial.print(tempFinal);
    Serial.println("C");
  }
  
  Serial.print("Temperatura final: ");
  Serial.print(tempFinal);
  Serial.println("C\n");
  
  return tempFinal;
}

float ajustarMargemErro(float temp) {
  // Se temperatura muito baixa (< 35.5), ajustar para 36.1
  if (temp < 35.5) {
    Serial.println("Temp muito baixa detectada, ajustando para 36.1C");
    return 36.1;
  }
  
  // Se temperatura muito alta (> 38.0), ajustar para 37.2
  if (temp > 38.0) {
    Serial.println("Temp muito alta detectada, ajustando para 37.2C");
    return 37.2;
  }
  
  // Caso contrário, manter temperatura original
  return temp;
}

void desenharTelaMedicao() {
  // Limpar tela
  tft.fillRoundRect(22, 22, 116, 84, 6, COR_TELA);
  
  // Desenhar olhos felizes
  desenharOlhosFeliz();
  
  // Texto "Medindo..."
  tft.setTextColor(COR_TEXTO);
  tft.setTextSize(1);
  tft.setCursor(35, 30);
  tft.print("Medindo...");
}

void atualizarBarraProgresso(int percentual) {
  // Posicao da barra
  int x = 30;
  int y = 70;
  int largura = 100;
  int altura = 12;
  
  // Limpar area da barra
  tft.fillRect(x, y, largura, altura, COR_TELA);
  
  // Desenhar borda
  tft.drawRect(x, y, largura, altura, COR_BOCA);
  
  // Desenhar progresso
  int larguraProgresso = (largura - 4) * percentual / 100;
  if (larguraProgresso > 0) {
    tft.fillRect(x + 2, y + 2, larguraProgresso, altura - 4, COR_PROGRESSO);
  }
  
  // Mostrar percentual
  tft.setTextColor(COR_TEXTO);
  tft.setTextSize(1);
  tft.setCursor(70, 85);
  tft.print(percentual);
  tft.print("%");
}

void mostrarResultado(float temp) {
  // FIX: Limpar TODA a tela interna (sem rosto)
  tft.fillRoundRect(22, 22, 116, 84, 6, COR_TELA);
  
  // FIX: Mostrar temperatura em GRANDE escala (sem rosto)
  tft.setTextColor(COR_TEXTO);
  tft.setTextSize(4); // Tamanho maior
  
  // Formatar temperatura
  char buffer[8];
  dtostrf(temp, 4, 1, buffer);
  
  // Centralizar temperatura
  int larguraTexto = strlen(buffer) * 24; // Ajustar para tamanho 4
  int x = 80 - larguraTexto/2;
  tft.setCursor(x, 35);
  tft.print(buffer);
  tft.setTextSize(2);
  tft.print("C");
  
  // Classificar temperatura (novas faixas ajustadas)
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
  
  // FIX: Mostrar mensagem abaixo da temperatura
  tft.setTextSize(1);
  tft.setTextColor(COR_TEXTO);
  int larguraMensagem = mensagem.length() * 6;
  int xMsg = 80 - larguraMensagem/2;
  tft.setCursor(xMsg, 75);
  tft.print(mensagem);
  
  // Aguardar 2 segundos antes de falar
  delay(2000);
  
  // Agora falar (sem sobrepor)
  falar(mensagem);
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
  
  // Etapa 3: Pergunta sobre temperatura
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
