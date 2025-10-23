# Salu Robot - Projeto de Robô Interativo

## Visão Geral
O Salu Robot é um projeto de robô interativo desenvolvido em Arduino, focado em criar um assistente de saúde amigável e acessível. O projeto é desenvolvido em versões modulares, permitindo a integração gradual de funcionalidades.

## Filosofia do Projeto
- **Modularidade**: Cada versão foca em uma funcionalidade específica
- **Acessibilidade**: Código simples e bem documentado
- **Interatividade**: Interface amigável e responsiva
- **Educação**: Projeto didático para aprendizado de Arduino e robótica

## Estrutura do Projeto

```
Salu Robot/
├── versoes/                    # Versões do código
│   ├── v1.0_display_st7735/   # Primeira versão - Display
│   ├── v1.1_sensores/         # Segunda versão - Sensores
│   ├── v1.2_rodas/            # Terceira versão - Movimento
│   └── v2.0_completo/         # Versão integrada
└── historico/                  # Documentação e histórico
    ├── CHANGELOG.md           # Histórico de mudanças
    └── README.md              # Este arquivo
```

## Roadmap de Desenvolvimento

### Fase 0: Protótipos e Testes (v0.x)
- ✅ v0.1: Display base com 7 expressões faciais
- ✅ v0.2: Sistema de fala com animação de boca
- 🔄 v0.3: Interatividade com botões
- 🔄 v0.4: Sensores básicos

### Fase 1: Versão Completa Display (v1.0)
- ⏳ Integração de todas funcionalidades de display
- ⏳ Sistema de menu
- ⏳ Configurações personalizáveis
- ⏳ Código otimizado e estável

### Fase 2: Sensores e Monitoramento (v1.x)
- ⏳ Sensores de temperatura e umidade
- ⏳ Monitoramento de ambiente
- ⏳ Reações baseadas em dados

### Fase 3: Movimento (v1.x)
- ⏳ Sistema de rodas
- ⏳ Controle de movimento
- ⏳ Navegação básica

### Fase 4: Integração Total (v2.0)
- ⏳ Sistema completo integrado
- ⏳ IA básica para decisões
- ⏳ Comunicação externa

## Tecnologias Utilizadas

### Hardware
- **Microcontrolador**: Arduino Uno
- **Display**: ST7735 (160x128)
- **Sensores**: DHT22, HC-SR04 (planejados)
- **Motores**: Servo motors, DC motors (planejados)
- **Comunicação**: Bluetooth, WiFi (planejados)

### Software
- **Linguagem**: C++ (Arduino)
- **Bibliotecas**: Adafruit GFX, ST7735
- **IDE**: Arduino IDE
- **Controle de versão**: Git

## Contribuição

### Como Contribuir
1. Escolha uma versão específica para trabalhar
2. Siga a estrutura de pastas estabelecida
3. Documente todas as mudanças no CHANGELOG.md
4. Mantenha o código comentado em português
5. Teste todas as funcionalidades antes de commitar

### Padrões de Código
- **Comentários**: Sempre em português
- **Nomes de variáveis**: camelCase em inglês
- **Funções**: Nomes descritivos em português
- **Estrutura**: Modular e reutilizável

### Documentação
- Cada versão deve ter seu próprio README.md
- CHANGELOG.md deve ser atualizado a cada mudança
- Código deve ser autoexplicativo com comentários

## Licença
Este projeto é desenvolvido para fins educacionais e de pesquisa. 
Uso livre para projetos não comerciais.

## Contato
Para dúvidas, sugestões ou contribuições, consulte a documentação específica de cada versão ou abra uma issue no repositório.

---

**Última atualização**: 23 de outubro de 2024
**Versão atual**: v0.2 - Sistema de Fala
