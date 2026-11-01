#pragma once

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void openFile();
    void showAbout();
    void onPointSizeChanged(int value);
    void onColorModeChanged(int index);

private:
    void setupConnections();

    Ui::MainWindow *ui;
};
