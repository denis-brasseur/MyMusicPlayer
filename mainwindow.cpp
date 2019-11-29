#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->player = new QMediaPlayer();
    this->loop = noLoop;
    this->songs = this->ui->songs_list;

//    this->ui->verticalSlider->resize(this->ui->verticalSlider->width(),this->songs->height());
    this->ui->verticalSlider->setValue(50);
    this->player->setVolume(50);

    connect(this,&MainWindow::play,this->player,&QMediaPlayer::play);
    connect(this,&MainWindow::pause,this->player,&QMediaPlayer::pause);
    connect(this,&MainWindow::stop,this->player,&QMediaPlayer::stop);
    connect(this->player,&QMediaPlayer::mediaStatusChanged,this,&MainWindow::end_of_music_file);
    connect(this->player,&QMediaPlayer::mediaChanged,this->ui->label,&DurationDisplay::mediaChanged);
    connect(this->player,&QMediaPlayer::durationChanged,this->ui->label,&DurationDisplay::durationChanged);
    //connect(this,&MainWindow::sendPlayer,this->ui->label,&DurationDisplay::durationChanged);
    connect(this,&MainWindow::pause,this->ui->label,&DurationDisplay::pause);
    connect(this,&MainWindow::stop,this->ui->label,&DurationDisplay::reset);
    connect(this,&MainWindow::play,this->ui->label,&DurationDisplay::start);
    connect(this->ui->verticalSlider,&QSlider::valueChanged,this->player,&QMediaPlayer::setVolume);


//    this->ui_rearangement();

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionOpen_File_triggered()
{
    QStringList paths = QFileDialog::getOpenFileNames(this, tr("Open Music File"), "/home/arinfo", tr("Music (*.mp3 *.ogg *.flac *.wav)"));
    this->songs->addItems(paths);
}

void MainWindow::on_playButton_clicked()
{
    if(this->songs->currentItem()!=nullptr){
        this->player->setMedia(QUrl::fromLocalFile(this->songs->currentItem()->filePath()));
    }
    emit play();
}

void MainWindow::on_pauseButton_clicked()
{
    emit pause();
}

void MainWindow::on_stopButton_clicked()
{
    emit stop();
}

void MainWindow::mediaChanged(const QMediaContent& media){
    emit sendPlayer(this->player);
}

void MainWindow::on_previousButton_clicked()
{
    int i = this->songs->currentRow();
    if(i>0 && this->songs->count()>1){
        this->songs->setCurrentItem(this->songs->item(i-1));
        this->player->setMedia(QUrl::fromLocalFile(this->songs->currentItem()->filePath()));
        emit play();
    }
    else if(this->loop == loopAll && i==0 && this->songs->count()>1){
        this->songs->setCurrentItem(this->songs->item(this->songs->count()-1));
        this->player->setMedia(QUrl::fromLocalFile(this->songs->currentItem()->filePath()));
        emit play();
    }
}

void MainWindow::on_nextButton_clicked()
{
    int i = this->songs->currentRow();
    if(i>=0 && i<(this->songs->count()-1)){
        this->songs->setCurrentItem(this->songs->item(i+1));
        this->player->setMedia(QUrl::fromLocalFile(this->songs->currentItem()->filePath()));
        emit play();
    }
    else if(this->loop == loopAll && i==(this->songs->count()-1) && this->songs->count()>1){
        this->songs->setCurrentItem(this->songs->item(0));
        this->player->setMedia(QUrl::fromLocalFile(this->songs->currentItem()->filePath()));
        emit play();
    }
}

void MainWindow::end_of_music_file(QMediaPlayer::MediaStatus status){
    if(status==QMediaPlayer::EndOfMedia){
        if(this->loop==loopOne){
            emit play();
        }
        else if(this->songs->currentRow()<(this->songs->count()-1)){
            this->songs->setCurrentItem(this->songs->item(this->songs->currentRow()+1));
            this->player->setMedia(QUrl::fromLocalFile(this->songs->currentItem()->filePath()));
            emit play();
        }
        else if(this->loop==loopAll){
            this->songs->setCurrentItem(this->songs->item(0));
            this->player->setMedia(QUrl::fromLocalFile(this->songs->currentItem()->filePath()));
            emit play();
        }
        else stop();
    }
}

