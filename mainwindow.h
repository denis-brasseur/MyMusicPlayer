#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <algorithm>
#include <random>
#include <sstream>

#include "song.h"
#include "durationdisplay.h"

#include <QMainWindow>
#include <QString>
#include <QList>
#include <QListIterator>
#include <QFileDialog>
#include <QtMultimedia/QMediaPlayer>
#include <QDebug>
#include <QListWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

enum loopflag  {noLoop, loopOne, loopAll};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();



private slots:
    void on_actionOpen_File_triggered();

    void on_playButton_clicked();

    void on_pauseButton_clicked();

    void on_stopButton_clicked();

    void on_previousButton_clicked();

    void on_nextButton_clicked();

    void on_loopOneButton_clicked();

    void on_loopAllButton_clicked();

    void on_noLoopButton_clicked();

    void on_shuffleButton_clicked();

    void on_songs_list_itemDoubleClicked(Song *item);

    void on_upButton_clicked();

    void on_downButton_clicked();

    void on_deleteButton_clicked();

    void on_songs_list_itemClicked(QListWidgetItem *item);

    void duration(qint64 duration);


public slots:
    void end_of_music_file(QMediaPlayer::MediaStatus status);
    void mediaChanged(const QMediaContent& media);

signals:
    void new_file_add(QString filename);
    void play(void);
    void pause(void);
    void stop(void);
    void clear(void);
    void sendPlayer(QMediaPlayer* player);

private:
    Ui::MainWindow *ui;
    QMediaPlayer *player;
    QStringList fileList;
    int current;
    enum loopflag loop;
    SongList *songs;

    void shuffle(void);
    void ui_rearangement(void);

};
#endif // MAINWINDOW_H
