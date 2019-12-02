#ifndef DURATIONDISPLAY_H
#define DURATIONDISPLAY_H

#include<QLabel>
#include <string>
#include <sstream>
#include <QTimer>
#include <QMediaPlayer>
#include <QSlider>
#include<QMouseEvent>

#include<QDebug>



class ControlBar: public QSlider
{

    Q_OBJECT

public:
    ControlBar(QWidget *parent=nullptr);
    void mousePressEvent(QMouseEvent *event);

signals:
    void Clicked(void);



};


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


class DurationDisplay2: public QObject
{
    Q_OBJECT

public:
    DurationDisplay2(QLabel *ElapsedTimeDisplay=nullptr, QLabel *remainingTimeDisplay=nullptr, ControlBar *controlBar=nullptr);
    static QString conversion(uint time);

signals:
    void setPosition(quint64 newPosition);

public slots:
    void mediaChanged(const QMediaContent& media);
    void durationChanged(qint64 duration);
    void elapsedSecond(void);
    void start(void);
    void pause(void);
    void reset(void);
    void valueChanged();

private:
    bool isMediaChanged, isStopButtonPushed;
    uint remainingTime, elapsedTime, duration;
    QTimer *timer;
    QLabel *elapsedTimeDisplay;
    QLabel *remainingTimeDisplay;
    ControlBar *controlBar;

};




#endif // DURATIONDISPLAY_H
