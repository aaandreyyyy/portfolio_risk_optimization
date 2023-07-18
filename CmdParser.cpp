#include <filesystem>
#include <iostream>
#include "CmdParser.h"

void PrintHelp() {
    std::cout << "Params:\nfile_in file_out period number_of_periods\nPeriod can be: DAY, WEEK, MONTH, THREE_MONTH"
              << std::endl;
}

bool hasEnding(std::string const &fullString, std::string const &ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
    }
    return false;
}

PeriodType StrToPeriod(const std::string &s) {
    if (s == "DAY") {
        return PeriodType::DAY;
    } else if (s == "WEEK") {
        return PeriodType::WEEK;
    } else if (s == "MONTH") {
        return PeriodType::MONTH;
    }
    return PeriodType::THREE_MONTH;
}

bool checkFile(const std::string &file_name) {
    return (hasEnding(file_name, ".txt") || hasEnding(file_name, ".csv")) && std::filesystem::exists(file_name);
}

bool checkPeriod(const std::string &period_name) {
    return period_name == "DAY" || period_name == "WEEK" || period_name == "MONTH" || period_name == "THREE_MONTH";
}

bool is_number(const std::string &s) {
    return !s.empty() && std::find_if(s.begin(),
                                      s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}

bool checkPeriodCount(const std::string &period_count_name) {
    return is_number(period_count_name) && std::stoi(period_count_name) > 0;
}


bool CmdParser::Parse(int argc, char *argv[]) {
    if (argc != PARAM_COUNT) {
        PrintHelp();
        return false;
    }

    if (checkFile(argv[1])) {
        file_in = argv[1];
    } else {
        return false;
    }

    if (hasEnding(file_in, "txt")) {
        file_type = "txt";
    } else if (hasEnding(file_in, "csv")) {
        file_type = "csv";
    }

    if (checkFile(argv[2])) {
        file_out = argv[2];
    } else {
        return false;
    }

    if (checkPeriod(argv[3])) {
        period_type = StrToPeriod(argv[3]);
    } else {
        return false;
    }

    if (checkPeriodCount(argv[4])) {
        period_count = std::stoi(argv[4]);
    } else {
        return false;
    }

    return true;
}

std::string CmdParser::GetFileIn() const {
    return file_in;
}

std::string CmdParser::GetFileOut() const {
    return file_out;
}

PeriodType CmdParser::GetPeriodType() const {
    return period_type;
}

int CmdParser::GetPeriodCount() const {
    return period_count;
}

std::string CmdParser::GetFileType() const {
    return file_type;
}
