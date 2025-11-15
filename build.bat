@echo off
g++ main.cpp closest_pair.cpp int_mult.cpp stars.cpp -o my_project.exe -I C:\Users\bk\Desktop\raylib\src -L C:\Users\bk\Desktop\raylib\src -lraylib -lopengl32 -lgdi32 -lwinmm -std=c++17
if %errorlevel%==0 (
    my_project.exe
)
pause