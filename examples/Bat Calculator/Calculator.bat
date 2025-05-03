@echo off
set a=%1
set op=%2
set b=%3

set expr=%a% %op% %b%

for /f %%r in ('powershell -command "Write-Output (%expr%)"') do set result=%%r

echo %a% %op% %b% = %result%
pause
