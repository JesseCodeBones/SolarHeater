
#include <chrono>
#include <cstdint>
#include <iostream>
#include <thread>
#include "src/Pump.hpp"
#include "src/SolarPanel.hpp"
#include "src/StorageTank.hpp"

int main() {
  StorageTank tank(1, -20, 1);
  SolarPanel solarPanel(WaterSpecificHeatCapacity * 10);
  Pump pump(solarPanel, tank);
  pump.turnOn();
  std::this_thread::sleep_for(std::chrono::milliseconds(20200U));
  pump.turnOff();
  std::cout << tank.getTemperature() << std::endl;
  return 0;
}