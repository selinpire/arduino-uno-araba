// --- TRIG PİNLERİ ---
const int trigOn = 2;
const int trigSag = 12;
const int trigSol = 4;
const int trigArka = 13;

// --- ECHO PİNLERİ ---
const int echoOn = A2;
const int echoSag = A3;
const int echoSol = A4;
const int echoArka = A5;

// --- MOTOR PİNLERİ ---
const int enA = 11;  // Sağ motor
const int in1 = 10;
const int in2 = 9;

const int enB = 6;   // Sol motor
const int in3 = 8;
const int in4 = 7;

// --- HIZLAR ---
int hiz = 80;
int hizDonus = 120;

bool ileriDurum = true;

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

// =======================
// MESAFE ÖLÇÜM
// =======================

int mesafeOlc(int trigPin, int echoPin) {

  long sure;

  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  sure = pulseIn(echoPin, HIGH, 30000);

  if (sure == 0)
    return 0;

  return sure * 0.034 / 2;
}

// =======================
// LOOP
// =======================

void loop() {

  int on = mesafeOlc(trigOn, echoOn);
  int sag = mesafeOlc(trigSag, echoSag);
  int sol = mesafeOlc(trigSol, echoSol);
  int arka = mesafeOlc(trigArka, echoArka);

   // Sağ-sol farkı
  int fark = sol - sag;

  // --- SERIAL MONITOR ---

  Serial.print("On: ");
  Serial.print(on);

  Serial.print(" Sag: ");
  Serial.print(sag);

  Serial.print(" Sol: ");
  Serial.print(sol);

  Serial.print(" Arka: ");
  Serial.println(arka);

 
 if (ileriDurum) {

  // =======================
  // ÖN ENGEL HER ZAMAN ÖNCELİKLİ
  // =======================

  if (on > 0 && on <= 20) {

    dur();
    delay(200);

    geri();

    ileriDurum = false;

    Serial.println("ON ENGEL -> GERI");
  }

  // Sola fazla yakın -> sağa yönel

    else if (fark < -10) {

      sagaYonel();

      Serial.println("SOLA YAKIN -> SAGA");

      delay(30);
    }

    // Sağa fazla yakın -> sola yönel

    else if (fark > 10) {

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

    if (arka > 0 && arka <= 20) {

      dur();
      delay(200);

      ileri();

      ileriDurum = true;

      Serial.println("ARKA ENGEL -> ILERI");
    }

    // Geri giderken sola yakın

    else if (fark < -10) {

      geriSagaYonel();

      Serial.println("GERI SOLA YAKIN -> SAGA");

      delay(30);
    }

    // Geri giderken sağa yakın

    else if (fark > 10) {

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

// =======================
// MOTOR FONKSİYONLARI
// =======================

// --- İLERİ ---

void ileri() {

  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);

  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);

  analogWrite(enA, hiz);
  analogWrite(enB, hiz);
}

// --- GERİ ---

void geri() {

  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);

  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);

  analogWrite(enA, hiz);
  analogWrite(enB, hiz);
}

// --- DUR ---

void dur() {

  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);

  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);

  analogWrite(enA, 0);
  analogWrite(enB, 0);
}

// =======================
// ILERI GIDERKEN DONUSLER
// =======================

// --- SAĞA YÖNEL ---
// Sağ motor yavaş
// Sol motor hızlı

void sagaYonel() {

  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);

  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);

  analogWrite(enA, hiz);
  analogWrite(enB, hizDonus);
}

// --- SOLA YÖNEL ---
// Sağ motor hızlı
// Sol motor yavaş

void solaYonel() {

  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);

  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);

  analogWrite(enA, hizDonus);
  analogWrite(enB, hiz);
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

  analogWrite(enA, hiz);
  analogWrite(enB, hizDonus);
}

// --- GERI GIDERKEN SOLA YÖNEL ---

void geriSolaYonel() {

  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);

  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);

  analogWrite(enA, hizDonus);
  analogWrite(enB, hiz);
}