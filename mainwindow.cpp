#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Work.h"

#include <QTableWidget>
#include <QHeaderView>
#include <QProgressBar>
#include <QTableWidgetItem>
#include <QThread>
#include <QVBoxLayout>



void SetupTable(QTableWidget& TableRef);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->ClearButton->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SetupTable(QTableWidget& TableRef){

    const int N = 10; //кол-во столбцов/потоков
    TableRef.setRowCount(N);
    TableRef.setColumnCount(3);

    TableRef.setHorizontalHeaderLabels(QStringList() << "ID" << "Прогресс" << "Статус");
    TableRef.verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    TableRef.horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);


    //наполнение таблицы
    for (int i = 0; i <= N; ++i) {

        //столбец 1: номер потока
        TableRef.setItem(i,0,new QTableWidgetItem(QString("Поток %1").arg(i+1)));

        //столбец 2: индикатор прогресса
        QProgressBar *Pb = new QProgressBar;
        Pb->setRange(0,100);
        Pb->setValue(0);
        TableRef.setCellWidget(i,1,Pb);

        //Столбец 2: Статус
        TableRef.setItem(i,2,new QTableWidgetItem("Ожидание"));
    }
}


void MainWindow::StartTasks(QTableWidget& TableRef){

    const int N = TableRef.rowCount();
    ActiveThreads = N;
    updateClearButtonState();

    for (int i = 0; i < N; ++i) {

        //Создание и перемещение потоков
        Work *WorkTask = new Work(i);
        QThread *thread = new QThread;

        WorkTask->moveToThread(thread);

        //соединяем поток и подсчёт
        connect(thread,&QThread::started,
                WorkTask,&Work::Process);
        //прогресс
        connect(WorkTask, &Work::progress,this,
                &MainWindow::onProgress);

        // Завершение
        connect(WorkTask, &Work::finished,this,
                &MainWindow::onFinished);

        //Когда поток отработал, останавливаем и чистим поток и WorkTask
        connect(WorkTask, &Work::finished, thread, &QThread::quit);
        connect(WorkTask, &Work::finished, WorkTask, &QObject::deleteLater);
        connect(thread, &QThread::finished, thread, &QObject::deleteLater);


        //Запуск потока
        thread->start();
    }

}

void MainWindow::ClearTable(){
    if(ActiveThreads == 0){
        ui->Table->clear();
        ui->Table->setRowCount(0);
        ui->Table->setColumnCount(0);
        Tableinitialized = false;
    }
}

//Launch
void MainWindow::on_LaunchButton_clicked()
{
    if(!Tableinitialized){
        SetupTable(*ui->Table);
        StartTasks(*ui->Table);
        Tableinitialized = true;
    }
}

//Clear
void MainWindow::on_ClearButton_clicked()
{
    ClearTable();
}

void MainWindow::onProgress(int index, int value){
    QProgressBar *pb = qobject_cast<QProgressBar*>(ui->Table->cellWidget(index,1));
    if (pb){
        pb->setValue(value);
    }
}

void MainWindow::onFinished(int index){
    ui->Table->item(index,2)->setText("Готово!");
    ActiveThreads--;
    updateClearButtonState();
}

void MainWindow::updateClearButtonState() {
    ui->ClearButton->setEnabled(ActiveThreads == 0);
}
