#include <iostream>
#include <vector>
#include <tuple>
#include <array>
#include <variant>
#include <fstream>
#include <string>
#include <sstream>
#include <optional>

#define TEST_SUCCEEDED(expr) \
        if (expr)\
        {\
            std::cout << "[TEST SUCCEEDED]: " << #expr << "\n";\
        }\
        else \
        {\
            std::cout << "[TEST FAILED]: " << LINE << "\n";\
            std::terminate();\
        }

namespace bl
{
    using ip_v4_type = std::array<uint8_t, 4>; // создание псевдонима
    using ip_v4_mask_element = std::variant<std::optional<uint8_t>, char>;
    using ip_v4_mask = std::array<ip_v4_mask_element, 4>;

    bool check_mask(const ip_v4_type& ip, const ip_v4_mask& ip_mask)
    {
        for (int i = 0; i < ip.size(); ++i)
        {
            if (const auto& u8 = std::get_if<std::optional<uint8_t>>(&ip_mask[i]))
            {
                if (*u8 && ip[i] != *u8)
                {
                    return false;
                }
            }
            else if (const auto* ch = std::get_if<char>(&ip_mask[i]))
            {
                if (*ch == '*' || *ch == '?')
                {
                    continue;
                }
                else
                {
                    std::cout << "Unknown mask character!" << std::endl;
                    return false;
                }
            }
            else {
                std::terminate();
            }
        }
        return true;
    }

    bool contains_ip(const std::vector<ip_v4_type>& ips, const std::string& inputMask)
    {
        std::stringstream stream{ inputMask };
        ip_v4_mask ip_mask;
        for (int i = 0; i < ip_mask.size(); ++i)
        {
            int ip_part;
            stream >> ip_part;
            if (stream)
            {
                if (ip_part < 0 || ip_part > 255)
                {
                    std::cout << "Invalid mask!" << std::endl;
                    return false;
                }
                ip_mask[i] = (uint8_t)ip_part;
            }
            else
            {
                char mask_character;
                stream.clear();
                stream >> mask_character;
                if (!stream)
                {
                    std::cout << "Invalid mask!" << std::endl;
                    return false;
                }
                ip_mask[i] = mask_character;
            }

            stream.get();
        }

        for (auto& ip : ips)
        {
            if (check_mask(ip, ip_mask))
            {
                return true;
            }
        }

        return false;
    }

    bool read_ips(const std::string& filePath, std::vector<ip_v4_type>& ips)
    {
        std::ifstream istream (filePath);
        if (!istream) {
            std::cerr << "File can't be opened" << std::endl;
            return false;
        }

        while (istream)
        {
            ip_v4_type ip;
            for (int i = 0; i < ip.size(); ++i)
            {
                int ip_part;
                istream >> ip_part;
                ip[i] = ip_part;
                istream.get();
            }
            ips.push_back(ip);
        }
        return true;
    }
}

void test_contains_ip()
{
    {
        std::vector<bl::ip_v4_type> ips;
        ips.push_back(std::array<uint8_t, 4>{1, 1, 1, 1});
        ips.push_back(std::array<uint8_t, 4>{192, 168, 0, 1});
        ips.push_back(std::array<uint8_t, 4>{10, 0, 0, 1});
        ips.push_back(std::array<uint8_t, 4>{172, 16, 0, 1});

        TEST_SUCCEEDED(bl::contains_ip(ips, "1.1.1.1"));
        TEST_SUCCEEDED(bl::contains_ip(ips, "*.*.*.*"));
        TEST_SUCCEEDED(bl::contains_ip(ips, "*.1.1.1"));

        TEST_SUCCEEDED(!bl::contains_ip(ips, "2.1.1.1"));

        TEST_SUCCEEDED(!bl::contains_ip(ips, "as\ndasd"));
        TEST_SUCCEEDED (!bl::contains_ip(ips, "*.1.a.v"));
        TEST_SUCCEEDED(bl::contains_ip(ips, "192.168.0.1"));
        TEST_SUCCEEDED(bl::contains_ip(ips, "*.168.*.*"));
        TEST_SUCCEEDED(bl::contains_ip(ips, "10.*.0.1"));
        TEST_SUCCEEDED(bl::contains_ip(ips, "?.?.?.?"));
        TEST_SUCCEEDED(bl::contains_ip(ips, "172.16.?.?"));
        TEST_SUCCEEDED(!bl::contains_ip(ips, "172.17.?.?"));
        TEST_SUCCEEDED(!bl::contains_ip(ips, "172.17.*.*"));
        TEST_SUCCEEDED(bl::contains_ip(ips, "172.16.*.*.*"));
    }

    TEST_SUCCEEDED(!bl::contains_ip({}, "2.1.1.1"));

    std::cout << "[ALL TESTS PASSED]" << std::endl;
}

int main()
{
    test_contains_ip();

    std::stringstream s;
    s << "1.1.1.1\n1.2.3.1\n";
    std::vector<bl::ip_v4_type> ips;
    TEST_SUCCEEDED(bl::read_ips(s, ips));
    TEST_SUCCEEDED(ips[0][0] == 1);

    return 0;
}

int main1()
{
    // запрос у пользователя ввода маски
    std::cout << "Enter the IP address mask in the format *.*.*.* or ?.?.?.?: ";
    std::string inputMask;
    std::cin >> inputMask;

    // открываем файл с ip
    std::ifstream inputFile("ipNumbers.txt");
    std::vector<bl::ip_v4_type> ips;
    if (!bl::read_ips(inputFile, ips)) {
        std::cout << "Failed to read ips" << std::endl;
        return 0;

    inputFile.close();

    if (bl::contains_ip(ips, inputMask))
    {
        std::cout << "Found!" << std::endl;
    }
    else
        std::cout << "IP-addresse can't be found" << std::endl;
    return 0;
}