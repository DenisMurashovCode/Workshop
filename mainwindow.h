#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ordermanager.h"
#include <QTableWidgetItem>
#include <QPixmap>
#include <QBuffer>
#include "editorderdialog.h"
#include "repairstatus.h"
#include <QSettings>
#include <QMessageBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_submitButton_clicked();
    void on_orderTable_itemDoubleClicked(QTableWidgetItem *item);
    void on_searchButton_clicked();

private:
    Ui::MainWindow *ui;
    OrderManager orderManager;

    void updateOrderTable();
    void openEditOrderDialog(int row);

    void setOrderStatus(int row, const RepairStatus &status, const QDateTime &startDate, const QDateTime &endDate);
    RepairStatus getOrderStatus(int row) const;
    QDateTime getOrderStartDate(int row) const;
    QDateTime getOrderEndDate(int row) const;

    void saveWindowState();
    void loadWindowState();
};

#endif // MAINWINDOW_H
