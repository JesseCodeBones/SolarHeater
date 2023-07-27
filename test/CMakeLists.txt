find_package(GTest REQUIRED)
file(GLOB test_sources CONFIGURE_DEPENDS ./*.cpp)
add_executable(
  solar_heater_test
  ${test_sources}
)

target_link_libraries(
  solar_heater_test
  GTest::gtest_main
)

file(GLOB sources CONFIGURE_DEPENDS ../src/*.cpp )
target_sources(solar_heater_test PUBLIC ${sources})
include(GoogleTest)
gtest_discover_tests(solar_heater_test)

if(CMAKE_COMPILER_IS_GNUCXX)
  LIST(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMakeModules)
  include(CodeCoverage)
  APPEND_COVERAGE_COMPILER_FLAGS()
  setup_target_for_coverage_gcovr_xml(NAME cov
                            EXECUTABLE solar_heater_test
                            DEPENDENCIES solar_heater_test
                            EXCLUDE "${PROJECT_SOURCE_DIR}/test/*"
                            BASE_DIRECTORY "${PROJECT_SOURCE_DIR}")

  setup_target_for_coverage_gcovr_html(NAME cov_html 
                            EXECUTABLE solar_heater_test
                            DEPENDENCIES solar_heater_test
                            EXCLUDE "${PROJECT_SOURCE_DIR}/test/*"
                            BASE_DIRECTORY "${PROJECT_SOURCE_DIR}") 
endif()