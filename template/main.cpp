#include <iostream>

template<typename T>
struct is_vector : std::false_type {
};

template<typename VectorValueT>
struct is_vector<std::vector<VectorValueT>>
        : std::true_type {
};

template<typename T>
static const auto is_vector_v = is_vector<T>::value;

template<typename FirstT, typename SecondT>
auto substr(FirstT first, SecondT second) {
    if constexpr (is_vector_v<FirstT>) {
        FirstT result;
        auto &max_vec = first.size() > second.size() ? first : second;
        auto &min_vec = first.size() <= second.size() ? first : second;
        for (size_t i = 0; i < min_vec.size(); ++i) {
            result.push_back(first[i] - second[i]);
        }
        for (size_t i = min_vec.size(); i < max_vec.size(); ++i) {
            result.push_back(max_vec[i]);
        }
        return result;
    } else {
        return first - second;
    }
}

template<typename FirstT, typename SecondT>
auto divide(FirstT first, SecondT second) {
    if constexpr (is_vector_v<FirstT>) {
        FirstT result;
        auto &max_vec = first.size() > second.size() ? first : second;
        auto &min_vec = first.size() <= second.size() ? first : second;
        for (size_t i = 0; i < min_vec.size(); ++i) {
            result.push_back(first[i] / second[i]);
        }
        for (size_t i = min_vec.size(); i < max_vec.size(); ++i) {
            result.push_back(max_vec[i]);
        }
        return result;
    } else {
        return first / second;
    }
}

template<typename FirstT, typename SecondT>
auto mult(FirstT first, SecondT second) {
    if constexpr (is_vector_v<FirstT>) {
        FirstT result;
        auto &max_vec = first.size() > second.size() ? first : second;
        auto &min_vec = first.size() <= second.size() ? first : second;
        for (size_t i = 0; i < min_vec.size(); ++i) {
            result.push_back(first[i] * second[i]);
        }
        for (size_t i = min_vec.size(); i < max_vec.size(); ++i) {
            result.push_back(max_vec[i]);
        }
        return result;
    } else {
        return first * second;
    }
}

int main() {
    std::vector<int> a{1, 2, 3, 4};
    std::vector<int> b{1, 2};

    std::cout << "Result of subtracting №1 (string): ";
    std::vector<int> res = substr(a, b);
    std::copy(res.begin(), res.end(),
              std::ostream_iterator<int>(std::cout, ", "));
    int res1_2 = substr(1, 2);

    std::cout << std::endl;
    std::cout << "Result of subtracting №2 (int): " << res1_2 << std::endl;
    std::cout << std::endl << std::endl;

    std::cout << "Result of dividing №1 (string): ";
    std::vector<int> res2 = divide(a, b);
    std::copy(res2.begin(), res2.end(),
              std::ostream_iterator<int>(std::cout, ", "));
    int res2_2 = divide(1, 2);

    std::cout << std::endl;
    std::cout << "Result of dividing №2 (int): " << res2_2 << std::endl;
    std::cout << std::endl << std::endl;

    std::cout << "Result of multiplication №1 (string): ";
    std::vector<int> res3 = mult(a, b);
    std::copy(res3.begin(), res3.end(),
              std::ostream_iterator<int>(std::cout, ", "));
    int res3_2 = mult(1, 2);

    std::cout << std::endl;
    std::cout << "Result of multiplication №2 (int): " << res3_2 << std::endl;

}