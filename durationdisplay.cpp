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


/*** DurationDisplay2 ***/

DurationDisplay2::DurationDisplay2(QLabel *l1,QLabel *l2,ControlBar *s){
    this->elapsedTimeDisplay=l1;
    if(this->elapsedTimeDisplay != nullptr) this->elapsedTimeDisplay->setText(DurationDisplay2::conversion(0));
    this->remainingTimeDisplay=l2;
    if(this->remainingTimeDisplay != nullptr) this->remainingTimeDisplay->setText(DurationDisplay2::conversion(0));
    this->controlBar=s;
    if(this->controlBar!= nullptr) connect(this->controlBar,&QSlider::sliderReleased,this,&DurationDisplay2::valueChanged);
    if(this->controlBar!= nullptr) connect(this->controlBar,&ControlBar::Clicked,this,&DurationDisplay2::valueChanged);
    this->timer = new QTimer();
    this->timer->setInterval(1000); //Timer will buzz every second
    connect(this->timer,&QTimer::timeout,this,&DurationDisplay2::elapsedSecond);
    this->remainingTime = this->elapsedTime = this->duration = 0;
    this->isMediaChanged = this->isStopButtonPushed =  false;
}

QString DurationDisplay2::conversion(uint time){

    uint hours, minutes, seconds;
    hours = time/3600;
    time -= hours*3600;
    minutes = time/60;
    time -= minutes*60;
    seconds = time;

    QString ret;
    if(hours>0) ret += QString::number(hours) + ":";
    if(minutes<10) ret += "0";
    ret += QString::number(minutes) + ":";
    if(seconds<10) ret += "0";
    ret += QString::number(seconds);

    return ret;
}

void DurationDisplay2::mediaChanged(const QMediaContent &media){
    this->isMediaChanged=true;
}

void DurationDisplay2::durationChanged(qint64 duration){
    if(this->isMediaChanged==false || duration==0){
        return;
    }
    else this->isMediaChanged=false;

    this->remainingTime = this->duration = duration/1000;
    this->elapsedTime = 0;

    this->elapsedTimeDisplay->setText(DurationDisplay2::conversion(this->elapsedTime));
    this->remainingTimeDisplay->setText(DurationDisplay2::conversion(this->remainingTime));

    this->controlBar->setValue(0);
}

void DurationDisplay2::start(){ this->timer->start();}

void DurationDisplay2::pause(){ this->timer->stop();}

void DurationDisplay2::reset(){
    this->isStopButtonPushed = true;
//    qDebug() << "reset";
    this->timer->stop();

    this->elapsedTime = 0;
    this->remainingTime = this->duration;

    this->elapsedTimeDisplay->setText(DurationDisplay2::conversion(this->elapsedTime));
    this->remainingTimeDisplay->setText(DurationDisplay2::conversion(this->remainingTime));

    this->controlBar->setValue(0);
}

void DurationDisplay2::elapsedSecond(){
    this->remainingTime--;
    this->elapsedTime++;

    this->elapsedTimeDisplay->setText(DurationDisplay2::conversion(this->elapsedTime));
    this->remainingTimeDisplay->setText(DurationDisplay2::conversion(this->remainingTime));


    int percent;
    percent = (100*this->elapsedTime)/this->duration;
    this->controlBar->setValue(percent);
}

void DurationDisplay2::valueChanged(){
//    if(this->isStopButtonPushed==true){
//        qDebug() << "ici";
//        this->isStopButtonPushed=false;
//        return;
//    }
    int value = this->controlBar->value();
    this->timer->stop();
    this->elapsedTime = (value*this->duration)/100;
    this->remainingTime = this->duration - this->elapsedTime;

    this->elapsedTimeDisplay->setText(DurationDisplay2::conversion(this->elapsedTime));
    this->remainingTimeDisplay->setText(DurationDisplay2::conversion(this->remainingTime));

    qint64 position = this->elapsedTime*1000;
    emit setPosition(position);

    this->timer->start();
}


/*** Control Bar ***/
ControlBar::ControlBar(QWidget* parent) : QSlider(parent){
}

void ControlBar::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        this->setValue(this->minimum() + (event->x()*(this->maximum() - this->minimum())/this->width()));
        event->accept();
    }
    emit Clicked();
    QSlider::mousePressEvent(event);
}
