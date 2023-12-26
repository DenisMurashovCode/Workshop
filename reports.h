#ifndef REPORTS_H
#define REPORTS_H

#include "ordermanager.h"

class Reports
{
public:
    static double calculateTotalRevenue(const OrderManager &orderManager);
};

#endif // REPORTS_H
