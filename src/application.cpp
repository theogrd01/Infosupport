#include "Application.h"
#include <windows.h>
#include "imgui.h"
#include <algorithm>
#include <string>

std::string WStringToString(const std::wstring& value)
{
    if (value.empty())
    {
        return std::string();
    }

    const int sizeRequired =
        WideCharToMultiByte(
            CP_UTF8,
            0,
            value.c_str(),
            static_cast<int>(value.size()),
            nullptr,
            0,
            nullptr,
            nullptr);

    if (sizeRequired <= 0)
    {
        return std::string();
    }

    std::string result(sizeRequired, '\0');

    const int bytesWritten =
        WideCharToMultiByte(
            CP_UTF8,
            0,
            value.c_str(),
            static_cast<int>(value.size()),
            result.data(),
            sizeRequired,
            nullptr,
            nullptr);

    if (bytesWritten <= 0)
    {
        return std::string();
    }

    return result;
}

static void DrawInfoRow(const char* label, const std::string& value)
{
    ImGui::TextDisabled("%s", label);
    ImGui::SameLine(170.0f);
    ImGui::TextUnformatted(value.c_str());
}

static void RemoveNullChars(std::string& text)
{
    text.erase(
        std::remove(text.begin(), text.end(), '\0'),
        text.end());
}

Application::Application()
{
}

Application::~Application()
{
}

void Application::SetLogoTexture(void* texture, int width, int height)
{
    m_logoTexture = texture;
    m_logoWidth = width;
    m_logoHeight = height;
}


void Application::LoadInfos()
{
    system.Load();

    os.Load();

    uptime.Load();

    memory.Load();

    hardware.Load();

    network.Load();

    disk.Load();

    printer.Load();

    domain.Load();
}


