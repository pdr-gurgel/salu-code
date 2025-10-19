/*
 * Robô de Saúde - Arduino Uno (Versão 2.0)
 * Controle de motores DC com ponte L298N, sensor ultrassônico HC-SR04
 * e sensor de trilha infravermelho para região de descanso
 * 
 * Pinagem:
 * Ponte L298N:
 * - IN1: pino 2
 * - IN2: pino 3
 * - IN3: pino 4
 * - IN4: pino 5
 * - ENA: pino 6 (PWM)
 * - ENB: pino 7 (PWM)
 * 
 * Sensor HC-SR04:
 * - TRIG: pino 8
 * - ECHO: pino 9
 * - VCC: 5V
 * - GND: GND
 * 
 * Sensor de Trilha Infravermelho:
 * - OUT: pino 10
 * - VCC: 5V
 * - GND: GND
 */

// Definição dos pinos da ponte L298N
#define IN1 2
#define IN2 3
#define IN3 4
#define IN4 5
#define ENA 6  // PWM
#define ENB 7  // PWM

// Definição dos pinos do sensor ultrassônico
#define TRIG_PIN 8
#define ECHO_PIN 9

// Definição do pino do sensor de trilha infravermelho
#define SENSOR_TRILHA 10

// Velocidade dos motores (0-255)
#define VELOCIDADE_NORMAL 150
#define VELOCIDADE_BAIXA 100

// Distância mínima para parar (em cm)
#define DISTANCIA_MINIMA 20

// Estados do sensor de trilha
#define REGIAO_BRANCA 0    // Sensor detecta superfície branca (descanso)
#define REGIAO_ESCURA 1    // Sensor detecta superfície escura (movimento)

void setup() {
  // Configuração dos pinos de saída para a ponte L298N
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  
  // Configuração dos pinos do sensor ultrassônico
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  // Configuração do pino do sensor de trilha
  pinMode(SENSOR_TRILHA, INPUT);
  
  // Inicialização da comunicação serial (apenas para debug)
  Serial.begin(9600);
  Serial.println("Robô de Saúde v2.0 iniciado!");
  Serial.println("Modo automático com detecção de região de descanso");
  
  // Parar motores na inicialização
  parar();
}

void loop() {
  // Verificar se está na região de descanso (superfície branca)
  if (verificarRegiaoDescanso()) {
    // Se estiver na região branca, parar e aguardar
    parar();
    Serial.println("Região de descanso detectada - Robô parado");
    delay(500);
    return;
  }
  
  // Se não estiver na região de descanso, operar normalmente
  // Modo automático - evita obstáculos
  modoAutomatico();
  
  delay(100);
}

// Função para verificar se está na região de descanso (superfície branca)
bool verificarRegiaoDescanso() {
  int estadoSensor = digitalRead(SENSOR_TRILHA);
  
  // Sensor retorna LOW (0) quando detecta superfície branca
  // Sensor retorna HIGH (1) quando detecta superfície escura
  if (estadoSensor == REGIAO_BRANCA) {
    return true;  // Está na região de descanso
  } else {
    return false; // Não está na região de descanso
  }
}

// Função para medir distância com o sensor ultrassônico
float medirDistancia() {
  // Limpar o pino TRIG
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  
  // Enviar pulso de 10 microssegundos
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  // Ler o tempo de resposta do ECHO
  long duracao = pulseIn(ECHO_PIN, HIGH);
  
  // Calcular distância (velocidade do som = 343 m/s)
  float distancia = duracao * 0.034 / 2;
  
  return distancia;
}

// Modo automático - evita obstáculos
void modoAutomatico() {
  float distancia = medirDistancia();
  
  Serial.print("Distância: ");
  Serial.print(distancia);
  Serial.println(" cm");
  
  if (distancia > DISTANCIA_MINIMA) {
    // Se não há obstáculo próximo, seguir em frente
    frente();
    Serial.println("Movendo para frente");
  } else {
    // Se há obstáculo, parar e virar
    parar();
    Serial.println("Obstáculo detectado - procurando caminho");
    delay(500);
    
    // Medir distâncias à esquerda e direita
    esquerda();
    delay(300);
    parar();
    delay(100);
    float distEsquerda = medirDistancia();
    
    direita();
    delay(600); // Virar mais para a direita
    parar();
    delay(100);
    float distDireita = medirDistancia();
    
    // Voltar para frente
    esquerda();
    delay(300);
    parar();
    delay(100);
    
    // Escolher direção com mais espaço
    if (distEsquerda > distDireita) {
      esquerda();
      delay(500);
      Serial.println("Virando para esquerda");
    } else {
      direita();
      delay(500);
      Serial.println("Virando para direita");
    }
  }
}

// Funções de movimentação

void frente() {
  // Motor esquerdo para frente
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, VELOCIDADE_NORMAL);
  
  // Motor direito para frente
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, VELOCIDADE_NORMAL);
}

void tras() {
  // Motor esquerdo para trás
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, VELOCIDADE_NORMAL);
  
  // Motor direito para trás
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, VELOCIDADE_NORMAL);
}

void esquerda() {
  // Motor esquerdo para trás
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, VELOCIDADE_BAIXA);
  
  // Motor direito para frente
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, VELOCIDADE_NORMAL);
}

void direita() {
  // Motor esquerdo para frente
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, VELOCIDADE_NORMAL);
  
  // Motor direito para trás
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, VELOCIDADE_BAIXA);
}

void parar() {
  // Parar ambos os motores
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}
