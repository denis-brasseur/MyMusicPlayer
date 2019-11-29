#include "durationdisplay.h"

DurationDisplay::DurationDisplay(QWidget *parent) : QLabel(parent)
{
    this->isMediaChanged = false;
    this->duration = std::string();
    this->elapsedTime = std::string();
    this->hours = this->minutes = this->seconds = 0;
    this->timer = new QTimer();
    this->timer->setInterval(1000);
    connect(this->timer,&QTimer::timeout,this,&DurationDisplay::elapsedSecond);
    this->setText("");
}

void DurationDisplay::mediaChanged(const QMediaContent& media){
    this->isMediaChanged = true;
}

void DurationDisplay::durationChanged(qint64 duration){
    if(this->isMediaChanged == false || duration==0) return ;
    this->isMediaChanged = false;
    /* from milliseconds to seconds */
    uint durationSeconds = duration/1000;
    /* divising in hours:minutes:seconds */
    uint hours = durationSeconds/3600;
    if(hours>0) durationSeconds -= hours*3600;
    uint minutes = durationSeconds/60;
    if(minutes>0) durationSeconds -= minutes*60;
    uint seconds = durationSeconds;

    std::stringstream buf;
    /* change duration */
    if(hours>0) buf << hours << ":";
    if(minutes<10) buf << "0";
    buf << minutes << ":";
    if(seconds<10) buf << "0";
    buf << seconds;
    buf >> this->duration;
//    qDebug() << this->duration.data();
//    qDebug() << buf.str().data();
    std::stringstream().swap(buf);

    /* reset elapsedTime */
    this->hours = this->minutes = this->seconds = 0;
    if(this->hours>1) buf << this->hours << ":";
    if(this->minutes<10) buf << "0";
    buf << this->minutes << ":";
    if(this->seconds<10) buf << "0";
    buf<< this->seconds;
    buf>> this->elapsedTime;
//    qDebug() << "here";
//    qDebug() << buf.str().data();
    std::stringstream().swap(buf);
    /* set Text */
    buf << this->elapsedTime  << "/" <<this->duration;
//    qDebug() << buf.str().data();
    this->setText(QString(buf.str().data()));
}

void DurationDisplay::elapsedSecond(){
//    qDebug() << "reset";
    /* incrementing elapsed time */
    this->seconds++;
    if(this->seconds>=60){
        this->seconds = 0;
        this->minutes++;
        if(this->minutes>=60){
            this->hours++;
            this->minutes = 0;
        }
    }

    /* refreching elapsedTime */
    std::stringstream buf;
    if(hours>1) buf << this->hours << ":";
    if(minutes<10) buf << "0";
    buf << this->minutes << ":";
    if(seconds<10) buf << "0";
    buf<< this->seconds;
    buf>> this->elapsedTime;
    std::stringstream().swap(buf);

    /* refreching displayed time */
    buf << this->elapsedTime << "/" << this->duration ;
//    qDebug() << buf.str().data();
    this->setText(QString(buf.str().data()));
}

void DurationDisplay::start(void) { this->timer->start();}
void DurationDisplay::pause(void) { this->timer->stop();}
void DurationDisplay::reset(void) {
//    qDebug() << "reset";
    this->timer->stop();
    /* reset elapsedTime */
    std::stringstream buf;
    this->hours = this->minutes = this->seconds = 0;
    if(hours>1) buf << this->hours << ":";
    if(minutes<10) buf << "0";
    buf << this->minutes << ":";
    if(seconds<10) buf << "0";
    buf<< this->seconds;
    buf>> this->elapsedTime;
    std::stringstream().swap(buf);
    /* set Text */
    buf << this->elapsedTime  << "/" <<this->duration;
//    qDebug() << buf.str().data();
    this->setText(QString(buf.str().data()));
}
