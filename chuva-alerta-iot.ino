// C++ code
//
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Configurações de dispositivos:

const int sensorUmidade = A0;
const int ledVermelho = 12;  
const int ledAmarelo = 8;
const int ledVerde = 10;
const int botao = 7;
const int alarme = 11;
const int trigPin = 2;
const int echoPin = 4;

#define col 16 
#define lin  2 
#define ende  0x27
LiquidCrystal_I2C lcd(ende,col,lin);

// Declaração de variáveis:
String mensagemAtualLinha1 = "";
String mensagemAtualLinha2 = "";

int umidade, chuva, mmChuva;
float percentualUmidade, dist;
int valorChuva = HIGH;
int contador = 0;

unsigned long tempoAnterior = 0;
const long intervalo = 800;
int telaAtual = 0;
int telaAlertaAtual = 0;

bool alarmeLigado = false;
bool alertaUmidade = false;
bool alertaNivelAgua = false;
bool alertaChuva = false;


void setup(){
  lcd.init();          
  lcd.backlight(); 
  
  Serial.begin(9600);
  pinMode(sensorUmidade, INPUT);   
  pinMode(echoPin, INPUT); 
  pinMode(trigPin, OUTPUT);
  pinMode(ledVermelho, OUTPUT);
  pinMode(ledAmarelo, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  pinMode(botao, INPUT_PULLUP);    
  pinMode(alarme, OUTPUT);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Iniciando...");

}

void loop()
{
  umidadeSolo();
  nivelDeAgua();
  pluviometro();
  atualizarLeds();
  controlarAlarme();

  // controla a alternância entre telas do display conforme estado dos sensores
	// commportamento: 
  	//- Alerta: Procura próxima tela de alerta ativo (pulando inativas)
 	//- Normal: Cicla sequencialmente entre todas as telas
  if (millis() - tempoAnterior >= intervalo){
    tempoAnterior = millis();

    if(alertaUmidade || alertaNivelAgua || alertaChuva) {
      for(int i = 1; i <= 3; i++) {
        int proximaTela = (telaAlertaAtual + i) % 3;

        if((proximaTela == 0 && alertaUmidade) || 
           (proximaTela == 1 && alertaNivelAgua) || 
           (proximaTela == 2 && alertaChuva)) {
          telaAlertaAtual = proximaTela;
          break;
        }
      }
      mostrarAlerta();
    } else {
      telaAtual = (telaAtual + 1) % 3;
      mostrarMensagem();
    }
  }

  atualizarLcd();
  
}

// função para medir o percentual de umidade
void umidadeSolo(){
  umidade = analogRead(sensorUmidade);
  percentualUmidade = map(umidade, 0, 876, 0, 100);
  Serial.println("\n=== DADOS DE UMIDADE ===");
  Serial.print("Valor bruto: ");
  Serial.print(umidade);
  Serial.print(" | Percentual: ");
  Serial.print(percentualUmidade);
  Serial.println("%");
    
  if (percentualUmidade > 80) {  
    alertaUmidade = true;
  }else if(percentualUmidade > 60){
    alertaUmidade = false;
  }else{
    alertaUmidade = false;
  }
}

// função para medir os niveis de agua
void nivelDeAgua(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  dist = pulseIn(echoPin, HIGH) / 58;
  
  Serial.println("\n=== NIVEL DE AGUA ===");
  Serial.print("Distancia medida: ");
  Serial.print(dist);
  Serial.println(" cm");
   
  
  if(dist <30){
    alertaNivelAgua = true;
  }else if(dist < 50 && dist > 30){
    alertaNivelAgua = false;
  }else{
    alertaNivelAgua = false;
  }
   	
}

// função para captar quantidade de chuva
// conta pulsos do pluviômetro (cada pulso = 2 mm de chuva)
void pluviometro(){
  chuva = digitalRead(botao);
  if(chuva == LOW && valorChuva == HIGH){
    contador++;
    mmChuva = contador * 2;
    Serial.println("\n=== DADOS PLUVIOMETRICOS ===");
    Serial.print("Acumulado: ");
    Serial.print(mmChuva);
    Serial.print(" mm"); 

    if(mmChuva > 20){
      alertaChuva = true;
    }else if(mmChuva > 10 && mmChuva < 20){
      alertaChuva = false;
    }else{
      alertaChuva = false;
    }
  }
  valorChuva = chuva;
 }

// função para mostrar niveis normais no LCD
void mostrarMensagem() {
  if(telaAtual == 0){
    mensagemAtualLinha1 = "Umidade: " + String(percentualUmidade) + "%";
    mensagemAtualLinha2 = (percentualUmidade > 60) ? "ATENCAO!" :
    					"Normal";
  }
  else if(telaAtual == 1){
    mensagemAtualLinha1 = "Distancia Agua:";
    mensagemAtualLinha2 = String(dist) + "cm " + ((dist < 50) ? "ATENCAO!" :
                                                  "Seguro");
  }
  else if(telaAtual == 2){
    mensagemAtualLinha1 = "Chuva: " + String(mmChuva) + "mm";
    mensagemAtualLinha2 = (mmChuva > 10) ? "ATENCAO!" :
                         "Normal";
  }
}

// função para exibir alertas no LCD
 void mostrarAlerta(){
  if(alertaUmidade && telaAlertaAtual == 0){
    mensagemAtualLinha1 = "ALERTA Umidade!";
    mensagemAtualLinha2 = "Solo saturado!";
  }
  else if(alertaNivelAgua && telaAlertaAtual == 1){
    mensagemAtualLinha1 = "AGUA PROXIMA!";
    mensagemAtualLinha2 = String(dist) + "cm da area";
  }
  else if(alertaChuva && telaAlertaAtual == 2){
    mensagemAtualLinha1 = "ALERTA Chuva!";
    mensagemAtualLinha2 = String(mmChuva) + "mm acumulados";
  }
 }
// função para atualizar o LCD de forma otimizada, evitando flickering e atualizações desnecessárias
 void atualizarLcd() {
  static unsigned long ultimaAtualizacao = 0;
  static String ultimaLinha1 = "";
  static String ultimaLinha2 = "";
  const unsigned long intervaloAtualizacao = 500;

  if (millis() - ultimaAtualizacao >= intervaloAtualizacao) {
    if (mensagemAtualLinha1 != ultimaLinha1 || mensagemAtualLinha2 != ultimaLinha2) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(mensagemAtualLinha1);
      lcd.setCursor(0, 1);
      lcd.print(mensagemAtualLinha2);

      ultimaLinha1 = mensagemAtualLinha1;
      ultimaLinha2 = mensagemAtualLinha2;
    }
    ultimaAtualizacao = millis();
  }
}

// função para atualizar o estado dos LEDs conforme os alertas
void atualizarLeds() {
  if(alertaUmidade || alertaNivelAgua || alertaChuva){
    digitalWrite(ledVermelho, HIGH);
    digitalWrite(ledAmarelo, LOW);
    digitalWrite(ledVerde, LOW);
  } 
  else if ((percentualUmidade > 60 && percentualUmidade <= 80) ||
           (dist >= 30 && dist < 50) ||
           (mmChuva > 10 && mmChuva <= 20)) {
    digitalWrite(ledVermelho, LOW);
    digitalWrite(ledAmarelo, HIGH);
    digitalWrite(ledVerde, LOW);
  } 
  else {
    digitalWrite(ledVermelho, LOW);
    digitalWrite(ledAmarelo, LOW);
    digitalWrite(ledVerde, HIGH);
  }
}

// função para controlar o buzzer conforme o estado dos alertas
void controlarAlarme() {
  if(alertaUmidade || alertaNivelAgua || alertaChuva){
    if(!alarmeLigado){
      tone(alarme, 1000);
      alarmeLigado = true;
    }
  } else {
    if(alarmeLigado){
      noTone(alarme);
      alarmeLigado = false;
    }
  }
}