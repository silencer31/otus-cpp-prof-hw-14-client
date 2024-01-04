#include "simple_delegate.h"

#include <QPainter>

// Делегат для таблицы

void SimpleItemDelegate::paint(QPainter *pPainter,
                               const QStyleOptionViewItem &option,
                               const QModelIndex &index) const
{
    //QStyle *style = option.widget->style();
    QStyleOptionViewItem option1 = option;
    this->initStyleOption(&option1, index);

#ifdef WIN32
    QFont font = QFont("Tahoma", 12, QFont::Normal);
#else
    QFont font = QFont("Open Sans", 12, QFont::Normal);
#endif

    option1.font = font;

    // Закрашивание прямоугольников таблицы.
    QRect cbRect = option1.rect;
    QColor bgCBColor = qRgba(212, 236, 247, 255);

    if (index.row() == 0) {
        pPainter->fillRect(cbRect, QBrush(bgCBColor));
    }

    QStyledItemDelegate::paint(pPainter, option1, index);
}

QSize SimpleItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex& ) const
{
    return QSize( option.rect.width(), option.rect.height());
}
