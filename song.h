#ifndef SONG_H
#define SONG_H
#include<QListWidgetItem>
#include<QFileInfo>
#include<QString>
#include<QListWidget>
#include<QList>

class Song : public QListWidgetItem, public QFileInfo
{
public:
    Song(QString path);
    Song(QListWidgetItem item);
    Song(QFileInfo fi);
};

class SongList : public QListWidget{

    Q_OBJECT

public:
    SongList(void);
    SongList(QWidget *parent=nullptr);
    void addItem(Song* song);
    void addItem(QString path);
    void addItems(QStringList paths);
    Song* currentItem(void);
    Song* item(int row);

signals:
    void itemDoubleClicked(Song* song);

private slots:
    void doubleClickedItem(QListWidgetItem* item);



};

#endif // SONG_H
