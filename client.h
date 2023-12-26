#ifndef CLIENT_H
#define CLIENT_H

#include <QString>

class Client
{
public:
    Client(const QString &name, const QString &address);

    QString getName() const;
    QString getAddress() const;

private:
    QString name;
    QString address;
};

#endif // CLIENT_H
