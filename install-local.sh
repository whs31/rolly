mkdir -p target &&
cd target &&
mkdir -p install-release-local &&
cd install-release-local &&
echo "configuring release version..." &&
cmake -GNinja -DCMAKE_BUILD_TYPE=Release -DLOCAL_INSTALL=ON ../.. &&
echo "building release version..." &&
cmake --build . &&
echo "installing release version..." &&
cmake --install . &&
echo "done!"
