/*####################################
MIT LICENCE                          #
######################################
Copyright 2021 Mehmet AKDEMİR        #
bilgi@basat.dev                      #
######################################
Permission is hereby granted, free of charge,
to any person obtaining a copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation the rights to use, copy,
modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "yazici.h"
//*****************************
#include <QString>
#include <QPrinter>
#include <QPrinterInfo>
#include <QTextDocument>
#include <QProcess>
#include <QDateTime>
#include <QSettings>
#include <QStandardPaths>
#include <QSysInfo>
#include <QDebug>
#include <QPainter>

Yazici::Yazici()
{

}

void Yazici::fisBas(QString _fisNo, Sepet _sepet)
{
    //genel ayarların okunması başlangıcı
    QSettings genelAyarlar(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/mhss/genel.ini", QSettings::IniFormat);
    //yazıcı ayarları okuma başlangıç
    genelAyarlar.beginGroup("fis-yazici");
    yaziciModel = genelAyarlar.value("yazici").toString();
    if(genelAyarlar.value("sirketAdi").isNull()){
        sirketAdi = "MAĞAZA ADI";
    }
    else{
        sirketAdi = genelAyarlar.value("sirketAdi").toString();
    }
    if(genelAyarlar.value("sirketAdres").isNull()){
        sirketAdresi = "MAĞAZA ADRESİ";
    }
    else{
        sirketAdresi = genelAyarlar.value("sirketAdres").toString();
    }
    QString tarih = QDateTime::currentDateTime().toString("hh:mm dd.MM.yyyy");
    QString html =
            "<html>"
            "<head>"
                "<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\"/>"
                "<title></title>"
                "<style type=\"text/css\">"
                    "@page { size: 2.83in 8.27in; margin-left: 0.1in; margin-right: 0.1in }"
                    "td p { orphans: 0; widows: 0; background: transparent }"
                    "p { margin-bottom: 0.1in; line-height: 115%; background: transparent }"
                "</style>"
            "</head>"
            "<body lang=\"tr-TR\" link=\"#000080\" vlink=\"#800000\" dir=\"ltr\"><p align=\"right\" style=\"margin-bottom: 0in; line-height: 100%\">"
            "<font face=\"DejaVu Sans Mono, monospace\"><font size=\"2\" style=\"font-size: 10pt\">" + tarih + "</font></font></p>"
            "<p align=\"center\" style=\"margin-bottom: 0in; line-height: 100%\"><font face=\"DejaVu Sans Mono, monospace\"><font size=\"2\" style=\"font-size: 10pt\"><b>" + sirketAdi + "</b></font></font></p>"
            "<p align=\"center\" style=\"margin-bottom: 0in; line-height: 100%\"><font face=\"DejaVu Sans Mono, monospace\"><font size=\"2\" style=\"font-size: 10pt\">" + sirketAdresi + "</font></font></p>"
            "<table width=\"100%\" cellpadding=\"4\" cellspacing=\"0\">"
                "<col width=\"164*\"/>"

                "<col width=\"34*\"/>"

                "<col width=\"57*\"/>"

                "<tr valign=\"top\">"
                    "<td width=\"58%\" style=\"border-top: 1px solid #000000; border-bottom: 1px solid #000000; border-left: none; border-right: none; padding: 0.04in 0in\"><p align=\"left\">"
                        "<font face=\"DejaVu Sans Mono, monospace\"><font size=\"1\" style=\"font-size: 8pt\">Ürün</font></font></p>"
                    "</td>"
                    "<td width=\"18%\" style=\"border-top: 1px solid #000000; border-bottom: 1px solid #000000; border-left: none; border-right: none; padding: 0.04in 0in\"><p align=\"center\">"
                        "<font face=\"DejaVu Sans Mono, monospace\"><font size=\"1\" style=\"font-size: 8pt\">Adet/KG</font></font></p>"
                    "</td>"
                    "<td width=\"24%\" style=\"border-top: 1px solid #000000; border-bottom: 1px solid #000000; border-left: none; border-right: none; padding: 0.04in 0in\"><p align=\"center\">"
                        "<font face=\"DejaVu Sans Mono, monospace\"><font size=\"1\" style=\"font-size: 8pt\">Tutar</font></font></p>"
                    "</td>"
                "</tr>";
    foreach (auto urun, _sepet.urunler) {
        html.append(QString(
                        "<tr valign=\"top\">"
                            "<td width=\"58%\" style=\"border: none; padding: 0in\"><p align=\"left\"><font face=\"DejaVu Sans Mono, monospace\"><font size=\"1\" style=\"font-size: 7pt\"><strong>" + urun.ad + "</strong></font></font></p></td>"
                            "<td width=\"18%\" style=\"border: none; padding: 0in\"><p align=\"center\"><font face=\"DejaVu Sans Mono, monospace\"><font size=\"2\" style=\"font-size: 7pt\"><strong>" + QString::number(urun.miktar) + "</strong></font></font></p></td>"
                            "<td width=\"24%\" style=\"border: none; padding: 0in\"><p align=\"right\"><font face=\"DejaVu Sans Mono, monospace\"><font size=\"2\" style=\"font-size: 7pt\"><strong>" + QString::number(urun.toplam, 'f', 2) + "</strong></font></font></p></td>"
                        "</tr>"
                        ));
    };
    html.append(QString(
                "<tr valign=\"top\">"
                    "<td colspan=\"2\" width=\"78%\" style=\"border-top: 1px solid #000000; border-bottom: 1px solid #000000; border-left: none; border-right: none; padding: 0.04in 0in\"><p align=\"center\">"
                        "<font face=\"DejaVu Sans Mono, monospace\"><font size=\"1\" style=\"font-size: 8pt\"><strong>Toplam<strong></font></font></p>"
                    "</td>"
                    "<td width=\"24%\" style=\"border-top: 1px solid #000000; border-bottom: 1px solid #000000; border-left: none; border-right: none; padding: 0.04in 0in\"><p align=\"right\">"
                        "<font face=\"DejaVu Sans Mono, monospace\"><font size=\"2\" style=\"font-size: 8pt\"><strong>" + QString::number(_sepet.sepetToplamTutari(), 'f', 2) + "<strong></font></font></p>"
                    "</td>"
                "</tr>"
            "</table>"
            "<p align=\"center\" style=\"margin-bottom: 0in; line-height: 100%\">"               /*</br> ekleyebilirsin 1 satır boşluk bırakır*/
            "</p>"
            "<p align=\"center\" style=\"margin-bottom: 0in; line-height: 100%\"><font face=\"DejaVu Sans Mono, monospace\"><font size=\"2\" style=\"font-size: 10pt\">NO:" + _fisNo + "</font></font></p>"
            "<p align=\"center\" style=\"margin-bottom: 0in; line-height: 100%\">"/*             </br> ekleyebilirsin 1 satır boşluk bırakır*/
            "</p>"
            "<p align=\"center\" style=\"margin-bottom: 0in; line-height: 100%\"><font face=\"DejaVu Sans Mono, monospace\"><font size=\"2\" style=\"font-size: 10pt\">BİLGİ FİŞİDİR MALİ DEĞERİ YOKTUR</font></font></p>"
            "</body>"
            "</html>"));

    QTextDocument document;
    document.setPageSize(QSize(204,595));
    document.setHtml(html);

    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPaperSize(QSizeF(72,210), QPrinter::Millimeter);
    printer.setPageMargins(QMarginsF(0, 0, 0, 0));
    printer.setOutputFileName("/tmp/mhss-fis.pdf");

    document.print(&printer);

    QProcess *processIslem = new QProcess();
    if(QSysInfo::prettyProductName().contains("milis", Qt::CaseInsensitive)){
        yazdirmaKomut = "lpr -P " + yaziciModel + " /tmp/mhss-fis.pdf";
    }
    else if(QSysInfo::prettyProductName().contains("pardus", Qt::CaseInsensitive)){
        yazdirmaKomut = "lp -d " + yaziciModel + " /tmp/mhss-fis.pdf";
    }
    else{
        qDebug() << "Yazdırma için İşletim Sistemi tespit edilemedi.";
    }
    processIslem->start(yazdirmaKomut);

}

