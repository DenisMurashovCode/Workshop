#ifndef ORDERMANAGER_H
#define ORDERMANAGER_H

#include "order.h"
#include <QVector>

class OrderManager
{
public:
    void addOrder(const Order &order);
    const QVector<Order>& getOrders() const;

private:
    QVector<Order> orders;
};

#endif // ORDERMANAGER_H
