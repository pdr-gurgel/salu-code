# Histórico de Atualizações - Salu Robot

## [v0.2] - 2024-10-23

### Adicionado
- **Sistema de Fala**: Robô agora pode "falar" frases com animação
- **Balões de Diálogo**: Balões de fala com texto acima do robô
- **Animação de Boca**: Boca abre e fecha sincronizada com a fala
- **10 Frases Diferentes**: Array com frases variadas para o robô falar
- **Fala Aleatória**: Sistema escolhe frases aleatoriamente
- **Controle de Estado**: Variável `estáFalando` para evitar conflitos de animação

### Funcionalidades de Fala
- Balão de diálogo com borda e "rabinho" apontando para o robô
- Animação de boca com ciclo abrir/fechar (150ms cada)
- Duração da fala baseada no tamanho do texto (150ms por caractere)
- Limpeza automática do balão após a fala
- Apresentação inicial ao ligar: "Olá! Eu sou o Salu!" + "Vou falar algumas coisas!"

### Frases Implementadas
- "Olá!", "Como vai?", "Eu sou o Salu!", "Tudo bem?"
- "Estou feliz!", "Que legal!", "Vamos lá!", "Oba!"
- "Legal!", "Massa!"

### Simplificações
- Reduzido de 7 para 3 estados de emoção (FELIZ, PISCANDO, FALANDO)
- Foco na funcionalidade de fala e animação de boca
- Remoção temporária de emoções complexas para otimização

### Arquivos Criados
- `versoes/v0.2_sistema_fala/salu_robot_display.ino`
- `versoes/v0.2_sistema_fala/README.md`

---

## [v0.1] - 2024-10-23

### Adicionado
- **Display ST7735**: Implementação completa do display 160x128
- **Modo Horizontal**: Display configurado em landscape (rotação 1)
- **Design Estilo Tamagotchi/BMO**: Corpo de console retro com bordas arredondadas
- **Robô Salu**: Desenho com tela interna, botões decorativos e antena
- **7 Expressões Faciais**:
  - FELIZ 😊 - Olhos normais com sorriso
  - PISCANDO 😌 - Animação de piscar
  - MUITO_FELIZ 😄 - Olhos em "^" e boca grande
  - SURPRESO 😮 - Olhos arregalados e boca "O"
  - DORMINDO 😴 - Olhos fechados com "Zzz"
  - PENSANDO 🤔 - Olhos olhando pro lado com "..."
  - AMOR 😍 - Corações nos olhos
- **Animações**:
  - Piscar automático (a cada 3.5 segundos)
  - Mudança de emoção (a cada 8 segundos)
  - Sequência cíclica de todas as emoções
  - Elementos visuais (texto, corações, "Zzz")
- **Paleta de Cores Retro**: Azul escuro, ciano brilhante, cinza claro
- **Estrutura Modular**: Funções separadas para cada expressão
- **Código Comentado**: Comentários em português

### Hardware Suportado
- Arduino Uno
- Display ST7735 (160x128 pixels)
- Pinagem otimizada evitando portas 2-7

### Funcionalidades Técnicas
- Comunicação SPI com display
- Sistema de cores personalizáveis (RGB565)
- Animações suaves com controle de timing
- Desenho de formas complexas (corações, expressões)
- Sistema de estados com enum

### Arquivos Criados
- `versoes/v0.1_display_base/salu_robot_display.ino`
- `versoes/v0.1_display_base/README.md`
- `historico/CHANGELOG.md`
- `historico/README.md`

---

## Convenções de Versionamento

### Formato: v[MAJOR].[MINOR]
- **MAJOR**: Versões principais com mudanças significativas (v1.0, v2.0)
- **MINOR**: Novas funcionalidades e melhorias (v0.1, v0.2, v0.3)

### Estrutura de Pastas
```
/versoes/
  /v[VERSION]_[FOCUS]/
    - salu_robot_display.ino (código principal)
    - README.md (documentação específica)
/historico/
  - CHANGELOG.md (este arquivo)
  - README.md (documentação geral)
```

### Padrão de Desenvolvimento
- **v0.x**: Versões de desenvolvimento e testes
- **v1.0**: Primeira versão completa e estável
- **v2.0+**: Versões com mudanças arquiteturais significativas

### Padrão de Commits
- `feat:` Nova funcionalidade
- `fix:` Correção de bug
- `docs:` Atualização de documentação
- `refactor:` Refatoração de código
- `test:` Adição de testes

---

## Próximas Versões Planejadas

### [v0.3] - Planejada
- **Interatividade**: Botões físicos para interagir com o robô
- **Mais frases**: Expandir biblioteca de falas
- **Respostas contextuais**: Frases baseadas em eventos

### [v0.4] - Planejada
- **Sensores básicos**: Temperatura, luminosidade
- **Reações**: Emoções baseadas em dados dos sensores
- **Indicadores visuais**: Mostrar dados na tela

### [v1.0] - Planejada
- **Versão completa**: Integração de todas as funcionalidades
- **Sistema de menu**: Navegação por diferentes modos
- **Configurações**: Ajustes personalizáveis
- **Estabilidade**: Código otimizado e testado