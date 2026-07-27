#include "black_scholes/pricing.hpp"
#include "black_scholes/greeks.hpp"
// #include "black_scholes/implied_vol.hpp"
#include <iostream>
#include <iomanip>
#include <string>

// Fixing numspace for brevity since this is a standalone project
using namespace black_scholes;

// Output formatting helper
void print_seperator(char c = '-', int n = 60) {
    std::cout << std::string(n, c) << "\n";
}

// Prints a nicely formatted output
void print_resuts(const OptionParams& p) {
    std::string option_type = (p.type == OptionType::Call) ? "Call" : "Put";

    double option_price = price(p);
    Greeks g = compute_greeks(p);

    print_seperator();
    std::cout << std::fixed << std::setprecision(4);
    std::cout << "  " << option_type << " Option Pricing Results \n";
    print_seperator();
        std::cout << "  Inputs:\n";
    std::cout << "    Spot (S)           : " << p.S     << "\n";
    std::cout << "    Strike (K)         : " << p.K     << "\n";
    std::cout << "    Rate (r)           : " << p.r * 100   << "%\n";
    std::cout << "    Volatility (sigma) : " << p.sigma * 100 << "%\n";
    std::cout << "    Time (T)           : " << p.T     << " years\n\n";
    std::cout << "  Price                : " << option_price << "\n\n";
    std::cout << "  Greeks:\n";
    std::cout << "    Delta              : " << g.delta << "\n";
    std::cout << "    Gamma              : " << g.gamma << "\n";
    std::cout << "    Vega               : " << g.vega / 100.0 << "  (per 1\% vol move)\n";
    std::cout << "    Theta              : " << g.theta << "  (per day)\n";
    std::cout << "    Rho                : " << g.rho / 100.0 << "  (per 1bp rate move)\n";
    print_seperator();
}

int main() {
    std:: cout << "\n Black-Scholes Pricer \n\n";

    // Example 1: At the money call
    OptionParams call_params{
        .S = 100.0,
        .K = 100.0,
        .r = 0.05,
        .sigma = 0.20,
        .T = 1.0,
        .type = OptionType::Call
    };
    print_resuts(call_params);

    //Example 2: Out of the money Put
    OptionParams put_params{
        .S = 100.0,
        .K = 95.0,
        .r = 0.05,
        .sigma = 0.20,
        .T = 0.5,
        .type = OptionType::Put
    };
    print_resuts(put_params);
}