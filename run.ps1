$ErrorActionPreference = 'Stop'

$projectRoot = $PSScriptRoot
$ucrtBin = Join-Path $projectRoot '.tools\msys64\ucrt64\bin'
$app = Join-Path $projectRoot 'build\PointCloudApp.exe'

if (-not (Test-Path $app)) {
    throw 'Chưa có build\PointCloudApp.exe. Hãy chạy .\build.ps1 trước.'
}

$env:PATH = "$ucrtBin;$env:PATH"
& $app
exit $LASTEXITCODE
