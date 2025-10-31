**Monitoring Tanaman Hidroponik**

Konsep Umum Sistem
Sensor DS18B20 → mengukur suhu air.

Sensor pH → mengukur keasaman larutan nutrisi.

Sensor TDS → mengukur kadar nutrisi (ppm).

Relay 2 channel → mengontrol pompa peristaltik dan (opsional) satu aktuator lain (misalnya pompa air utama atau lampu).

Pompa peristaltik → menyala otomatis saat nilai TDS (ppm) < 1000 untuk menambah nutrisi.

| Sensor / Aktuator | Fungsi                                            | Logika Otomatis                                                                   |
| ----------------- | ------------------------------------------------- | --------------------------------------------------------------------------------- |
| DS18B20           | Baca suhu air                                     | Monitoring (ditampilkan ke Blynk/LCD)                                             |
| Sensor pH         | Baca tingkat keasaman (ideal 5.5–6.5)             | Monitoring                                                                        |
| Sensor TDS        | Baca konsentrasi nutrisi (ideal 1000–1200 ppm)    | Jika **< 1000 ppm**, pompa peristaltik **ON**, jika **≥ 1000 ppm**, pompa **OFF** |
| Relay 1           | Kontrol pompa peristaltik                         | Aktif LOW atau HIGH sesuai wiring                                                 |
| Relay 2           | Cadangan (pompa air, aerator, atau LED indikator) | Manual / Otomatis sesuai kebutuhan                                                |


🌿 Penjelasan Tambahan

Tanaman bayam dan pakcoy cocok untuk hidroponik, dengan kisaran ideal:

TDS: 1000–1200 ppm

pH: 5.5–6.5

Suhu air: 20–28°C

Pompa peristaltik digunakan untuk menambah larutan nutrisi A & B secara terukur.

Dengan integrasi Blynk, data pH, TDS, dan suhu bisa dimonitor real-time lewat HP.
