#ifndef _IPTOCOUNTRYSLOW_HPP_
# define _IPTOCOUNTRYSLOW_HPP_

#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <stdexcept>

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, char delim);

template <class T>
T stringTo(const std::string &str)
{
    T result;
    std::stringstream ss(str);
    ss >> result;
    return result;
}

template <class Ty, class Container>
std::vector<Ty> convertContainerTo(const Container &source)
{
    std::vector<Ty> result;
    std::for_each(source.begin(), source.end(),
                  [&result](const std::string &it){ result.push_back(stringTo<Ty>(it)); });
    return result;
}

typedef unsigned int uint32_t;
typedef uint32_t IpAddress_t;

/** IP address mapping entry */
class IpAddressMapping {
public:
    std::string country;
    IpAddress_t startAddress;
};

/** Class for mapping IP addresses to countries using database
 * from http://software77.net/geo-ip/ */

class IpToCountrySlow
{
    std::vector<IpAddressMapping> m_countryIpList;
public:
    /** Construct new IP-to-country mapper from the specified file. */
    IpToCountrySlow(const std::string &FileName = "../IpToCountry.csv");
    /** Find the country for given IP address or throw std::exception. */
    IpAddressMapping GetCountry(const std::string &address) const;
    
    /** Convert a human-readable ipv4 address to integer */
    static IpAddress_t IntegerFromIp(const std::string &ip);
    
private:
    static IpAddressMapping ParseSingleLine(const std::string &line);
};

#endif