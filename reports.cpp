#include "reports.h"

double Reports::calculateTotalRevenue(const OrderManager &orderManager)
{
    double totalRevenue = 0.0;
    const QVector<Order>& orders = orderManager.getOrders();

    for (const Order& order : orders) {
        totalRevenue += order.getCost();
    }

    return totalRevenue;
}
