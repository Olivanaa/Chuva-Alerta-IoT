# 🌧️ Chuva Alerta - Sistema Inteligente de Prevenção a Enchentes

![Banner do Projeto](./imgs/chuva-alerta2.png)

## 📌 Problema: O Desafio das Enchentes no Brasil

**Dados Alarmantes:**
- ⚠️ 110 milhões de brasileiros afetados por enchentes (1991-2022)
- 💸 Prejuízos anuais de R$ 5 bilhões
- 🚨 Tempo médio de alerta atual: 2-6 horas (insuficiente para evacuações)

**Principais Gargalos:**
1. Sistemas de monitoramento desatualizados
2. Falta de integração entre dados meteorológicos e sensores locais
3. Comunicação ineficaz em áreas remotas
4. Dificuldade na priorização de áreas de risco

## 🚀 Nossa Solução: Tecnologia para Salvar Vidas

### Visão Geral
Sistema IoT integrado que combina:
- **Monitoramento em tempo real** (sensores locais)
- **Previsão avançada** (IA + imagens de satélite)
- **Alertas inteligentes** (multicanal e georreferenciados)

### Arquitetura do Sistema
```mermaid
graph TD
    A[Sensores IoT] --> B[Processamento de Dados]
    C[Imagens de Satélite] --> B
    B --> D[IA Preditiva]
    D --> E[Análise de Risco]
    E --> F[Painéis de Controle]
    E --> G[Sistema de Alerta Inteligente]
    
    F --> H[Defesa Civil]
    G --> I[Notificações Georreferenciadas]
    G --> J[Rotas de Fuga]
    G --> K[Modo Off-line]
    
    style A fill:#4CAF50,stroke:#388E3C
    style C fill:#2196F3,stroke:#0D47A1
    style D fill:#FF9800,stroke:#F57C00
    style F fill:#9C27B0,stroke:#7B1FA2
    style G fill:#F44336,stroke:#D32F2F
```
### 🔍 Como Funciona?

1. **Detecção Precisa**:
   - Rede de sensores coleta dados críticos
   - IA analisa padrões históricos e em tempo real

2. **Alertas Antecipados**:
   - Até 48h de antecedência para enchentes
   - 3 níveis de gravidade (atenção, alerta, emergência)

3. **Comunicação Eficiente**:
   - SMS e notificações push para moradores
   - Painéis para Defesa Civil
   - Funciona mesmo sem internet

## 🛠️ Hardware: Componentes e Funcionamento

### Diagrama do Circuito
![Simulação no Tinkercad](./imgs/simulação.png)

### Lista de Componentes
| Qtd | Componente               | Função                          |
|-----|--------------------------|---------------------------------|
| 1   | Sensor HC-SR04           | Medir nível de água            |
| 1   | Higrômetro               | Monitorar umidade do solo      |
| 1   | Arduino Uno              | Microcontolador                |
| 3   | LEDs (Verm/Amare/Verde)  | Indicar status                 |
| 1   | Buzzer                   | Alerta sonoro                  |
| 1   | LCD 16x2 I2C             | Exibir informações             |
| 1   | Botão                    | Simulador de pluviômetro - cada pulso equivale a 2mm      |
   

### 📋 Fluxo de Operação
1. Sensores coletam dados a cada 5s
2. Arduino processa e classifica riscos
3. Sistema ativa alertas conforme necessidade
4. Dados são exibidos no LCD

## 💻 Código: Funcionalidades Principais

### Principais Funções:

- lerSensores(): Coleta dados de umidade, nível d'água e precipitação
- avaliarRiscos(): Usa limiares pré-definidos para classificar perigo
- ativarAlertas(): Controla LEDs e buzzer conforme a gravidade
- atualizarDisplay(): Mostra informações no LCD de forma otimizada

## 🔌 Simulação do Projeto

### Como testar no Tinkercad:
1. Acesse o [projeto no Tinkercad](https://www.tinkercad.com/things/5xo0xr1HNxI-globalsolution?sharecode=tCowqLBeVExokEyz3WGyMYYNyjTtXARzz4a2c3ot0c0)
2. Clique em "Copiar e Editar" para criar sua versão
3. Experimente ajustar:
   - Controles de umidade
   - Distância do sensor de água
   - Contador de pulsos de chuva

### Cenários recomendados para teste:
- Simule solo saturado (umidade >80%)
- Crie risco de inundação (nível água <30cm)
- Gere chuva intensa (>20mm)

## 🎥 Vídeo Demonstrativo

[YouTube](#)

## 📊 Especificações Técnicas

| Parâmetro        | Faixa Normal | Estado de Atenção | Nível de Alerta |
|------------------|--------------|-------------------|-----------------|
| Umidade do Solo  | <60%         | 60-80%            | >80%            |
| Nível de Água    | >50cm        | 30-50cm           | <30cm           |
| Precipitação     | <10mm        | 10-20mm           | >20mm           |

## 👷‍♂️👷‍♀️ Equipe

Ana Clara Rocha de Oliveira<br>
Matheus Von Koss Wildeisen<br>
Vinicius Mafra Paiva

