# MİLİS HIZLI SATIŞ SİSTEMİ

Küçük ölçekli işletmeler için stok takibi, cari takibi, barkodlu satış yapabileceğiniz bir uygulama.

### Satış ekranı
<p align="center">
  <img alt="Satış ekranı" src="https://mls.akdeniz.edu.tr/git/mamoo/mhss/raw/branch/master/screenshots/mhss-ana-ekran.png">
</p>
### Stok ekranı
![stok](https://mls.akdeniz.edu.tr/git/mamoo/mhss/raw/branch/master/screenshots/mhss-stok.png)
### Kasa ekranı
![kasa](https://mls.akdeniz.edu.tr/git/mamoo/mhss/raw/branch/master/screenshots/mhss-kasa.png)

#### Tavsiye edilen sistem gereksinimleri
* Milis Linux işletim sistemi
* 1 GB sistem belleği
* Minimum 1366x768px çözünürlüğünde monitör (isteğe göre dokunmatik monitör)

#### Gerekler
* Postgresql
* Crontab (isteğe bağlı)

## Kurulum
#### Milis Linux

```sudo mps gun```

```sudo mps kur mhss```

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
