#include <iostream>
#include <vector>
#include <tuple>

template<typename T>
struct is_vector : std::false_type {
};

template<typename VectorValueT>
struct is_vector<std::vector<VectorValueT> > : std::true_type {
};

template<typename T>
inline constexpr bool is_vector_v = is_vector<T>::value;

template<typename T>
struct is_tuple : std::false_type {
};

template<typename... Args>
struct is_tuple<std::tuple<Args...>> : std::true_type {
};

template<typename T>
inline constexpr bool is_tuple_v = is_tuple<T>::value;

template<std::size_t... Is, typename... Ts>
void apply_subtraction(
        std::tuple<Ts...> &result, std::tuple<Ts...> &first, std::tuple<Ts...> &second,
        std::index_sequence<Is...>
) {
    result = std::make_tuple(std::get<Is>(first) - std::get<Is>(second) ...);
}

template<typename... Ts>
void apply_subtraction(std::tuple<Ts...> &result, std::tuple<Ts...> &first, std::tuple<Ts...> &second) {
    apply_subtraction(result, first, second, std::make_index_sequence<std::tuple_size<std::tuple<Ts...>>::value>{});
}

template<typename FirstT, typename SecondT, typename... RestT>
auto substr(FirstT first, SecondT second, RestT... rest) {
    if constexpr (is_vector_v<FirstT>) {
        using VectorValueT = typename FirstT::value_type;
        std::vector<VectorValueT> result;
        auto min_size = std::min(first.size(), second.size());
        auto max_size = std::max(first.size(), second.size());
        for (size_t i = 0; i < min_size; ++i) {
            result.push_back(first[i] - second[i]);
        }
        if constexpr (std::is_same_v<FirstT, SecondT>) {
            for (size_t i = min_size; i < max_size; ++i) {
                result.push_back(first.size() > second.size() ? first[i] : second[i]);
            }
        } else {
            // If FirstT and SecondT are different types, handle the sizes accordingly.
            if constexpr (sizeof...(RestT) > 0) {
                return substr(result, substr(second, rest...));
            } else {
                return substr(result, second);
            }
        }
        return result;
    } else if constexpr (is_tuple_v<FirstT>) {
        FirstT result;
        apply_subtraction(result, first, second);
        if constexpr (sizeof...(rest) == 0) {
            return result;
        } else {
            return substr(result, rest...);
        }
    } else {
        return first - second;
    }
}

template<std::size_t... Is, typename... Ts>
void apply_multiplication(
        std::tuple<Ts...> &result, std::tuple<Ts...> &first, std::tuple<Ts...> &second,
        std::index_sequence<Is...>
) {
    result = std::make_tuple(std::get<Is>(first) * std::get<Is>(second) ...);
}

template<typename... Ts>
void apply_multiplication(std::tuple<Ts...> &result, std::tuple<Ts...> &first, std::tuple<Ts...> &second) {
    apply_multiplication(result, first, second, std::make_index_sequence<std::tuple_size<std::tuple<Ts...>>::value>{});
}

template<typename FirstT, typename SecondT, typename... RestT>
auto multiply(FirstT first, SecondT second, RestT... rest) {
    if constexpr (is_vector_v<FirstT>) {
        using VectorValueT = typename FirstT::value_type;
        std::vector<VectorValueT> result;
        auto min_size = std::min(first.size(), second.size());
        for (size_t i = 0; i < min_size; ++i) {
            result.push_back(first[i] * second[i]);
        }
        if constexpr (std::is_same_v<FirstT, SecondT>) {
            for (size_t i = min_size; i < first.size(); ++i) {
                result.push_back(first[i]);
            }
        } else {
            // If FirstT and SecondT are different types, handle the sizes accordingly.
            if constexpr (sizeof...(RestT) > 0) {
                return multiply(result, multiply(second, rest...));
            } else {
                return multiply(result, second);
            }
        }
        return result;
    } else if constexpr (is_tuple_v<FirstT>) {
        FirstT result;
        apply_multiplication(result, first, second);
        if constexpr (sizeof...(rest) == 0) {
            return result;
        } else {
            return multiply(result, rest...);
        }
    } else {
        return first * second;
    }
}

template<typename T>
std::vector<T> div(const std::vector<T>& a, const std::vector<T>& b) {
    if (a.size() != b.size()) {
        throw std::runtime_error("Vector sizes do not match!");
    }

    std::vector<T> result;
    result.reserve(a.size());

    for (size_t i = 0; i < a.size(); ++i) {
        if (b[i] == 0) {
            throw std::runtime_error("Division by zero is not allowed!");
        }
        result.push_back(a[i] / b[i]);
    }

    return result;
}

template<typename Tuple, size_t... Indices>
Tuple div_helper(const Tuple& a, const Tuple& b, std::index_sequence<Indices...>) {
    return std::make_tuple(std::get<Indices>(a) / std::get<Indices>(b)...);
}

template<typename... Args>
std::tuple<Args...> div(const std::tuple<Args...>& a, const std::tuple<Args...>& b) {
    static_assert(sizeof...(Args) > 0, "Tuple should not be empty!");

    if constexpr (sizeof...(Args) > 0) {
        return div_helper(a, b, std::index_sequence_for<Args...>());
    }
}

int main() {
    std::vector<int> a{1, 2, 3, 4};
    std::vector<int> b{1, 2};
    std::vector<int> c{1, 3, 5, 7};
    std::tuple<int, int> d{2, 1};

    {
        std::cout << "Result of subtracting №1 (vector): ";
        std::vector<int> res = substr(a, b, c);
        for (const auto &val: res) {
            std::cout << val << ", ";
        }

        int res2 = substr(1, 2, 3, 4);
        std::cout << std::endl;
        std::cout << "Result of subtracting №2 (int): " << res2 << std::endl;

        std::cout << "Result of subtracting №3 (tuple): ";
        auto res3 = substr(d, std::make_tuple(1, 2), std::make_tuple(3, 4));
        std::apply([](const auto &... args) { ((std::cout << args << ", "), ...); }, res3);
        std::cout << std::endl;
    }

    {
        std::cout << std::endl;
        std::cout << "Result of multiplication №1 (vector): ";
        std::vector<int> res = multiply(a, b, c);
        for (const auto &val: res) {
            std::cout << val << ", ";
        }

        int res2 = multiply(1, 2, 3, 4);
        std::cout << std::endl;
        std::cout << "Result of multiplication №2 (int): " << res2 << std::endl;

        std::cout << "Result of multiplication №3 (tuple): ";
        auto res3 = multiply(d, std::make_tuple(1, 2), std::make_tuple(3, 4));
        std::apply([](const auto &... args) { ((std::cout << args << ", "), ...); }, res3);
        std::cout << std::endl;
    }

    try {
        {
            std::cout << "Result of dividing №1 (vector): ";
            std::vector<int> res = div(a, b);
            for (const auto &val: res) {
                std::cout << val << ", ";
            }
            std::cout << std::endl;

            std::cout << "Result of dividing №2 (tuple): ";
            auto res2 = div(d, std::make_tuple(1, 2));
            std::apply([](const auto &... args) { ((std::cout << args << ", "), ...); }, res2);
            std::cout << std::endl;
        }
    } catch (const std::exception& ex) {
        std::cout << "Error: " << ex.what() << std::endl;
    }

    return 0;
}