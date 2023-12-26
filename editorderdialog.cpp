#include "editorderdialog.h"
#include "ui_editorderdialog.h"
#include <QMessageBox>
#include "repairstatus.h"
#include <QFileDialog>
#include <QSqlQuery>
#include <QSqlError>
#include <QBuffer>

EditOrderDialog::EditOrderDialog(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::EditOrderDialog),
      orderMarkedForDeletion(false)
{
    ui->setupUi(this);

    connect(ui->saveButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(ui->deleteButton, &QPushButton::clicked, this, &EditOrderDialog::on_deleteButton_clicked);
    connect(ui->loadPhotoButton, &QPushButton::clicked, this, &EditOrderDialog::loadPhoto);

    imagePath = "";
    photoPixmap = QPixmap();
    ui->photoLabel->clear();
    imageByteArray.clear();
}

EditOrderDialog::~EditOrderDialog()
{
    delete ui;
}



void EditOrderDialog::setOrderData(const QString &name, const QString &description, const QString &phone, double cost, const RepairStatus &status, const QDateTime &startDate, const QDateTime &endDate)
{
    ui->nameEdit->setText(name);
    ui->descriptionEdit->setText(description);
    ui->phoneEdit->setText(phone);
    ui->costEdit->setText(QString::number(cost));
    ui->statusComboBox->setCurrentText(status.getStatusString());
    ui->startDateEdit->setDateTime(startDate);
    ui->endDateEdit->setDateTime(endDate);

    // Загрузка фотографии из базы данных и установка ее на виджет
    QSqlQuery query;
    query.prepare("SELECT photo FROM orders WHERE name = :name");
    query.bindValue(":name", name);

    if (query.exec() && query.first()) {
        QByteArray imageByteArray = query.value(0).toByteArray();
        QPixmap photoPixmap;
        photoPixmap.loadFromData(imageByteArray);
        ui->photoLabel->setPixmap(photoPixmap);
        ui->photoLabel->setScaledContents(true);
    } else {
        // Обработка ошибки запроса или отсутствия данных
    }
}




QString EditOrderDialog::getName() const
{
    return ui->nameEdit->text();
}

QString EditOrderDialog::getDescription() const
{
    return ui->descriptionEdit->toPlainText();
}

QString EditOrderDialog::getPhone() const
{
    return ui->phoneEdit->text();
}

double EditOrderDialog::getCost() const
{
    return ui->costEdit->text().toDouble();
}

RepairStatus EditOrderDialog::getStatus() const
{
    QString statusString = ui->statusComboBox->currentText();
    RepairStatus::Status status;

    if (statusString == "Ожидается") {
        status = RepairStatus::Pending;
    } else if (statusString == "В процессе") {
        status = RepairStatus::InProgress;
    } else if (statusString == "Завершено") {
        status = RepairStatus::Completed;
    } else {
        status = RepairStatus::Pending;  // По умолчанию
    }

    return RepairStatus(status);
}

QDateTime EditOrderDialog::getStartDate() const
{
    return ui->startDateEdit->dateTime();
}

QDateTime EditOrderDialog::getEndDate() const
{
    return ui->endDateEdit->dateTime();
}

bool EditOrderDialog::isOrderMarkedForDeletion() const
{
    return orderMarkedForDeletion;
}

void EditOrderDialog::on_deleteButton_clicked()
{
    orderMarkedForDeletion = true;
    accept();
}

// Функция загрузки фотографии
void EditOrderDialog::loadPhoto()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Выберите фото"), "", tr("Изображения (*.png *.jpg *.jpeg *.bmp *.gif)"));

    if (!fileName.isEmpty()) {
        imagePath = fileName;

        if (photoPixmap.load(imagePath)) {
            ui->photoLabel->setPixmap(photoPixmap);
            ui->photoLabel->setScaledContents(true);

            // Преобразование изображения в массив байт для сохранения в базе данных
            QBuffer buffer(&imageByteArray);
            buffer.open(QIODevice::WriteOnly);
            photoPixmap.save(&buffer, "PNG");
        } else {
            // Обработка ошибки загрузки изображения
        }
    }
}

// Метод для получения данных изображения в виде массива байт
QByteArray EditOrderDialog::getImageByteArray() const
{
    return imageByteArray;
}
