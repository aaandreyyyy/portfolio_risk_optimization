#ifndef RISK_OPTIMIZATION_CMDPARSER_H
#define RISK_OPTIMIZATION_CMDPARSER_H

#include <string>
#include "StockData.h"

class CmdParser {
public:
    static const int PARAM_COUNT = 5;

    bool Parse(int argc, char **argv);

    [[nodiscard]] std::string GetFileIn() const;

    [[nodiscard]] std::string GetFileOut() const;

    [[nodiscard]] PeriodType GetPeriodType() const;

    [[nodiscard]] int GetPeriodCount() const;

    [[nodiscard]] std::string GetFileType() const;

private:
    std::string file_in;
    std::string file_out;
    std::string file_type = "txt";
    PeriodType period_type;
    int period_count;
};


#endif //RISK_OPTIMIZATION_CMDPARSER_H
