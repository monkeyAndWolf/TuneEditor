#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "tunepusher.h"

#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QLineEdit>
#include <QTextEdit>
#include <QTimer>
#include <QSettings>
#include <QStatusBar>
#include <QTextStream>
#include <QWebFrame>
#include <QWebPage>

#ifdef DEBUG_WEBKIT
#include <QDebug>
#include <QWebInspector>
#include <QWebSettings>
#endif

QString MainWindow::AUTOSAVE = "autosave";
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


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
  , ownFilename(false)
  , autosaveOn(false)
  , autosaveTimer(0)
{
    pusher = new TunePusher(this);
    ui->setupUi(this);
    setWindowTitle("ABC TuneEditor");
    settings = new QSettings("monkey&wolf", "TuneEditor", this);

    connect(ui->webView->page()->mainFrame(), &QWebFrame::javaScriptWindowObjectCleared, this, &MainWindow::addJSObject);

    QUrl url("qrc:///index.html");
    ui->webView->setUrl(url);
    ui->webView->setZoomFactor(0.68);

#ifdef DEBUG_WEBKIT
    QWebSettings::globalSettings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
    QWebInspector *inspector = new QWebInspector;
    inspector->setPage(ui->webView->page());
    inspector->show();
#endif

    connect(ui->buttonSave, &QPushButton::clicked, this, &MainWindow::trySave);
    connect(ui->buttonDir, &QPushButton::clicked, this, &MainWindow::openDirectorySelector);
    connect(ui->buttonNew, &QPushButton::clicked, this, &MainWindow::clearFields);

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
    connect(ui->checkAutosave, &QCheckBox::stateChanged, this, &MainWindow::changeAutosave);


    autosaveTimer = new QTimer(this);
    autosaveTimer->setInterval(50);
    autosaveTimer->setSingleShot(true);

    connect(autosaveTimer, &QTimer::timeout, this, &MainWindow::updateUI);

    populateFields();
}

void MainWindow::addJSObject()
{
    ui->webView->page()->mainFrame()->addToJavaScriptWindowObject(QString("TunePusher"), pusher);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateUI()
{
    QString abcString = this->toString();
    pusher->updateABC(abcString);
    if (autosaveOn)
    {
        trySave();
    }
}

void MainWindow::flickTimer()
{
    if (autosaveTimer->isActive())
    {
        autosaveTimer->stop();
    }
    autosaveTimer->start();
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

    autosaveOn = settings->value(MainWindow::AUTOSAVE).toBool();
    changeAutosave(autosaveOn);
    ui->checkAutosave->setChecked(autosaveOn);

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
    static QChar cr('\n');
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

void MainWindow::changeAutosave(int state)
{
    if (state)
        trySave();
    settings->setValue(MainWindow::AUTOSAVE, state);
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
    flickTimer();
}

void MainWindow::changeUrl()
{
    settings->setValue(MainWindow::URL, ui->editUrl->text());
    flickTimer();
}

void MainWindow::changeType()
{
    settings->setValue(MainWindow::TYPE, ui->editType->text());
    flickTimer();
}

void MainWindow::changeMeter()
{
    settings->setValue(MainWindow::METER, ui->editMeter->text());
    flickTimer();
}

void MainWindow::changeNoteLength()
{
    settings->setValue(MainWindow::NOTE_LENGTH, ui->editNoteLength->text());
    flickTimer();
}

void MainWindow::changeKey()
{
    settings->setValue(MainWindow::KEY, ui->editKey->text());
    flickTimer();
}

void MainWindow::changeAbc()
{
    settings->setValue(MainWindow::ABC, ui->editABC->toPlainText());
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


