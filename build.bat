@echo off
REM Сборка Vue.js части
echo Building Vue.js client...
cd vue-client
call npm install
call npm run build
cd ..

REM Сборка C++ части
echo Building C++ server...
if not exist cpp-server\build mkdir cpp-server\build
cd cpp-server\build
cmake ..
cmake --build . --config Release
cd ..\..

echo Build completed. The executable is in cpp-server\build\Release\