#include "pongwidget.h"
#include <QPainter>

PongWidget::PongWidget(QWidget* parent) : QWidget(parent){
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

PongWidget::~PongWidget(){}

QSize PongWidget::sizeHint() const {
    return m_size;
}

void PongWidget::setImage(const QImage& image) {
    m_size = image.size();
    m_image = image.scaled(size());
    update();
}

void PongWidget::paintEvent(QPaintEvent* evt) {
    QPainter painter(this);
    if (!m_image.isNull())
        painter.drawImage(QPoint(0,20), m_image);
}
