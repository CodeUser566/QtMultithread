#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QThread>

class QTableWidget;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindow *ui;
    bool Tableinitialized = false;
    int ActiveThreads = 0;
    void updateClearButtonState();

public:
    void SetupTable(QTableWidget& TableRef);
    void StartTasks(QTableWidget& TableRef);
    void ClearTable();
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void on_LaunchButton_clicked();
    void on_ClearButton_clicked();
    void onProgress(int index, int value);
    void onFinished(int index);
};
#endif // MAINWINDOW_H
