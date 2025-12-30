#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mGridView(new GridView(this))
{
    ui->setupUi(this);
    
    setCentralWidget(mGridView);
}

MainWindow::~MainWindow()
{
    delete ui;
}


