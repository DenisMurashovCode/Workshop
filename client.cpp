#include "client.h"

Client::Client(const QString &name, const QString &address)
    : name(name), address(address)
{
}

QString Client::getName() const
{
    return name;
}

QString Client::getAddress() const
{
    return address;
}
