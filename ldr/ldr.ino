// --- LDR VE HEDEF TANIMI ---

const int ldrSag = A0;
const int ldrSol = A1;

bool hedefAydinlik = false; // true: En aydınlık, false: En karanlık noktayı bulur

// --- KAYIT SİSTEMİ ---
const int MAX_KAYIT = 700;


int isikKayitlari[MAX_KAYIT];
int kayitIndeksi = 0;
unsigned long sonKayitZamani = 0;
const int kayitAraligi = 400; // Her 100ms'de bir ışık örneği al

// --- TRIG PİNLERİ ---
const int trigOn = 2;
const int trigSag = 12;
const int trigSol = 4;
const int trigArka = 13;

// --- ECHO PİNLERİ ---
const int echoOn = A2;
const int echoSag = A3;
const int echoSol = A5;
const int echoArka = A4;

// --- MOTOR PİNLERİ ---
const int enA = 11;  // Sağ motor
const int in1 = 10;
const int in2 = 9;

const int enB = 6;   // Sol motor
const int in3 = 8;
const int in4 = 7;


// --- HIZLAR ---

// Düz giderken
int hizSag = 110;   // Sağ motor
int hizSol = 130;   // Sol motor (daha yavaş olduğu için artırdık)

// Dönüşlerde ekstra hız
int hizDonusSag = 180;
int hizDonusSol = 160;

bool ileriDurum = true;
bool gorevTamamlandi = false;

void setup() {
  pinMode(trigOn, OUTPUT);
  pinMode(trigSag, OUTPUT);
  pinMode(trigSol, OUTPUT);
  pinMode(trigArka, OUTPUT);

  pinMode(echoOn, INPUT);
  pinMode(echoSag, INPUT);
  pinMode(echoSol, INPUT);
  pinMode(echoArka, INPUT);

  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  Serial.begin(9600);
}

int mesafeOlc(int trigPin, int echoPin) {
  long sure;

  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  sure = pulseIn(echoPin, HIGH, 30000);

  if (sure == 0) return 0;

  return sure * 0.034 / 2;
}

