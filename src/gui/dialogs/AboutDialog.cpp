// SPDX-License-Identifier: GPL-2.0-or-later
// About dialog (handoff 8.12). The "Based on qBittorrent" credit and the licence text live here, which is the
// one place the label audit allows the name. Licensed GPL-2.0-or-later.

#include "InfoDialogs.h"

#include <QDialogButtonBox>
#include <QFrame>
#include <QGridLayout>
#include <QGroupBox>
#include <QSizePolicy>
#include <QSpacerItem>
#include <QImage>
#include <QLabel>
#include <QPixmap>
#include <QPushButton>
#include <QTabWidget>
#include <QTextBrowser>
#include <QVBoxLayout>

#include "base/utils/misc.h"
#include "theme/RoTheme.h"
#include "RoDialog.h"

void AboutDialog::show(QWidget *parent)
{
    RoDialog dialog(parent, QObject::tr("About"), 640);
    QVBoxLayout *layout = nullptr;
    auto *host = new QDialog;
    host->setObjectName(QStringLiteral("roUi"));
    host->setWindowFlags(Qt::Widget);
    layout = new QVBoxLayout(host);
    layout->setContentsMargins(12, 12, 12, 12);
    layout->setSpacing(8);

    auto *name = new QLabel(QObject::tr("RoTorrent %1 (64-bit)").arg(QStringLiteral("1.0")));
    QFont nf = name->font();
    nf.setPixelSize(20);
    nf.setWeight(QFont::Bold);
    name->setFont(nf);
    layout->addWidget(name);

    const QString link = RoTheme::instance().pal().accent.name();
    auto page = [&](const QString &html)
    {
        auto *b = new QTextBrowser;
        b->setOpenExternalLinks(true);
        b->setFrameShape(QFrame::NoFrame);
        b->setHtml(html);
        b->setMinimumHeight(250);
        return b;
    };
    const auto a = [&link](const QString &url, const QString &text)
    {
        return QStringLiteral("<a href=\"%1\" style=\"color:%2; text-decoration:none\">%3</a>").arg(url, link, text);
    };

    // About tab: logo on the left, text on the right, both centred vertically (as in the ro-qss demo).
    auto aboutTab = [&dialog](const QString &html)
    {
        const qreal dpr = dialog.devicePixelRatioF();
        const int targetH = qRound(70 * dpr);
        QImage img = QPixmap(RoTheme::instance().logoPath()).toImage();
        auto *tab = new QWidget;
        auto *row = new QGridLayout(tab);   // default margins and spacing, as in the demo's aboutdialog.ui
        if (!img.isNull())
        {
            while (img.height() > 2 * targetH)
                img = img.scaledToHeight(qMax(targetH, img.height() / 2), Qt::SmoothTransformation);
            img = img.scaledToHeight(targetH, Qt::SmoothTransformation);
            QPixmap pixmap = QPixmap::fromImage(img);
            pixmap.setDevicePixelRatio(dpr);
            auto *logo = new QLabel;
            logo->setPixmap(pixmap);
            logo->setContentsMargins(10, 0, 22, 0);
            row->addWidget(logo, 0, 0, Qt::AlignVCenter | Qt::AlignHCenter);
        }
        auto *text = new QLabel;
        text->setTextFormat(Qt::RichText);
        text->setWordWrap(true);
        text->setOpenExternalLinks(true);
        text->setTextInteractionFlags(Qt::LinksAccessibleByMouse | Qt::LinksAccessibleByKeyboard);
        text->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        text->setText(html);
        row->addWidget(text, 0, 1);
        return tab;
    };

    // Authors tab: two boxes of label / value rows, as in the ro-qss demo.
    auto infoBox = [](const QString &title, const QList<QPair<QString, QString>> &rows)
    {
        auto *group = new QGroupBox(title);
        auto *grid = new QGridLayout(group);
        int r = 0;
        for (const auto &kv : rows)
        {
            auto *k = new QLabel(kv.first);
            auto *v = new QLabel(kv.second);
            v->setTextFormat(Qt::RichText);
            v->setOpenExternalLinks(true);
            v->setTextInteractionFlags(Qt::LinksAccessibleByMouse | Qt::TextSelectableByMouse);
            grid->addWidget(k, r, 0);
            grid->addWidget(v, r, 1);
            ++r;
        }
        grid->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum), r - 1, 2);
        return group;
    };

    auto *tabs = new QTabWidget;
    tabs->addTab(aboutTab(QStringLiteral("<p>%1</p><p>%2</p><table cellspacing=6>"
                                         "<tr><td>%3</td><td>%4</td></tr><tr><td>%5</td><td>%6</td></tr>"
                                         "<tr><td>%7</td><td>%8</td></tr></table>")
                          .arg(QObject::tr("A fast, modern BitTorrent client with colour coded status, live counts and a clean interface. Built with Qt.")
                               , QObject::tr("Copyright © 2026 The RoTorrent project")
                               , QObject::tr("Home Page:"), a(QStringLiteral("https://rotorrent.org"), QStringLiteral("https://rotorrent.org"))
                               , QObject::tr("Forum:"), QObject::tr("Coming Soon")
                               , QObject::tr("Bug Tracker:"), QObject::tr("Coming Soon"))), QObject::tr("About"));

    auto *authors = new QWidget;
    auto *authorsLayout = new QVBoxLayout(authors);
    authorsLayout->addWidget(infoBox(QObject::tr("Current maintainer"),
        { {QObject::tr("Name:"), QStringLiteral("Rohit Malla")}
        , {QObject::tr("Region:"), QObject::tr("Bengaluru, India")}
        , {QObject::tr("E-mail:"), a(QStringLiteral("mailto:rohitinwork@gmail.com"), QStringLiteral("rohitinwork@gmail.com"))} }));
    authorsLayout->addWidget(infoBox(QObject::tr("Based on"),
        { {QObject::tr("Name:"), QStringLiteral("qBittorrent project")}
        , {QObject::tr("Website:"), a(QStringLiteral("https://www.qbittorrent.org"), QStringLiteral("https://www.qbittorrent.org"))}
        , {QObject::tr("License:"), QObject::tr("GNU GPL v2 or later")} }));
    authorsLayout->addStretch(1);
    tabs->addTab(authors, QObject::tr("Authors"));
    tabs->addTab(page(QStringLiteral("<p>%1</p><ul><li>qBittorrent</li><li>libtorrent-rasterbar</li><li>Boost</li>"
                                     "<li>OpenSSL</li><li>zlib</li><li>Qt</li></ul>")
                      .arg(QObject::tr("Thanks to the testers and contributors, and to the projects below:"))), QObject::tr("Special Thanks"));
    tabs->addTab(page(QStringLiteral("<p>%1</p>").arg(QObject::tr("Translations are contributed by the community."))), QObject::tr("Translators"));
    tabs->addTab(page(QStringLiteral("<p>%1</p><p>%2</p>")
                      .arg(QObject::tr("RoTorrent is based on qBittorrent and is distributed under the GNU General Public License, "
                                       "version 2 or (at your option) any later version.")
                           , QObject::tr("This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; "
                                         "without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE."))), QObject::tr("License"));
    tabs->addTab(page(QStringLiteral("<p>%1</p><ul><li>Qt: %2</li><li>libtorrent-rasterbar: %3</li><li>Boost: %4</li>"
                                     "<li>OpenSSL: %5</li><li>zlib: %6</li></ul><p>%7</p>")
                      .arg(QObject::tr("The following libraries are used in RoTorrent:")
                           , QString::fromLatin1(qVersion()), Utils::Misc::libtorrentVersionString(), Utils::Misc::boostVersionString()
                           , Utils::Misc::opensslVersionString(), Utils::Misc::zlibVersionString()
                           , QObject::tr("IP geolocation data by DB-IP (db-ip.com), licensed under CC BY 4.0."))), QObject::tr("Software Used"));
    layout->addWidget(tabs, 1);
    auto *box = new QDialogButtonBox(QDialogButtonBox::Close);
    box->button(QDialogButtonBox::Close)->setProperty("primary", true);
    QObject::connect(box, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    layout->addWidget(box);

    dialog.setContent(host);
    dialog.run();
}

