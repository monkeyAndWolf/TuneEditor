#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class QSettings;
class QSvgWidget;
class QTimer;
class TunePusher;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QSettings *s, QWidget *parent = 0);
    ~MainWindow();

    static QString TITLE;
    static QString TRANSCRIBER;
    static QString URL;
    static QString TYPE;
    static QString METER;
    static QString NOTE_LENGTH;
    static QString KEY;
    static QString ABC;
    static QString DIRECTORY;
    static QString FILENAME;


public slots:
    void trySave();
    void openDirectorySelector();
    void populateFields();
    void setUpTabs();
    void clearFields();

    void changeTitle();
    void changeTranscriber();
    void changeUrl();
    void changeType();
    void changeMeter();
    void changeNoteLength();
    void changeKey();
    void changeAbc();
    void changeFilename();
    void changeDirectory();

    void flickTimer();
    void updateUI();

    void addJSObject();

    void setSheetMusic();
    void setPrimer();
    void setOverview();

    void resetTheABC(QString filename);

    void exportToPdf();

protected:
    virtual void resizeEvent(QResizeEvent *event);

private:
    Ui::MainWindow *ui;
    QSettings *settings;

    QString toString();
    bool save();

    bool ownFilename;
    QTimer *updateTimer;
    bool musicShown;

    TunePusher *pusher;

    QSvgWidget *svgWidget;

    QString     renderFile;

};

#endif // MAINWINDOW_H
