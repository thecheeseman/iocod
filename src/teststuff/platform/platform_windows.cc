#include "platform_windows.h"
#include <Windows.h>

//
std::unique_ptr<Platform> platform = std::make_unique<PlatformWindows>();
//

static void get_major_version_label(PlatformVersion& version,
                                    SYSTEM_INFO& sysinfo,
                                    OSVERSIONINFOEX& osinfo)
{
    switch (osinfo.dwMajorVersion) {
    //
    // Windows 2000 - Server 2003
    //
    case 5:
        switch (osinfo.dwMinorVersion) {
        case 0:
            version.label = "Windows 2000";
            break;
        case 1:
            version.label = "Windows XP";
            break;
        case 2:
            if (osinfo.wProductType == VER_NT_WORKSTATION &&
                sysinfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
                version.label = "Windows XP x64";
            else if (GetSystemMetrics(SM_SERVERR2) == 0)
                version.label = "Windows Server 2003";
            else if (osinfo.wSuiteMask & VER_SUITE_WH_SERVER)
                version.label = "Windows Home Server";
            else if (GetSystemMetrics(SM_SERVERR2) != 0)
                version.label = "Windows Server 2003 R2";
            break;
        default:
            version.error = PlatformVersion::ErrorCode::UnknownVersion;
            break;
        }
        break;
    //
    // Vista thru 8.1
    //
    case 6:
        switch (osinfo.dwMinorVersion) {
        case 0:
            if (osinfo.wProductType == VER_NT_WORKSTATION)
                version.label = "Windows Vista";
            else
                version.label = "Windows Server 2008";
            break;
        case 1:
            if (osinfo.wProductType == VER_NT_WORKSTATION)
                version.label = "Windows 7";
            else
                version.label = "Windows Server 2008 R2";
            break;
        case 2:
            if (osinfo.wProductType == VER_NT_WORKSTATION)
                version.label = "Windows 8";
            else
                version.label = "Windows Server 2012";
            break;
        case 3:
            if (osinfo.wProductType == VER_NT_WORKSTATION)
                version.label = "Windows 8.1";
            else
                version.label = "Windows Server 2012 R2";
            break;
        default:
            version.error = PlatformVersion::ErrorCode::UnknownVersion;
            break;
        }
        break;
    //
    // windows 10/11
    //
    case 10:
        switch (osinfo.dwMinorVersion) {
        case 0:
            if (osinfo.wProductType == VER_NT_WORKSTATION) {
                if (osinfo.dwBuildNumber >= 22000)
                    version.label = "Windows 11";
                else
                    version.label = "Windows 10";
            } else {
                if (osinfo.dwBuildNumber >= 20348)
                    version.label = "Windows Server 2022";
                else
                    version.label = "Windows Server 2019";
            }
            break;
        default:
            version.error = PlatformVersion::ErrorCode::UnknownVersion;
            break;
        }
        break;
    }
}

static void get_minor_version_label(PlatformVersion& version,
                                    SYSTEM_INFO& sysinfo,
                                    OSVERSIONINFOEX& osinfo)
{
    // skip 2003 and earlier
    if (osinfo.dwMajorVersion < 6)
        return;

    typedef BOOL(WINAPI* GPI)(DWORD, DWORD, DWORD, DWORD, PDWORD);
    GPI get_product_info = (GPI) GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")), 
                                                "GetProductInfo");

    if (get_product_info == nullptr) {
        version.sublabel = "unknown";
        version.error = PlatformVersion::ErrorCode::ProductInfoFailed;
        return;
    }

    DWORD type = 0;
    get_product_info(osinfo.dwMajorVersion, osinfo.dwMinorVersion, 0, 0, &type);
    switch (type) {
    case PRODUCT_ULTIMATE:
        version.sublabel = "Ultimate Edition";
        break;
    case PRODUCT_PROFESSIONAL:
        version.sublabel = "Professional";
        break;
    case PRODUCT_HOME_PREMIUM:
        version.sublabel = "Home Premium Edition";
        break;
    case PRODUCT_HOME_BASIC:
        version.sublabel = "Home Basic Edition";
        break;
    case PRODUCT_ENTERPRISE:
        version.sublabel = "Enterprise Edition";
        break;
    case PRODUCT_BUSINESS:
        version.sublabel = "Business Edition";
        break;
    case PRODUCT_STARTER:
        version.sublabel = "Starter Edition";
        break;
    case PRODUCT_CLUSTER_SERVER:
        version.sublabel = "Cluster Server Edition";
        break;
    case PRODUCT_DATACENTER_SERVER:
        version.sublabel = "Datacenter Edition";
        break;
    case PRODUCT_DATACENTER_SERVER_CORE:
        version.sublabel = "Datacenter Edition (core installation)";
        break;
    case PRODUCT_ENTERPRISE_SERVER:
        version.sublabel = "Enterprise Edition";
        break;
    case PRODUCT_ENTERPRISE_SERVER_CORE:
        version.sublabel = "Enterprise Edition (core installation)";
        break;
    case PRODUCT_ENTERPRISE_SERVER_IA64:
        version.sublabel = "Enterprise Edition for Itanium-based Systems";
        break;
    case PRODUCT_SMALLBUSINESS_SERVER:
        version.sublabel = "Small Business Server";
        break;
    case PRODUCT_SMALLBUSINESS_SERVER_PREMIUM:
        version.sublabel = "Small Business Server Premium Edition";
        break;
    case PRODUCT_STANDARD_SERVER:
        version.sublabel = "Standard Edition";
        break;
    case PRODUCT_STANDARD_SERVER_CORE:
        version.sublabel = "Standard Edition (core installation)";
        break;
    case PRODUCT_WEB_SERVER:
        version.sublabel = "Web Server Edition";
        break;
    default:
        break;
    }
}

PlatformVersion PlatformWindows::get_os_version()
{
    PlatformVersion version;
    SYSTEM_INFO sysinfo {};

    if (is_64bit())
        GetNativeSystemInfo(&sysinfo);
    else
        GetSystemInfo(&sysinfo);

    OSVERSIONINFOEX osinfo = { 0 };
    osinfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

    version.label = "Windows (unknown version)";
    version.sublabel = "";

    if (!GetVersionEx((LPOSVERSIONINFO) &osinfo)) {
        version.error = PlatformVersion::ErrorCode::GetVersionFailed;
        return version;
    }

    version.major = osinfo.dwMajorVersion;
    version.minor = osinfo.dwMinorVersion;
    version.build = osinfo.dwBuildNumber;

    get_major_version_label(version, sysinfo, osinfo);
    get_minor_version_label(version, sysinfo, osinfo);

    return version;
}

std::string PlatformWindows::get_os_version_string()
{
    PlatformVersion version = get_os_version();
    return { 
        version.label + " " + version.sublabel + " [" +
        std::to_string(version.major) + "." +
        std::to_string(version.minor) + "." +
        std::to_string(version.build) + "]" 
    };
}
