# BHSS — Basat Hızlı Satış Sistemi

![Versiyon](https://img.shields.io/badge/versiyon-v0.2.7-blue)
![Lisans](https://img.shields.io/badge/lisans-MIT-green)

Küçük ve orta ölçekli işletmeler için barkodlu satış, stok takibi, cari hesap ve kasa yönetimi çözümü.

## Ekran Görüntüleri

| Satış Ekranı | Stok Ekranı | Kasa Ekranı |
|:------------:|:-----------:|:-----------:|
| ![Satış](https://raw.githubusercontent.com/mamoo81/bhss/master/screenshots/mhss-ana-ekran.png) | ![Stok](https://raw.githubusercontent.com/mamoo81/bhss/master/screenshots/mhss-stok.png) | ![Kasa](https://raw.githubusercontent.com/mamoo81/bhss/master/screenshots/mhss-kasa.png) |

## Özellikler

- **Stok Takibi:** Stoklu/stoksuz ürün yönetimi
- **Cari Takibi:** Müşteri ve tedarikçi cari hesapları
- **Barkodlu Satış:** Hızlı sepet yönetimi ve çoklu ödeme
- **Kasa Yönetimi:** Gün sonu raporları ve tahsilat makbuzu
- **Etiket Yazdırma:** Raf etiketi basımı (80×38 mm ve 100×38 mm desteği)
- **Fiş/Adisyon Yazdırma:** 80 mm termal yazıcı desteği
- **Terazi Entegrasyonu:** RS-232 seri port ile tartım
- **Kullanıcı Yetkilendirme:** Rol bazlı erişim kontrolü
- **Otomatik Yedekleme:** Veritabanı periyodik yedekleme (crontab)

## Sistem Gereksinimleri

| Bileşen | Minimum |
|---------|---------|
| İşletim Sistemi | Milis Linux / Pardus 23.x / Debian 12+ |
| RAM | 1 GB |
| Ekran | 1366×768 px |
| Veritabanı | PostgreSQL 13+ |

## Kurulum

### Debian / Pardus / Ubuntu

```bash
# Sistem güncellemesi
sudo apt update && sudo apt upgrade -y

# Gerekli bağımlılıklar
sudo apt install postgresql zint qmake6 make g++ git \
    qt6-declarative-dev qt6-tools-dev qt6-serialport-dev \
    qt6-multimedia-dev qt6-charts-dev libqt6sql6-psql

# Kaynak kodu indirme
git clone https://github.com/mamoo81/bhss.git
cd bhss

# Derleme
mkdir build && cd build
qmake6 ../mhss.pro
make -j$(nproc)
sudo make install
```

### Milis Linux

```bash
sudo mps gun
sudo mps kur mhss postgresql

# PostgreSQL servisini başlatma
sudo servis ekle postgresql
sudo servis kos postgresql
sudo servis aktif postgresql
```

## İlk Çalıştırma

```bash
mhss
```

**Varsayılan giriş bilgileri:**
- Kullanıcı: `admin`
- Şifre: `admin`

## Klavye Kısayolları

### Ana Ekran

| Kısayol | İşlev |
|---------|-------|
| `Enter` | Satış ekranını açar |
| `Esc` | Satış ekranını kapatır |
| `Ctrl + F` | Ürün ara ve sepete ekle |
| `F` | Ürünün fiyatını gösterir |
| `+` | Sepetteki ürün adetini artırır |
| `-` | Sepetteki ürün adetini azaltır |
| `*` | Sepetteki ürün adetini çarpar |
| `F3` | Sepetten ürün siler |
| `F5` | Seçili sepeti siler |
| `F9` | Stok kartları ekranını açar |
| `F10` | Cari kartlar ekranını açar |
| `F11` | Kasa ekranını açar |
| `F12` | Ayarlar ekranını açar |
| `← →` | Sepetler arası geçiş |
| `↑ ↓` | Sepetteki seçili ürünü değiştirir |

### Stok Ekranı

| Kısayol | İşlev |
|---------|-------|
| `F1` | Yeni stok kartı açar |
| `F2` | Seçili kartı düzenler |
| `F3` | Seçili kartı siler |
| `F4` | Seçili karta stok ekler |
| `F5` | Seçili karttan stok düşer |
| `Esc` | Stok ekranını kapatır |

### Satış Ekranı

| Kısayol | İşlev |
|---------|-------|
| `F` | Fiş yazdırma aktif/pasif |
| `Enter` | Satışı tamamlar |

## Lisans

Bu proje [MIT Lisansı](LICENSE) ile lisanslanmıştır.

---

© 2021 — Mehmet AKDEMİR · [bilgi@basat.dev](mailto:bilgi@basat.dev)
