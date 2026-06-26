#include <SoftwareSerial.h>

// HC-05
SoftwareSerial BT(12,13); // RX, TX

// --- MOTOR PİNLERİ ---
const int enA = 11;  // Sağ motor
const int in1 = 10;
const int in2 = 9;

const int enB = 6;   // Sol motor
const int in3 = 8;
const int in4 = 7;

int hiz = 240;
int yavasHiz = 110;
int donusHizi = 100;   // sadece R ve L için

void setup() {
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  BT.begin(9600);

  dur();
}

void loop() {
  if (BT.available()) {
    char komut = BT.read();

    if (komut == 'F') ileri();
    else if (komut == 'B') geri();
    else if (komut == 'L') sol();
    else if (komut == 'R') sag();
    else if (komut == 'S') dur();

    // İKİ TUŞ KOMBİNASYONLARI
    else if (komut == 'G') ileriSol();   // F + L
    else if (komut == 'H') ileriSag();   // F + R
    else if (komut == 'I') geriSol();    // B + L
    else if (komut == 'J') geriSag();    // B + R

    // HIZ AYARLARI
    else if (komut == '0') hiz = 0;
    else if (komut == '1') hiz = 25;
    else if (komut == '2') hiz = 50;
    else if (komut == '3') hiz = 75;
    else if (komut == '4') hiz = 100;
    else if (komut == '5') hiz = 125;
    else if (komut == '6') hiz = 150;
    else if (komut == '7') hiz = 175;
    else if (komut == '8') hiz = 200;
    else if (komut == '9') hiz = 255;

    yavasHiz = hiz / 3;
  }
}

// =======================
// MOTOR FONKSİYONLARI
// =======================

void ileri() {
  analogWrite(enA, hiz);
  analogWrite(enB, hiz);

  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);

  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void geri() {
  analogWrite(enA, hiz);
  analogWrite(enB, hiz);

  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);

  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void sag() {

  analogWrite(enA, donusHizi);
  analogWrite(enB, donusHizi);

  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);

  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void sol() {

  analogWrite(enA, donusHizi);
  analogWrite(enB, donusHizi);

  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);

  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}


void ileriSol() {

  analogWrite(enA, hiz);
  analogWrite(enB, 80);

  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);

  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void ileriSag() {

  analogWrite(enA, 80);   // sağ motor baya yavaş
  analogWrite(enB, hiz);  // sol motor full

  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);

  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void geriSol() {
  // Sola kıvrılarak geri
  analogWrite(enA, hiz);
  analogWrite(enB, yavasHiz);

  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);

  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void geriSag() {
  // Sağa kıvrılarak geri
  analogWrite(enA, yavasHiz);
  analogWrite(enB, hiz);

  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);

  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void dur() {
  analogWrite(enA, 0);
  analogWrite(enB, 0);

  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);

  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}