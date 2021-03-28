mkdir build
cd build
set OLDDIR=%CD%
chdir /d %OLDDIR%
cmake -DInfluxDB_DIR=../vcpkg/packages/influxdb-cxx_x64-windows-static/share/influxdb-cxx/ -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake -G "Visual Studio 16 2019" -DCMAKE_BUILD_TYPE=Release -A x64 -DMQTT_TO_INFLUXDB_BUILD_SHARED=off -DWITH_TESTS=off -DVCPKG_TARGET_TRIPLET=x64-windows-static -DWITH_MQTT_NAME_UUID=off ..
cmake --build . --config Release
