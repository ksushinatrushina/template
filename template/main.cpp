#include <iostream>

template<typename FirstT, typename ... ArgsT>
auto summ(FirstT first, ArgsT ... args) {
    return first + (args + ...);
}

template<typename FirstT, typename ... ArgsT>
auto substr(FirstT first, ArgsT ... args) {
    return first - (args - ...);
}

template<typename FirstT, typename ... ArgsT>
auto mult(FirstT first, ArgsT ... args) {
    return first * (args * ...);
}

template<typename FirstT, typename ... ArgsT>
auto divid(FirstT first, ArgsT ... args) {
    return first / (args / ...);
}

template<int N>
struct Factorial {
    static const int value = N * Factorial<N - 1>::value;
};

template<>
struct Factorial<0> {
    static const int value = 1;
};

int main() {
    int a{12};
    double b{10.8};

    auto result = summ(a, b, 10, 23);
    std::cout << "Result of adding: " << result << std::endl;

    auto result2 = substr(a, b, 3, 156);
    std::cout << "Result of subtracting: " << result2 << std::endl;

    auto result3 = mult(a, b, 6, 3, 8);
    std::cout << "Result of multiplication: " << result3 << std::endl;

    auto result4 = divid(a, b, 1);
    std::cout << "Result of dividing: " << result4 << std::endl;

    const int n = 5;
    std::cout << "Factorial of " << n << " is: " << Factorial<n>::value << std::endl;
    return 0;
}
