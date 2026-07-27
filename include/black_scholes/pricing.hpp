#pragma once
// #include <cmath>
#include <stdexcept>
// #include <string>

namespace black_scholes {
    // enum to represent the type of option
    enum class OptionType { Call, Put };

    // struct to hold the parameters for option pricing and validate
    struct OptionParams {
        double S;       // Stock price
        double K;       // Strike price
        double r;       // Risk-free interest rate
        double sigma;   // Volatility
        double T;       // Time to expiration
        OptionType type;// Option type (Call or Put)
        
        void validate() const {
            if (S <= 0) throw std::invalid_argument("Stock price must be positive.");
            if (K <= 0) throw std::invalid_argument("Strike price must be positive.");
            if (sigma < 0) throw std::invalid_argument("Volatility cannot be negative.");
            if (T < 0) throw std::invalid_argument("Time to expiration cannot be negative.");
        }
    };

    // struct to hold the computed d1 and d2 values
    struct BSTerms {
        double d1;
        double d2;
    };

    // function to compute d1 and d2 based on the Black-Scholes formula
    BSTerms compute_d1_d2(const OptionParams& p);

    // Normal distribution values
    double norm_cdf(double x);
    double norm_pdf(double x);

    // Price the option
    double price(const OptionParams& p);
}


 // namespace black_scholes