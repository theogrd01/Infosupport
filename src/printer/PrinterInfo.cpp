#include "printer/PrinterInfo.h"

#include <Windows.h>
#include <winspool.h>
#undef GetDefaultPrinter

void PrinterInfo::Load()
{
    DWORD size = 0;

    ::GetDefaultPrinterW(nullptr, &size);

    if (size == 0)
    {
        m_defaultPrinter = L"Aucune imprimante";
        return;
    }

    std::wstring printer(size, L'\0');

    if (!::GetDefaultPrinterW(printer.data(), &size))
    {
        m_defaultPrinter = L"Erreur";
        return;
    }

    printer.resize(size - 1);

    m_defaultPrinter = printer;
}


std::wstring PrinterInfo::GetDefaultPrinterName() const
{
    return m_defaultPrinter;
}