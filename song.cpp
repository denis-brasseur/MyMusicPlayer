#include "song.h"


/* Song methods*/
Song::Song(QString path) : QFileInfo(path), QListWidgetItem(path)
{
    this->setText(this->fileName());
}
Song::Song(QFileInfo fi) : QFileInfo(fi), QListWidgetItem(fi.fileName()){}

/* SongList methods*/
SongList::SongList() : QListWidget(){
    connect(this,&QListWidget::itemDoubleClicked,this,&SongList::doubleClickedItem);
}

SongList::SongList(QWidget *parent) : QListWidget(parent){
    connect(this,&QListWidget::itemDoubleClicked,this,&SongList::doubleClickedItem);
}

void SongList::addItem(Song *song){
    this->QListWidget::addItem(song);
}

void SongList::addItems(QStringList paths){
    for(int i=0; i<paths.size(); i++){
        this->SongList::addItem(paths[i]);
    }
}
void SongList::addItem(QString path){
    this->QListWidget::addItem(new Song(path));
}

Song* SongList::currentItem(){
    return dynamic_cast<Song*> (this->QListWidget::currentItem());
}

Song* SongList::item(int row){
    return dynamic_cast<Song*>(this->QListWidget::item(row));
}

void SongList::doubleClickedItem(QListWidgetItem *item){
    emit itemDoubleClicked(dynamic_cast<Song*> (item));
}
