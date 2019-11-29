#ifndef DURATIONDISPLAY_H
#define DURATIONDISPLAY_H

#include<QLabel>
#include <string>
#include <sstream>
#include <QTimer>
#include <QMediaPlayer>

#include<QDebug>


class DurationDisplay: public QLabel
{

    Q_OBJECT

public:
    DurationDisplay(QWidget *parent=nullptr);
public slots:
    void mediaChanged(const QMediaContent& media);
    void durationChanged(qint64 duration);
    void elapsedSecond(void);
    void start(void);
    void pause(void);
    void reset(void);
private:
    bool isMediaChanged;
    uint hours, minutes, seconds;
    std::string duration;
    std::string elapsedTime;
    QTimer *timer;
};

#endif // DURATIONDISPLAY_H
