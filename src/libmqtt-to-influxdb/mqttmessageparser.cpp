#include <libmqtt-to-influxdb/mqttmessageparser.hpp>
#include <libmqtt-to-influxdb/stringhelper.hpp>

mqttmessageparser::mqttmessageparser(const std::shared_ptr<document> &document) : m_document(document) { }

std::vector<mqtt_parse_result> mqttmessageparser::parse(const std::string &msg, const std::string &payload) const {
    std::vector<mqtt_parse_result> result{};
    for(const auto &device : m_document->devices) {
        for (const auto &topic : device.topics) {
            if (topic.name == msg) {
                for (auto &measurement : topic.measurements) {
                    for (auto &field : measurement.fields) {
                        if (stringhelper::is_equal(field.payload, payload, !field.ignore_case)) {
                            auto &data = result.emplace_back();
                            data.measurement = measurement.name;
                            if (field.value == "") {
                                data.value = field.payload;
                            } else{
                                data.value = field.value;
                            }
                            data.data_type = field.data_type;
                            data.dbfield = field.name;
                        }
                    }
                }
            }
        }
    }
    return result;
}