void Application::Render()
{
    const ImVec2 displaySize = ImGui::GetIO().DisplaySize;

    if (displaySize.x < 64.0f || displaySize.y < 64.0f)
    {
        return;
    }

    ImGui::SetNextWindowSize(displaySize, ImGuiCond_Always);
    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);

    ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoScrollWithMouse;

    const bool visible = ImGui::Begin("hcc-infosupport", nullptr, flags);

    if (!visible)
    {
        ImGui::End();
        return;
    }

    std::string printerName =
        WStringToString(printer.GetDefaultPrinterName());

    const std::string dhcpStatus =
        network.IsDHCPEnabled() ? "Oui" : "Non";

    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.96f, 0.97f, 0.99f, 1.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 10.0f);

    const float contentStartX = ImGui::GetCursorPosX();
    const float availableWidth = ImGui::GetContentRegionAvail().x;
    const float safeAvailableWidth = (availableWidth > 320.0f) ? availableWidth : 320.0f;
    const float desiredDashboardWidth = 540.0f;
    const float dashboardWidth =
        (safeAvailableWidth < desiredDashboardWidth)
        ? safeAvailableWidth
        : desiredDashboardWidth;

    const float centeredX =
        contentStartX +
        ((safeAvailableWidth - dashboardWidth) * 0.5f);
    

    if (m_logoTexture != nullptr && m_logoWidth > 0 && m_logoHeight > 0)
    {
        const float maxLogoWidth = 340.0f;
        const float maxLogoHeight = 110.0f;

        const float widthScale = maxLogoWidth / static_cast<float>(m_logoWidth);
        const float heightScale = maxLogoHeight / static_cast<float>(m_logoHeight);
        const float scale = (widthScale < heightScale) ? widthScale : heightScale;

        const float logoWidth = static_cast<float>(m_logoWidth) * scale;
        const float logoHeight = static_cast<float>(m_logoHeight) * scale;

        ImGui::SetCursorPosX(centeredX + ((dashboardWidth - logoWidth) * 0.5f));
        ImGui::Image((ImTextureID)m_logoTexture, ImVec2(logoWidth, logoHeight));

        ImGui::Spacing();
        ImGui::Spacing();
    }

    ImGui::SetCursorPosX(centeredX);
    ImGui::Separator();
    ImGui::Spacing();

    ImGui::SetCursorPosX(centeredX);

    if (ImGui::BeginChild("PosteCard", ImVec2(dashboardWidth, 300.0f), true, ImGuiWindowFlags_NoScrollbar))
    {
        ImGui::TextColored(ImVec4(0.13f, 0.31f, 0.58f, 1.0f), "Poste");
        ImGui::Separator();
        ImGui::Spacing();

        DrawInfoRow("Nom du poste", system.GetPCName());
        DrawInfoRow("Utilisateur", system.GetCurrentUser());
        DrawInfoRow("Systeme", os.GetOSName());
        DrawInfoRow("Uptime", uptime.GetUptime());
        DrawInfoRow("RAM", memory.GetMemory());
        DrawInfoRow("Lecteur C", disk.GetDiskUsage());
        DrawInfoRow("Imprimante", printerName);
        DrawInfoRow("Domaine AD", domain.GetDomain());
    }
    ImGui::EndChild();

    ImGui::Spacing();

    ImGui::SetCursorPosX(centeredX);

    if (ImGui::BeginChild("ReseauCard", ImVec2(dashboardWidth, 170.0f), true, ImGuiWindowFlags_NoScrollbar))
    {
        ImGui::TextColored(ImVec4(0.13f, 0.31f, 0.58f, 1.0f), "Reseau");
        ImGui::Separator();
        ImGui::Spacing();

        DrawInfoRow("Adresse IP", network.GetIPAddress());
        DrawInfoRow("Passerelle", network.GetGateway());
        DrawInfoRow("Serveurs DNS", network.GetDNS());
        DrawInfoRow("DHCP", dhcpStatus);
    }
    ImGui::EndChild();

    ImGui::PopStyleVar();
    ImGui::PopStyleColor();

    ImGui::Spacing();

    const float closeButtonWidth = 150.0f;
    const float copyButtonWidth = 170.0f;
    const float buttonsSpacing = 12.0f;
    const float totalButtonsWidth = closeButtonWidth + buttonsSpacing + copyButtonWidth;
    const float centeredButtonsX = (ImGui::GetWindowWidth() - totalButtonsWidth) * 0.5f;
    const float buttonsStartX = (centeredButtonsX < 0.0f) ? 0.0f : centeredButtonsX;

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.13f, 0.31f, 0.58f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.18f, 0.38f, 0.67f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.10f, 0.25f, 0.48f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));

    ImGui::SetCursorPosX(buttonsStartX);

   
   

    if (ImGui::Button("Copier le contenu", ImVec2(copyButtonWidth, 34.0f)))
    {
        std::string report;

        report += "=== Informations systeme ===\n";
        report += "Nom du poste: " + system.GetPCName() + "\n";
        report += "Utilisateur: " + system.GetCurrentUser() + "\n";
        report += "Systeme: " + os.GetOSName() + "\n";
        report += "Uptime: " + uptime.GetUptime() + "\n";
        report += "RAM: " + memory.GetMemory() + "\n";
        report += "Lecteur C: " + disk.GetDiskUsage() + "\n";
        report += "Imprimante: " + printerName + "\n";
        report += "Domaine AD: " + domain.GetDomain() + "\n\n";
        
        report += "=== Informations reseau ===\n";
        report += "Adresse IP: " + network.GetIPAddress() + "\n";
        report += "Passerelle: " + network.GetGateway() + "\n";
        report += "Serveurs DNS: " + network.GetDNS() + "\n";
        report += "DHCP: " + dhcpStatus + "\n";

        RemoveNullChars(report);

        ImGui::SetClipboardText(report.c_str());
    }

    ImGui::SameLine(0.0f, buttonsSpacing);

    if (ImGui::Button("Fermer la fenetre", ImVec2(closeButtonWidth, 34.0f)))
    {
        PostQuitMessage(0);
    }


    ImGui::PopStyleColor(4);

    ImGui::Spacing();

    ImGui::End();
}