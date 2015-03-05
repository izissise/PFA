#include "IpToCountrySlow.hpp"

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while(std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    return split(s, delim, elems);
}


/** Construct new IP-to-country mapper from the specified file. */
IpToCountrySlow::IpToCountrySlow(const std::string &FileName)
{
    std::ifstream file(FileName);
    while(file.good() && !file.eof())
    {
        std::string line;
        std::getline(file, line);
        if(line.find_first_of('#') == std::string::npos && line.length() > 0)
            m_countryIpList.push_back(ParseSingleLine(line));
    }
}

/** Find the country for given IP address or throw std::exception. */
IpAddressMapping IpToCountrySlow::GetCountry(const std::string &address) const
{
    IpAddress_t integerIp = IntegerFromIp(address);
    auto it = std::find_if(m_countryIpList.rbegin(), m_countryIpList.rend(),
                           [integerIp](IpAddressMapping it) { return it.startAddress <= integerIp; });
    if(it == m_countryIpList.rend())
        throw std::runtime_error("Address not found");
    return *it;
}

/** Convert a human-readable ipv4 address to integer */
IpAddress_t IpToCountrySlow::IntegerFromIp(const std::string &ip)
{
    auto tokens = split(ip, '.');
    auto integers = convertContainerTo<uint32_t>(tokens);
    return (integers[0] << (3*8)) +
    (integers[1] << (2*8)) +
    (integers[2] << (1*8)) +
    integers[3];
}

IpAddressMapping IpToCountrySlow::ParseSingleLine(const std::string &line)
{
    IpAddressMapping mapping;
    auto tokens = split(line, ',');
    mapping.country = tokens[6].substr(1, tokens[6].length()-2);
    mapping.startAddress = stringTo<uint32_t>(tokens[0].substr(1, tokens[0].length()-2));
    return mapping;
}
