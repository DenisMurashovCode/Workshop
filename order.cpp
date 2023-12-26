#include "order.h"

Order::Order() : cost(0.0) {}

Order::Order(const QString &name, const QString &description, const QString &phone, double cost, const RepairStatus &status, const QDateTime &startDate, const QDateTime &endDate)
    : name(name), description(description), phone(phone), cost(cost), status(status), startDate(startDate), endDate(endDate) {}

QString Order::getName() const
{
    return name;
}

QString Order::getDescription() const
{
    return description;
}

QString Order::getPhone() const
{
    return phone;
}

double Order::getCost() const
{
    return cost;
}

RepairStatus Order::getStatus() const
{
    return status;
}

QDateTime Order::getStartDate() const
{
    return startDate;
}

QDateTime Order::getEndDate() const
{
    return endDate;
}

void Order::setStartDate(const QDateTime &startDate)
{
    this->startDate = startDate;
}

void Order::setEndDate(const QDateTime &endDate)
{
    this->endDate = endDate;
}
