#include "yazici.h"
//*****************************
#include <QString>
#include <QPrinter>
#include <QPrinterInfo>
#include <QTextDocument>
#include <QProcess>
#include <QDateTime>

Yazici::Yazici()
{

}

void Yazici::fisBas(QString _fisNo, Sepet _sepet)
{
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
            "<p align=\"center\" style=\"margin-bottom: 0in; line-height: 100%\"><font face=\"DejaVu Sans Mono, monospace\"><font size=\"2\" style=\"font-size: 10pt\"><b>ERTUĞRUL MARKET</b></font></font></p>"
            "<p align=\"center\" style=\"margin-bottom: 0in; line-height: 100%\"><font face=\"DejaVu Sans Mono, monospace\"><font size=\"2\" style=\"font-size: 10pt\">Şıralık Mah. 5536.CD No:46/A</font></font></p>"
            "<table width=\"100%\" cellpadding=\"4\" cellspacing=\"0\">"
                "<col width=\"176*\"/>"

                "<col width=\"34*\"/>"

                "<col width=\"46*\"/>"

                "<tr valign=\"top\">"
                    "<td width=\"69%\" style=\"border-top: 1px solid #000000; border-bottom: 1px solid #000000; border-left: none; border-right: none; padding: 0.04in 0in\"><p align=\"left\">"
                        "<font face=\"DejaVu Sans Mono, monospace\"><font size=\"1\" style=\"font-size: 8pt\">Ürün</font></font></p>"
                    "</td>"
                    "<td width=\"13%\" style=\"border-top: 1px solid #000000; border-bottom: 1px solid #000000; border-left: none; border-right: none; padding: 0.04in 0in\"><p align=\"center\">"
                        "<font face=\"DejaVu Sans Mono, monospace\"><font size=\"1\" style=\"font-size: 8pt\">Adet</font></font></p>"
                    "</td>"
                    "<td width=\"18%\" style=\"border-top: 1px solid #000000; border-bottom: 1px solid #000000; border-left: none; border-right: none; padding: 0.04in 0in\"><p align=\"center\">"
                        "<font face=\"DejaVu Sans Mono, monospace\"><font size=\"1\" style=\"font-size: 8pt\">Tutar</font></font></p>"
                    "</td>"
                "</tr>";
    foreach (auto urun, _sepet.urunler) {
        html.append(QString(
                        "<tr valign=\"top\">"
                            "<td width=\"69%\" style=\"border: none; padding: 0in\"><p align=\"left\"><font face=\"DejaVu Sans Mono, monospace\"><font size=\"1\" style=\"font-size: 7pt\"><strong>" + urun.ad + "</strong></font></font></p></td>"
                            "<td width=\"13%\" style=\"border: none; padding: 0in\"><p align=\"center\"><font face=\"DejaVu Sans Mono, monospace\"><font size=\"2\" style=\"font-size: 7pt\"><strong>" + QString::number(urun.miktar) + "</strong></font></font></p></td>"
                            "<td width=\"18%\" style=\"border: none; padding: 0in\"><p align=\"center\"><font face=\"DejaVu Sans Mono, monospace\"><font size=\"2\" style=\"font-size: 7pt\"><strong>" + QString::number(urun.toplam) + "</strong></font></font></p></td>"
                        "</tr>"
                        ));
    };
    html.append(QString(
                "<tr valign=\"top\">"
                    "<td colspan=\"2\" width=\"82%\" style=\"border-top: 1px solid #000000; border-bottom: 1px solid #000000; border-left: none; border-right: none; padding: 0.04in 0in\"><p align=\"center\">"
                        "<font face=\"DejaVu Sans Mono, monospace\"><font size=\"1\" style=\"font-size: 8pt\"><strong>Toplam<strong></font></font></p>"
                    "</td>"
                    "<td width=\"18%\" style=\"border-top: 1px solid #000000; border-bottom: 1px solid #000000; border-left: none; border-right: none; padding: 0.04in 0in\"><p align=\"center\">"
                        "<font face=\"DejaVu Sans Mono, monospace\"><font size=\"2\" style=\"font-size: 8pt\"><strong>" + QString::number(_sepet.sepetToplamTutari()) + "<strong></font></font></p>"
                    "</td>"
                "</tr>"
            "</table>"
            "<p align=\"center\" style=\"margin-bottom: 0in; line-height: 100%\">"/*</br> ekleyebilirsin 1 satır boşluk bırakır*/
            "</p>"
            "<p align=\"center\" style=\"margin-bottom: 0in; line-height: 100%\"><font face=\"DejaVu Sans Mono, monospace\"><font size=\"2\" style=\"font-size: 10pt\">NO:" + _fisNo + "</font></font></p>"
            "<p align=\"center\" style=\"margin-bottom: 0in; line-height: 100%\">"/*</br> ekleyebilirsin 1 satır boşluk bırakır*/
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
    processIslem->start("lpr -P XP-Q600 /tmp/mhss-fis.pdf");

}
