#include <fstream>
#include "App.h"
#include <iostream>
#include <sstream>

void App::Run(const CmdParser &parser) {
    if (parser.GetFileType() == "txt") {
        std::ifstream input_file(parser.GetFileIn());
        std::ofstream output_file(parser.GetFileOut());
        std::string line;
        std::vector<std::string> tickers;

        if (!input_file) {
            std::cerr << "Error opening input file (1st param)" << std::endl;
            return;
        }

        if (!output_file) {
            std::cerr << "Error opening output file (2nd param)" << std::endl;
            return;
        }

        while (std::getline(input_file, line)) {
            if (!line.empty()) {
                tickers.push_back(line);
                auto *stock = new StockData(
                        line,
                        parser.GetPeriodType(),
                        parser.GetPeriodCount());
                if (stock->IsAbleToLoadEnough()) {
                    sra_model.AddStock(stock);
                } else {
                    std::cerr << "Not found " << parser.GetPeriodCount() << " periods in " << line << std::endl;
                }
            }
        }

        auto distribution = sra_model.CountWealthPortions();

        for (std::size_t i = 0; i < tickers.size(); ++i) {
            if (distribution[i] > 0) {
                output_file << tickers[i] << ": Long " << distribution[i] << std::endl;
            } else {
                output_file << tickers[i] << ": Short " << -distribution[i] << std::endl;
            }
        }

        input_file.close();
        output_file.close();

    } else if (parser.GetFileType() == "csv") {
        std::ifstream input_file(parser.GetFileIn());
        std::string line;
        std::vector<std::string> tickers;

        if (!input_file) {
            std::cerr << "Error opening input file" << std::endl;
            return;
        }

        while (std::getline(input_file, line)) {
            std::stringstream ss(line);
            std::string value;

            while (std::getline(ss, value, ',')) {
                tickers.push_back(value);
                auto *stock = new StockData(
                        value,
                        parser.GetPeriodType(),
                        parser.GetPeriodCount());
                sra_model.AddStock(stock);
            }
        }

        std::ofstream output_file;
        output_file.open(parser.GetFileOut());

        auto distribution = sra_model.CountWealthPortions();
        output_file << "ticker,long/short,portion" << std::endl;
        for (std::size_t i = 0; i < tickers.size(); ++i) {
            if (distribution[i] > 0) {
                output_file << tickers[i] << ",Long," << distribution[i] << std::endl;
            } else {
                output_file << tickers[i] << ",Short," << -distribution[i] << std::endl;
            }
        }

        input_file.close();
        output_file.close();
    }
}
