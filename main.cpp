#include "mainwindow.h"
#include "loadingwindow.h"
#include <QApplication>
#include <QSplashScreen>
#include <QTime>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Создаем окно загрузки
    LoadingWindow loadingWindow(QPixmap("ремонтник1.jpg"));
    loadingWindow.show();

    // Моделирование процесса загрузки
    QTime time;
    time.start();
    for (int i = 0; i < 100; )
    {
        if (time.elapsed() > 40)
        {
            time.start();
            ++i;
            loadingWindow.setProgress(i);
        }
        loadingWindow.setMessage("Loading modules: " + QString::number(i) + "%");
        qApp->processEvents();
    }

    // Закрываем окно загрузки перед открытием основного окна
    loadingWindow.finish(nullptr);

    // Открываем основное окно
    MainWindow w;
    w.show();

    return a.exec();
}

