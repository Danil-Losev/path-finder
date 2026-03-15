#ifndef GALLERYVIEW_H
#define GALLERYVIEW_H

#include "CardWidget.h"
#include "ImageInfo.h"
#include <QGroupBox>
#include <QVBoxLayout>
#include <QWidget>

class GalleryView : public QWidget
{
    Q_OBJECT
  public:
    GalleryView(const std::vector<ImageInfo> &images, QWidget *parent = nullptr);

  signals:
    void instantRequested(const ImageInfo &);
    void stepwiseRequested(const ImageInfo &);

  private:
    QVBoxLayout *mainLayout;
    void addSection(const QString &title, const std::vector<ImageInfo> &sectionImages);
};

#endif // GALLERYVIEW_H
