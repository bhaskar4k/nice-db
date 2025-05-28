# Enable strict mode
Set-StrictMode -Version Latest

# Output binary name
$Output = "main.exe"

# Compiler flags
$CxxFlags = "-std=c++17"

# Include directories
$IncludeDirs = "-Icommand-control/header-file -Ienvironment/header-file -Isrc"

# Library flags (for Windows networking)
$Libs = "-lws2_32"

# Find all .cpp files recursively
$CppFiles = Get-ChildItem -Recurse -Filter *.cpp | ForEach-Object { $_.FullName }

# Compile
Write-Host "Compiling..."
g++ $CxxFlags $CppFiles $IncludeDirs -o $Output $Libs

# Run
Write-Host "Running $Output"
Start-Process -NoNewWindow -Wait "./$Output"
