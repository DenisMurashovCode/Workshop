#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    loadWindowState();

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("workshop.db");

    if (!db.open()) {
        QMessageBox::critical(this, "Ошибка", "Не удалось открыть базу данных: " + db.lastError().text());
    } else {
        QSqlQuery query;
        if (!query.exec("CREATE TABLE IF NOT EXISTS orders (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT, description TEXT, phone TEXT, cost REAL, status INTEGER, start_date DATETIME, end_date DATETIME, photo BLOB)")) {
            qDebug() << "Ошибка при создании таблицы: " << query.lastError().text();
        }
    }


    ui->orderTable->setColumnWidth(1, 500);
    updateOrderTable();
}

MainWindow::~MainWindow()
{
    saveWindowState();
    delete ui;
}

void MainWindow::on_submitButton_clicked()
{
    QString name = ui->nameEdit->text();
    QString description = ui->descriptionEdit->toPlainText();
    QString phone = ui->phoneEdit->text();
    double cost = ui->costEdit->text().toDouble();

    if (name.isEmpty() || description.isEmpty() || phone.isEmpty() || cost <= 0) {
        QMessageBox::warning(this, "Внимание", "Пожалуйста, заполните все поля корректно.");
        return;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO orders (name, description, phone, cost, status, start_date, end_date) VALUES (:name, :description, :phone, :cost, :status, :start_date, :end_date)");
    query.bindValue(":name", name);
    query.bindValue(":description", description);
    query.bindValue(":phone", phone);
    query.bindValue(":cost", cost);
    query.bindValue(":status", RepairStatus::Pending);
    query.bindValue(":start_date", QDateTime::currentDateTime());  // Инициализация даты и времени начала
    query.bindValue(":end_date", QDateTime::currentDateTime());    // Инициализация даты и времени окончания

    if (query.exec()) {
        QMessageBox::information(this, "Успех", "Заказ добавлен успешно.");
    } else {
        QMessageBox::critical(this, "Ошибка", "Не удалось добавить заказ: " + query.lastError().text());
    }

    Order newOrder(ui->nameEdit->text(), ui->descriptionEdit->toPlainText(), ui->phoneEdit->text(), ui->costEdit->text().toDouble(), RepairStatus::Pending, QDateTime::currentDateTime(), QDateTime::currentDateTime());
    orderManager.addOrder(newOrder);

    updateOrderTable();

    ui->nameEdit->clear();
    ui->descriptionEdit->clear();
    ui->phoneEdit->clear();
    ui->costEdit->clear();
}

void MainWindow::updateOrderTable()
{
    ui->orderTable->clearContents();
    ui->orderTable->setRowCount(0);

    QSqlQuery query;
    if (query.exec("SELECT * FROM orders")) {
        while (query.next()) {
            int rowPosition = ui->orderTable->rowCount();
            ui->orderTable->insertRow(rowPosition);

            QString name = query.value(1).toString();
            QString description = query.value(2).toString();
            QString phone = query.value(3).toString();
            double cost = query.value(4).toDouble();
            int statusValue = query.value(5).toInt();
            QDateTime startDate = query.value(6).toDateTime();
            QDateTime endDate = query.value(7).toDateTime();

            QTableWidgetItem* itemName = new QTableWidgetItem(name);
            itemName->setFlags(itemName->flags() & ~Qt::ItemIsEditable);
            ui->orderTable->setItem(rowPosition, 0, itemName);

            QTableWidgetItem* itemDescription = new QTableWidgetItem(description);
            itemDescription->setFlags(itemDescription->flags() & ~Qt::ItemIsEditable);
            ui->orderTable->setItem(rowPosition, 1, itemDescription);


            QTableWidgetItem* itemPhone = new QTableWidgetItem(phone);
            itemPhone->setFlags(itemPhone->flags() & ~Qt::ItemIsEditable);
            ui->orderTable->setItem(rowPosition, 2, itemPhone);

            QTableWidgetItem* itemCost = new QTableWidgetItem(QString::number(cost));
            itemCost->setFlags(itemCost->flags() & ~Qt::ItemIsEditable);
            ui->orderTable->setItem(rowPosition, 3, itemCost);


            setOrderStatus(rowPosition, RepairStatus::fromInt(statusValue), startDate, endDate);

            QTableWidgetItem* itemStartDate = new QTableWidgetItem(startDate.toString(Qt::ISODate));
            itemStartDate->setFlags(itemStartDate->flags() & ~Qt::ItemIsEditable);
            ui->orderTable->setItem(rowPosition, 5, itemStartDate);

            QTableWidgetItem* itemEndDate = new QTableWidgetItem(endDate.toString(Qt::ISODate));
            itemEndDate->setFlags(itemEndDate->flags() & ~Qt::ItemIsEditable);
            ui->orderTable->setItem(rowPosition, 6, itemEndDate);
        }
        ui->orderTable->setSortingEnabled(true);
        ui->orderTable->sortByColumn(3, Qt::AscendingOrder);
    } else {
        QMessageBox::critical(this, "Ошибка", "Ошибка при выполнении SQL-запроса: " + query.lastError().text());
    }

}




void MainWindow::on_orderTable_itemDoubleClicked(QTableWidgetItem *item)
{
    int row = item->row();
    openEditOrderDialog(row);
}



void MainWindow::openEditOrderDialog(int row)
{
    if (row < 0 || row >= ui->orderTable->rowCount()) {
        return;
    }

    QString name = ui->orderTable->item(row, 0)->text();
    QString description = ui->orderTable->item(row, 1)->text();
    QString phone = ui->orderTable->item(row, 2)->text();
    double cost = ui->orderTable->item(row, 3)->text().toDouble();
    RepairStatus orderStatus = getOrderStatus(row);
    QDateTime startDate = getOrderStartDate(row);
    QDateTime endDate = getOrderEndDate(row);


    EditOrderDialog editDialog;
    editDialog.setOrderData(name, description, phone, cost, orderStatus, startDate, endDate);

    if (editDialog.exec() == QDialog::Accepted) {
        QString editedName = editDialog.getName();
        QString editedDescription = editDialog.getDescription();
        QString editedPhone = editDialog.getPhone();
        double editedCost = editDialog.getCost();
        RepairStatus editedStatus = editDialog.getStatus();
        QDateTime editedStartDate = editDialog.getStartDate();
        QDateTime editedEndDate = editDialog.getEndDate();
        QByteArray editedImageByteArray = editDialog.getImageByteArray();  // Получаем массив байт изображения

        if (editDialog.isOrderMarkedForDeletion()) {
            QSqlQuery deleteQuery;
            deleteQuery.prepare("DELETE FROM orders WHERE name = :name");
            deleteQuery.bindValue(":name", name);

            if (!deleteQuery.exec()) {
                QMessageBox::critical(this, "Ошибка", "Не удалось удалить заказ: " + deleteQuery.lastError().text());
            }

            updateOrderTable();
        } else {
            ui->orderTable->item(row, 0)->setText(editedName);
            ui->orderTable->item(row, 1)->setText(editedDescription);
            ui->orderTable->item(row, 2)->setText(editedPhone);
            ui->orderTable->item(row, 3)->setText(QString::number(editedCost));

            QSqlQuery updateQuery;
            updateQuery.prepare("UPDATE orders SET name = :name, description = :description, phone = :phone, cost = :cost, status = :status, start_date = :start_date, end_date = :end_date, photo = :photo WHERE name = :oldName");
            updateQuery.bindValue(":name", editedName);
            updateQuery.bindValue(":description", editedDescription);
            updateQuery.bindValue(":phone", editedPhone);
            updateQuery.bindValue(":cost", editedCost);
            updateQuery.bindValue(":oldName", name);
            updateQuery.bindValue(":status", editedStatus.toInt());
            updateQuery.bindValue(":start_date", editedStartDate);
            updateQuery.bindValue(":end_date", editedEndDate);
            updateQuery.bindValue(":photo", editedImageByteArray);  // Сохраняем массив байт изображения в базу данных

            if (!updateQuery.exec()) {
                QMessageBox::critical(this, "Ошибка", "Не удалось обновить данные заказа: " + updateQuery.lastError().text());
            }

            setOrderStatus(row, editedStatus, editedStartDate, editedEndDate);
        }
    }
}




void MainWindow::setOrderStatus(int row, const RepairStatus &status, const QDateTime &startDate, const QDateTime &endDate)
{
    QTableWidgetItem *item = ui->orderTable->item(row, 4);
    if (!item) {
        item = new QTableWidgetItem();
        ui->orderTable->setItem(row, 4, item);
    }

    item->setText(status.getStatusString());

    // Проверка на просроченный статус
    if (status == RepairStatus::InProgress && QDateTime::currentDateTime() > endDate) {
        QSqlQuery updateQuery;
        updateQuery.prepare("UPDATE orders SET status = :status WHERE name = :name");
        updateQuery.bindValue(":status", RepairStatus::Overdue);
        updateQuery.bindValue(":name", ui->orderTable->item(row, 0)->text());

        if (!updateQuery.exec()) {
            qDebug() << "Ошибка при установке статуса 'Просрочено': " << updateQuery.lastError().text();
        }

        RepairStatus overdueStatus(RepairStatus::Overdue);
            item->setText(overdueStatus.getStatusString());
    }
}

RepairStatus MainWindow::getOrderStatus(int row) const
{
    QTableWidgetItem *item = ui->orderTable->item(row, 4);
    if (item) {
        QString statusString = item->text();
        if (statusString == "Ожидается") {
            return RepairStatus(RepairStatus::Pending);
        } else if (statusString == "В процессе") {
            return RepairStatus(RepairStatus::InProgress);
        } else if (statusString == "Завершено") {
            return RepairStatus(RepairStatus::Completed);
        } else if (statusString == "Просрочено") {
            return RepairStatus(RepairStatus::Overdue);
        }
    }

    return RepairStatus(RepairStatus::Pending);
}

QDateTime MainWindow::getOrderStartDate(int row) const
{
    QTableWidgetItem *item = ui->orderTable->item(row, 5);
    if (item && !item->text().isEmpty()) {
        return QDateTime::fromString(item->text(), Qt::ISODate);
    }
    return QDateTime(); // Возвращаем пустой объект QDateTime, если данные недоступны или некорректны.
}

QDateTime MainWindow::getOrderEndDate(int row) const
{
    QTableWidgetItem *item = ui->orderTable->item(row, 6);
    if (item && !item->text().isEmpty()) {
        return QDateTime::fromString(item->text(), Qt::ISODate);
    }
    return QDateTime(); // Возвращаем пустой объект QDateTime, если данные недоступны или некорректны.
}


void MainWindow::saveWindowState()
{
    QSettings settings("MyCompany", "MyApp");

    // Сохраните размер и положение окна
    settings.setValue("MainWindow/Size", size());
    settings.setValue("MainWindow/Position", pos());
}

void MainWindow::loadWindowState()
{
    QSettings settings("MyCompany", "MyApp");

    // Загрузите размер и положение окна
    resize(settings.value("MainWindow/Size", QSize(800, 454)).toSize());
    move(settings.value("MainWindow/Position", QPoint(200, 200)).toPoint());
}




QString getStatusText(int status) {
    switch (status) {
    case 0:
        return "Ожидается";
    case 1:
        return "В процессе";
    case 2:
        return "Завершено";
    case 3:
        return "Просрочено";

    default:
        return "Неизвестный статус";
    }
}

void MainWindow::on_searchButton_clicked()
{
    QString searchText = ui->searchLineEdit->text().trimmed();

    if (searchText.isEmpty()) {
        // Если строка поиска пуста, отобразите все заказы
        updateOrderTable();
        return;
    }

    // Иначе выполните поиск по имени заказа
    QSqlQuery query;
    query.prepare("SELECT * FROM orders WHERE name LIKE :searchText");
    query.bindValue(":searchText", "%" + searchText + "%");

    if (query.exec()) {
        if (query.next()) {
            QString message = "Найденные заказы:\n";

            do {
                // Извлеките данные из результата запроса
                QString name = query.value("name").toString();

                QString description = query.value("description").toString();

                // Замените символ новой строки на <br> для обеспечения переноса строки в HTML
                description.replace("\n", "<br>");

                QString phone = query.value("phone").toString();
                QString cost = query.value("cost").toString();
                QString status = getStatusText(query.value("status").toInt());
                QString start_date = query.value("start_date").toDateTime().toString();
                QString end_date = query.value("end_date").toDateTime().toString();

                // Форматируйте данные и добавьте их к сообщению
                message += QString("Имя: %1\nОписание: %2\nТелефон: %3\nСтоимость: %4\nСтатус: %5\nНачало выполнения: %6\nОкончание выполнения: %7\n\n")
                               .arg(name)
                               .arg(description)
                               .arg(phone)
                               .arg(cost)
                               .arg(status)
                               .arg(start_date)
                               .arg(end_date);
            } while (query.next());

            // Выведите уведомление с найденными заказами
            QMessageBox::information(this, "Найденные заказы", message);
        } else {
            QMessageBox::information(this, "Поиск заказов", "Заказы с указанным именем не найдены.");
        }
    } else {
        QMessageBox::critical(this, "Ошибка", "Ошибка при выполнении SQL-запроса: " + query.lastError().text());
    }
}


