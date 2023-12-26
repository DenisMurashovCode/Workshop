#ifndef ORDER_H
#define ORDER_H

#include <QString>
#include <QDateTime>
#include "repairstatus.h"

class Order
{
public:
    Order();
    Order(const QString &name, const QString &description, const QString &phone, double cost, const RepairStatus &status, const QDateTime &startDate = QDateTime(), const QDateTime &endDate = QDateTime());

    QString getName() const;
    QString getDescription() const;
    QString getPhone() const;
    double getCost() const;
    RepairStatus getStatus() const;
    QDateTime getStartDate() const;
    QDateTime getEndDate() const;

    void setStartDate(const QDateTime &startDate);
    void setEndDate(const QDateTime &endDate);

private:
    QString name;
    QString description;
    QString phone;
    double cost;
    RepairStatus status;
    QDateTime startDate;
    QDateTime endDate;
};

#endif // ORDER_H
