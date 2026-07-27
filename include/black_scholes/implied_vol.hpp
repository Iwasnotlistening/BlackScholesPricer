#pragma once
#include "pricing.hpp"
#include <optional>

namespace black_scholes{
    struct IVResult {
        double implied_vol;
        int iterations;
        bool converged;
    };

    std::optional<IVResult> implied_volatility_newton(
        const OptionParams& p,
        double market_proce,
        double initial_guess,
        double tolerance = 1e-8,
        int max_iter = 100
    );
}