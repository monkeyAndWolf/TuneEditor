#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "tunepusher.h"

#include <QApplication>
#include <QClipboard>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QLineEdit>
#include <QPainter>
#include <QPalette>
#include <QPrinter>
#include <QResizeEvent>
#include <QTextEdit>
#include <QTimer>
#include <QSettings>
#include <QStatusBar>
#include <QSvgRenderer>
#include <QSvgWidget>
#include <QTextStream>

QString MainWindow::TITLE = "title";
QString MainWindow::TRANSCRIBER = "transcriber";
QString MainWindow::URL = "url";
QString MainWindow::TYPE = "type";
QString MainWindow::METER = "meter";
QString MainWindow::NOTE_LENGTH = "noteLength";
QString MainWindow::KEY = "key";
QString MainWindow::ABC = "abc";
QString MainWindow::DIRECTORY = "directory";
QString MainWindow::FILENAME = "filename";


MainWindow::MainWindow(QSettings *s, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
  , settings(s)
  , ownFilename(false)
  , updateTimer(0)
  , musicShown(true)
  , textShown(false)
{
    pusher = new TunePusher(this);
    ui->setupUi(this);

    connect(pusher, &TunePusher::abcUpdated, this, &MainWindow::resetTheABC);

    setWindowTitle("ABC TuneEditor");
    setUpTabs();

    connect(ui->buttonSave, &QPushButton::clicked, this, &MainWindow::trySave);
    connect(ui->buttonDir, &QPushButton::clicked, this, &MainWindow::openDirectorySelector);
    connect(ui->buttonNew, &QPushButton::clicked, this, &MainWindow::clearFields);
    connect(ui->buttonPdf, &QPushButton::clicked, this, &MainWindow::exportToPdf);
//    connect(ui->buttonCopyText, &QPushButton::clicked, this, &MainWindow::copyToClipboard);

    connect(ui->editABC, &QTextEdit::textChanged, this, &MainWindow::changeAbc);
    connect(ui->editDirectory, &QLineEdit::textChanged, this, &MainWindow::changeDirectory);
    connect(ui->editFilename, &QLineEdit::textChanged, this, &MainWindow::changeFilename);
    connect(ui->editKey, &QLineEdit::textChanged, this, &MainWindow::changeKey);
    connect(ui->editMeter, &QLineEdit::textChanged, this, &MainWindow::changeMeter);
    connect(ui->editNoteLength, &QLineEdit::textChanged, this, &MainWindow::changeNoteLength);
    connect(ui->editTitle, &QLineEdit::textChanged, this, &MainWindow::changeTitle);
    connect(ui->editTranscriber, &QLineEdit::textChanged, this, &MainWindow::changeTranscriber);
    connect(ui->editType, &QLineEdit::textChanged, this, &MainWindow::changeType);
    connect(ui->editUrl, &QLineEdit::textChanged, this, &MainWindow::changeUrl);

//    connect(ui->radioOverview, &QRadioButton::clicked, this, &MainWindow::setOverview);
//    connect(ui->radioPrimer, &QRadioButton::clicked, this, &MainWindow::setPrimer);
//    connect(ui->radioSheetMusic, &QRadioButton::clicked, this, &MainWindow::setSheetMusic);


    updateTimer = new QTimer(this);
    updateTimer->setInterval(50);
    updateTimer->setSingleShot(true);

    connect(updateTimer, &QTimer::timeout, this, &MainWindow::updateUI);

    populateFields();
}

void MainWindow::addJSObject()
{
//    ui->webView->page()->mainFrame()->addToJavaScriptWindowObject(QString("TunePusher"), pusher);
}

void MainWindow::setSheetMusic()
{
//    ui->webView->setUrl(QUrl("qrc:///index.html"));
}

void MainWindow::setPrimer()
{
//    ui->webView->setUrl(QUrl("qrc:///primer.html"));
}

void MainWindow::setOverview()
{
//    ui->webView->setUrl(QUrl("qrc:///intro.html"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QRect geom = this->geometry();
    geom.setWidth(event->size().width());
    geom.setHeight(event->size().height());
    settings->setValue("dimensions", geom);
}

void MainWindow::updateUI()
{
    if (musicShown)
    {
        QString abcString = this->toString();
        pusher->updateABC(abcString);
    }
    else if (textShown)
    {
        QString abcString = toString();
        ui->textEditDisplayText->setText(abcString);
        copyToClipboard();
    }
}

void MainWindow::resetTheABC(QString filename)
{
    renderFile = filename;
    svgWidget->load(filename);
    settings->setValue(MainWindow::ABC, ui->editABC->toPlainText());
}

void MainWindow::flickTimer()
{
    if (updateTimer->isActive())
    {
        updateTimer->stop();
    }
    updateTimer->start();
}

void MainWindow::populateFields()
{
    ui->editABC->setText(settings->value(MainWindow::ABC).toString());
    ui->editDirectory->setText(settings->value(MainWindow::DIRECTORY).toString());
    ui->editFilename->setText(settings->value(MainWindow::FILENAME).toString());
    ui->editKey->setText(settings->value(MainWindow::KEY).toString());
    ui->editMeter->setText(settings->value(MainWindow::METER).toString());
    ui->editNoteLength->setText(settings->value(MainWindow::NOTE_LENGTH).toString());
    ui->editTitle->setText(settings->value(MainWindow::TITLE).toString());
    ui->editTranscriber->setText(settings->value(MainWindow::TRANSCRIBER).toString());
    ui->editType->setText(settings->value(MainWindow::TYPE).toString());
    ui->editUrl->setText(settings->value(MainWindow::URL).toString());
}

void MainWindow::setUpTabs()
{
    ui->tabWidget->setTabText(0, tr("Sheet Music"));
    QHBoxLayout *svgLayout = new QHBoxLayout();
    svgLayout->setSpacing(6);
    svgWidget = new QSvgWidget;
    ui->scrollArea->setBackgroundRole(QPalette::Light);
    ui->scrollArea->setWidget(svgWidget);
    svgWidget->setGeometry(0,0,ui->scrollArea->width(), 600);

    ui->tabWidget->setTabText(1, "Text");
    ui->textEditDisplayText->setReadOnly(true);
    connect(ui->tabWidget, &QTabWidget::currentChanged, this, &MainWindow::tabChanged);

    ui->tabWidget->setTabText(2, tr("The ABC Primer"));
    QFile primer(":/primer.html");
    // I should check this, but I'm 100% certain it will open.
    primer.open(QIODevice::ReadOnly);
    QString primerHTML = primer.readAll();
    primer.close();
    ui->textPrimer->setHtml(primerHTML);
    ui->textPrimer->setReadOnly(true);
    ui->textPrimer->setOpenExternalLinks(true);

    ui->tabWidget->setTabText(3, tr("Instructions and credits"));
    QFile intro(":/intro.html");
    intro.open(QIODevice::ReadOnly);
    QString introHTML = intro.readAll();
    intro.close();
    // For some reason, the selected name wasn't being used, perkele!
    ui->textPerkele->setHtml(introHTML);
    ui->textPerkele->setReadOnly(true);
    ui->textPerkele->setOpenExternalLinks(true);

    ui->tabWidget->setCurrentIndex(0);
}

void MainWindow::tabChanged(int which)
{
    musicShown = textShown = false;
    if (0 == which)
    {
        musicShown = true;
    }
    if (1 == which)
    {
        textShown = true;
        copyToClipboard();
    }
}

void MainWindow::copyToClipboard()
{
    QString contents = toString();
    ui->textEditDisplayText->setText(contents);
    QApplication::clipboard()->setText(contents);
}

void MainWindow::clearFields()
{
    settings->setValue(MainWindow::ABC, "");
    settings->setValue(MainWindow::FILENAME, "");
    settings->setValue(MainWindow::KEY, "");
    settings->setValue(MainWindow::METER, "");
    settings->setValue(MainWindow::TITLE, "");
    settings->setValue(MainWindow::TYPE, "");
    settings->setValue(MainWindow::URL, "");
    populateFields();
    ownFilename = false;
    ui->statusBar->showMessage("");
}

QString MainWindow::toString()
{
#ifdef Q_OS_WIN
    static QString cr("\r\n");
#else
    static QChar cr('\n');
#endif
    QString content;
    content += "X: 1";
    content += cr;

    content += "T: ";
    content += ui->editTitle->text();
    content += cr;

    content += "Z: ";
    content += ui->editTranscriber->text();
    content += cr;

    content += "S: ";
    content += ui->editUrl->text();
    content += cr;

    content += "R: ";
    content += ui->editType->text();
    content += cr;

    content += "M: ";
    content += ui->editMeter->text();
    content += cr;

    content += "L: ";
    content += ui->editNoteLength->text();
    content += cr;

    content += "K: ";
    content += ui->editKey->text();
    content += cr;

    content += ui->editABC->toPlainText();
    content += cr;

    return content;
}

void MainWindow::trySave()
{
    if (save())
    {
        ui->statusBar->showMessage("Saved. Awesome.");
    }
    else
    {
        ui->statusBar->showMessage("Not saved. Bummer.");
    }
}

bool MainWindow::save()
{
    bool success = false;
    QString content = toString();
    QString directory = ui->editDirectory->text();
    QDir dir(directory);
    if (!dir.exists())
    {
        if (!dir.mkdir(directory))
            return false;
    }
    QString filename = ui->editFilename->text();
    if (!filename.endsWith(".abc"))
    {
        filename.append(".abc");
    }

    QFile f(directory + QDir::separator() + filename);

    if (f.open(QIODevice::WriteOnly))
    {
        QTextStream stream(&f);
        stream << content;
        stream.flush();
        f.close();
        success = true;
    }

    return success;
}

void MainWindow::openDirectorySelector()
{
    QString directory = QFileDialog::getExistingDirectory(this, tr("Select a directory to keep stuff"), QDir::homePath(), QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);
    settings->setValue(MainWindow::DIRECTORY, directory);
    ui->editDirectory->setText(directory);
}


void MainWindow::changeTitle()
{
    settings->setValue(MainWindow::TITLE, ui->editTitle->text());
    if (!ownFilename)
    {
        QString newFilename = ui->editTitle->text();
        newFilename.replace(QRegularExpression("[\\/:*?\"<>|]"), "_");
        newFilename.append(".abc");
        ui->editFilename->setText(newFilename);
        changeFilename();
        ownFilename = false;
    }
    else
    {
        flickTimer();
    }
}

void MainWindow::changeTranscriber()
{
    settings->setValue(MainWindow::TRANSCRIBER, ui->editTranscriber->text());
//    flickTimer();
}

void MainWindow::changeUrl()
{
    settings->setValue(MainWindow::URL, ui->editUrl->text());
//    flickTimer();
}

void MainWindow::changeType()
{
    settings->setValue(MainWindow::TYPE, ui->editType->text());
//    flickTimer();
}

void MainWindow::changeMeter()
{
    settings->setValue(MainWindow::METER, ui->editMeter->text());
    flickTimer();
}

void MainWindow::changeNoteLength()
{
    settings->setValue(MainWindow::NOTE_LENGTH, ui->editNoteLength->text());
//    flickTimer();
}

void MainWindow::changeKey()
{
    settings->setValue(MainWindow::KEY, ui->editKey->text());
    flickTimer();
}

void MainWindow::changeAbc()
{
    // ABC only gets changed when the ABC renderer responds.
    flickTimer();
}

void MainWindow::changeFilename()
{
    ownFilename = true;
    settings->setValue(MainWindow::FILENAME, ui->editFilename->text());
}

void MainWindow::changeDirectory()
{
    settings->setValue(MainWindow::DIRECTORY, ui->editDirectory->text());
    flickTimer();
}

void MainWindow::exportToPdf()
{
    // OK, so I need a file name first.
    QString pdfFileName = QFileDialog::getSaveFileName(this, ("PDF File for \"" + this->ui->editTitle->text() + "\""), QDir::homePath(), "*.pdf");

    if (pdfFileName!="")
    {
        if (!pdfFileName.endsWith(".pdf"))
                    pdfFileName.append(".pdf");
        QPrinter printer;
        printer.setPageSize(QPrinter::A4);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(pdfFileName);
        printer.setFullPage(true);
        QPainter painterNotPrinter(&printer);

        // I have no idea where these values come from.
        // I could look, but I would rather play tunes and not care.
        QSvgRenderer renderer(renderFile);
        QRect viewBox(-30,-50,600,600);
        renderer.setViewBox(viewBox);
        renderer.render(&painterNotPrinter);

        int fonty = 50;
        QSize bottomLine = renderer.defaultSize();
        double weirdMagicNumber = 1.65;
        static QFont secondFont("Times", 12, -1, true);

        if (ui->editTranscriber->text()!="")
        {
            QRect transRect(50,((bottomLine.height()*weirdMagicNumber)+fonty), 600, 20);
            painterNotPrinter.setFont(secondFont);
            QString transcriber = tr("Transcribed by: ");
            transcriber += ui->editTranscriber->text();
            painterNotPrinter.drawText(transRect, transcriber);
            fonty += 15;
        }
        if (ui->editUrl->text()!="")
        {
            QRect transRect(50,((bottomLine.height()*weirdMagicNumber)+fonty), 600, 20);
            painterNotPrinter.setFont(secondFont);
            QString urler = tr("Url: ");
            urler += ui->editUrl->text();
            painterNotPrinter.drawText(transRect, urler);
        }
    }
}
