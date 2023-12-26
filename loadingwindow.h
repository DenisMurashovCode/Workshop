#ifndef LOADINGWINDOW_H
#define LOADINGWINDOW_H

#include <QSplashScreen>
#include <QProgressBar>

namespace Ui {
class LoadingWindow;
}

class LoadingWindow : public QSplashScreen
{
    Q_OBJECT

public:
    explicit LoadingWindow(const QPixmap &pixmap = QPixmap());
    ~LoadingWindow();

    void setMessage(const QString &message);
    void setProgress(int value);

private:
    Ui::LoadingWindow *ui;
};

#endif // LOADINGWINDOW_H
