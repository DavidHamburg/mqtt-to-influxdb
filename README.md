[![CMake](https://github.com/DavidHamburg/mqtt-to-influxdb/actions/workflows/cmake.yml/badge.svg)](https://github.com/DavidHamburg/mqtt-to-influxdb/actions/workflows/cmake.yml)

# mqtt-to-influxdb

mqtt-to-influxdb is a [MQTT](https://en.wikipedia.org/wiki/MQTT) message parser for writing statistics into InfluxDB.

## Table of Content

* [Requirements](#requirements)
* [Supported OS](#supported-os)
* [Usage](#usage)
* [Configuration](#configuration)
  + [Settings](#settings)
  + [Devices and Topics](#devices-and-topics)
  + [Measurements](#measurements)
  + [Fields](#fields)
    - [Supported Data Types](#supported-data-types)
    - [Use payload as value to store](#use-payload-as-value-to-store)
    - [Match payload and store specific value](#match-payload-and-store-specific-value)
    - [Parse Json Fields](#parse-json-fields)
    - [Parse Json Field of another Field](#parse-json-field-of-another-field)
  + [Complete Example](#complete-example)
* [How to Build](#how-to-build)
  + [Dependencies](#dependencies)

## Requirements

- mqtt message broker (e.g. mosquitto)
- InfluxDB

## Supported OS

- Raspberry OS (arm)
- Ubuntu 18.04 (amd64)
- Windows 10

Any other linux distribution has not been tested, but should work properly out of the box.

## Usage

To validate the configuration you can specify a sample mqtt message.

```bash
./mqtt-to-influxdb-validate -c example.yaml \
  --topic "stat/bedroom/POWER" \
  --message "ON"
```

The data will not be stored in InfluxDB by default. Specify ```--store``` to write the data in InfluxDB.

The main application should be started as systemd service.

```
[Unit]
After=influxdb.service
After=mosquitto.service
Description=mqtt-to-influxdb
 
[Service]
ExecStart=/usr/bin/mqtt-to-influxdb -c /etc/mqtt-to-influxdb/config.yaml
 
[Install]
WantedBy=multi-user.target
```

## Configuration

### Settings

```yaml
settings:
  influxdb:
    host: 127.0.0.1
    database: iot
  broker:
    ip: 127.0.0.1
```

|name|type|required|remarks|
|----|----|--------|-------|
|influxdb.host|string| | default: 127.0.0.1 |
|influxdb.port|int| | default: 8086 |
|influxdb.database|string|x| |
|influxdb.user|string| | |
|influxdb.password|string| | |
|broker.ip|string| | default: 127.0.0.1 |
|broker.port|int| | default: 1883 |

### Devices and Topics

```yaml
bedroom_tv:
  - topic: "stat/bedroom/POWER"
    measurements:
      ...
  - topic: "stat/bedroom/ENERGY"
    measurements:
      ...
```

Defines an new device and specifies the mqtt-topics to listen for.


### Measurements

```yaml
measurements:
  - name: "bedroom_tv"
    fields:
      - name: "state"
        ...
      - name: "state"
        ...
```

The name defines the [Measurement](https://docs.influxdata.com/influxdb/v1.8/concepts/glossary/#measurement) name in influxdb.

### Fields


|name|type|default|required|remarks|
|----|----|-------|--------|-------|
|name|string|     |x| Defines the [field](https://docs.influxdata.com/influxdb/v1.8/concepts/glossary/#field) in influxdb |
|data-type|string|string| | string, float, int, bool |
|match|string| | | looks for specified payload values |
|ignore-case|bool|true| | specfies if match is case-sensitive |
|json-field|string| | |json-field in payload to read|
|value| | | | value to store in influxdb, when not specified the payload will be used as value |
|optional|bool|false| |  |


#### Supported Data Types

 - string (default)
 - bool
 - float
 - int


#### Use payload as value to store

mqtt-message: ```state/plug/temperature 22.5```

```yaml
- name: "temperature"
  data-type: float
```

Result:

 - field: temperature
 - value:          22.5
 - data-type:      float

#### Match payload and store specific value

mqtt-message: ```state/plug/POWER on```

```yaml
- name: "state"
  data-type: bool
  match: "on"
  value: true
```

Result:

 - field:     state
 - value:     true
 - data-type: bool

#### Store payload directly

mqtt-message: ```state/plug/POWER on```

```yaml
- name: "state"
  data-type: bool
```

Result:

 - field:     state
 - value:     true
 - data-type: bool

Hint: when data-type bool is specified it parses ```true```, ```on``` and ```1``` as boolean values.

#### Parse Json Fields

mqtt-message: 

```json
{
  "battery": 25
}
```


```yaml
- name: "state_of_charge"
  data-type: int
  json-field: battery
```

Result:

 - field:     state_of_charge
 - value:     25
 - data-type: int

#### Parse Json Field of another Field

mqtt-message: 

```json
{
  "update": {
    "state": "available"
  }
}
```

```yaml
- name: "hasUpdates"
  json-field: update.state
  match: "available"
  value: true
  data-type: bool
```

Result:

 - field:     hasUpdates
 - value:     true
 - data-type: bool

### Complete Example

```json
settings:
  influxdb:
    host: 127.0.0.1
    database: iot
  broker:
    ip: 127.0.0.1

bureau_pc:
  - topic: "shellies/shellyplug-s-F8C008/relay/0/power"
    measurements:
      - name: "bureau"
        fields:
          - name: "power"
            data-type: float
  - topic: "shellies/shellyplug-s-F8C008/relay/0/energy"
    measurements:
      - name: "bureau"
        fields:
          - name: "energy"
            data-type: int
  - topic: "shellies/shellyplug-s-F8C008/relay/0"
    measurements:
      - name: "bureau"
        fields:
          - name: "state"
            data-type: bool
            match: "on"
            value: true
          - name: "state"
            data-type: bool
            match: "off"
            value: false

living_room_thermostat:
  - topic: "zigbee2mqtt/0x00158d00053d224e"
    measurements:
      - name: "thermostat_living_room"
        fields:
          - name: "room_temperature"
            data-type: float
            json-field: "local_temperature"
          - name: "set_temperature"
            data-type: float
            json-field: "occupied_heating_setpoint"
          - name: "battery"
            data-type: int
            json-field: "battery"
            optional: true
          - name: "update_available"
            data-type: bool
            json-field: "update.available"
            optional: true
          - name: "linkquality"
            data-type: int
            json-field: "linkquality"
```

## How to Build

- cmake (>= 3.10)
- c++ 17 compiler

```bash
mkdir build && cd build && \
cmake .. && \
cmake --build . --config Release
```

### Dependencies

- [libyaml-cpp-dev](https://github.com/jbeder/yaml-cpp)
- [libcxxopts-dev](https://github.com/jarro2783/cxxopts)
- [libspdlog-dev](https://github.com/gabime/spdlog.git)
- [catch2 (optional, WITH_TESTS=ON)](https://github.com/catchorg/Catch2)
- [stduuid (optional, WITH_MQTT_NAME_UUID=ON)](https://github.com/mariusbancila/stduuid)
- [libmsgsl-dev (WITH_MQTT_NAME_UUID=ON)](https://github.com/microsoft/GSL.git)
- [eventpp, git submodule](https://github.com/wqking/eventpp)
- https://github.com/eclipse/paho.mqtt.cpp
- https://github.com/offa/influxdb-cxx


```
vcpkg.exe install spdlog:x64-windows-static yaml-cpp:x64-windows-static cxxopts:x64-windows-static nlohmann-json:x64-windows-static influxdb-cxx:x64-windows-static paho-mqttpp3:x64-windows-static
```
