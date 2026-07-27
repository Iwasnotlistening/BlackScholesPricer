#include "black_scholes/implied_vol.hpp"
#include "black_scholes/greeks.hpp"
#include <cmath>
// #include <stdexcept>

namespace black_scholes{
    // If market price < intrinsic price, arbotrage oppurtunity and BS invalid
    static double intrinsic_value(const OptionParams& p) {
        if (p.type == OptionType::Call) {
            return std::fmax(0.0, p.S - p.K * std::exp(-p.r * p.T));
        } else {
            return std::fmax(0.0, p.K * std::exp(-p.r * p.T) - p.S);
        }
    }

    std::optional<IVResult> implied_volatility_newton(
        const OptionParams& p,
        double market_price,
        double initial_guess,
        double tolerance,
        int max_iter
    ) {
        if (market_price <= intrinsic_value(p)) {
            return std::nullopt;
        }

        OptionParams params = p;
        // Setup the initial guess
        params.sigma = std::sqrt(2 * 3.141592653589 * market_price / (p.T * p.S));

        // Iteration for newton-raphson
        for (int i = 0; i < max_iter; i++) {
            double bs_p = price(params);
            double error = bs_p - market_price;

            if (std::abs(error) < tolerance) {
                return IVResult{params.sigma, i+1, true};
            }

            double vega = compute_greeks(params).vega;

            // Caution against alsmot 0 vega
            if (std::abs(vega) < 1e-8) {
                break;
            }

            // Newton Raphson step
            params.sigma -= error / vega;

            // Make sure it is positive
            params.sigma = std::fmax(1e-6, params.sigma);
        }

        return std::nullopt; // In case it does not converge
    }
}