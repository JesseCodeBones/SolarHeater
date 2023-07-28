#include <chrono>
#include <gtest/gtest.h>
#include <stdexcept>
#include <thread>
#include "../src/Pump.hpp"
#include "../src/SolarPanel.hpp"
#include "../src/StorageTank.hpp"
TEST(StorageTank, empty_with_temperature) {
  StorageTank tank1(0U, 100);
  ASSERT_EQ(tank1.getTemperature(), 0.0);
  StorageTank tank2(0U, -100.0);
  ASSERT_EQ(tank2.getTemperature(), 0.0);
}

TEST(StorageTank, kg_1_neg_temp) {
  StorageTank tank(1U, -100, 1U);
  ASSERT_EQ(tank.getTemperature(), -100.0);
}

TEST(StorageTank, create_with_ice) {
  ASSERT_THROW(StorageTank tank(10, 0, 20);, std::runtime_error);
}
TEST(StorageTank, create_with_ice_2) {
  ASSERT_THROW(StorageTank tank(10, 10, 5);, std::runtime_error);
}
TEST(StorageTank, create_with_ice_3) {
  ASSERT_THROW(StorageTank tank(10, -1, 5);, std::runtime_error);
}

TEST(StorageTank, create_overflow) {
  ASSERT_THROW(StorageTank tank(1000U, -100);, std::runtime_error);
}

TEST(StorageTank, create_lower_than_absolute_zero) {
  ASSERT_THROW(StorageTank tank(1000U, AbsoluteZero-1);, std::runtime_error);
}

TEST(StorageTank, heat_one_degree) {
  StorageTank tank(1U, 1);
  tank.receiveHeat(WaterSpecificHeatCapacity);
  ASSERT_EQ(tank.getTemperature(), 2.0);
  tank.receiveHeat(WaterSpecificHeatCapacity);
  ASSERT_EQ(tank.getTemperature(), 3.0);
}

TEST(StorageTank, boiled) {
  StorageTank tank(1U, 99);
  tank.receiveHeat(WaterSpecificHeatCapacity * 2U);
  ASSERT_EQ(tank.getTemperature(), 100);
}

TEST(StorageTank, invalid_temperature) {
  ASSERT_ANY_THROW(StorageTank tank(1U, 199));
}


TEST(StorageTank, fusion_ice) {
  StorageTank tank(2U, 0, 1U);
  tank.receiveHeat(EnthalpyOfFusion);
  ASSERT_EQ(tank.getTemperature(), 0.0);
  ASSERT_EQ(tank.getIce(), 0U);
  ASSERT_EQ(tank.getVolume(), 2U);

  StorageTank tank2(3U, 0, 2U);
  tank2.receiveHeat(EnthalpyOfFusion);
  ASSERT_EQ(tank2.getTemperature(), 0.0);
  ASSERT_EQ(tank2.getIce(), 1U);
  ASSERT_EQ(tank2.getVolume(), 3U);

  StorageTank tank3(2U, 0, 1U);
  tank3.receiveHeat(EnthalpyOfFusion + 2*WaterSpecificHeatCapacity);
  ASSERT_EQ(tank3.getTemperature(), 1.0);
  ASSERT_EQ(tank3.getIce(), 0U);
  ASSERT_EQ(tank3.getVolume(), 2U);

  StorageTank tank4(1U, -1, 1U);
  tank4.receiveHeat(EnthalpyOfFusion + WaterSpecificHeatCapacity + IceSpecificHeatCapacity);
  ASSERT_EQ(tank4.getTemperature(), 1.0);
  ASSERT_EQ(tank4.getIce(), 0U);
  ASSERT_EQ(tank4.getVolume(), 1U);

  StorageTank tank5(3U, 0, 2U);
  tank5.receiveHeat(EnthalpyOfFusion );
  ASSERT_EQ(tank5.getTemperature(), 0.0);
  ASSERT_EQ(tank5.getIce(), 1U);
  ASSERT_EQ(tank5.getVolume(), 3U);

  StorageTank tank6(1U, -2, 1U);
  tank6.receiveHeat(IceSpecificHeatCapacity );
  ASSERT_EQ(tank6.getTemperature(), -1.0);
  ASSERT_EQ(tank6.getIce(), 1U);
  ASSERT_EQ(tank6.getVolume(), 1U);

  StorageTank tank7(0.5, -2, 0.5);
  tank7.receiveHeat(EnthalpyOfFusion/2 +  IceSpecificHeatCapacity + WaterSpecificHeatCapacity);
  ASSERT_EQ(tank7.getTemperature(), 2.0);
  ASSERT_EQ(tank7.getIce(), 0U);
  ASSERT_EQ(tank7.getVolume(), 0.5);
}

TEST(SolarPanel, nag) {
  SolarPanel panel(-1);
  ASSERT_EQ(panel.getHeat(), UINT32_MAX);
}

TEST(SolarPanel, change_heat_per_second) {
  SolarPanel panel(-1);
  panel.setHeatPerSecond(WaterSpecificHeatCapacity);
  ASSERT_EQ(panel.getHeat(), WaterSpecificHeatCapacity);
}

TEST(CombineTest, already_boiled) {
  StorageTank tank(1U, 100);
  SolarPanel panel(WaterSpecificHeatCapacity);
  Pump pump(panel, tank);
  pump.turnOn();
  ASSERT_EQ(tank.getTemperature(), 100.0);
}

TEST(CombineTest, normal) {
  StorageTank tank(1U, 97.0);
  SolarPanel panel(WaterSpecificHeatCapacity);
  Pump pump(panel, tank);
  pump.turnOn();
  std::this_thread::sleep_for(std::chrono::milliseconds(3200U));
  ASSERT_EQ(tank.getTemperature(), 100.0);
}

TEST(CombineTest, turnon_and_turnoff) {
  StorageTank tank(1U, 97.0);
  SolarPanel panel(WaterSpecificHeatCapacity);
  Pump pump(panel, tank);
  pump.turnOn();
  pump.turnOff();
  ASSERT_EQ(tank.getTemperature(), 97.0);
}
