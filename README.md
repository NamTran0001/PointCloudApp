# PointCloudApp

Ứng dụng xem đám mây điểm (point cloud) 3D, dùng **Qt 6** cho giao diện và **VTK 9** cho vùng hiển thị 3D.

> **Trạng thái: bản mock tập trung vào giao diện.** Vùng 3D hiện hiển thị một đám điểm mẫu được tạo trong code. File `data/sample.pcd` đã có nhưng chưa được đọc.

## Cấu trúc thư mục

```text
PointCloudApp/
├── CMakeLists.txt            Cấu hình biên dịch và liên kết thư viện
├── src/
│   ├── main.cpp              Điểm vào của ứng dụng
│   ├── MainWindow.h          Khai báo cửa sổ chính
│   ├── MainWindow.cpp        Quản lý giao diện Qt (menu, bảng điều khiển, sự kiện)
│   ├── PointCloudViewer.h    Khai báo vùng hiển thị 3D
│   └── PointCloudViewer.cpp  Quản lý vùng hiển thị 3D bằng VTK
├── ui/
│   └── MainWindow.ui         Bố cục giao diện (Qt Designer)
├── include/                  Header dùng chung (hiện trống)
├── data/
│   └── sample.pcd            Dữ liệu mẫu để thử nghiệm
└── README.md
```

## Yêu cầu

- CMake 3.16 trở lên
- Qt 6 (module `Widgets` và `OpenGLWidgets`)
- VTK 9.x được build có hỗ trợ Qt (module `GUISupportQt`)
- Trình biên dịch hỗ trợ C++17 (MSVC, MinGW, GCC hoặc Clang)

## Build và chạy

Trên Windows, môi trường MSYS2 UCRT64/MinGW được cài cục bộ trong `.tools`. Mở PowerShell tại thư mục dự án rồi chạy:

```powershell
.\build.ps1
.\run.ps1
```

`build.ps1` tạo bản Release bằng Ninja tại `build/PointCloudApp.exe`; `run.ps1` tự thêm thư mục DLL của Qt và VTK vào `PATH` cho phiên chạy.

Nếu dùng bộ cài Qt/VTK bên ngoài, có thể cấu hình CMake thủ công:

```powershell
cmake -S . -B build -DCMAKE_PREFIX_PATH="C:/Qt/6.x.x/msvc2022_64;C:/VTK"
cmake --build build --config Release
```

- Thay `C:/Qt/6.x.x/msvc2022_64` và `C:/VTK` bằng đường dẫn cài đặt thực tế trên máy bạn.
- Với cấu hình nhiều kiểu build, chạy file `build/Release/PointCloudApp.exe`. Nếu báo thiếu DLL, thêm thư mục `bin` của Qt và VTK vào `PATH`, hoặc dùng `windeployqt`.

## Trạng thái mock

| Thành phần | Trạng thái |
|---|---|
| Cửa sổ chính, menu, toolbar, thanh trạng thái | Xong (giao diện) |
| Bảng điều khiển: kích thước điểm, màu sắc, trục tọa độ, đặt lại góc nhìn | Đã nối với vùng 3D |
| Vùng 3D (`PointCloudViewer`) | Hiển thị đám điểm mẫu tạo trong code |
| Mở file PCD | Chỉ kiểm tra file tồn tại, chưa đọc dữ liệu |

## Việc tiếp theo

- Đọc file PCD trong `PointCloudViewer::loadPointCloud()` và thay đám điểm mẫu bằng dữ liệu thật.
