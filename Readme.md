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
* Kasa raporu yazdırma
* Veritabanı otomatik yedekleme

### Satış ekranı

![ana](https://mls.akdeniz.edu.tr/git/mamoo/mhss/raw/branch/master/screenshots/mhss-ana-ekran.png)

### Stok ekranı

![stok](https://mls.akdeniz.edu.tr/git/mamoo/mhss/raw/branch/master/screenshots/mhss-stok.png)

### Kasa ekranı

![kasa](https://mls.akdeniz.edu.tr/git/mamoo/mhss/raw/branch/master/screenshots/mhss-kasa.png)

#### Tavsiye edilen sistem gereksinimleri
* Milis Linux işletim sistemi
* 1 GB sistem belleği
* Minimum 1366x768px çözünürlüğünde monitör (isteğe göre dokunmatik monitör)

#### Gerekler
* Postgresql (çalışma gereği)
* Crontab (isteğe bağlı)
* CUPS (isteğe bağlı)

## Kurulum
#### Milis Linux

Terminalden Milis Hızlı Satış Sistemi kurulumu
```
sudo mps gun
sudo mps kur mhss
```

Postgresql servisinin kurulumu ve aktifleştirilmesi
```
sudo servis ekle postgresql
sudo servis kos postgresql
sudo servis aktif postgresql
```

#### MHSS Giriş bilgileri
```
kullanıcı: admin
şifre    : admin
```

#### Ana ekran klavye kısayolları

* [F9]    -> Stok kartları sayfasını açar
* [F3]    -> Sepetten kalem siler
* [F5]    -> Sepeti siler
* [F11]   -> Kasa ekranını açar
* [ENTER] -> Sepetin satış ekranını açar
* [F]     -> Ürünün fiyatını görme
* [+]     -> Sepette ki ürünün adetini artırır
* [-]     -> Sepette ki ürünün adetini azaltır
* [*]     -> Sepette ki ürünün adetini girilen değer kadar artırır.
* [sol ve sağ yön tuşu] -> Sepetler arası geçiş yapar
* [yukarı ve aşağı yön tuşu] -> Sepette seçili ürünü değiştirir.

#### Satış ekranı klavye kısayolları

* [F]     -> Bilgi Fişi yazdır/yazdırma
