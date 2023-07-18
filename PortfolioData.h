#ifndef RISK_OPTIMIZATION_PORTFOLIODATA_H
#define RISK_OPTIMIZATION_PORTFOLIODATA_H

#include <vector>
#include "StockData.h"

class PortfolioData {
public:
    void AddStock(StockData *stock);

    std::vector<StockData *> stocks;
private:
    PeriodType period;

};


#endif //RISK_OPTIMIZATION_PORTFOLIODATA_H