void MainWindow::on_loopOneButton_clicked()
{
    if(this->ui->loopOneButton->isChecked()){
        this->loop = loopOne;
        qDebug() << "loopOne";
        if(this->ui->loopAllButton->isChecked()) this->ui->loopAllButton->setChecked(false);
        this->ui->loopOneButton->setChecked(true);
    }
}

void MainWindow::shuffle(){
    unsigned int size = this->songs->count();
    int current = this->songs->currentRow();
    std::vector<int> index(size);
    QList<Song*> buf = QList<Song*>();
    for(unsigned int i=0; i<size; i++){
        index[i]=i;
        buf.append((dynamic_cast<Song*>(this->songs->takeItem(0))));
    }
    std::random_shuffle(index.begin(),index.end());
    this->songs->clear();
    for(unsigned int i=0; i<size; i++){
        this->songs->addItem(buf[index[i]]);
    }
    if(current!=-1) this->songs->setCurrentItem(buf[current]);
}

void MainWindow::on_loopAllButton_clicked()
{
    if(this->ui->loopAllButton->isChecked()){
        this->loop = loopAll;
        qDebug() << "loopAll";
        if(this->ui->loopOneButton->isChecked()) this->ui->loopOneButton->setChecked(false);
        this->ui->loopAllButton->setChecked(true);
    }
}

void MainWindow::on_noLoopButton_clicked()
{
    this->loop = noLoop;
    qDebug() << "noLoop";
    if(this->ui->loopOneButton->isChecked()) this->ui->loopOneButton->setChecked(false);
    if(this->ui->loopAllButton->isChecked()) this->ui->loopAllButton->setChecked(false);
}

void MainWindow::on_shuffleButton_clicked()
{
    this->shuffle();
}

void MainWindow::on_songs_list_itemDoubleClicked(Song *item)
{
    this->songs->setCurrentItem(item);
    this->player->setMedia(QUrl::fromLocalFile(this->songs->currentItem()->filePath()));
    emit play();
}

void MainWindow::on_upButton_clicked()
{
    if(this->songs->currentItem()!=nullptr){
        int i=this->songs->currentRow();
        if(i>0){
            Song* buf = dynamic_cast<Song*>(this->songs->takeItem(i));
            this->songs->insertItem(i-1,buf);
            this->songs->setCurrentRow(i-1);
        }
    }
}

void MainWindow::on_downButton_clicked()
{
    if(this->songs->currentItem()!=nullptr){
        int i=this->songs->currentRow();
        if(i<(this->songs->count()-1)){
            Song* buf = dynamic_cast<Song*>(this->songs->takeItem(i));
            this->songs->insertItem(i+1,buf);
            this->songs->setCurrentRow(i+1);
        }
    }
}

void MainWindow::on_deleteButton_clicked()
{
    if(this->songs->currentItem()!=nullptr){
        int i=this->songs->currentRow();
        this->songs->takeItem(i);
        if(this->songs->count()>0) this->player->setMedia(QUrl::fromLocalFile(this->songs->currentItem()->filePath()));
        else this->player->setMedia(nullptr);
    }
}

void MainWindow::on_songs_list_itemClicked(QListWidgetItem *item)
{
    this->songs->setCurrentItem(dynamic_cast<Song*>(item));
}

void MainWindow::duration(qint64 duration){
    qDebug() << duration;
    uint totalDuration = duration/1000;
    uint minutes = totalDuration/60;
    uint seconds = totalDuration%60;
    std::stringstream buf;
    buf << minutes << ":" << seconds;
    QString readableDuration = QString (buf.str().data());
    this->ui->label->setText(readableDuration);
}



void MainWindow:: ui_rearangement(void){
    uint minimalMargin = 10;
    // Vertical Slider on right border
    this->ui->verticalSlider->move(this->width()-(minimalMargin+this->ui->verticalSlider->width()),minimalMargin);
    // Song List on the left border of vertical slider
    QPoint p = this->ui->verticalSlider->pos();
    p.setX(p.x()-minimalMargin-this->songs->width());
    this->songs->move(p);

}
