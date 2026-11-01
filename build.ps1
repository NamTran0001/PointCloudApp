$ErrorActionPreference = 'Stop'

$projectRoot = $PSScriptRoot
$ucrtBin = Join-Path $projectRoot '.tools\msys64\ucrt64\bin'
$cmake = Join-Path $ucrtBin 'cmake.exe'

if (-not (Test-Path $cmake)) {
    throw 'Không tìm thấy môi trường MSYS2 trong .tools. Hãy cài đặt môi trường theo README.md.'
}

$env:PATH = "$ucrtBin;$env:PATH"
$buildDir = Join-Path $projectRoot 'build'
$prefixPath = (Join-Path $projectRoot '.tools\msys64\ucrt64').Replace('\', '/')

& $cmake -S $projectRoot -B $buildDir -G Ninja `
    -DCMAKE_BUILD_TYPE=Release `
    -DCMAKE_PREFIX_PATH=$prefixPath
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

& $cmake --build $buildDir
exit $LASTEXITCODE
