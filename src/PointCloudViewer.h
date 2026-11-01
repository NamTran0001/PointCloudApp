#pragma once

#include <QString>
#include <QVTKOpenGLNativeWidget.h>
#include <QWidget>

#include <vtkActor.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>

#include <array>

class PointCloudViewer : public QWidget
{
    Q_OBJECT

public:
    explicit PointCloudViewer(QWidget *parent = nullptr);

    bool loadPointCloud(const QString &filePath);
    int pointCount() const;

    void setPointSize(float size);
    void setColorByHeight(bool enabled);
    void setAxesVisible(bool visible);
    void resetCamera();

private:
    void createDemoCloud();
    void createAxes();
    void setDefaultCamera();

    QVTKOpenGLNativeWidget *m_vtkWidget = nullptr;
    vtkSmartPointer<vtkRenderWindow> m_renderWindow;
    vtkSmartPointer<vtkRenderer> m_renderer;
    vtkSmartPointer<vtkPolyDataMapper> m_cloudMapper;
    vtkSmartPointer<vtkActor> m_cloudActor;
    std::array<vtkSmartPointer<vtkActor>, 3> m_axisActors;
    int m_pointCount = 0;
};
