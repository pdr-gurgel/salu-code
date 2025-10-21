/*
 * Robô de Saúde - Arduino Uno (Versão 0.3)
 * V0.1 -> Movimentação bruta do Sensor apenas com os motores configurados.
 * V0.2 -> Adição do Sensor ultrassônico para evitar obstáculos.
 * V0.3 -> Adição do Sensor de Trilha Infravermelho para detectar a região de descanso.
 * 
 * Pinagem:
 * Ponte L298N -> Motores DC:
 * - IN1: pino 2
 * - IN2: pino 3
 * - IN3: pino 4
 * - IN4: pino 5
 * - ENA: pino 6 (PWM)
 * - ENB: pino 7 (PWM)
 * Sensor Ultrassônico:
 * - TRIG: pino 8
 * - ECHO: pino 9
 * - VCC: 5V
 * - GND: GND
 * Sensor de Trilha Infravermelho:
 * - OUT: pino 10
 * - VCC: 5V
 * - GND: GND
 */

// Definição dos pinos
#define IN1 2 // MOTOR ESQUERDO
#define IN2 3 // MOTOR ESQUERDO
#define IN3 4 // MOTOR DIREITO
#define IN4 5 // MOTOR DIREITO
#define ENA 6  // PWM
#define ENB 7  // PWM
#define TRIG_PIN 8 // ULTRASSÔNICO
#define ECHO_PIN 9 // ULTRASSÔNICO
#define SENSOR_TRILHA 10 // TRILHA INFRAVERMELHO
#define VELOCIDADE_NORMAL 150
#define VELOCIDADE_BAIXA 100
#define DISTANCIA_MINIMA 20 // DISTÂNCIA ULTRASSÔNICO
#define REGIAO_BRANCA 0    // Sensor detecta superfície branca (descanso)
#define REGIAO_ESCURA 1    // Sensor detecta superfície escura (movimento)

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(SENSOR_TRILHA, INPUT);
  Serial.begin(9600);
  Serial.println("Salú v0.3 iniciado!");
  Serial.println("Modo automático com detecção de região de descanso");
  
  parar();
}

void loop() {
  if (verificarRegiaoDescanso()) {
    // Se estiver na região branca(folha sulfite), parar e aguardar
    parar();
    Serial.println("Região de descanso detectada - Salú parado");
    delay(500);
    return;
  }
  
  // Se não estiver na região de descanso, operar normalmente (qualquer área fora da folha sulfite)
  modoAutomatico();
  
  delay(100);
}

bool verificarRegiaoDescanso() {
  int estadoSensor = digitalRead(SENSOR_TRILHA);
  
  // Sensor retorna LOW (0) quando detecta superfície branca(folha sulfite)
  // Sensor retorna HIGH (1) quando detecta superfície escura(qualquer área fora da folha sulfite)
  if (estadoSensor == REGIAO_BRANCA) {
    return true;  // Está na região de descanso
  } else {
    return false; // Não está na região de descanso
  }
}

float medirDistancia() {
  // Limpar o pino TRIG do sensor ultrassônico
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  
  // Enviar pulso de 10 ms
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  long duracao = pulseIn(ECHO_PIN, HIGH);
  float distancia = duracao * 0.034 / 2;
  return distancia;
}

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
    parar();
    Serial.println("Obstáculo detectado - procurando caminho");
    delay(500);
    
    // Medir distâncias à esquerda
    esquerda();
    delay(300);
    parar();
    delay(100);
    float distEsquerda = medirDistancia();
    
    // Medir distâncias à direita
    direita();
    delay(600);
    parar();
    delay(100);
    float distDireita = medirDistancia();
    
    // Voltar para frente após medir distâncias à esquerda e direita
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