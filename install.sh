mkdir -p target &&
cd target &&
mkdir -p install-release &&
cd install-release &&
echo "configuring release version..." &&
cmake -GNinja -DCMAKE_BUILD_TYPE=Release ../.. &&
echo "building release version..." &&
cmake --build . &&
echo "installing release version..." &&
sudo cmake --install . &&
echo "done!"