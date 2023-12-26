#include "ordermanager.h"

void OrderManager::addOrder(const Order &order)
{
    orders.push_back(order);
}

const QVector<Order>& OrderManager::getOrders() const
{
    return orders;
}
