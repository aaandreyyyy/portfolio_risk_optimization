#ifndef RISK_OPTIMIZATION_STOCKDATA_H
#define RISK_OPTIMIZATION_STOCKDATA_H

#include <chrono>
#include <curl/curl.h>
#include <string>
#include <vector>

struct StockPeriodData {
    int64_t date;
    float open;
    float high;
    float low;
    float close;
    float adj_close;
    uint64_t volume;
    float revenue;
};

enum PeriodType {
    DAY,
    WEEK,
    MONTH,
    THREE_MONTH,
};

class StockData {
public:
    StockData(std::string stock_ticker, PeriodType period_type_, int periods_);

    ~StockData();

    [[nodiscard]] float ExpectedValue() const;

    friend float CalculateCov(const StockData &stock_a, const StockData &stock_b);

    bool IsAbleToLoadEnough() const;
private:
    int64_t GetPeriodFromDateDMY(std::string &date) const;

    int64_t GetPeriodFromDateYMD(std::string &date) const;

    void ParseHistoricalData(std::string &response);

    PeriodType period_type;
    CURL *curl;
    std::vector<StockPeriodData> stock_data;
    int periods;
};


#endif //RISK_OPTIMIZATION_STOCKDATA_H
