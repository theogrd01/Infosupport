#pragma once

#include <string>


class DomainInfo
{

public:

    void Load();

    std::string GetDomain() const;


private:

    std::string m_domain;

};