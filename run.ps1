param([bool]$recompile=$true)
if ($recompile) {
    if (Test-Path -Path './a.exe') {
        Remove-Item ./a.exe
    }
    if (Test-Path -Path './main.txt') {
        Remove-Item ./main.txt
    }
    g++ -O3 -fpermissive -fassociative-math -ffast-math -ffloat-store ./main.cpp
}
if (Test-Path -Path './output.txt') {
    Remove-Item ./output.txt
}
if (Test-Path -Path './a.exe') {
    Get-Content ./input.txt | ./a.exe | Tee-Object ./output.txt
    Copy-Item ./main.cpp ./main.txt
}