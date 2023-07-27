
#include <chrono>
#include <cstdint>
#include <iostream>
#include <thread>
#include "src/Pump.hpp"
#include "src/SolarPanel.hpp"
#include "src/StorageTank.hpp"


int main() {
  StorageTank tank(1, 88);
  SolarPanel solarPanel(WaterSpecificHeatCapacity);
  Pump pump(solarPanel, tank);
  pump.turnOn();
  std::this_thread::sleep_for(std::chrono::milliseconds(999U));
  pump.turnOff();
  std::cout << tank.getTemperature() << std::endl;
  return 0;
}