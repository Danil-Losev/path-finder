#ifndef CARDWIDGET_H
#define CARDWIDGET_H

#include "ImageInfo.h"
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

class CardWidget : public QFrame
{
    Q_OBJECT
  public:
    CardWidget(const ImageInfo &info, QWidget *parent = nullptr);

  signals:
    void instantRequested(const ImageInfo &info);
    void stepwiseRequested(const ImageInfo &info);

  private:
    ImageInfo info;
};

#endif // CARDWIDGET_H
