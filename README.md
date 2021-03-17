# mqtt-to-influxdb

mqtt-to-influxdb is a [MQTT](https://en.wikipedia.org/wiki/MQTT) message parser for writing statistics into InfluxDB.

## Install

- mqtt message broker (e.g. mosquitto)
- InfluxDB
- libyaml-cpp0.6

## Supported OS

- Raspberry Pi OS Lite (debian buster)
- Windows 10

## How to Build

- libyaml-cpp-dev
- libcxxopts-dev
- catch2
- libspdlog-dev

## Dependencies

- https://github.com/jbeder/yaml-cpp
- https://github.com/orca-zhang/influxdb-cpp

## Usage

To validate the configuration you can specify a sample mqtt message.

```bash
./mqtt-to-influxdb-validate -c example.yaml \
  --topic "stat/bedroom/POWER" \
  --message "ON"
```

The data will not be stored in InfluxDB by default. Specify ```--store``` to write the data in InfluxDB.

## Examples
### Json Payload

```json
{
  "battery": 25,
  "battery_low": false,
  "current_heating_setpoint": 12,
  "eurotronic_error_status": 0,
  "eurotronic_host_flags": {
    "boost": false,
    "child_protection": false,
    "mirror_display": false,
    "window_open": false
  },
  "eurotronic_system_mode": 1,
  "linkquality": 105,
  "local_temperature": 16.5,
  "occupied_heating_setpoint": 12,
  "pi_heating_demand": 0,
  "system_mode": "auto",
  "unoccupied_heating_setpoint": 16,
  "update": {
    "state": "available"
  },
  "update_available": true
}
```

```yaml
thermostat:
- topic: "zigbee2mqtt/0x00158d00053d224e"
  json: true
  measurement:
  - name: "thermostat_living_room"
    field:
    - name: "battery"
      payload-field: battery
      optional: true
      data-type: float32
    - name: "linkquality"
      payload-field: linkquality
      data-type: int
    - name: local_temperature
      payload-field: local_temperature
      data-type: float32
    - name: window_open
      payload-field: "eurotronic_host_flags.boost"
      data-type: boolean
```

### Text Payload

```yaml
bedroom_tv:
- topic: "stat/bedroom/POWER"
  description: ""
  measurement:
  - name: "plug_bed_room_tv"
    field: 
    - name: "state"
      value: true
      payload: "on"
      data-type: boolean
    - name: "state"
      value: false
      payload: "OFF"
      ignore-case: false
      data-type: boolean
```
