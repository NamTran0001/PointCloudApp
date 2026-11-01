#include "PointCloudViewer.h"

#include <QFileInfo>
#include <QVBoxLayout>

#include <vtkCamera.h>
#include <vtkCellArray.h>
#include <vtkDoubleArray.h>
#include <vtkLineSource.h>
#include <vtkPointData.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkProperty.h>

#include <cmath>
#include <cstddef>
#include <random>

namespace {
constexpr double kPi = 3.14159265358979323846;
constexpr int kDemoPointCount = 20000;
constexpr double kAxisLength = 1.6;
}

PointCloudViewer::PointCloudViewer(QWidget *parent)
    : QWidget(parent)
    , m_vtkWidget(new QVTKOpenGLNativeWidget(this))
{
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_vtkWidget);

    m_renderWindow = m_vtkWidget->renderWindow();
    m_renderer = vtkSmartPointer<vtkRenderer>::New();
    m_renderer->SetBackground(0.12, 0.12, 0.15);
    m_renderWindow->AddRenderer(m_renderer);

    createDemoCloud();
    createAxes();
    setDefaultCamera();
}

bool PointCloudViewer::loadPointCloud(const QString &filePath)
{
    if (!QFileInfo::exists(filePath)) {
        return false;
    }
    // TODO: đọc dữ liệu PCD thật. Hiện tại chỉ giữ đám điểm mẫu và đặt lại góc nhìn.
    resetCamera();
    return true;
}

int PointCloudViewer::pointCount() const
{
    return m_pointCount;
}

void PointCloudViewer::setPointSize(float size)
{
    m_cloudActor->GetProperty()->SetPointSize(size);
    m_renderWindow->Render();
}

void PointCloudViewer::setColorByHeight(bool enabled)
{
    m_cloudMapper->SetScalarVisibility(enabled);
    m_renderWindow->Render();
}

void PointCloudViewer::setAxesVisible(bool visible)
{
    for (const auto &axis : m_axisActors) {
        axis->SetVisibility(visible);
    }
    m_renderWindow->Render();
}

void PointCloudViewer::resetCamera()
{
    setDefaultCamera();
    m_renderWindow->Render();
}

void PointCloudViewer::setDefaultCamera()
{
    vtkCamera *camera = m_renderer->GetActiveCamera();
    camera->SetFocalPoint(0.0, 0.0, 0.0);
    camera->SetPosition(2.5, -3.5, 2.5);
    camera->SetViewUp(0.0, 0.0, 1.0);
    m_renderer->ResetCamera();
}

void PointCloudViewer::createDemoCloud()
{
    constexpr double ringRadius = 1.0;
    constexpr double tubeRadius = 0.35;

    std::mt19937 rng(42);
    std::uniform_real_distribution<double> angle(0.0, 2.0 * kPi);
    std::normal_distribution<double> jitter(0.0, 0.01);

    auto points = vtkSmartPointer<vtkPoints>::New();
    auto heights = vtkSmartPointer<vtkDoubleArray>::New();
    heights->SetName("height");

    for (int i = 0; i < kDemoPointCount; ++i) {
        const double u = angle(rng);
        const double v = angle(rng);
        const double ring = ringRadius + tubeRadius * std::cos(v);
        const double x = ring * std::cos(u) + jitter(rng);
        const double y = ring * std::sin(u) + jitter(rng);
        const double z = tubeRadius * std::sin(v) + jitter(rng);
        points->InsertNextPoint(x, y, z);
        heights->InsertNextValue(z);
    }

    const vtkIdType count = points->GetNumberOfPoints();
    auto vertices = vtkSmartPointer<vtkCellArray>::New();
    for (vtkIdType id = 0; id < count; ++id) {
        vertices->InsertNextCell(1, &id);
    }

    auto cloud = vtkSmartPointer<vtkPolyData>::New();
    cloud->SetPoints(points);
    cloud->SetVerts(vertices);
    cloud->GetPointData()->SetScalars(heights);
    m_pointCount = static_cast<int>(count);

    m_cloudMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    m_cloudMapper->SetInputData(cloud);

    m_cloudActor = vtkSmartPointer<vtkActor>::New();
    m_cloudActor->SetMapper(m_cloudMapper);
    m_cloudActor->GetProperty()->SetPointSize(2.0f);
    m_cloudActor->GetProperty()->SetColor(0.4, 0.9, 1.0);
    m_renderer->AddActor(m_cloudActor);
}

void PointCloudViewer::createAxes()
{
    const double ends[3][3] = {
        {kAxisLength, 0.0, 0.0},
        {0.0, kAxisLength, 0.0},
        {0.0, 0.0, kAxisLength},
    };
    const double colors[3][3] = {
        {1.0, 0.3, 0.3},
        {0.3, 1.0, 0.3},
        {0.4, 0.6, 1.0},
    };

    for (std::size_t i = 0; i < m_axisActors.size(); ++i) {
        auto line = vtkSmartPointer<vtkLineSource>::New();
        line->SetPoint1(0.0, 0.0, 0.0);
        line->SetPoint2(ends[i][0], ends[i][1], ends[i][2]);

        auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper->SetInputConnection(line->GetOutputPort());

        m_axisActors[i] = vtkSmartPointer<vtkActor>::New();
        m_axisActors[i]->SetMapper(mapper);
        m_axisActors[i]->GetProperty()->SetColor(colors[i][0], colors[i][1], colors[i][2]);
        m_axisActors[i]->GetProperty()->SetLineWidth(2.0f);
        m_axisActors[i]->SetVisibility(false);
        m_renderer->AddActor(m_axisActors[i]);
    }
}
