/*
 * Divide positive or negative dividend by positive divisor and round
 * to closest integer. Result is undefined for negative divisors and
 * for negative dividends if the divisor variable type is unsigned.
 */
#define DIV_ROUND(numerator, denominator)    \
(                                            \
    {                                        \
        typeof(numerator) n = numerator;     \
        typeof(denominator) d = denominator; \
        (typeof(numerator))-1 > 0 ||         \
        (typeof(denominator))-1 > 0 ||       \
        n>0 ? (n + d/2)/d : (n - d/2)/d;     \
    }                                        \
)

