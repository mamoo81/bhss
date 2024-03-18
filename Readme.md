# MİLİS HIZLI SATIŞ SİSTEMİ

Küçük ölçekli işletmeler için stok takibi, cari takibi, barkodlu satış yapabileceğiniz bir uygulama.

### Özellikler

* Stok takibi
* Cari takibi (temel seviyede)
* Cariye stoklu/stoksuz satış
* Kasa takibi
* Kullanıcı yönetimi ve yetkilendirme
* Adisyon/fiş yazdırma (80mm adisyon yazıcılar ile uyumlu)
* Terazi entegrasyonu (RS232 seriport)
* Etiket yazıcı entegrasyonu
* Kasa raporu yazdırma
* Veritabanı otomatik yedekleme

### Satış ekranı

![ana](https://mls.akdeniz.edu.tr/git/mamoo/mhss/raw/branch/master/screenshots/mhss-ana-ekran.png)

### Stok ekranı

![stok](https://mls.akdeniz.edu.tr/git/mamoo/mhss/raw/branch/master/screenshots/mhss-stok.png)

### Kasa ekranı

![kasa](https://mls.akdeniz.edu.tr/git/mamoo/mhss/raw/branch/master/screenshots/mhss-kasa.png)

#### Tavsiye edilen sistem gereksinimleri
* Milis Linux işletim sistemi (Dokunmatik kullanım isteniyorsa Pardus GNU/Linux kullanılabilir)
* 1 GB sistem belleği
* Minimum 1366x768px çözünürlüğünde monitör

#### Gerekler
* Postgresql (çalışma gereği)
* Crontab (isteğe bağlı)
* CUPS (isteğe bağlı)

## Kurulum
#### Milis Linux

Terminalden Milis Hızlı Satış Sistemi kurulumu
```
sudo mps gun
sudo mps kur mhss postgresql
```

Postgresql servisinin kurulumu ve aktifleştirilmesi
```
sudo servis ekle postgresql
sudo servis kos postgresql
sudo servis aktif postgresql
```

#### Pardus 23.x GNU/Linux
Güncellemerin kurulumu
```
sudo apt-get update
sudo apt-get upgrade
```
Gereklerin kurulumu
```
sudo apt-get install build-essential postgresql zint qt5-qmake qmake6 make git qt6-declarative-dev qt6-tools-dev libqt6serialport6 qt6-multimedia-dev qt6-charts-dev libqt6sql6-psql
```
Postgresql'de ```postgres``` kullanıcısının localhost bağlantısına izin verilmeli.
```postgres``` kullanıcısının şifresi ```postgres``` olmalı.

MHSS nin kaynaktan kurulumu
```
git clone https://mls.akdeniz.edu.tr/git/mamoo/mhss.git
cd mhss && mkdir build && cd build
qmake ../mhss.pro
make
sudo make install
```

MHSS nin çalıştırılması.
terminalden ```mhss```

#### MHSS Giriş bilgileri
```
kullanıcı: admin
şifre    : admin
```

#### Ana ekran klavye kısayolları

| Tuş   |     | Tuş    | İşlevi                            |
| :---  | :-: | :----: | ---:                              |
| Enter |     |        | Satış ekranını açar               |
| Esc   |     |        | Satış ekranını kapatır            |
| Ctrl  | +   | F      | Ürün ara ve sepete ekle           |
| F     |     |        | Ürünün fiyatını gösterir          |
| +     |     |        | Sepetteki ürünün adetini artırır  |
| -     |     |        | Sepetteki ürünün adetini azaltır  |
| *     |     |        | Sepetteki ürünün adetini çarpar   |
| F3    |     |        | Sepetten ürün siler               |
| F5    |     |        | Seçili sepeti siler               |
| F9    |     |        | Stok kartları ekranını açar       |
| F10   |     |        | Cari kartlar ekranını açar        |
| F11   |     |        | Kasa ekranını açar                |
| F12   |     |        | Ayarlar ekranını açar             |
| ← →   |     |        | Sepetler arası geçiş yapar        |
| ↑ ↓   |     |        | Sepetteki seçili ürünü değiştirir |

#### Stok ekranı klavye kısayolları

| Tuş   |     | Tuş    | İşlevi                            |
| :---  | :-: | :----: | ---:                              |
| F1    |     |        | Yeni stok kartı açar              |
| F2    |     |        | Tabloda ki seçili kartı düzenler  |
| F3    |     |        | Tabloda ki seçili kartı siler     |
| F4    |     |        | Tabloda ki seçili kartı stok ekler|
| F5    |     |        | Tabloda ki seçili kartı stok düşer|
| Esc   |     |        | Stok ekranını kapatır             |

#### Satış ekranı klavye kısayolları

| Tuş   |     | Tuş    | İşlevi                            |
| :---  | :-: | :----: | ---:                              |
| F     |     |        | Fiş yazdırma aktif/pasif          |
| Enter |     |        | Satış yapar                       |
