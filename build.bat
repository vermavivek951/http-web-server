@echo off
echo Compiling...

g++ ./src/main.cpp ./src/server/server.cpp -o server.exe -lws2_32

if %errorlevel% neq 0 (
    echo Build failed.
) else (
    echo Build successful. Run: server.exe
)

pause
