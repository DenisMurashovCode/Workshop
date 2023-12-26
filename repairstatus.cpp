#include "repairstatus.h"

RepairStatus::RepairStatus() : status(Pending), startDate(QDateTime()), endDate(QDateTime()) {}

RepairStatus::RepairStatus(Status status, const QDateTime &startDate, const QDateTime &endDate)
    : status(status), startDate(startDate), endDate(endDate) {}

QString RepairStatus::getStatusString() const {
    switch (status) {
    case Pending:
        return "Ожидается";
    case InProgress:
        return "В процессе";
    case Completed:
        return "Завершено";
    case Overdue:
        return "Просрочено";
    }

    return "";
}

int RepairStatus::toInt() const {
    return static_cast<int>(status);
}

RepairStatus RepairStatus::fromInt(int value)
{
    if (value == Overdue) {
        return RepairStatus(Overdue);
    }
    return RepairStatus(static_cast<Status>(value));
}


bool RepairStatus::operator==(const RepairStatus &other) const {
    return status == other.status && startDate == other.startDate && endDate == other.endDate;
}

QDateTime RepairStatus::getStartDate() const {
    return startDate;
}

void RepairStatus::setStartDate(const QDateTime &startDate) {
    this->startDate = startDate;
}

QDateTime RepairStatus::getEndDate() const {
    return endDate;
}

void RepairStatus::setEndDate(const QDateTime &endDate) {
    this->endDate = endDate;
}
