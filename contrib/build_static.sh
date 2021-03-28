#!/bin/sh
cd ..
directory=$(pwd)
cd -
docker build --network=host -f Dockerfile.static-linking -t alpine_mqtt_to_influxdb . && \
docker run --network=host -it -v ${directory}:/code alpine_mqtt_to_influxdb sh -c "cd /code/ && rm -rf buildstatic && mkdir buildstatic && cd buildstatic && cmake -DWITH_TESTS=off -DMQTT_TO_INFLUXDB_BUILD_SHARED=off .. && make -j4 && cpack"
