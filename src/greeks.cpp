#include "black_scholes/greeks.hpp"
#include <cmath>

namespace black_scholes {
    Greeks compute_greeks (const OptionParams& p) {
        p.validate();
        auto [d1, d2] = compute_d1_d2(p);

        // Some precursors for calculations below
        double sqrt_T = std::sqrt(p.T);
        double discount = std::exp(-p.r * p.T);
        double pdf_d1 = norm_pdf(d1);

        Greeks g{};

        // Delta
        if (p.type == OptionType::Call) {
            g.delta = norm_cdf(d1);
        } else {
            g.delta = norm_cdf(d1) - 1.0;
        }

        // Gamma
        g.gamma = pdf_d1 / (p.S * p.sigma * sqrt_T);

        // Vega
        g.vega = p.S * pdf_d1 * sqrt_T;

        // Theta
        double common_theta = -(p.S * pdf_d1 * p.sigma) / (2.0 * sqrt_T);
        if (p.type == OptionType::Call) {
            g.theta = (common_theta - p.r * p.K * discount * norm_cdf(d2)) / 365.0;
        } else {
            g.theta = (common_theta + p.r * p.K * discount * norm_cdf(-d2)) / 365.0;
        }

        // Rho
        if (p.type == OptionType::Call) {
            g.rho = p.K * p.T * discount * norm_cdf(d2);
        } else {
            g.rho = -p.K * p.T * discount * norm_cdf(-d2);
        }

        return g;
    }
}