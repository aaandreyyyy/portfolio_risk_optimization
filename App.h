#ifndef RISK_OPTIMIZATION_APP_H
#define RISK_OPTIMIZATION_APP_H

#include "CmdParser.h"
#include "StockRiskAnalysis.h"

class App {
public:
    void Run(const CmdParser &parser);

private:
    StockRiskAnalysis sra_model;

};

#endif //RISK_OPTIMIZATION_APP_H
