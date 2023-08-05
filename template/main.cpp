#include <iostream>
#include <fstream>
#include <string>
#include <vector>

bool match(const std::string &ip, const std::string &mask,
    std::vector<std::string>& ip_parts,
    std::vector<std::string>& mask_parts) {

    // разбиваем IP-адрес и маску на отдельные элементы
    size_t startPosition = 0, endPosition = 0;
    while ((endPosition = ip.find('.', startPosition)) != std::string::npos) { // цикл ищет символ точки и возвращает позицию найденной точки, либо через npose, что ее не найдено
        ip_parts.push_back(ip.substr(startPosition, endPosition - startPosition)); // когда мы нашли точку, мы забираем все, что было между точками через функцию substr и записываем в ip_parts
        startPosition = endPosition + 1;
    }
    ip_parts.push_back(ip.substr(startPosition));

    startPosition = 0; // то же самое для маски
    while ((endPosition = mask.find('.', startPosition)) != std::string::npos) {
        mask_parts.push_back(mask.substr(startPosition, endPosition - startPosition));
        startPosition = endPosition + 1;
    }
    mask_parts.push_back(mask.substr(startPosition));

// переходим к сравнению маски и IP через цикл for
    for (size_t i = 0; i < 4; ++i) {
        if (mask_parts[i] != "*" && mask_parts[i] != ip_parts[i]) {
            return false;
        }
    }
    return true; }


int main() {

    // запрос у пользователя ввода маски
    std::cout << "Enter the IP address mask in the format *.*.*.*: ";
    std::string inputMask;
    std::cin >> inputMask;

    // открываем файл с ip
    std::ifstream inputFile("/Users/kseniatrusina/template/template/ipNumbers.txt");
    if (!inputFile) {
        std::cerr << "File can't be opened" << std::endl;
        return 1;
    }

    std::string line;
    bool found = false;

    //векторы для хранения IP и маски
    std::vector<std::string> ip_parts;
    std::vector<std::string> mask_parts;

    // считываем IP из файла и ищем совпадения
    while (std::getline (inputFile, line)) { // обнуляем startPosition перед каждым новым IP-адресом
        size_t startPosition = 0;
        mask_parts.clear(); // очищаем части маски перед каждым новым IP-адресом

        // разбиваем маску из файла на отдельные элементы
        size_t endPosition = 0;
        while ((endPosition = line.find('.', startPosition)) != std::string::npos) {
            mask_parts.push_back(line.substr(startPosition, endPosition - startPosition));
            startPosition = endPosition + 1;
        }
        mask_parts.push_back(line.substr(startPosition));

        // вызываем функцию match, передаем векторы с элементами IP-адреса
        if (match(line, inputMask, ip_parts, mask_parts)) {
            std::cout << "The following addresses were found: " << line << std::endl;
            found = true;
        }
    }

    inputFile.close();

    // если совпадений нет
    if (!found) {
        std::cout << "IP-addresse can't be found" << std::endl;
    }
    return 0;
}





//#include <iostream>
//#include <fstream>
//#include <vector>
//#include <array>
//#include <variant>
//#include <string>
//#include <sstream>
//
//using ip_v4_type = std::array<uint8_t, 4>; // создание псевдонима
//
//using ip_v4_mask_element = std::variant<std::uint8_t, char>;
//using ip_v4_mask = std::array<ip_v4_mask_element, 4>;
//
//bool match(const ip_v4_type &ip, const ip_v4_mask &mask) {
//    for (size_t i = 0; i < 4; i++) {
//        auto &mask_element = mask[i];
//        if (std::holds_alternative<char>(mask_element)) { // ищем тип чар в элементах маски
//            continue;
//        }
//        if (std::get<std::uint8_t>(mask_element) != ip[i]) {
//            return false;
//        }
//    }
//    return true;
//}
//
//int main() {
//
//    ip_v4_mask mask;
//
//    // запрос у пользователя ввода маски
//    std::cout << "Enter the IP address mask in the format *.*.*.*: ";
//    std::string inputMask;
//    std::getline(std::cin, inputMask);
//
//    std::stringstream ss(inputMask);
//    char divider; // ищем число в маске
//    for (size_t i = 0; i < 4; ++i) {
//        std::string part;
//        ss >> part;
//        if (part == "*") {
//            mask[i] = '*';
//        }
//        else {
//            mask[i] = static_cast<std::uint8_t>(std::stoi(part)); // stoi преобразует последовательность чисел из part в целое число
//        }
//        ss >> divider;
//    }
//
//    // открываем файл с ip
//    std::ifstream inputFile("ipNumbers.txt");
//    if (!inputFile) {
//        std::cerr << "File can't be opened" << std::endl;
//        return 1;
//    }
//
//    std::vector<ip_v4_type> matchIP;
//
//    std::string line;
//    while (std::getline(inputFile, line)) { // вычисляем все строчки в открытом файле
//        std::stringstream ss(line);
//        ip_v4_type ip;
//
//        for (size_t i = 0; i < 4; ++i) {
//            int num;
//            char dot;
//            ss >> num;
//            ss >> dot;
//            ip[i] = static_cast<uint8_t>(num);
//        }
//
//        if (match(ip, mask)) {
//            matchIP.push_back(ip);
//        }
//    }
//
//    inputFile.close();
//
//    std::cout << "The following addresses were found: " << std::endl;
//    for (const auto &ip: matchIP) {
//        std::cout << static_cast<int>(ip[0]) << '.' << static_cast<int>(ip[1]) << '.'
//                  << static_cast<int>(ip[2]) << '.' << static_cast<int>(ip[3]) << std::endl;
//    }
//    return 0;
//    }
//
//    // подготавливаем маску
//
//    std::variant<std::uint8_t, char> v = std::uint8_t{255};
//    v = '*';
//
//    if (auto *u8 = std::get_if<std::uint8_t>(&v)) {
//        // process uin8_t
//    } else if (auto *ch = std::get_if<char>(&v)) {
//        // process char
//    } else {
//        std::terminate();
//    }
//
//
//    std::array<std::uint8_t, 4>
//
//    // узнаём от пользователя маску ip, который ему нужен
//
//    // 2.1.2.1
//    // 255.*.*.*
//    // *.*.1.*
//
//    // делаем поиск по этой маске среди считанных ip
//
//    // если такие ip есть выводим их в консоль
//    // если их нет, то выыводим ах и увы.