void loop() {
  
  if (gorevTamamlandi) {
  dur();
  return;
  }

  int on = mesafeOlc(trigOn, echoOn);
  int sag = mesafeOlc(trigSag, echoSag);
  int sol = mesafeOlc(trigSol, echoSol);
  int arka = mesafeOlc(trigArka, echoArka);

     // Sağ-sol farkı
  int fark = sol - sag;
if (ileriDurum) {
    // --- IŞIK KAYDI ---
if (millis() - sonKayitZamani > kayitAraligi && kayitIndeksi < MAX_KAYIT) {

  int sagLdr = analogRead(ldrSag);
  int solLdr = analogRead(ldrSol);
  int isikDegeri = (sagLdr + solLdr) / 2;

  isikKayitlari[kayitIndeksi] = isikDegeri;
  kayitIndeksi++;

  // --- SERIAL MONITOR ÇIKTISI ---
  Serial.print("Sag LDR: ");
  Serial.print(sagLdr);

  Serial.print(" | Sol LDR: ");
  Serial.print(solLdr);

  Serial.print(" | Ortalama: ");
  Serial.print(isikDegeri);

  Serial.print(" | Kayit No: ");
  Serial.println(kayitIndeksi);

Serial.print("SOL: ");
Serial.print(sol);

Serial.print(" | SAG: ");
Serial.print(sag);

Serial.print(" | FARK: ");
Serial.println(fark);

  sonKayitZamani = millis();
}

  // =======================
  // ÖN ENGEL HER ZAMAN ÖNCELİKLİ
  // =======================

  if (on > 0 && on <= 25) {

    dur();
    delay(200);

    //geri();

    //ileriDurum = false;

    Serial.println("ON ENGEL -> GERI");
    bulVeParkEt();
  }

  // Sola fazla yakın -> sağa yönel

    else if (sol > 0 && sol <= 15 && sag > 20) {

      sagaYonel();

      Serial.println("SOLA YAKIN -> SAGA");

      delay(30);
    }

    // Sağa fazla yakın -> sola yönel

    else if (sag > 0 && sag <= 15 && sol > 20) {

      solaYonel();

      Serial.println("SAGA YAKIN -> SOLA");

      delay(30);
    }

    // Yol temiz -> düz git

    else {

      ileri();
    }
  }

// =======================
  // GERİ GİDERKEN
  // =======================

  else {

    // Arka engel öncelikli

    if (arka > 0 && arka <= 25) {

      dur();
      delay(200);

      ileri();

      ileriDurum = true;

      Serial.println("ARKA ENGEL -> ILERI");
    }

    // Geri giderken sola yakın

    else if (sol > 0 && sol <= 15 && sag > 20) {

      geriSagaYonel();

      Serial.println("GERI SOLA YAKIN -> SAGA");

      delay(30);
    }

    // Geri giderken sağa yakın

    else if (sag > 0 && sag <= 15 && sol > 20) {

      geriSolaYonel();

      Serial.println("GERI SAGA YAKIN -> SOLA");

      delay(30);
    }

    // Temiz -> geri git

    else {

      geri();
    }
  }
}
void bulVeParkEt() {

  int hedefDeger = isikKayitlari[0];

  // =========================
  // EN AYDINLIK / EN KARANLIK
  // =========================

  for (int i = 0; i < kayitIndeksi; i++) {

    if (hedefAydinlik) {

      // En aydınlık

      if (isikKayitlari[i] > hedefDeger) {

        hedefDeger = isikKayitlari[i];
      }

    } else {

      // En karanlık

      if (isikKayitlari[i] < hedefDeger) {

        hedefDeger = isikKayitlari[i];
      }
    }
  }
Serial.print("Hedef Isik Degeri: ");
  Serial.println(hedefDeger);

  delay(500);

  // =========================
  // GERI GIDERKEN ARAMA
  // =========================

  while (true) {

    // Yeni LDR ölçümü
    int sagLdr = analogRead(ldrSag);
    int solLdr = analogRead(ldrSol);

    int mevcutIsik = (sagLdr + solLdr) / 2;

    // Mesafe ölç
    int sag = mesafeOlc(trigSag, echoSag);
    int sol = mesafeOlc(trigSol, echoSol);

    //int fark = sol - sag;

    // =========================
    // SERI MONITOR
    // =========================

    Serial.print("Anlik Isik: ");
    Serial.println(mevcutIsik);

    // =========================
    // HEDEF BULUNDU MU
    // =========================

    // +/- tolerans bırakıyoruz
    if (abs(mevcutIsik - hedefDeger) <= 10) {

      dur();

      Serial.println("HEDEF BULUNDU");

      gorevTamamlandi = true;

      return;
    }

    // =========================
    // GERI GIDERKEN DUVAR TAKIBI
    // =========================

    if (sol > 0 && sol <= 15 && sag > 20) {

      geriSagaYonel();
    }

    else if (sag > 0 && sag <= 15 && sol > 20) {

      geriSolaYonel();
    }

    else {

      geri();
    }

    delay(30);
  }
}

// --- MOTOR FONKSİYONLARI ---
void ileri() {

  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);

  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);

  analogWrite(enA, hizSag); // Sağ motor
  analogWrite(enB, hizSol); // Sol motor
}

void geri() {

  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);

  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);

  analogWrite(enA, hizSag);
  analogWrite(enB, hizSol);
}

void dur() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);

  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);

  analogWrite(enA, 0);
  analogWrite(enB, 0);
}

// --- HAFİF SAĞA YÖNEL ---
// Sol motor hızlı, sağ motor yavaş

void sagaYonel() {

  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);

  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);

  analogWrite(enA, hizSag);
  analogWrite(enB, hizDonusSol);
}

// --- HAFİF SOLA YÖNEL ---
// Sağ motor hızlı, sol motor yavaş

void solaYonel() {

  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);

  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);

  analogWrite(enA, hizDonusSag);
  analogWrite(enB, hizSol);
}
// =======================
// GERI GIDERKEN DONUSLER
// =======================

// --- GERI GIDERKEN SAĞA YÖNEL ---

void geriSagaYonel() {

  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);

  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);

  analogWrite(enA, hizSag );
  analogWrite(enB, hizDonusSol);
}
// --- GERI GIDERKEN SOLA YÖNEL ---

void geriSolaYonel() {

  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);

  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);

  analogWrite(enA, hizDonusSag);
  analogWrite(enB, hizSol );
}



