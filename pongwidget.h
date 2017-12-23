#ifndef PONGWIDGET_H
#define PONGWIDGET_H

#include <QWidget>
#include <QImage>

class PongWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PongWidget(QWidget* parent = 0);
    ~PongWidget();
    QSize sizeHint() const;
    void paintEvent(QPaintEvent* evt);

public slots:
    void setImage(const QImage& image);
private:
    QImage  m_image;
    QSize   m_size;
};

#endif // PONGWIDGET_H
