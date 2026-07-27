#pragma once
#include "pricing.hpp"

namespace black_scholes {
    // Store all the Greeks in one place
    struct Greeks {
        double delta;
        double gamma;
        double vega;
        double theta;
        double rho;
    };

    // Compute all Greeks at once
    Greeks compute_greeks(const OptionParams& p);
}