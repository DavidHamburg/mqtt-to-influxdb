#include <libmqtt-to-influxdb/mqttmessageparser.hpp>
#include <libmqtt-to-influxdb/stringhelper.hpp>
#include <nlohmann/json.hpp>

mqttmessageparser::mqttmessageparser(const std::shared_ptr<document> &document) : m_document(document) {}

std::map<std::string, std::vector<mqtt_parse_result>> mqttmessageparser::parse(const std::string &msg, const std::string &payload) const {
    std::map<std::string, std::vector<mqtt_parse_result>> result{};
    for (const auto &device : m_document->devices) {
        for (const auto &topic : device.topics) {
            if (topic.name == msg) {
                for (auto &measurement : topic.measurements) {
                    for (auto &field : measurement.fields) {

                        if (!stringhelper::is_empty_or_whitespace(field.json_field)) {
                            auto json = nlohmann::json::parse(payload);
                            auto fields = stringhelper::split(field.json_field, '.');
                            auto jsonValue = json;
                            bool found = true;
                            for (const auto &field : fields){
                                if (jsonValue.contains(field)) {
                                    jsonValue = jsonValue[field];
                                } else {
                                    found = false;
                                }
                            }
                            if (found) {
                                std::string value = "";
                                if (jsonValue.is_number_float()){
                                    value = std::to_string(jsonValue.get<float>());
                                } else if (jsonValue.is_number_integer()){
                                    value = std::to_string(jsonValue.get<int>());
                                } else {
                                    value = jsonValue.get<std::string>();
                                }
                                auto[success, data] = create_mqtt_parse_result(measurement, field, value);
                                if (success) {
                                    result[data.measurement].push_back(data);
                                }
                            }
                        } else {
                            auto[success, data] =create_mqtt_parse_result(measurement, field, payload);
                            if (success) {
                                result[data.measurement].push_back(data);
                            }
                        }
                    }
                }
            }
        }
    }
    return result;
}

std::tuple<bool, mqtt_parse_result>
mqttmessageparser::create_mqtt_parse_result(const measurement &measurement, const field &field,
                                            const std::string &mqtt_payload) const {
    mqtt_parse_result data{};
    if (stringhelper::is_equal(field.match, mqtt_payload, !field.ignore_case)) {
        data.measurement = measurement.name;
        if (field.value == "") {
            data.value = mqtt_payload;
        } else {
            data.value = field.value;
        }
        data.data_type = field.data_type;
        data.dbfield = field.name;
        return std::make_tuple(true, data);
    }
    return std::make_tuple(false, data);
}


