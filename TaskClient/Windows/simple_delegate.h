#ifndef SIMPLE_DELEGATE_H
#define SIMPLE_DELEGATE_H

#pragma once

#define ROW_HEIGHT 28

#include <QStyledItemDelegate>
#include <QStandardItemModel>

// Делегат для таблицы
class SimpleItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit SimpleItemDelegate(QStandardItemModel *imodel = 0) : QStyledItemDelegate(imodel) {}

    void paint(QPainter *pPainter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const;

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex& ) const;

private:

};

#endif // SIMPLE_DELEGATE_H
