// Saat doğru çalışmıyorsa bu değeri ayarlayın.
// Teorik olarak, bu değerin standardı 60000'dir.
#define MILLIS_PER_MIN 60000 // bir dakikadaki milisaniye sayısı

// Motor ve saat parametreleri
// 4096 * 90 / 12 = 30720
#define STEPS_PER_ROTATION 30720 // dakika rotoru için tam dönüş adım sayısı

// Step motorun tek bir adımı için bekleme süresi
int delaytime = 2;

// Step motoru kontrol etmek için kullanılan portlar
// Eğer motor ters yönde dönüyorsa, 
// sıralamayı {5, 4, 3, 2} olarak değiştirin;
int port[4] = {2, 3, 4, 5};

// Step motor kontrol dizisi
int seq[8][4] = {
  {  LOW, HIGH, HIGH,  LOW},
  {  LOW,  LOW, HIGH,  LOW},
  {  LOW,  LOW, HIGH, HIGH},
  {  LOW,  LOW,  LOW, HIGH},
  { HIGH,  LOW,  LOW, HIGH},
  { HIGH,  LOW,  LOW,  LOW},
  { HIGH, HIGH,  LOW,  LOW},
  {  LOW, HIGH,  LOW,  LOW}
};

// Motoru döndürme fonksiyonu
void rotate(int step) {
  static int phase = 0;
  int i, j;
  int delta = (step > 0) ? 1 : 7;
  int dt = 20;

  step = (step > 0) ? step : -step;
  for(j = 0; j < step; j++) {
    phase = (phase + delta) % 8;
    for(i = 0; i < 4; i++) {
      digitalWrite(port[i], seq[phase][i]);
    }
    delay(dt);
    if(dt > delaytime) dt--;
  }
  // Gücü kes
  for(i = 0; i < 4; i++) {
    digitalWrite(port[i], LOW);
  }
}

void setup() {
  pinMode(port[0], OUTPUT);
  pinMode(port[1], OUTPUT);
  pinMode(port[2], OUTPUT);
  pinMode(port[3], OUTPUT);
  rotate(-20); // Yaklaşma çalışması
  rotate(20); // Ağır yük olmadan yaklaşma çalışması
  rotate(STEPS_PER_ROTATION / 60);
}

void loop() {
  static long prev_min = 0, prev_pos = 0;
  long min;
  static long pos;
  
  min = millis() / MILLIS_PER_MIN;
  if(prev_min == min) {
    return;
  }
  prev_min = min;
  pos = (STEPS_PER_ROTATION * min) / 60;
  rotate(-20); // Yaklaşma çalışması
  rotate(20); // Ağır yük olmadan yaklaşma çalışması
  if(pos - prev_pos > 0) {
    rotate(pos - prev_pos);
  }
  prev_pos = pos;
}
