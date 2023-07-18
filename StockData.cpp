#include "StockData.h"
#include <sstream>
#include <iomanip>
#include <iostream>

size_t WriteCallback(char *content, size_t size, size_t nmemb, std::string *str) {
    size_t total_size = size * nmemb;
    str->append(content, total_size);
    return total_size;
}

std::string GetCurrentDate() {
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%d-%m-%Y");

    return ss.str();
}

std::string CalcPastDate(const std::string &currentDate, PeriodType periodType, int periods) {
    std::tm time = {};
    std::stringstream ss(currentDate);
    ss >> std::get_time(&time, "%d-%m-%Y");

    switch (periodType) {
        case DAY:
            time.tm_mday -= periods;
            break;
        case WEEK:
            time.tm_mday -= (7 * periods);
            break;
        case MONTH:
            time.tm_mon -= periods;
            break;
        case THREE_MONTH:
            time.tm_mon -= (3 * periods);
            break;
        default:
            break;
    }

    std::mktime(&time);

    std::ostringstream oss;
    oss << std::put_time(&time, "%d-%m-%Y");

    return oss.str();
}

std::pair<std::string, std::string> CalcStartAndEndDate(PeriodType period_type, int periods) {
    std::string currentDate = GetCurrentDate();
    std::string pastDate = CalcPastDate(currentDate, period_type, periods);

    return std::make_pair(currentDate, pastDate);
}


StockData::StockData(std::string stock_ticker, PeriodType period_type_, int periods_) : periods(periods_) {
    period_type = period_type_;

    auto [end_date, start_date] = CalcStartAndEndDate(period_type, periods);

    std::string url;
    std::string period1 = std::to_string(GetPeriodFromDateDMY(start_date));
    std::string period2 = std::to_string(GetPeriodFromDateDMY(end_date));
    std::string interval;
    switch (period_type) {
        case DAY:
            interval = "1d";
            break;
        case WEEK:
            interval = "1wk";
            break;
        case MONTH:
            interval = "1mo";
            break;
        case THREE_MONTH:
            interval = "3mo";
            break;
    }
    url = "https://query1.finance.yahoo.com/v7/finance/download/" + stock_ticker +
          "?period1=" + period1 +
          "&period2=" + period2 +
          "&interval=" + interval + "&events=history";

    curl_global_init(CURL_GLOBAL_ALL);

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        std::string response;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        CURLcode res = curl_easy_perform(curl);

        if (res == CURLE_OK) {
            ParseHistoricalData(response);
        } else {
            std::cerr << "Error connecting to finance.yahoo.com\n";
        }

        curl_easy_cleanup(curl);
    }
}

int64_t StockData::GetPeriodFromDateDMY(std::string &date) const {
    std::tm date_tm = {};
    std::istringstream date_ss(date);
    date_ss >> std::get_time(&date_tm, "%d-%m-%Y");

    auto dateTp = std::chrono::system_clock::from_time_t(std::mktime(&date_tm));

    auto period = std::chrono::duration_cast<std::chrono::seconds>(dateTp.time_since_epoch()).count();
    return period;
}

StockData::~StockData() {
    curl_global_cleanup();
}

std::vector<std::string> split(std::string s, std::string delimiter) { // TODO copying string cringe
    size_t pos_start = 0;
    size_t pos_end;
    size_t delim_len = delimiter.length();

    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }

    res.push_back(s.substr(pos_start));
    return res;
}

void StockData::ParseHistoricalData(std::string &response) {
    std::vector<std::string> split_by_nl = split(response, "\n");

    for (size_t i = 1; i < split_by_nl.size(); ++i) {
        std::string day_data_raw = split_by_nl[i];
        std::vector<std::string> day_data = split(day_data_raw, ",");

        int64_t date = GetPeriodFromDateYMD(day_data[0]);
        float open = std::stof(day_data[1]);
        float high = std::stof(day_data[2]);
        float low = std::stof(day_data[3]);
        float close = std::stof(day_data[4]);
        float adj_close = std::stof(day_data[5]);
        uint64_t volume = std::stoull(day_data[6]);
        float revenue = close - open;

        stock_data.push_back({date, open, high, low, close, adj_close, volume, revenue});
    }
}

int64_t StockData::GetPeriodFromDateYMD(std::string &date) const {
    std::stringstream ss(date);
    std::string year, month, day;
    std::getline(ss, year, '-');
    std::getline(ss, month, '-');
    std::getline(ss, day);
    std::string res = day + "-" + month + "-" + year;
    return GetPeriodFromDateDMY(res);
}

float StockData::ExpectedValue() const {
    float E = 0;
    for (auto period: stock_data) {
        E += period.revenue;
    }
    E /= static_cast<float>(stock_data.size());
    return E;
}

float CalculateCov(const StockData &stock_a, const StockData &stock_b) {
    if (stock_a.stock_data.size() != stock_b.stock_data.size()) {
        // TODO: do smth about it
    }
    float meanA = 0.0;
    float meanB = 0.0;
    for (size_t i = 0; i < stock_a.stock_data.size(); ++i) {
        meanA += stock_a.stock_data[i].revenue;
        meanB += stock_b.stock_data[i].revenue;
    }

    meanA /= static_cast<float>(stock_a.stock_data.size());
    meanB /= static_cast<float>(stock_b.stock_data.size());

    float covariance = 0.0;
    for (size_t i = 0; i < stock_a.stock_data.size(); ++i) {
        covariance += (stock_a.stock_data[i].revenue - meanA) * (stock_b.stock_data[i].revenue - meanB);
    }

    covariance /= static_cast<float>(stock_a.stock_data.size());
    return covariance;
}

bool StockData::IsAbleToLoadEnough() const {
    return stock_data.size() == periods;
}
