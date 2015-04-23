#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QStringList>
#include <QStringListModel>
#include <QAbstractItemView>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    setWindowFlags( (windowFlags() | Qt::CustomizeWindowHint) & ~Qt::WindowMaximizeButtonHint);
    ui->setupUi(this);
    ui->pushButton_play->setIcon(QIcon(":/img/btn_play.png"));
    repeatMode = 0;
    moving = false;
    setRepeatModeIcon();
    shuffle = false;
    position = 0;
    ui->listView->setEditTriggers(0);
    ui->listView->setAttribute(Qt::WA_MacShowFocusRect, false);


    ui->horizontalSlider->setMaximum(0);

    trackModel = new TrackModel;
    trackModel->list = tracklist;

    ui->listView->setModel(trackModel);
    ui->listView->setItemDelegate(new TrackDelegate);
    ui->closeButton->setVisible(false);
    setAcceptDrops(true);

    QDir dir =QDir::home();
    scanner = new Scanner(dir,this);
    connect(scanner,SIGNAL(fileAdded(QString)),this,SLOT(onFileAdded(QString)));
    //if (tracklist.size()==0) {
        scanner->start();
    //}

    audio = new AudioThread(this);
    connect(audio, SIGNAL(startOfPlayback(double )), this, SLOT(onStartOfPlayback(double)));
    connect(audio, SIGNAL(endOfPlayback()), this, SLOT(onEndOfPlayback()));
    connect(audio, SIGNAL(pauseOfPlayback()), this, SLOT(onPauseOfPlayback()));
    connect(audio, SIGNAL(curPos(double , double )), this, SLOT(onCurPos(double, double)));
}

void MainWindow::dropEvent(QDropEvent *ev)
{
   QList<QUrl> urls = ev->mimeData()->urls();
   foreach(QUrl url, urls)
   {
       qDebug()<<url.toString();
       addItem(url.toString());
       audio->play(url.path());
   }
}

void MainWindow::dragEnterEvent(QDragEnterEvent *ev)
{
    ev->accept();
}

void MainWindow::addItem(QString s) {
    QFileInfo fi(s);
    Track *track = new Track;
    track->path = s;
    QString dirPath = fi.absoluteDir().absolutePath();
    track->artist = dirPath.split("/").last();
    track->title = fi.completeBaseName();
    tracklist.append(track);
    trackModel->appendTrack(track);
}

MainWindow::~MainWindow()
{
    delete ui;
    scanner->stop();
    audio->stop();
    delete scanner;
    delete audio;
}

void MainWindow::on_closeButton_clicked()
{
    audio->stop();
    MainWindow::close();
}
void MainWindow::onStartOfPlayback(double total) {
    ui->horizontalSlider->setMaximum(total);
    ui->pushButton_play->setIcon(QIcon(":/img/btn_pause.png"));
    QModelIndex index = trackModel->index(position, 0);

    if ( index.isValid() ) {
        ui->listView->selectionModel()->clear();
        ui->listView->selectionModel()->select( index, QItemSelectionModel::Select );
        ui->label_artist->setText(tracklist.at(position)->artist);
        ui->label_title->setText(tracklist.at(position)->title);
    }
}

void MainWindow::onEndOfPlayback() {
    ui->pushButton_play->setIcon(QIcon(":/img/btn_play.png"));
    nextTrack(true);
}

void MainWindow::nextTrack(bool next) {
    short add = 1;
    if (!next) add = -1;
    switch (repeatMode) {
    case 1:
        //looped track
        break;
    case 2:
        //looped playlist
        position=position+add;
        if (position>=tracklist.size()) {
            position = 0;
        }
        if (position<0) {
            position = tracklist.size()-1;
        }
        break;
    default:
        position=position+add;
        break;
    }
    if (hasNextTrack()) {
        audio->play(tracklist.at(position)->path);
    }
    else {
        audio->stop();
    }
}

void MainWindow::onPauseOfPlayback() {
    ui->pushButton_play->setIcon(QIcon(":/img/btn_play.png"));
}

void MainWindow::onCurPos(double position, double total) {
    //qDebug() << "Position" << position << "Total" << total;
    if (!moving) {
        ui->horizontalSlider->setValue(position);
    }
}

void MainWindow::on_horizontalSlider_sliderPressed()
{
    moving = true;
}

void MainWindow::on_horizontalSlider_sliderReleased()
{
    moving = false;
    audio->changePosition(ui->horizontalSlider->value());
}

void MainWindow::on_pushButton_play_clicked()
{
    if (hasNextTrack()) {
        audio->playOrPause(tracklist.at(position)->path);
    }
}

bool MainWindow::hasNextTrack() {
    if (tracklist.size()<=0) return false;
    if (position<0) return false;
    if (position>(tracklist.size()-1)) return false;
    return true;
}

void MainWindow::setRepeatModeIcon() {
    if (repeatMode>2) {
        repeatMode = 0;
    }
    switch (repeatMode) {
    case 1:
        ui->pushButton_repeat->setIcon(QIcon(":/img/btn_repeat_one.png"));
        break;
    case 2:
        ui->pushButton_repeat->setIcon(QIcon(":/img/btn_repeat_all_highlighted.png"));
        break;
    default:
        ui->pushButton_repeat->setIcon(QIcon(":/img/btn_repeat_off.png"));
        break;
    }
}

void MainWindow::on_pushButton_repeat_clicked()
{
    repeatMode++;
    setRepeatModeIcon();
}

void MainWindow::on_pushButton_shuffle_clicked()
{
    shuffle = !shuffle;
    if (shuffle) {
        ui->pushButton_shuffle->setIcon(QIcon(":/img/btn_shuffle_highlighted.png"));
    }
    else {
        ui->pushButton_shuffle->setIcon(QIcon(":/img/btn_shuffle_off.png"));
    }
}

void MainWindow::on_pushButton_prev_clicked()
{
    nextTrack(false);
}

void MainWindow::on_pushButton_next_clicked()
{
    nextTrack(true);
}

void MainWindow::onFileAdded(QString file) {
    addItem(file);
}

void MainWindow::on_listView_clicked(const QModelIndex &index)
{
    position = index.row();
    if (hasNextTrack()) {
        audio->play(tracklist.at(position)->path);
    }
}