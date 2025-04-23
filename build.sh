#!/bin/bash
# Сборка Vue.js части
echo "Building Vue.js client..."
cd vue-client
npm install
npm run build
cd ..

# Сборка C++ части
echo "Building C++ server..."
mkdir -p cpp-server/build
cd cpp-server/build
cmake ..
make
cd ../..

echo "Build completed. The executable is in cpp-server/build/"