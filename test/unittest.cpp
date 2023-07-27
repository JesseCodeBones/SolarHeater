#include <chrono>
#include <gtest/gtest.h>
#include <stdexcept>
#include <thread>
#include "../src/StorageTank.hpp"
#include "../src/SolarPanel.hpp"
#include "../src/Pump.hpp"
TEST(StorageTank, empty_with_temperature) {
  StorageTank tank1(0U, 100);
  ASSERT_EQ(tank1.getTemperature(), 0.0);
  StorageTank tank2(0U, -100.0);
  ASSERT_EQ(tank2.getTemperature(), 0.0);
}

TEST(StorageTank, kg_1_neg_temp) {
  StorageTank tank(1U, -100);
  ASSERT_EQ(tank.getTemperature(), -100.0);
}

TEST(StorageTank, create_overflow) {
  ASSERT_THROW(StorageTank tank(1000U, -100);, std::runtime_error);
}

TEST(StorageTank, heat_one_degree) {
  StorageTank tank(1U, 1);
  tank.receiveHeat(4200U);
  ASSERT_EQ(tank.getTemperature(), 2.0);
  tank.receiveHeat(4200U);
  ASSERT_EQ(tank.getTemperature(), 3.0);
}

TEST(StorageTank, add_overflow) {
  StorageTank tank(1U, -1);
  ASSERT_THROW(tank.addWater(100, 1), std::runtime_error);
}

TEST(StorageTank, add_water) {
  StorageTank tank(1U, -1);
  tank.addWater(1U, 1);
  ASSERT_EQ(tank.getTemperature(), 0.0);
  tank.receiveHeat(4200U);
  ASSERT_EQ(tank.getTemperature(), 0.5);
  tank.addWater(2U, 0);
  ASSERT_EQ(tank.getTemperature(), 0.25);
}

TEST(StorageTank, boiled) {
  StorageTank tank(1U, 99);
  tank.receiveHeat(4200U*2U);
  ASSERT_EQ(tank.getTemperature(), 100);
}

TEST(StorageTank, invalid_temperature) {
  ASSERT_ANY_THROW(StorageTank tank(1U, 199));
}

TEST(SolarPanel, nag) {
  SolarPanel panel(-1);
  ASSERT_EQ(panel.getHeat(), UINT32_MAX);
}

TEST(SolarPanel, change_heat_per_second) {
  SolarPanel panel(-1);
  panel.setHeatPerSecond(4200U);
  ASSERT_EQ(panel.getHeat(), 4200U);
}

TEST(CombineTest, already_boiled) {
  StorageTank tank(1U, 100);
  SolarPanel panel(4200U);
  Pump pump(panel, tank);
  pump.turnOn();
  ASSERT_EQ(tank.getTemperature(), 100.0);
}

TEST(CombineTest, normal) {
  StorageTank tank(1U, 97.0);
  SolarPanel panel(4200U);
  Pump pump(panel, tank);
  pump.turnOn();
  std::this_thread::sleep_for(std::chrono::milliseconds(3200U));
  ASSERT_EQ(tank.getTemperature(), 100.0);
}

TEST(CombineTest, turnon_and_turnoff) {
  StorageTank tank(1U, 97.0);
  SolarPanel panel(4200U);
  Pump pump(panel, tank);
  pump.turnOn();
  pump.turnOff();
  ASSERT_EQ(tank.getTemperature(), 97.0);
}
