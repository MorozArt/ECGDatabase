#include "ecgchartview.h"
#include "globalvariable.h"

ECGChartView::ECGChartView(QWidget *parent) : QChartView(parent)
{
    setMouseTracking(true);
}

ECGChartView::ECGChartView(QChart *chart, QWidget *parent) : QChartView(chart, parent)
{
    setMouseTracking(true);
}

void ECGChartView::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Equal:
        chart()->zoom(1+zoomValue);
        break;
    case Qt::Key_Minus:
        chart()->zoom(1-zoomValue);
        break;
    case Qt::Key_Left:
        chart()->scroll(-scrollValue, 0);
        break;
    case Qt::Key_Right:
        chart()->scroll(scrollValue, 0);
        break;
    case Qt::Key_Up:
        chart()->scroll(0, scrollValue);
        break;
    case Qt::Key_Down:
        chart()->scroll(0, -scrollValue);
        break;
    default:
        QGraphicsView::keyPressEvent(event);
        break;
    }
}

void ECGChartView::wheelEvent(QWheelEvent *event) {
    if(event->angleDelta().y()>0) {
        if(QApplication::keyboardModifiers() & Qt::ControlModifier) {
            axisXRange += axisXRangeScale;
            axisXTimeRange += axisXTimeRangeScale;
            chart()->axes(Qt::Horizontal).first()->setRange(0, axisXRange);
            chart()->axes(Qt::Horizontal).at(1)->setRange(0, axisXTimeRange);
            chart()->axes(Qt::Horizontal).last()->setRange(globalTimeStart, axisXTimeRange+globalTimeStart);
        } else {
            chart()->zoom(1+zoomValue);
        }
    } else {
        if(QApplication::keyboardModifiers() & Qt::ControlModifier) {
            if(axisXRange > (axisXRangeScale+0.0001)) {
                axisXRange -= axisXRangeScale;
                axisXTimeRange -= axisXTimeRangeScale;
                chart()->axes(Qt::Horizontal).first()->setRange(0, axisXRange);
                chart()->axes(Qt::Horizontal).at(1)->setRange(0, axisXTimeRange);
                chart()->axes(Qt::Horizontal).last()->setRange(globalTimeStart, axisXTimeRange+globalTimeStart);
            }
        } else {
            chart()->zoom(1-zoomValue);
        }
    }
}

void ECGChartView::mouseMoveEvent(QMouseEvent *event) {
    if(leftClick) {
        if(event->x()>mousePos.x()) {
            if(event->y()==mousePos.y()) {
                chart()->scroll(-scrollMouseValue, 0);
                mousePos = event->pos();
            }
            if(event->y()>mousePos.y()) {
                chart()->scroll(-scrollMouseValue, scrollMouseValue);
                mousePos = event->pos();
            }
            if(event->y()<mousePos.y()) {
                chart()->scroll(-scrollMouseValue, -scrollMouseValue);
                mousePos = event->pos();
            }
        }
        if(event->x()<mousePos.x()) {
            if(event->y()==mousePos.y()) {
                chart()->scroll(scrollMouseValue, 0);
                mousePos = event->pos();
            }
            if(event->y()>mousePos.y()) {
                chart()->scroll(scrollMouseValue, scrollMouseValue);
                mousePos = event->pos();
            }
            if(event->y()<mousePos.y()) {
                chart()->scroll(scrollMouseValue, -scrollMouseValue);
                mousePos = event->pos();
            }
        }
        if(event->x()==mousePos.x()) {
            if(event->y()>mousePos.y()) {
                chart()->scroll(0, scrollMouseValue);
                mousePos = event->pos();
            }
            if(event->y()<mousePos.y()) {
                chart()->scroll(0, -scrollMouseValue);
                mousePos = event->pos();
            }
        }
    }
    QGraphicsView::mouseMoveEvent(event);
}

void ECGChartView::mousePressEvent(QMouseEvent *event) {
    if(event->button() == Qt::LeftButton) {
        leftClick = true;
        mousePos = event->pos();
    }
    QGraphicsView::mousePressEvent(event);
}

void ECGChartView::mouseReleaseEvent(QMouseEvent *event) {
    if(event->button() == Qt::LeftButton) {
        leftClick = false;
    }
    QGraphicsView::mouseReleaseEvent(event);
}

void ECGChartView::tooltip(QPointF point, bool state) {
    QString tooltipText{""};
    for(int i=0;i<timeMarksCallout.count();++i) {
        if( (point.x()> (timeMarksCallout[i].first-7)) && (point.x()< (timeMarksCallout[i].first+7)) ) {
            tooltipText = timeMarksCallout[i].second;
        }
    }

    if (m_tooltip == nullptr)
        m_tooltip = new Callout(this->chart());

    if (state) {
        m_tooltip->setText(tooltipText);
        m_tooltip->setAnchor(point);
        m_tooltip->setZValue(11);
        m_tooltip->updateGeometry();
        m_tooltip->show();
    } else {
        m_tooltip->hide();
    }
}
