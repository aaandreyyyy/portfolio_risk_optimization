# Portfolio risk optimization
This risk optimization uses Markowitz's model. \
The formula to evaluate the portion of wealth to allocate to each asset is:

$\pi _i = \frac{C^{-1} * E_i}{E_i^{-1} * C^{-1} * E}$ \
Where:
- $\pi _i$ - portion of wealth invested in i-th stock
- $C$ - covariance matrix of stocks' revenue
- $E_i$ - expected value of i-th stock

Params:
input_file output_file period_size number_of_periods \
period_size can be: DAY, WEEK, MONTH, THREE_MONTH
