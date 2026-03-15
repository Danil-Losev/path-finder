#include "ResultViewer.h"
#include <QHBoxLayout>
#include <QLabel>


ResultViewer::ResultViewer(const ProcessResult &result, QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Результаты распознавания");
    setMinimumSize(1200, 320);
    QHBoxLayout *hbox = new QHBoxLayout(this);

    auto makeFrame = [](const QImage &img, const QString &labelText = QString()) -> QFrame * {
        QFrame *frame = new QFrame;
        frame->setFrameShape(QFrame::Box);
        frame->setLineWidth(1);

        QVBoxLayout *vbox = new QVBoxLayout(frame);
        QLabel *pic = new QLabel;
        pic->setPixmap(QPixmap::fromImage(img).scaled(360, 300, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        pic->setAlignment(Qt::AlignCenter);
        vbox->addWidget(pic);

        if (!labelText.isEmpty())
        {
            QLabel *lbl = new QLabel(labelText);
            lbl->setAlignment(Qt::AlignLeft | Qt::AlignTop);
            vbox->addWidget(lbl);
        }
        else
        {
            vbox->addStretch(1);
        }
        return frame;
    };

    QFrame *f1 = makeFrame(result.pathOnProcessed, "Путь на обработанном");
    QFrame *f2 = makeFrame(result.pathOnColorMap, "Путь на финально обработанном");
    QFrame *f3 = makeFrame(result.pathOnOriginal, "Путь на оригинале");

    hbox->addWidget(f1, 1);
    hbox->addWidget(f2, 1);
    hbox->addWidget(f3, 1);
};
