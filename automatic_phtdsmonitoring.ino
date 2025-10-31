// ------------------ Konfigurasi Blynk ------------------
#define BLYNK_TEMPLATE_ID "TMPL6sMEM6lT8"
#define BLYNK_TEMPLATE_NAME "GroupProject"
#define BLYNK_AUTH_TOKEN "3ywH5kYH2z4_dHxTj6VGsFFQak40TyVY"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// ------------------ WiFi ------------------
char ssid[] = "Hotspot_MeetingRoom";
char pass[] = "Meeting@Tsu2025";

// ------------------ Pin & Variabel ------------------
#define ONE_WIRE_BUS 4
#define RELAY_POMPA 26
#define SENSOR_TDS 35
#define SENSOR_PH 34
#define VREF 3.3
#define ADC_RES 4095.0

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// ------------------ Variabel Global ------------------
unsigned long int avgval;
int buffer_arr[34], temp;
float ph, tds, suhuAir, voltage;
bool manualMode = false;
bool pompaStatus = false;

// ------------------ Fungsi Manual Pompa ------------------
BLYNK_WRITE(V4) { // tombol manual pompa
  int tombol = param.asInt();
  manualMode = tombol;
  if (manualMode) {
    digitalWrite(RELAY_POMPA, LOW);  // Pompa ON manual
    Blynk.virtualWrite(V3, 1);
  } else {
    digitalWrite(RELAY_POMPA, HIGH); // Pompa OFF manual
    Blynk.virtualWrite(V3, 0);
  }
}

// ------------------ Fungsi Baca pH ------------------
float readPH() {
  int phBuffer[34];
  unsigned long int avgValPH;
  int tempVal;

  for (int i = 0; i < 34; i++) {
    phBuffer[i] = analogRead(PH_PIN);
    delay(30);
  }

  for (int i = 0; i < 33; i++) {
    for (int j = i + 1; j < 34; j++) {
      if (phBuffer[i] > phBuffer[j]) {
        tempVal = phBuffer[i];
        phBuffer[i] = phBuffer[j];
        phBuffer[j] = tempVal;
      }
    }
  }

  // Ambil rata-rata dari nilai tengah
  avgValPH = 0;
  for (int i = 2; i < 32; i++) avgValPH += phBuffer[i];
  avgValPH = avgValPH / 30;

  float phValue = (avgValPH - 5053.2) / -238.95; // rumus kalibrasi kamu
  return phValue;
}


// ------------------ Fungsi Baca TDS ------------------
float readTDS() {
  int tdsBuffer[10];
  unsigned long int avgValTDS;
  int tempVal;
  float voltageTDS;

  for (int i = 0; i < 10; i++) {
    tdsBuffer[i] = analogRead(TDS_PIN);
    delay(30);
  }

  for (int i = 0; i < 9; i++) {
    for (int j = i + 1; j < 10; j++) {
      if (tdsBuffer[i] > tdsBuffer[j]) {
        tempVal = tdsBuffer[i];
        tdsBuffer[i] = tdsBuffer[j];
        tdsBuffer[j] = tempVal;
      }
    }
  }

  avgValTDS = 0;
  for (int i = 2; i < 8; i++) avgValTDS += tdsBuffer[i];
  avgValTDS = avgValTDS / 6;

  voltageTDS = avgValTDS * (VREF / ADC_RES);
  float tdsValue = (avgValTDS - 6704.3) / -3.8312; // rumus kalibrasi kamu
  return tdsValue;
}

// ------------------ Setup ------------------
void setup() {
  Serial.begin(115200);
  sensors.begin();
  pinMode(RELAY_POMPA, OUTPUT);
  digitalWrite(RELAY_POMPA, HIGH); // relay off (active low)

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  Serial.println("Sistem Monitoring Hidroponik Siap!");
}

// ------------------ Loop ------------------
void loop() {
  Blynk.run();
  sensors.requestTemperatures();

 suhuAir = sensors.getTempCByIndex(0);
  ph = readPH();
  tds = readTDS();

  // Kirim ke Blynk
  // Blynk.virtualWrite(V0, suhuAir);
  // Blynk.virtualWrite(V1, phValue);
  // Blynk.virtualWrite(V2, ppm);

  // Logika otomatis jika mode manual tidak aktif
  if (!manualMode) {
    if (ppm < 1000) {
      digitalWrite(RELAY_POMPA, LOW); // Pompa ON
      pompaStatus = true;
    } else {
      digitalWrite(RELAY_POMPA, HIGH); // Pompa OFF
      pompaStatus = false;
    }
    Blynk.virtualWrite(V3, pompaStatus);
  }

  // Debug
  Serial.println("=== Monitoring Hidroponik ===");
  Serial.print("Suhu Air: "); Serial.print(suhuAir); Serial.println(" °C");
  Serial.print("pH Air: "); Serial.println(phValue);
  Serial.print("TDS: "); Serial.print(ppm); Serial.println(" ppm");
  Serial.print("Pompa: "); Serial.println(pompaStatus ? "ON" : "OFF");
  Serial.println("-----------------------------");

  delay(2000);
}
