#include "black_scholes/pricing.hpp"
#include <cmath>
// #include <stdexcept>

namespace black_scholes {
    double norm_cdf(double x) {
        // Use the error function to compute the cumulative distribution function
        return 0.5 * std::erfc(-x / std::sqrt(2));
    }

    double norm_pdf(double x) {
        // Probability density function for the standard normal
        constexpr double inv_sqrt_2pi = 0.3989422804014337; // 1/sqrt(2*pi)
        return inv_sqrt_2pi * std::exp(-0.5 * x * x);
    }

    BSTerms compute_d1_d2(const OptionParams& p) {
        p.validate();
        // Pricing assuming t = 0
        double d1 = (std::log(p.S/p.K) + (p.r + 0.5 * p.sigma * p.sigma) * p.T)
                    / (p.sigma * std::sqrt(p.T));
        double d2 = d1 - p.sigma * std::sqrt(p.T);
        return {d1, d2};
    }

    double price(const OptionParams& p) {
        auto [d1, d2] = compute_d1_d2(p);
        double discount = std::exp(-p.r * p.T);

        if (p.type == OptionType::Call) {
            return p.S * norm_cdf(d1) - p.K * discount * norm_cdf(d2);
        } else {
            return p.K * discount * norm_cdf(-d2) - p.S * norm_cdf(-d1);
        }
    }
}