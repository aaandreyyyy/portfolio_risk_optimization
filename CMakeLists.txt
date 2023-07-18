cmake_minimum_required(VERSION 3.25)
project(risk_optimization)


find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 17)

add_executable(risk_optimization main.cpp StockData.cpp StockData.h CmdParser.cpp CmdParser.h StockRiskAnalysis.cpp StockRiskAnalysis.h PortfolioData.cpp PortfolioData.h App.cpp App.h)

target_link_libraries(risk_optimization ${CURL_LIBRARIES})
