#include "loadingwindow.h"
#include "ui_loadingwindow.h"

LoadingWindow::LoadingWindow(const QPixmap &pixmap) : QSplashScreen(pixmap), ui(new Ui::LoadingWindow)
{
    ui->setupUi(this);

    setPixmap(pixmap.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

LoadingWindow::~LoadingWindow()
{
    delete ui;
}

void LoadingWindow::setMessage(const QString &message)
{
    ui->loadingLabel->setText(message);
}

void LoadingWindow::setProgress(int value)
{
    ui->progressBar->setValue(value);
}
