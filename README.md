# Robô de Saúde - Arduino (Versão 2.0)

Este projeto implementa um robô autônomo controlado por Arduino Uno, com detecção de obstáculos e sistema de região de descanso usando sensor de trilha infravermelho.

## Componentes Necessários

- Arduino Uno
- Ponte L298N (driver de motores)
- 2x Motores DC
- Sensor ultrassônico HC-SR04
- Sensor de trilha infravermelho (TCRT5000 ou similar)
- Fonte de alimentação (6-12V para os motores)
- Folha sulfite branca (região de descanso)
- Jumpers e protoboard

## Pinagem

### Ponte L298N
| Pino L298N | Pino Arduino | Função |
|------------|--------------|---------|
| IN1        | 2            | Controle motor esquerdo |
| IN2        | 3            | Controle motor esquerdo |
| IN3        | 4            | Controle motor direito |
| IN4        | 5            | Controle motor direito |
| ENA        | 6 (PWM)      | Velocidade motor esquerdo |
| ENB        | 7 (PWM)      | Velocidade motor direito |

### Sensor HC-SR04
| Pino HC-SR04 | Pino Arduino | Função |
|--------------|--------------|---------|
| VCC          | 5V           | Alimentação |
| GND          | GND          | Terra |
| TRIG         | 8            | Trigger |
| ECHO         | 9            | Echo |

### Sensor de Trilha Infravermelho
| Pino Sensor | Pino Arduino | Função |
|-------------|--------------|---------|
| VCC         | 5V           | Alimentação |
| GND         | GND          | Terra |
| OUT         | 10           | Sinal digital |

## Funcionalidades

### Operação Autônoma
O robô opera de forma completamente autônoma:

- **Detecção de Região de Descanso**: Para automaticamente quando detecta superfície branca (folha sulfite)
- **Navegação Inteligente**: Move-se apenas quando está fora da região de descanso
- **Evitação de Obstáculos**: Detecta obstáculos usando sensor ultrassônico
- **Tomada de Decisão**: Escolhe automaticamente a melhor direção para contornar obstáculos

### Comportamento do Sistema
1. **Na região branca (descanso)**: Robô permanece parado
2. **Fora da região branca**: Robô navega automaticamente
3. **Obstáculo detectado**: Robô para, analisa e escolhe nova direção
4. **Retorno ao descanso**: Robô para automaticamente ao detectar superfície branca

## Instalação e Uso

1. **Conecte os componentes** conforme a pinagem descrita acima
2. **Carregue o código** `robot_saude.ino` no Arduino
3. **Coloque uma folha sulfite branca** no chão (região de descanso)
4. **Ligue o robô** - ele iniciará automaticamente
5. **Monitore via Serial** (9600 baud) para debug (opcional)

## Configurações

### Velocidades
- `VELOCIDADE_NORMAL`: 150 (para movimentos diretos)
- `VELOCIDADE_BAIXA`: 100 (para curvas)

### Detecção de Obstáculos
- `DISTANCIA_MINIMA`: 20cm (distância para parar e evitar obstáculos)

### Sensor de Trilha
- `REGIAO_BRANCA`: 0 (sensor detecta superfície branca - descanso)
- `REGIAO_ESCURA`: 1 (sensor detecta superfície escura - movimento)

## Estrutura do Código

### Funções Principais
- `verificarRegiaoDescanso()` - Verifica se está na região branca
- `frente()` - Move o robô para frente
- `tras()` - Move o robô para trás
- `esquerda()` - Vira o robô para esquerda
- `direita()` - Vira o robô para direita
- `parar()` - Para ambos os motores
- `medirDistancia()` - Mede distância com sensor ultrassônico
- `modoAutomatico()` - Lógica de navegação automática

### Lógica de Controle
O código implementa:
- Controle PWM para velocidade variável dos motores
- Medição de distância por ultrassom
- Detecção de superfície branca com sensor infravermelho
- Algoritmo de evitação de obstáculos
- Operação completamente autônoma

## Próximos Passos

Para expandir o projeto, considere adicionar:
- Sensor de linha para seguir trilhas
- Buzzer para alertas sonoros
- LEDs indicadores de status
- Bluetooth para controle remoto
- Módulo WiFi para controle via web
- Sensores de inclinação (acelerômetro/giroscópio)

## Segurança

- Certifique-se de que a fonte de alimentação suporta a corrente necessária dos motores
- Use diodos de proteção se necessário
- Teste em ambiente controlado antes de uso em espaços abertos
- Mantenha o robô longe de escadas e áreas perigosas
