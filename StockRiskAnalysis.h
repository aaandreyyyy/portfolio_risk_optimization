#ifndef RISK_OPTIMIZATION_STOCKRISKANALYSIS_H
#define RISK_OPTIMIZATION_STOCKRISKANALYSIS_H

#include "PortfolioData.h"

class StockRiskAnalysis : public PortfolioData {
public:
    [[nodiscard]] std::vector<float> CountWealthPortions();
};


#endif //RISK_OPTIMIZATION_STOCKRISKANALYSIS_H
