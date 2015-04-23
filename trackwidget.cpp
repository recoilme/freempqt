#include "trackwidget.h"

TrackWidget::TrackWidget(QWidget *parent) :
    QWidget(parent)
{
    title = new QLabel;
    QPalette mainRowPalette;
    mainRowPalette.setColor(QPalette::WindowText, Qt::white);
    title->setPalette(mainRowPalette);

    description = new QLabel;
    QPalette slaveRowPalette;
    slaveRowPalette.setColor(QPalette::WindowText, QColor(180,180,180));
    description->setPalette(slaveRowPalette);
    layout = new QVBoxLayout;
    layout->addWidget(title);
    layout->addWidget(description);
    //QFont fontMain;
    //font.setStyleStrategy(QFont::PreferAntialias);
    //title->setFont(font);
    //description->setFont(font);
    this->setLayout(layout);
}

TrackWidget::~TrackWidget()
{

}
