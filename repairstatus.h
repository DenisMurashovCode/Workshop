#ifndef REPAIRSTATUS_H
#define REPAIRSTATUS_H

#include <QString>
#include <QDateTime>

class RepairStatus {
public:
    enum Status {
        Pending,
        InProgress,
        Completed,
        Overdue
    };

    RepairStatus();
    RepairStatus(Status status, const QDateTime &startDate = QDateTime(), const QDateTime &endDate = QDateTime());

    QString getStatusString() const;
    int toInt() const;
    static RepairStatus fromInt(int value);

    bool operator==(const RepairStatus &other) const;

    QDateTime getStartDate() const;
    void setStartDate(const QDateTime &startDate);

    QDateTime getEndDate() const;
    void setEndDate(const QDateTime &endDate);

private:
    Status status;
    QDateTime startDate;
    QDateTime endDate;
};

#endif // REPAIRSTATUS_H
