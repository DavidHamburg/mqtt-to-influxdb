#include <cxxopts.hpp>
#include <spdlog/spdlog.h>
#include <app/mqtt/mqttlistenerfactory.hpp>

#ifdef __cpp_lib_filesystem
    #include <filesystem>
    namespace fs = std::filesystem;
#else
    #include <experimental/filesystem>
    namespace fs = std::experimental::filesystem;
#endif
#include <thread>
#include <libmqtt-to-influxdb/config/configuration.hpp>
#include <libmqtt-to-influxdb/mqttmessageparser.hpp>
#include <libmqtt-to-influxdb/database/iotrepository.hpp>

int main(int argc, char **argv) {
    spdlog::set_level(spdlog::level::info);
    cxxopts::Options options("mqtt-to-influxdb",
                             "mqtt-to-influxdb is a MQTT message parser for writing statistics into InfluxDB.");

    options.add_options()
            ("c,config", "configuration file", cxxopts::value<std::string>())
            ("h,help", "Print usage");
    try {
        auto result = options.parse(argc, argv);

        if (result.arguments().empty() || result.count("help")) {
            std::cout << options.help() << std::endl;
            return 0;
        }
        if (result.count("config") == 0 || result["config"].as<std::string>() == "") {
            std::cout << options.help() << std::endl;
            std::cout << "Missing required configuration file." << std::endl;
            return -1;
        }
        auto config_filename = result["config"].as<std::string>();

        if (!fs::exists(config_filename)) {
            std::cout << "Configuration file does not exist." << std::endl;
            return -1;
        }

        configuration config{};
        auto [success, document] = config.load_file(config_filename);
        if (!success){
            std::cout << "Configuration is not valid." << std::endl;
            return -2;
        }

        auto &con = document->connection;
        iot_repository repo{con.influxdb_user, con.influxdb_password, con.influxdb_host, con.influxdb_port, con.influxdb_protocol, con.influxdb_database};
        mqttmessageparser parser{document};
        mqttlistenerfactory listener_factory{};
        auto listener = listener_factory.create(con.broker_ip, con.broker_port, "mqtt-to-influxdb", "#");
        listener->subscribe([&](mqtt::const_message_ptr msg){

            auto topic = msg->get_topic();
            auto payload = msg->get_payload_str();
            auto data = parser.parse(topic, payload);
            for(const auto &d : data) {
                std::cout << "Measurement:    " << d.first << std::endl;
                for(const auto &field : d.second){
                    std::cout << "Database field: " << field.dbfield << std::endl;
                    std::cout << "Value:          " << field.value << std::endl;
                    std::cout << "Data-type:      " << field.data_type << std::endl;
                    std::cout << "---" << std::endl;
                }

                std::cout << "-----------" << std::endl;
                repo.write_mqtt_result(d.first, d.second);
            }
        });

        while (true) {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }

    } catch (cxxopts::OptionParseException &e) {
        std::cout << options.help() << std::endl;
        std::cout << e.what() << std::endl;
        return -1;
    }
    return 0;
}
