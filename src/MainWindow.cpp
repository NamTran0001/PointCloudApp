#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "PointCloudViewer.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupConnections();

    ui->pointCountLabel->setText(tr("Số điểm: %1").arg(ui->viewer->pointCount()));
    statusBar()->showMessage(tr("Sẵn sàng. Đang hiển thị dữ liệu mẫu."));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupConnections()
{
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::openFile);
    connect(ui->openButton, &QPushButton::clicked, this, &MainWindow::openFile);
    connect(ui->actionExit, &QAction::triggered, this, &QWidget::close);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::showAbout);

    connect(ui->actionResetView, &QAction::triggered, ui->viewer, &PointCloudViewer::resetCamera);
    connect(ui->resetViewButton, &QPushButton::clicked, ui->viewer, &PointCloudViewer::resetCamera);

    connect(ui->pointSizeSlider, &QSlider::valueChanged, this, &MainWindow::onPointSizeChanged);
    connect(ui->colorModeComboBox, &QComboBox::currentIndexChanged, this,
            &MainWindow::onColorModeChanged);

    connect(ui->axesCheckBox, &QCheckBox::toggled, ui->viewer, &PointCloudViewer::setAxesVisible);
    connect(ui->axesCheckBox, &QCheckBox::toggled, ui->actionShowAxes, &QAction::setChecked);
    connect(ui->actionShowAxes, &QAction::toggled, ui->axesCheckBox, &QCheckBox::setChecked);
}

void MainWindow::openFile()
{
    const QString filePath = QFileDialog::getOpenFileName(
        this, tr("Mở tệp PCD"), QString(), tr("Point Cloud (*.pcd);;Tất cả tệp (*)"));
    if (filePath.isEmpty()) {
        return;
    }

    if (!ui->viewer->loadPointCloud(filePath)) {
        QMessageBox::warning(this, tr("Lỗi"), tr("Không thể mở tệp:\n%1").arg(filePath));
        return;
    }

    ui->fileLabel->setText(QFileInfo(filePath).fileName());
    ui->pointCountLabel->setText(tr("Số điểm: %1").arg(ui->viewer->pointCount()));
    statusBar()->showMessage(
        tr("Đã chọn %1. Chưa đọc dữ liệu thật, đang hiển thị dữ liệu mẫu.").arg(filePath));
}

void MainWindow::showAbout()
{
    QMessageBox::about(this, tr("Giới thiệu"),
                       tr("<b>PointCloudApp</b> - bản mock giao diện<br>"
                          "Qt 6 (giao diện) + VTK 9 (hiển thị 3D)"));
}

void MainWindow::onPointSizeChanged(int value)
{
    ui->pointSizeLabel->setText(tr("%1 px").arg(value));
    ui->viewer->setPointSize(static_cast<float>(value));
}

void MainWindow::onColorModeChanged(int index)
{
    // Chỉ số 0: theo độ cao, 1: màu đơn sắc.
    ui->viewer->setColorByHeight(index == 0);
}
