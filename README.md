# BlackScholesPricer
Implement Pricing algorithms commonly used for trading using C++. Currently prices European options, mathematically described below.
## The code
```bash
mkdir build 
cd build
cmake ..
make
./black_scholes     # Run examples
./run_tests         # Run tests
```
## The Maths
### Derivation of Black-Scholes
The Black-Scholes pricing says the following. 
The price of the underlying asset, $S$ follows a geometric Brownian motion with volatility $\sigma$ $$dS = \mu S dt + \sigma S dW_t$$
Given this, we can derive that the price of an European call option is $$C = S_0 N(d_1) - K e^{-rT}N(d_2)$$
For the price of an European put option, $$P = K e^{-rT}N(-d_2)-S_0N(-d_1)$$
where $d_1 = \frac{\ln\left(S\over K\right) + \left(r + \sigma^2 \over 2\right)T}{\sigma \sqrt{T}}$, and $d_2 = d_1 - \sigma \sqrt{T}$

### Greeks
Derivative products naturally come with risk. In our pricing model, this can stem from various sources. To assess risk, we compute the sensitivity of the price to the market parameters. These sensitivities are commonly referred to as __the Greeks__. The most commonly used Greeks are _Delta_ - $\Delta$, _Gamma_ - $\Gamma$, _Vega_ - $v$, _Theta_ - $\Theta$ and _Rho_ - $\rho$. These are the ones we will be calculating in this pricer. There are some additional Greeks like _Volga_, _Vanna_, _Charm_, and _Speed_ which we will be leaving out for now.

##### Delta $\Delta$
This measures the rate of change of the theoretical option value with respect to the changes in the underlying asset price. 
$$\Delta = \frac{\partial V}{\partial S} = \begin{cases} N(d_1) && \text{Call Option} \\ N(d_1) - 1 && \text{Put Option} \end{cases}$$
Intuitively, the option will (instantaneously) behave like the number of shares indicated by the delta, as a percentage of the total underlying shares.

##### Gamma $\Gamma$
Measures the rate of change in the $\Delta$ with respect to the changes in the underlying price. $$\Gamma = \frac{\partial \Delta}{\partial\S} = \frac{\partial^2 V}{\partial S^2} = \frac{N'(d_1)}{S \sigma \sqrt T}$$
$\Gamma$ is important because it corrects for the convexity of value. Typically, when a trader seeks a $\Delta$-hedge, they may also seek to $\Gamma$-hedge to ensure that the hedge will be effective over a wider range of underlying price movements.

##### Vega $v$
Measures sensitivity to volatility.
$$v = \frac{\partial V}{\partial \sigma} = S \cdot N'(d_1) \cdot \sqrt T$$
This is typically expressed as the amount of money per underlying asset that the option's value will gain or lose as volaitlity rises or falls. 

##### Theta $\Theta$
Measures the sensitivity of the value of the derivative to the passage of time. I.e. the "Time decay"
$$\Theta = -\frac{\partial V}{\partial t} = \begin{cases} -\frac{SN'(d_1)\sigma}{2 \sqrt T} - r K e^{-rT}N(d_2) && \text{Call Option} \\ -\frac{S N'(d_1)\sigma}{2 \sqrt T} + r K e^{-rT}N(-d_2) && \text{Put Option} \end{cases}$$
Typically, an option loses value with time. By convention, $t$ is defined in years. However, $\Theta$ is usually seen in terms of change in number of days to the expiry.

##### Rho $\rho$
Measures sensitivity to the interest rate. 
$$\rho = \frac{\partial V}{\partial r} = \begin{cases} KTe^{-rT}N(d_2) && \text{Call Option} \\ -KTe^{-rT}N(-d_2) && \text{Put Option} \end{cases}$$
This is typically expressed as the amnount of money, per share of the underlying, that the value of the option will gain or lose as the rigk-free interest rate rises or galls by 1.0% p.a. This is usually the least used of the first-order Greeks.

### Implied Volatility
In practice, given a market price $C$ for an option priced using Black-Scholes, calculating the implied volatility parameter $\sigma$ is very useful. We use the Newton-Raphson method, which tends to be the industry standard.
$$\sigma_{n+1} = \sigma_n - \frac{V(\sigma_n) - C}{v(\sigma_n)}$$
where $V(\sigma_n)$ is the Black-scholes price at volatility $\sigma_n$ and $v(\sigma_n)$ is the Greek Vega.
With an initial guess of $$ \sigma_0 = \sqrt{\frac{2 \pi C}{T \cdot S}} $$
where $T$ is the strike period, $S$ is the price of the underlying stock. This tends to be close to the true value when an option is at the market, and deviates further as the stock moves further in/out the market.

### Error Function and Normal Distribution
We use the error function to numerically calculate the values for the cumulative normal distribution to machine precision. C++ gives us the function using `cmath`, `double erf (double x)` as well as the complementary error function `double erfc (double x)`. The error function and the complemetary error function are defined as $$\text{erf}(x) = \frac{2}{\sqrt{\pi}}\int_0^x e^{-t^2}dt, \quad \text{erfc}(x) = 1 - \text{erf}(x)$$
This is extremely reminiscent of the cumulative standard normal distribution, which is defined as $$\Phi(x) = \frac{1}{\sqrt{2\pi}}\int_{-\infty}^{x} e^{-\frac{t^2}{2}}dt$$
In fact, with a change of variables, we get the following relationships:
$$\Phi(x) = \frac{1 + \text{erf}\left(\frac{x}{\sqrt{2}}\right)}{2}, \quad \Phi(x) = \frac{\text{erfc}\left(-\frac{x}{\sqrt{2}}\right)}{2}$$