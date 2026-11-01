#include "MainWindow.h"

#include <QApplication>
#include <QSurfaceFormat>

#include <QVTKOpenGLNativeWidget.h>

int main(int argc, char *argv[])
{
    // Phải đặt trước khi tạo QApplication để QVTK có OpenGL context phù hợp.
    QSurfaceFormat::setDefaultFormat(QVTKOpenGLNativeWidget::defaultFormat());

    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    return app.exec();
}
