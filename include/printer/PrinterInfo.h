#pragma once

#include <string>

class PrinterInfo
{
public:

    void Load();

    std::wstring GetDefaultPrinterName() const;

private:

    std::wstring m_defaultPrinter;
};