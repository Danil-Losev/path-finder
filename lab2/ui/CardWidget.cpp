#include "CardWidget.h"

CardWidget::CardWidget(const ImageInfo &info, QWidget *parent) : QFrame(parent), info(info)
{
    setFrameShape(QFrame::Box);
    setLineWidth(1);
    setMidLineWidth(1);

    QVBoxLayout *mainL = new QVBoxLayout(this);
    QLabel *img = new QLabel;
    img->setPixmap(QPixmap::fromImage(QImage(QString::fromStdString(info.pathToImage)))
                       .scaled(220, 220, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    mainL->addWidget(img, 0, Qt::AlignCenter);

    QHBoxLayout *btns = new QHBoxLayout;
    QPushButton *instant = new QPushButton("Моментально");
    QPushButton *stepwise = new QPushButton("По шагам");
    btns->addWidget(instant);
    btns->addWidget(stepwise);
    mainL->addLayout(btns);

    connect(instant, &QPushButton::clicked, this, [=]() { emit instantRequested(info); });
    connect(stepwise, &QPushButton::clicked, this, [=]() { emit stepwiseRequested(info); });
}