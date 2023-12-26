#ifndef EDITORDERDIALOG_H
#define EDITORDERDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QDateTimeEdit>
#include <QPixmap>
#include <QBuffer>
#include "repairstatus.h"

namespace Ui {
class EditOrderDialog;
}

class EditOrderDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditOrderDialog(QWidget *parent = nullptr);
    ~EditOrderDialog();

    void setOrderData(const QString &name, const QString &description, const QString &phone, double cost, const RepairStatus &status, const QDateTime &startDate, const QDateTime &endDate);
    QString getName() const;
    QString getDescription() const;
    QString getPhone() const;
    double getCost() const;
    RepairStatus getStatus() const;
    QDateTime getStartDate() const;
    QDateTime getEndDate() const;
    bool isOrderMarkedForDeletion() const;
    QByteArray getImageByteArray() const;

private slots:
    void on_deleteButton_clicked();
    void loadPhoto();

private:
    Ui::EditOrderDialog *ui;
    bool orderMarkedForDeletion;
    QString imagePath;  // Путь к выбранному изображению
    QPixmap photoPixmap;  // Переменная для хранения изображения
    QByteArray imageByteArray;
};

#endif // EDITORDERDIALOG_H
