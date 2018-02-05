#include "dronecore.h"
#include "integration_test_helper.h"
#include <iostream>

using namespace dronecore;

void print_mode(Telemetry::FlightMode flight_mode);
static Telemetry::FlightMode _flight_mode = Telemetry::FlightMode::UNKNOWN;

TEST_F(SitlTest, TelemetryFlightModes)
{
    DroneCore dc;

    DroneCore::ConnectionResult ret = dc.add_udp_connection();
    ASSERT_EQ(ret, DroneCore::ConnectionResult::SUCCESS);
    std::this_thread::sleep_for(std::chrono::seconds(2));

    Device &device = dc.device();
    device.telemetry().flight_mode_async(
        std::bind(&print_mode, std::placeholders::_1));

    while (!device.telemetry().health_all_ok()) {
        std::cout << "waiting for device to be ready" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    device.action().arm();
    std::this_thread::sleep_for(std::chrono::seconds(2));
    device.action().takeoff();
    std::this_thread::sleep_for(std::chrono::seconds(2));
    ASSERT_EQ(_flight_mode, Telemetry::FlightMode::TAKEOFF);
    device.action().land();
    std::this_thread::sleep_for(std::chrono::seconds(2));
    ASSERT_EQ(_flight_mode, Telemetry::FlightMode::LAND);
}

void print_mode(Telemetry::FlightMode flight_mode)
{
    std::cout << "Got FlightMode: " << Telemetry::flight_mode_str(flight_mode)
              << std::endl;
    _flight_mode = flight_mode;
}
