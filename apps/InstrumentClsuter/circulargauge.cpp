#include "circulargauge.h"
#include <QPainter>
#include <QConicalGradient>
#include <QVBoxLayout>
#include <QPushButton>
#include <cmath>

CircularGauge::CircularGauge(QWidget *parent)
    : QWidget(parent), m_value(0), m_min(0), m_max(260)
{
    setMinimumSize(200, 200);  // Setting the minimum size of a widget
    m_animation = new QPropertyAnimation(this, "value");    // Create animation for the “value” property
    m_animation->setEasingCurve(QEasingCurve::InOutQuad);   // Define easing curve for a smooth transition of the animation
}

CircularGauge::~CircularGauge() {
    if (m_animation) {
        delete m_animation;
        m_animation = nullptr;
    }
}

/**
 * @brief Set the gauge's value
 * @param value
 */
void CircularGauge::setValue(int value)
{
    // Adjust the value so that it is not outside the range
    if (value < m_min) value = m_min;
    if (value > m_max) value = m_max;

    m_value = value;
    update();  // Redraw widget if the value changes
}

void CircularGauge::setRange(int min, int max)
{
    m_min = min;
    m_max = max;
}

void CircularGauge::startAnimation(int targetValue, int duration)
{
    m_animation->stop();                    // Stop an existing animation
    m_animation->setDuration(duration);     // Set animation duration
    m_animation->setStartValue(m_value);    // Start at current value
    m_animation->setEndValue(targetValue);  // Animating to a Target Value
    m_animation->start();                   // Start animation
}

/**
 * @brief Draw the speed meter
 * @param event
 */
void CircularGauge::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    int side = qMin(width(), height());  // Set the size based on the smaller width and height of the widget
    int radius = side / 2;  // Set the radius of the circle

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);  // Smooth graphics with anti-aliasing
    painter.fillRect(rect(), Qt::black);    // Set background to black

    QRectF rect((width() - side) / 2, (height() - side) / 2, side, side);

    drawSpeedGuage(painter, rect);
    printSpeed(painter, rect);
    drawBigScaleAndNumber(painter, radius);
    drawSmallScale(painter, radius);

}

void CircularGauge::drawSpeedGuage(QPainter &painter, QRectF &rect) {

    // Set the angle of the progress bar
    int startAngle = 240 * 14.99;
    int spanAngle = -int(240.0 * (double)(m_value - m_min) / (m_max - m_min) * 16);

    // Setting the color and thickness of the progress bar
    QPen pen(Qt::red, 10);
    pen.setCapStyle(Qt::RoundCap);
    painter.setPen(pen);

    // bar display
    painter.drawArc(rect.adjusted(50, 50, -50, -50), startAngle, spanAngle);

}

void CircularGauge::printSpeed(QPainter &painter, QRectF &rect) {

    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 20));
    painter.drawText(rect, Qt::AlignCenter, QString::number(m_value) + " cm/s");
}

void CircularGauge::drawBigScaleAndNumber(QPainter &painter, int radius){

    painter.setPen(QPen(Qt::white, 4));
    painter.setFont(QFont("Arial", 10));

    int numTicks = (m_max - m_min) / 20 + 1;
    double angleStep = 243.5 / (numTicks - 1);  // 눈금 간격 각도 계산

    int startAngel = 240 *15;
    for (int i = 0; i < numTicks; ++i) {
        int angle = startAngel / 16 + i * angleStep;

        QPointF startPoint_line(radius * 0.80 * cos((angle - 90) * M_PI / 180.0) + width() / 2,
                                radius * 0.80 * sin((angle - 90) * M_PI / 180.0) + height() / 2);
        QPointF endPoint(radius * 0.85 * cos((angle - 90) * M_PI / 180.0) + width() / 2,
                         radius * 0.85 * sin((angle - 90) * M_PI / 180.0) + height() / 2);
        QPointF startPoint(radius * 0.95 * cos((angle - 90) * M_PI / 180.0) + width() / 2,
                           radius * 0.95 * sin((angle - 90) * M_PI / 180.0) + height() / 2);

        painter.drawLine(startPoint_line, endPoint);

        int value = m_min + i * (m_max - m_min) / (numTicks - 1);
        QString label = QString::number(value);
        QRectF textRect = QRectF(startPoint.x() -8, startPoint.y() -6, 20, 10);
        painter.drawText(textRect, Qt::AlignCenter, label);
    }
}

void CircularGauge::drawSmallScale(QPainter &painter, int radius) {

    painter.setPen(QPen(Qt::white, 2));

    int startAngle = 240 * 15;
    int numTicks = (m_max - m_min) / 10 + 1;    // number of scale
    double angleStep = 243.5 / (numTicks - 1);  // calculate the small scale's angle

    for (int i = 0; i < numTicks; ++i) {
        int angle = startAngle / 16 + i * angleStep;

        QPointF startPoint_line(radius * 0.81 * cos((angle - 90) * M_PI / 180.0) + width() / 2,
                                radius * 0.82 * sin((angle - 90) * M_PI / 180.0) + height() / 2);
        QPointF endPoint(radius * 0.84 * cos((angle - 90) * M_PI / 180.0) + width() / 2,
                         radius * 0.84 * sin((angle - 90) * M_PI / 180.0) + height() / 2);

        painter.drawLine(startPoint_line, endPoint);
    }
}