void Yazici::setKullanici(const User &newKullanici)
{
    kullanici = newKullanici;
}

void Yazici::rafEtiketiBas(StokKarti kart)
{
    //genel ayarların okunması başlangıcı
    QSettings genelAyarlar(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/mhss/genel.ini", QSettings::IniFormat);
    // etiket yazıcı ayarları okuma başlangıç
    genelAyarlar.beginGroup("etiket-yazici");
    etiketYazici = genelAyarlar.value("yazici").toString();
    genelAyarlar.endGroup();

    // sayfa tanımlamaları
    printer->setPrinterName(etiketYazici);
    printer->setPageSizeMM(QSize(72,33));
    printer->setPageMargins(0,4,0,6, QPrinter::Millimeter);
    printer->setResolution(300);

    QPainter painter(printer);
    // ürün adı
    QFont font("Halvetica", 7);
    painter.setFont(font);
    painter.drawText(QPoint(0,5), kart.getAd());

    // urun barkod img
    painter.drawImage(QPoint(0,25), kart.getBarkodImg(), QRect(0,0, 350,125));

    // urun guncelleme tarihi
    font = QFont("Halvetica", 5, QFont::Bold);
    painter.setFont(font);
    painter.drawText(QPoint(300, 200), "Güncelleme: " + kart.getTarih().toString("dd.MM.yyyy"));

    // ürün birimi
    font = QFont("Halvetica", 5);
    painter.setFont(font);
    painter.drawText(QPoint(250, 50), ("1 x " + vt.getBirimAd(kart.getBirim()) + " ="));

    // TL logosu
    font = QFont("Halvetica", 14);
    painter.setFont(font);
    painter.drawText(QPoint(220, 140), QString("₺"));

    // ürün fiyatı
    font = QFont("Halvetica", 20, QFont::Bold);
    painter.setFont(font);
    painter.drawText(QPoint(255, 140), QString::number(kart.getSFiyat(), 'f', 2));

    // yerli ürün logosu
    if(kart.getBarkod().front() == '8'){
        painter.drawImage(QPoint(0, 150), QImage(":/dosyalar/dosyalar/yerli-uretim-logo.png"));
    }
    painter.end();
}

void Yazici::cikisRaporuBas(User _user)
{
    //genel ayarların okunması başlangıcı
    QSettings genelAyarlar(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/mhss/genel.ini", QSettings::IniFormat);
    //yazıcı ayarları okuma başlangıç
    genelAyarlar.beginGroup("fis-yazici");
    yaziciModel = genelAyarlar.value("yazici").toString();

    if(genelAyarlar.value("raporHerZaman").toBool()){
        if(genelAyarlar.value("sirketAdi").isNull()){
            sirketAdi = "MAĞAZA ADI";
        }
        else{
            sirketAdi = genelAyarlar.value("sirketAdi").toString();
        }
        if(genelAyarlar.value("sirketAdres").isNull()){
            sirketAdresi = "MAĞAZA ADRESİ";
        }
        else{
            sirketAdresi = genelAyarlar.value("sirketAdres").toString();
        }
        // veritabanından giriş yapan kullanıcının oturum bilgilerini alıyorum.
        QSqlQuery raporSorgu = vt.getOturum();
        QString oturumGirisSaati = raporSorgu.value(2).toTime().toString("hh:mm");
        QString oturumGirisTarihi = raporSorgu.value(2).toDate().toString("dd.MM.yyyy");
        double giren = vt.getKasaToplamGiren(raporSorgu.value(2).toDateTime(), QDateTime::currentDateTime());
        double cikan = vt.getKasaToplamCikan(raporSorgu.value(2).toDateTime(), QDateTime::currentDateTime());
        QString html =
                "<html>"
                "<head>"
                    "<style type=\"text/css\">"
                        "@page { size: 2.83in 8.27in; margin-left: 0.1in; margin-right: 0.1in }"
                        "td p { orphans: 0; widows: 0; background: transparent }"
                        "p { line-height: 115%; margin-bottom: 0.1in; background: transparent }"
                    "</style>"
                "</head>"
                "<body lang=\"tr-TR\" link=\"#000080\" vlink=\"#800000\" dir=\"ltr\"><p align=\"center\" style=\"line-height: 100%; margin-bottom: 0in\">"
                "<font face=\"DejaVu Sans Mono, monospace\"><font size=\"2\" style=\"font-size: 10pt\">MHSS KASA RAPORU</font></font></p>"
                "<p align=\"right\" style=\"line-height: 100%; margin-bottom: 0in\"><font face=\"DejaVu Sans Mono, monospace\"><font size=\"2\" style=\"font-size: 10pt\">" + QDateTime::currentDateTime().toString("hh:mm dd.MM.yyyy") + "</font></font></p>"
                "<table width=\"100%\" cellpadding=\"4\" cellspacing=\"0\">"
                    "<col width=\"103*\"/>"

                    "<col width=\"153*\"/>"

                    "<tr valign=\"top\">"
                        "<td width=\"40%\" style=\"border-top: 1px solid #000000; border-bottom: 1px solid #000000; border-left: 1px solid #000000; border-right: none; padding-top: 0.04in; padding-bottom: 0.04in; padding-left: 0.04in; padding-right: 0in\"><p align=\"center\">"
                            "<font face=\"DejaVu Sans Mono, monospace\"><font size=\"2\" style=\"font-size: 10pt\">Kullanıcı</font></font></p>"
                        "</td>"
                        "<td width=\"60%\" style=\"border: 1px solid #000000; padding: 0.04in\"><p align=\"center\">"
                            "<font face=\"DejaVu Sans Mono, monospace\"><font size=\"2\" style=\"font-size: 10pt\">" + _user.getUserName() + "</font></font></p>"
                        "</td>"
                    "</tr>"
                "</table>"
                "<p align=\"center\" style=\"line-height: 100%; margin-bottom: 0in\"><font face=\"DejaVu Sans Mono, monospace\"><font size=\"2\" style=\"font-size: 10pt\">ÇALIŞMA SAATİ</font></font></p>"
                "<table width=\"100%\" cellpadding=\"4\" cellspacing=\"0\">"
                    "<col width=\"103*\"/>"

                    "<col width=\"153*\"/>"

                    "<tr valign=\"top\">"
                        "<td width=\"40%\" style=\"border-top: 1px solid #000000; border-bottom: 1px solid #000000; border-left: 1px solid #000000; border-right: none; padding-top: 0.04in; padding-bottom: 0.04in; padding-left: 0.04in; padding-right: 0in\"><p align=\"center\">"
                            "<font face=\"DejaVu Sans Mono, monospace\"><font size=\"2\" style=\"font-size: 9pt\">Giriş</font></font></p>"
                        "</td>"
                        "<td width=\"60%\" style=\"border: 1px solid #000000; padding: 0.04in\"><p align=\"center\">"
                            "<font face=\"DejaVu Sans Mono, monospace\"><font size=\"2\" style=\"font-size: 10pt\">" + oturumGirisSaati + " " + oturumGirisTarihi + "</font></font></p>"
                        "</td>"
                    "</tr>"
                    "<tr valign=\"top\">"
                        "<td width=\"40%\" style=\"border-top: none; border-bottom: 1px solid #000000; border-left: 1px solid #000000; border-right: none; padding-top: 0in; padding-bottom: 0.04in; padding-left: 0.04in; padding-right: 0in\"><p align=\"center\">"
                            "<font face=\"DejaVu Sans Mono, monospace\"><font size=\"2\" style=\"font-size: 9pt\">Çıkış</font></font></p>"
                        "</td>"
                        "<td width=\"60%\" style=\"border-top: none; border-bottom: 1px solid #000000; border-left: 1px solid #000000; border-right: 1px solid #000000; padding-top: 0in; padding-bottom: 0.04in; padding-left: 0.04in; padding-right: 0.04in\"><p align=\"center\">"
                            "<font face=\"DejaVu Sans Mono, monospace\"><font size=\"2\" style=\"font-size: 10pt\">" + QDateTime::currentDateTime().toString("hh:mm dd.MM.yyyy") + "</font></font></p>"
                        "</td>"
                    "</tr>"
                "</table>"
                "<p align=\"center\" style=\"line-height: 100%; margin-bottom: 0in\"><font face=\"DejaVu Sans Mono, monospace\"><font size=\"2\" style=\"font-size: 10pt\">TUTARLAR</font></font></p>"
                "<table width=\"100%\" cellpadding=\"4\" cellspacing=\"0\">"
                    "<col width=\"103*\"/>"

                    "<col width=\"153*\"/>"

                    "<tr valign=\"top\">"
                        "<td width=\"40%\" style=\"border-top: 1px solid #000000; border-bottom: 1px solid #000000; border-left: 1px solid #000000; border-right: none; padding-top: 0.04in; padding-bottom: 0.04in; padding-left: 0.04in; padding-right: 0in\"><p align=\"center\">"
                            "<font face=\"DejaVu Sans Mono, monospace\"><font size=\"2\" style=\"font-size: 9pt\">Toplam Giren</font></font></p>"
                        "</td>"
                        "<td width=\"60%\" style=\"border: 1px solid #000000; padding: 0.04in\"><p align=\"center\">"
                            "<font face=\"DejaVu Sans Mono, monospace\"><font size=\"2\" style=\"font-size: 10pt\">" + QString::number(giren, 'f', 2) + "</font></font></p>"
                        "</td>"
                    "</tr>"
                    "<tr valign=\"top\">"
                        "<td width=\"40%\" style=\"border-top: none; border-bottom: 1px solid #000000; border-left: 1px solid #000000; border-right: none; padding-top: 0in; padding-bottom: 0.04in; padding-left: 0.04in; padding-right: 0in\"><p align=\"center\">"
                            "<font face=\"DejaVu Sans Mono, monospace\"><font size=\"2\" style=\"font-size: 9pt\">Toplam Çıkan</font></font></p>"
                        "</td>"
                        "<td width=\"60%\" style=\"border-top: none; border-bottom: 1px solid #000000; border-left: 1px solid #000000; border-right: 1px solid #000000; padding-top: 0in; padding-bottom: 0.04in; padding-left: 0.04in; padding-right: 0.04in\"><p align=\"center\">"
                            "<font face=\"DejaVu Sans Mono, monospace\"><font size=\"2\" style=\"font-size: 10pt\">" + QString::number(cikan, 'f', 2) + "</font></font></p>"
                        "</td>"
                    "</tr>"
                    "<tr valign=\"top\">"
                        "<td width=\"40%\" style=\"border-top: none; border-bottom: 1px solid #000000; border-left: 1px solid #000000; border-right: none; padding-top: 0in; padding-bottom: 0.04in; padding-left: 0.04in; padding-right: 0in\"><p align=\"center\">"
                            "<font face=\"DejaVu Sans Mono, monospace\"><font size=\"2\" style=\"font-size: 9pt\">Kasada</font></font></p>"
                        "</td>"
                        "<td width=\"60%\" style=\"border-top: none; border-bottom: 1px solid #000000; border-left: 1px solid #000000; border-right: 1px solid #000000; padding-top: 0in; padding-bottom: 0.04in; padding-left: 0.04in; padding-right: 0.04in\"><p align=\"center\">"
                            "<font face=\"DejaVu Sans Mono, monospace\"><font size=\"2\" style=\"font-size: 10pt\">" + QString::number(vt.getKasadakiPara(), 'f', 2) + "</font></font></p>"
                        "</td>"
                    "</tr>"
                "</table>"
                "</body>"
                "</html>";

        QTextDocument document;
        document.setPageSize(QSize(204,595));
        document.setHtml(html);

        QPrinter printer(QPrinter::PrinterResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setPaperSize(QSizeF(72,210), QPrinter::Millimeter);
        printer.setPageMargins(QMarginsF(0, 0, 0, 0));
        printer.setOutputFileName("/tmp/mhss-kasa-rapor.pdf");

        document.print(&printer);

        QProcess *processIslem = new QProcess();
        if(QSysInfo::prettyProductName().contains("milis", Qt::CaseInsensitive)){
            yazdirmaKomut = "lpr -P " + yaziciModel + " /tmp/mhss-kasa-rapor.pdf";
        }
        else if(QSysInfo::prettyProductName().contains("pardus", Qt::CaseInsensitive)){
            yazdirmaKomut = "lp -d " + yaziciModel + " /tmp/mhss-kasa-rapor.pdf";
        }
        else{
            qDebug() << "Yazdırma için İşletim Sistemi tespit edilemedi.";
        }
        processIslem->start(yazdirmaKomut);
    }

}

void Yazici::tahsilatMakbuzuBas(User _user, Cari _cari, const double _tutar, QString _islemNo, QDateTime _islemTarihi, QString _aciklama)
{
    //genel ayarların okunması başlangıcı
    QSettings genelAyarlar(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/mhss/genel.ini", QSettings::IniFormat);
    //yazıcı ayarları okuma başlangıç
    genelAyarlar.beginGroup("fis-yazici");
    yaziciModel = genelAyarlar.value("yazici").toString();
    if(genelAyarlar.value("sirketAdi").isNull()){
        sirketAdi = "MAĞAZA ADI";
    }
    else{
        sirketAdi = genelAyarlar.value("sirketAdi").toString();
    }
    if(genelAyarlar.value("sirketAdres").isNull()){
        sirketAdresi = "MAĞAZA ADRESİ";
    }
    else{
        sirketAdresi = genelAyarlar.value("sirketAdres").toString();
    }
    QString tarih = QDateTime::currentDateTime().toString("hh:mm dd.MM.yyyy");
    double cariToplamBorc = vt.getCariToplamBorc(QString::number(_cari.getId()));
    QString html =
            "<html>"
            "<head>"
                "<style type=\"text/css\">"
                    "@page { size: 2.83in 8.27in; margin-left: 0.1in; margin-right: 0.1in }"
                    "td p { orphans: 0; widows: 0; background: transparent }"
                    "p { line-height: 115%; margin-bottom: 0.1in; background: transparent }"
                "</style>"
            "</head>"
            "<body lang=\"tr-TR\" link=\"#000080\" vlink=\"#800000\" dir=\"ltr\"><p align=\"center\" style=\"line-height: 100%; margin-bottom: 0in\">"
            "<font face=\"DejaVu Sans Mono, monospace\"><font size=\"2\" style=\"font-size: 10pt\">MHSS TAHSİLAT BİLGİ FİŞİ</font></font></p>"
            "<p align=\"center\" style=\"line-height: 100%; margin-bottom: 0in\"><font face=\"DejaVu Sans Mono, monospace\"><font size=\"2\" style=\"font-size: 10pt\">" + sirketAdi + "</font></font></p>"
            "<p align=\"center\" style=\"line-height: 100%; margin-bottom: 0in\"><font face=\"DejaVu Sans Mono, monospace\"><font size=\"2\" style=\"font-size: 10pt\">" + sirketAdresi + "</font></font></p>"
            "<table width=\"100%\" cellpadding=\"4\" cellspacing=\"0\">"
                "<col width=\"84*\"/>"
                "<col width=\"172*\"/>"
                "<tr>"
                    "<td width=\"33%\" valign=\"top\" style=\"border-top: 1px solid #000000; border-bottom: 1px solid #000000; border-left: 1px solid #000000; border-right: none; padding-top: 0.04in; padding-bottom: 0.04in; padding-left: 0.04in; padding-right: 0in\"><p align=\"left\">"
                        "<font face=\"DejaVu Sans Mono, monospace\"><font size=\"2\" style=\"font-size: 10pt\">Tarih</font></font></p>"
                    "</td>"
                    "<td width=\"67%\" style=\"border: 1px solid #000000; padding: 0.04in\"><p align=\"center\">"
                        "<font face=\"DejaVu Sans Mono, monospace\"><font size=\"2\" style=\"font-size: 10pt\">" + tarih + "</font></font></p>"
                    "</td>"
                "</tr>"
                "<tr>"
                    "<td width=\"33%\" valign=\"top\" style=\"border-top: none; border-bottom: 1px solid #000000; border-left: 1px solid #000000; border-right: none; padding-top: 0in; padding-bottom: 0.04in; padding-left: 0.04in; padding-right: 0in\"><p align=\"left\">"
                        "<font face=\"DejaVu Sans Mono, monospace\"><font size=\"2\" style=\"font-size: 10pt\">Kullanıcı</font></font></p>"
                    "</td>"
                    "<td width=\"67%\" style=\"border-top: none; border-bottom: 1px solid #000000; border-left: 1px solid #000000; border-right: 1px solid #000000; padding-top: 0in; padding-bottom: 0.04in; padding-left: 0.04in; padding-right: 0.04in\"><p align=\"center\">"
                        "<font face=\"DejaVu Sans Mono, monospace\"><font size=\"2\" style=\"font-size: 10pt\">" + _user.getUserName() + "</font></font></p>"
                    "</td>"
                "</tr>"
                "<tr>"
                    "<td width=\"33%\" valign=\"top\" style=\"border-top: none; border-bottom: 1px solid #000000; border-left: 1px solid #000000; border-right: none; padding-top: 0in; padding-bottom: 0.04in; padding-left: 0.04in; padding-right: 0in\"><p align=\"left\">"
                        "<font face=\"DejaVu Sans Mono, monospace\"><font size=\"2\" style=\"font-size: 10pt\">Cari</font></font></p>"
                    "</td>"
                    "<td width=\"67%\" style=\"border-top: none; border-bottom: 1px solid #000000; border-left: 1px solid #000000; border-right: 1px solid #000000; padding-top: 0in; padding-bottom: 0.04in; padding-left: 0.04in; padding-right: 0.04in\"><p align=\"center\">"
                        "<font face=\"DejaVu Sans Mono, monospace\"><font size=\"2\" style=\"font-size: 10pt\">" + _cari.getAd() + "</font></font></p>"
                    "</td>"
                "</tr>"
                "<tr>"
                    "<td width=\"33%\" style=\"border-top: none; border-bottom: 1px solid #000000; border-left: 1px solid #000000; border-right: none; padding-top: 0in; padding-bottom: 0.04in; padding-left: 0.04in; padding-right: 0in\"><p align=\"left\">"
                        "<font face=\"DejaVu Sans Mono, monospace\"><font size=\"2\" style=\"font-size: 10pt\">Açıklama</font></font></p>"
                    "</td>"
                    "<td width=\"67%\" style=\"border-top: none; border-bottom: 1px solid #000000; border-left: 1px solid #000000; border-right: 1px solid #000000; padding-top: 0in; padding-bottom: 0.04in; padding-left: 0.04in; padding-right: 0.04in\"><p align=\"center\">"
                        "<font face=\"DejaVu Sans Mono, monospace\"><font size=\"2\" style=\"font-size: 10pt\">" + _aciklama + "</font></font></p>"
                    "</td>"
                "</tr>"
            "</table>"
            "<p align=\"center\" style=\"line-height: 100%; margin-bottom: 0in\"><font face=\"DejaVu Sans Mono, monospace\"><font size=\"2\" style=\"font-size: 10pt\">TUTARLAR</font></font></p>"
            "<table width=\"100%\" cellpadding=\"4\" cellspacing=\"0\">"
                "<col width=\"96*\"/>"

                "<col width=\"160*\"/>"

                "<tr>"
                    "<td width=\"38%\" valign=\"top\" style=\"border-top: 1px solid #000000; border-bottom: 1px solid #000000; border-left: 1px solid #000000; border-right: none; padding-top: 0.04in; padding-bottom: 0.04in; padding-left: 0.04in; padding-right: 0in\"><p align=\"left\">"
                        "<font face=\"DejaVu Sans Mono, monospace\"><font size=\"2\" style=\"font-size: 9pt\">İşlem No</font></font></p>"
                    "</td>"
                    "<td width=\"62%\" style=\"border: 1px solid #000000; padding: 0.04in\"><p align=\"center\">"
                        "<font face=\"DejaVu Sans Mono, monospace\"><font size=\"2\" style=\"font-size: 10pt\">" + _islemNo + "</font></font></p>"
                    "</td>"
                "</tr>"
                "<tr>"
                    "<td width=\"38%\" valign=\"top\" style=\"border-top: none; border-bottom: 1px solid #000000; border-left: 1px solid #000000; border-right: none; padding-top: 0in; padding-bottom: 0.04in; padding-left: 0.04in; padding-right: 0in\"><p align=\"left\">"
                        "<font face=\"DejaVu Sans Mono, monospace\"><font size=\"2\" style=\"font-size: 9pt\">Ödeme tarihi</font></font></p>"
                    "</td>"
                    "<td width=\"62%\" style=\"border-top: none; border-bottom: 1px solid #000000; border-left: 1px solid #000000; border-right: 1px solid #000000; padding-top: 0in; padding-bottom: 0.04in; padding-left: 0.04in; padding-right: 0.04in\"><p align=\"center\">"
                        "<font face=\"DejaVu Sans Mono, monospace\"><font size=\"2\" style=\"font-size: 10pt\">" + _islemTarihi.toString("hh:mm dd.MM.yyyy") + "</font></font></p>"
                    "</td>"
                "</tr>"
                "<tr>"
                    "<td width=\"38%\" valign=\"top\" style=\"border-top: none; border-bottom: 1px solid #000000; border-left: 1px solid #000000; border-right: none; padding-top: 0in; padding-bottom: 0.04in; padding-left: 0.04in; padding-right: 0in\"><p align=\"left\">"
                        "<font face=\"DejaVu Sans Mono, monospace\"><font size=\"2\" style=\"font-size: 9pt\">Tahsilat</font></font></p>"
                    "</td>"
                    "<td width=\"62%\" style=\"border-top: none; border-bottom: 1px solid #000000; border-left: 1px solid #000000; border-right: 1px solid #000000; padding-top: 0in; padding-bottom: 0.04in; padding-left: 0.04in; padding-right: 0.04in\"><p align=\"center\">"
                        "<font face=\"DejaVu Sans Mono, monospace\"><font size=\"2\" style=\"font-size: 10pt\">" + QString::number(_tutar, 'f', 2) + "</font></font></p>"
                    "</td>"
                "</tr>"
                "<tr>"
                    "<td width=\"38%\" valign=\"top\" style=\"border-top: none; border-bottom: 1px solid #000000; border-left: 1px solid #000000; border-right: none; padding-top: 0in; padding-bottom: 0.04in; padding-left: 0.04in; padding-right: 0in\"><p align=\"left\">"
                        "<font face=\"DejaVu Sans Mono, monospace\"><font size=\"2\" style=\"font-size: 9pt\">Güncel Borç</font></font></p>"
                    "</td>"
                    "<td width=\"62%\" style=\"border-top: none; border-bottom: 1px solid #000000; border-left: 1px solid #000000; border-right: 1px solid #000000; padding-top: 0in; padding-bottom: 0.04in; padding-left: 0.04in; padding-right: 0.04in\"><p align=\"center\">"
                        "<font face=\"DejaVu Sans Mono, monospace\"><font size=\"2\" style=\"font-size: 10pt\">" + QString::number(cariToplamBorc, 'f', 2) + "</font></font></p>"
                    "</td>"
                "</tr>"
            "</table>"
            "<p align=\"center\" style=\"line-height: 100%; margin-bottom: 0in\"><font face=\"DejaVu Sans Mono, monospace\"><font size=\"2\" style=\"font-size: 10pt\">MHSS TAHSİLAT BİLGİ FİŞİ</font></font></p>"
            "</body>"
            "</html>";

    QTextDocument document;
    document.setPageSize(QSize(204,595));
    document.setHtml(html);

    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPaperSize(QSizeF(72,210), QPrinter::Millimeter);
    printer.setPageMargins(QMarginsF(0, 0, 0, 0));
    printer.setOutputFileName("/tmp/mhss-tahsilat-fis.pdf");

    document.print(&printer);

    QProcess *processIslem = new QProcess();
    if(QSysInfo::prettyProductName().contains("milis", Qt::CaseInsensitive)){
        yazdirmaKomut = "lpr -P " + yaziciModel + " /tmp/mhss-tahsilat-fis.pdf";
    }
    else if(QSysInfo::prettyProductName().contains("pardus", Qt::CaseInsensitive)){
        yazdirmaKomut = "lp -d " + yaziciModel + " /tmp/mhss-tahsilat-fis.pdf";
    }
    else{
        qDebug() << "Yazdırma için İşletim Sistemi tespit edilemedi.";
    }
    processIslem->start(yazdirmaKomut);
}
