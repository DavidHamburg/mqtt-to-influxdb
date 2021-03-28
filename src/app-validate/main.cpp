#include <cxxopts.hpp>
#include <spdlog/spdlog.h>
#ifdef __cpp_lib_filesystem
    #include <filesystem>
    namespace fs = std::filesystem;
#else
#include <experimental/filesystem>
    namespace fs = std::experimental::filesystem;
#endif
#include <libmqtt-to-influxdb/config/configuration.hpp>
#include <libmqtt-to-influxdb/mqttmessageparser.hpp>
#include <libmqtt-to-influxdb/database/iotrepository.hpp>

int main(int argc, char **argv) {
    spdlog::set_level(spdlog::level::info);
    cxxopts::Options options("mqtt-to-influxdb-validate",
                             "mqtt-to-influxdb is a MQTT message parser for writing statistics into InfluxDB.");

    options.add_options()
            ("t,topic", "example mqtt topic (required)", cxxopts::value<std::string>())
            ("m,message", "example mqtt message (required)", cxxopts::value<std::string>())
            ("c,config", "configuration file", cxxopts::value<std::string>())
            ("s,store", "store passed example mqtt message in InfluxDB")
            ("h,help", "Print usage");
    try {
        auto result = options.parse(argc, argv);

        if (result.arguments().empty() || result.count("help")) {
            std::cout << options.help() << std::endl;
            return 0;
        }
        if (result.count("topic") == 0 || result["topic"].as<std::string>() == "") {
            std::cout << options.help() << std::endl;
            std::cout << "Missing required mqtt topic." << std::endl;
            return -1;
        }
        if (result.count("message") == 0 || result["message"].as<std::string>() == "") {
            std::cout << options.help() << std::endl;
            std::cout << "Missing required mqtt message." << std::endl;
            return -1;
        }
        if (result.count("config") == 0 || result["config"].as<std::string>() == "") {
            std::cout << options.help() << std::endl;
            std::cout << "Missing required configuration file." << std::endl;
            return -1;
        }
        auto topic = result["topic"].as<std::string>();
        auto msg = result["message"].as<std::string>();
        auto config_filename = result["config"].as<std::string>();
        auto store = result["store"].as<bool>();

        if (!fs::exists(config_filename)) {
            std::cout << "Configuration file does not exist." << std::endl;
            return -1;
        }

        configuration config{};
        auto [success, document] = config.load_file(config_filename);
        if (success){
            mqttmessageparser parser{document};
            auto data = parser.parse(topic, msg);

            auto &con = document->connection;
            iot_repository repo{con.influxdb_user, con.influxdb_password, con.influxdb_host, con.influxdb_port, con.influxdb_protocol, con.influxdb_database};

            for(const auto &d : data) {
                std::cout << "Measurement:    " << d.first << std::endl;
                for(const auto &field : d.second){
                    std::cout << "Database field: " << field.dbfield << std::endl;
                    std::cout << "Value:          " << field.value << std::endl;
                    std::cout << "Data-type:      " << field.data_type << std::endl;
                    std::cout << "---" << std::endl;
                }

                std::cout << "-----------" << std::endl;
                if (store) {
                    repo.write_mqtt_result(d.first, d.second);
                }
            }
        }
        else{
            std::cout << "Configuration is not valid." << std::endl;
        }

    } catch (cxxopts::OptionParseException &e) {
        std::cout << options.help() << std::endl;
        std::cout << e.what() << std::endl;
        return -1;
    }
    return 0;
}
