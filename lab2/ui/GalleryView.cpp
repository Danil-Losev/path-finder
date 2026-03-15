#include "GalleryView.h"

GalleryView::GalleryView(const std::vector<ImageInfo> &images, QWidget *parent)
{
    mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);
    std::vector<ImageInfo> simple, medium;
    for (const auto &i : images)
    {
        if (i.imgClass == CLASS_SIMPLE)
        {
            simple.push_back(i);
        }
        else
        {
            medium.push_back(i);
        }
    }

    addSection("Simple", simple);
    addSection("Medium", medium);
    setMinimumHeight(700);
}
void GalleryView::addSection(const QString &title, const std::vector<ImageInfo> &sectionImages)
{
    QGroupBox *box = new QGroupBox(title);
    QVBoxLayout *boxLayout = new QVBoxLayout();
    QGridLayout *cardsGrid = new QGridLayout();

    const int columns = 3;
    for (size_t idx = 0; idx < sectionImages.size(); ++idx)
    {
        const auto &info = sectionImages[idx];
        CardWidget *card = new CardWidget(info);
        connect(card, &CardWidget::instantRequested, this, &GalleryView::instantRequested);
        connect(card, &CardWidget::stepwiseRequested, this, &GalleryView::stepwiseRequested);
        int row = idx / columns;
        int col = idx % columns;
        cardsGrid->addWidget(card, row, col);
    }
    boxLayout->addLayout(cardsGrid);
    box->setLayout(boxLayout);
    mainLayout->addWidget(box);
}