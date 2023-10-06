
//
// created by AheadLib
// github:https://github.com/strivexjun/AheadLib-x86-x64
//

#include <windows.h>
#include <Shlwapi.h>

#pragma comment( lib, "Shlwapi.lib")

#pragma comment(linker, "/EXPORT:GetFileVersionInfoA=AheadLib_GetFileVersionInfoA,@1")
#pragma comment(linker, "/EXPORT:GetFileVersionInfoByHandle=AheadLib_GetFileVersionInfoByHandle,@2")
#pragma comment(linker, "/EXPORT:GetFileVersionInfoExA=AheadLib_GetFileVersionInfoExA,@3")
#pragma comment(linker, "/EXPORT:GetFileVersionInfoExW=AheadLib_GetFileVersionInfoExW,@4")
#pragma comment(linker, "/EXPORT:GetFileVersionInfoSizeA=AheadLib_GetFileVersionInfoSizeA,@5")
#pragma comment(linker, "/EXPORT:GetFileVersionInfoSizeExA=AheadLib_GetFileVersionInfoSizeExA,@6")
#pragma comment(linker, "/EXPORT:GetFileVersionInfoSizeExW=AheadLib_GetFileVersionInfoSizeExW,@7")
#pragma comment(linker, "/EXPORT:GetFileVersionInfoSizeW=AheadLib_GetFileVersionInfoSizeW,@8")
#pragma comment(linker, "/EXPORT:GetFileVersionInfoW=AheadLib_GetFileVersionInfoW,@9")
#pragma comment(linker, "/EXPORT:VerFindFileA=AheadLib_VerFindFileA,@10")
#pragma comment(linker, "/EXPORT:VerFindFileW=AheadLib_VerFindFileW,@11")
#pragma comment(linker, "/EXPORT:VerInstallFileA=AheadLib_VerInstallFileA,@12")
#pragma comment(linker, "/EXPORT:VerInstallFileW=AheadLib_VerInstallFileW,@13")
#pragma comment(linker, "/EXPORT:VerLanguageNameA=KERNEL32.VerLanguageNameA,@14")
#pragma comment(linker, "/EXPORT:VerLanguageNameW=KERNEL32.VerLanguageNameW,@15")
#pragma comment(linker, "/EXPORT:VerQueryValueA=AheadLib_VerQueryValueA,@16")
#pragma comment(linker, "/EXPORT:VerQueryValueW=AheadLib_VerQueryValueW,@17")


extern "C"
{
    PVOID pfnAheadLib_GetFileVersionInfoA;
    PVOID pfnAheadLib_GetFileVersionInfoByHandle;
    PVOID pfnAheadLib_GetFileVersionInfoExA;
    PVOID pfnAheadLib_GetFileVersionInfoExW;
    PVOID pfnAheadLib_GetFileVersionInfoSizeA;
    PVOID pfnAheadLib_GetFileVersionInfoSizeExA;
    PVOID pfnAheadLib_GetFileVersionInfoSizeExW;
    PVOID pfnAheadLib_GetFileVersionInfoSizeW;
    PVOID pfnAheadLib_GetFileVersionInfoW;
    PVOID pfnAheadLib_VerFindFileA;
    PVOID pfnAheadLib_VerFindFileW;
    PVOID pfnAheadLib_VerInstallFileA;
    PVOID pfnAheadLib_VerInstallFileW;
    PVOID pfnAheadLib_VerQueryValueA;
    PVOID pfnAheadLib_VerQueryValueW;
}


static
HMODULE g_OldModule = NULL;

VOID WINAPI Free()
{
    if (g_OldModule)
    {
        FreeLibrary(g_OldModule);
    }
}


BOOL WINAPI Load()
{
    TCHAR tzPath[MAX_PATH];
    TCHAR tzTemp[MAX_PATH * 2];

    //
    // 这里是否从系统目录或当前目录加载原始DLL
    //
    //GetModuleFileName(NULL,tzPath,MAX_PATH); //获取本目录下的
    //PathRemoveFileSpec(tzPath);

    GetSystemDirectory(tzPath, MAX_PATH); //默认获取系统目录的

    lstrcat(tzPath, TEXT("\\version.dll"));

    g_OldModule = LoadLibrary(tzPath);
    if (g_OldModule == NULL)
    {
        wsprintf(tzTemp, TEXT("无法找到模块 %s,程序无法正常运行"), tzPath);
        MessageBox(NULL, tzTemp, TEXT("AheadLib"), MB_ICONSTOP);
    }

    return (g_OldModule != NULL);

}


FARPROC WINAPI GetAddress(PCSTR pszProcName)
{
    FARPROC fpAddress;
    CHAR szProcName[64];
    TCHAR tzTemp[MAX_PATH];

    fpAddress = GetProcAddress(g_OldModule, pszProcName);
    if (fpAddress == NULL)
    {
        if (HIWORD(pszProcName) == 0)
        {
            wsprintfA(szProcName, "#%d", pszProcName);
            pszProcName = szProcName;
        }

        wsprintf(tzTemp, TEXT("无法找到函数 %hs,程序无法正常运行"), pszProcName);
        MessageBox(NULL, tzTemp, TEXT("AheadLib"), MB_ICONSTOP);
        ExitProcess(-2);
    }
    return fpAddress;
}

BOOL WINAPI Init()
{
    pfnAheadLib_GetFileVersionInfoA = GetAddress("GetFileVersionInfoA");
    pfnAheadLib_GetFileVersionInfoByHandle = GetAddress("GetFileVersionInfoByHandle");
    pfnAheadLib_GetFileVersionInfoExA = GetAddress("GetFileVersionInfoExA");
    pfnAheadLib_GetFileVersionInfoExW = GetAddress("GetFileVersionInfoExW");
    pfnAheadLib_GetFileVersionInfoSizeA = GetAddress("GetFileVersionInfoSizeA");
    pfnAheadLib_GetFileVersionInfoSizeExA = GetAddress("GetFileVersionInfoSizeExA");
    pfnAheadLib_GetFileVersionInfoSizeExW = GetAddress("GetFileVersionInfoSizeExW");
    pfnAheadLib_GetFileVersionInfoSizeW = GetAddress("GetFileVersionInfoSizeW");
    pfnAheadLib_GetFileVersionInfoW = GetAddress("GetFileVersionInfoW");
    pfnAheadLib_VerFindFileA = GetAddress("VerFindFileA");
    pfnAheadLib_VerFindFileW = GetAddress("VerFindFileW");
    pfnAheadLib_VerInstallFileA = GetAddress("VerInstallFileA");
    pfnAheadLib_VerInstallFileW = GetAddress("VerInstallFileW");
    pfnAheadLib_VerQueryValueA = GetAddress("VerQueryValueA");
    pfnAheadLib_VerQueryValueW = GetAddress("VerQueryValueW");
    return TRUE;
}

// --------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------

#include <string>
#include "log.h"
#include "MinHook.h"

template <typename T>
class ProtectMemory
{
public:
    __forceinline ProtectMemory(intptr_t address) :address_(address)
    {
        VirtualProtect(reinterpret_cast<LPVOID>(address_), sizeof(T), PAGE_EXECUTE_READWRITE, &old_protect_);
    }
    __forceinline ~ProtectMemory()
    {
        VirtualProtect(reinterpret_cast<LPVOID>(address_), sizeof(T), old_protect_, &old_protect_);
    }
private:
    intptr_t address_;
    DWORD old_protect_;
};
template <typename T>
__forceinline T ReadMemory(intptr_t address, bool protect_memory = false)
{
    T return_value;
    if (protect_memory)
    {
        ProtectMemory<T> pm = address;
        return_value = *(T*)address;
    }
    else
    {
        return_value = *(T*)address;
    }
    return return_value;
}

template <typename T>
__forceinline void WriteMemory(intptr_t address, T get_value, bool protect_memory = false)
{
    if (protect_memory)
    {
        ProtectMemory<T> pm = address;
        *(T*)address = get_value;
    }
    else
    {
        *(T*)address = get_value;
    }
}

// --------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------



#pragma pack(push,1)
struct stdstring
{
    union 
    {
        char _short_buf[23];

        struct {
            char* _long_buf;
            int32_t _long_size;
            int32_t _align;
            int32_t _alloc_size;
        };
    };

    int8_t _short_size : 7;
    bool _is_long : 1;

    uint8_t size() const
    {
        if (_is_long)
        {
            return _long_size;
        }
        else
        {
            return _short_size;
        }
    }

    const char* c_str() const
    {
        if (_is_long)
        {
            return _long_buf;
        }
        else
        {
            return _short_buf;
        }
    }

    std::string from_u8()
    {
        if (_is_long)
        {
            int nwLen = MultiByteToWideChar(CP_UTF8, 0, _long_buf, -1, NULL, 0);

            wchar_t* pwBuf = new wchar_t[nwLen + 1];//一定要加1，不然会出现尾巴 
            memset(pwBuf, 0, nwLen * 2 + 2);

            MultiByteToWideChar(CP_UTF8, 0, _long_buf, _long_size, pwBuf, nwLen);

            int nLen = WideCharToMultiByte(CP_ACP, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

            char* pBuf = new char[nLen + 1];
            memset(pBuf, 0, nLen + 1);

            WideCharToMultiByte(CP_ACP, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

            std::string retStr = pBuf;

            delete[]pBuf;
            delete[]pwBuf;

            pBuf = NULL;
            pwBuf = NULL;

            return retStr;
        }
        else
        {
            int nwLen = MultiByteToWideChar(CP_UTF8, 0, _short_buf, -1, NULL, 0);

            wchar_t* pwBuf = new wchar_t[nwLen + 1];//一定要加1，不然会出现尾巴 
            memset(pwBuf, 0, nwLen * 2 + 2);

            MultiByteToWideChar(CP_UTF8, 0, _short_buf, _short_size, pwBuf, nwLen);

            int nLen = WideCharToMultiByte(CP_ACP, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

            char* pBuf = new char[nLen + 1];
            memset(pBuf, 0, nLen + 1);

            WideCharToMultiByte(CP_ACP, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

            std::string retStr = pBuf;

            delete[]pBuf;
            delete[]pwBuf;

            pBuf = NULL;
            pwBuf = NULL;

            return retStr;
        }
    }

    void operator=(const char* const _Ptr) {
        int32_t new_size = strlen(_Ptr);
        if (_is_long)
        {
            if (_long_size < new_size)
            {
                D("new string too long.");
                return;
            }
        }
        else
        {
            if (_short_size < new_size)
            {
                D("new string too long.");
                return;
            }
        }


        if (new_size > 22)
        {
            memset(_long_buf, 0, _long_size);
            D("%p,%d,%p", _long_buf, _long_size, _Ptr);
            strcpy_s(_long_buf, _alloc_size, _Ptr);
            _long_size = new_size;
            _is_long = true;
        }
        else
        {
            memset(_short_buf, 0, sizeof(_short_buf));
            strcpy_s(_short_buf, _Ptr);
            _short_size = new_size;

            if (_is_long)
            {
                //delete[] _long_buf;
                free(_long_buf);
            }
            _is_long = false;
        }
    }
};
#pragma pack(pop)

struct AppletInitConf {
    stdstring appId; // 0x0
    stdstring brandName; // 0x18
    stdstring iconUrl; // 0x30
    uint32_t debugType; // 0x48
    char _align_0x4C[4]; // 0x4C
    stdstring orientation; // 0x50
    stdstring pkgDirPath; // 0x68
    stdstring publicPkgDirPath; // 0x80
    uint32_t publicVer; // 0x98
    char _align_0x9C[4]; // 0x9C
    stdstring moduleListInfo; // 0xA0
    stdstring dataPath; // 0xB8
    stdstring app_icon_path; // 0xD0  tmpPath?
    stdstring username; // 0xE8
    stdstring nickName; // 0x100
    stdstring signature; // 0x118
    stdstring logPath; // 0x130
    stdstring clientJsExtInfo; // 0x148
    stdstring operationInfo; // 0x160
    stdstring passThroughInfo; // 0x178
    stdstring shareName; // 0x190
    stdstring shareKey; // 0x1A8
    stdstring remote_debug_endpoint; // 0x1C0
    uint32_t appVersion; // 0x1D8
    uint32_t versionState; // 0x1DC
    uint16_t width; // 0x1E0
    uint16_t height; // 0x1E2
    uint32_t sysBtn; // 0x1E4
    uint32_t launchScene; // 0x1E8
    char _align_0x1EC[4]; // 0x1EC
    stdstring appId_; // 0x1F0
    stdstring extraData; // 0x208
    stdstring privateExtraData; // 0x220
    stdstring messageExtraData; // 0x238
    stdstring url; // 0x250
    stdstring agentId; // 0x268
    uint32_t sourceType; // 0x280
    char _align_0x284[4]; // 0x284
    stdstring openapiInvokeData; // 0x288
    stdstring transitiveData; // 0x2A0
    stdstring enterPath; // 0x2B8
    uint32_t originalFlag; // 0x2D0
    char _align_0x2D4[4]; // 0x2D4
    stdstring originalRedirectUrl; // 0x2D8
    bool isNativeView; // 0x2F0
    char _align_0x2F1[3]; // 0x2F1
    uint32_t version_type; // 0x2F4
    char _align_0x2F8[0x18]; // 0x2F8
    stdstring uin; // 0x310
    stdstring deviceType; // 0x328
    uint32_t clientVersion; // 0x340
    bool isTest; // 0x344
    char _align_0x345[3]; // 0x345
    stdstring wxIconUrl; // 0x348
    stdstring wxNickName; // 0x360
    uint32_t productId; // 0x378
    char _align_0x37C[4]; // 0x37C
    stdstring commonJsInfo; // 0x380
    bool isMiniGame; // 0x398
    char _align_0x399[7]; // 0x399
    stdstring ozone_platform; // 0x3A0
    stdstring pluginDir; // 0x3B8
    stdstring hostAppId; // 0x3D0
};


intptr_t FakeLaunchApplet(intptr_t a1, intptr_t a2, AppletInitConf* conf);
decltype(&FakeLaunchApplet) g_oLaunchApplet = nullptr;

intptr_t FakeLaunchApplet(intptr_t a1, intptr_t a2, AppletInitConf* conf)
{
    {
        D("commonJsInfo:%s", conf->commonJsInfo.c_str());
        std::string commonJsInfo = conf->commonJsInfo.c_str();

        size_t pos = std::string::npos;
        const char* s = nullptr;

        s = "\"enable_vconsole\":false";
        pos = commonJsInfo.find(s);
        if (pos != std::string::npos)
        {
            commonJsInfo.replace(pos, strlen(s), "\"enable_vconsole\":true");
        }

        conf->commonJsInfo = commonJsInfo.c_str();
        D("commonJsInfo:%s", conf->commonJsInfo.c_str());
    }

    //D("appId:%s", conf->appId.c_str());
    //D("brandName:%s", conf->brandName.c_str());
    //D("iconUrl:%s", conf->iconUrl.c_str());
    //D("debugType:%d", conf->debugType);

    //D("orientation:%s", conf->orientation.c_str());
    //D("pkgDirPath:%s", conf->pkgDirPath.c_str());
    //D("publicPkgDirPath:%s", conf->publicPkgDirPath.c_str());
    //D("publicVer:%d", conf->publicVer);

    //D("moduleListInfo:%s", conf->moduleListInfo.c_str());
    //D("dataPath:%s", conf->dataPath.c_str());
    //D("app_icon_path:%s", conf->app_icon_path.c_str());
    //D("username:%s", conf->username.c_str());

    ////conf->nickName = u8"鲨鱼辣椒";
    //D("nickName:%s", conf->nickName.from_u8().c_str());
    //D("signature:%s", conf->signature.from_u8().c_str());
    //D("logPath:%s", conf->logPath.c_str());
    //D("clientJsExtInfo:%s", conf->clientJsExtInfo.c_str());
    //D("operationInfo:%s", conf->operationInfo.c_str());
    //D("passThroughInfo:%s", conf->passThroughInfo.c_str());
    //D("shareName:%s", conf->shareName.c_str());
    //D("shareKey:%s", conf->shareKey.c_str());
    //D("remote_debug_endpoint:%s", conf->remote_debug_endpoint.c_str());


    //D("appVersion:%d", conf->appVersion);
    //D("versionState:%d", conf->versionState);
    //D("width:%d", conf->width);
    //D("height:%d", conf->height);
    //D("sysBtn:%d", conf->sysBtn);
    //D("launchScene:%d", conf->launchScene);


    //D("appId_:%s", conf->appId_.c_str());
    //D("extraData:%s", conf->extraData.c_str());
    //D("privateExtraData:%s", conf->privateExtraData.c_str());
    //D("messageExtraData:%s", conf->messageExtraData.c_str());
    //D("url:%s", conf->url.c_str());
    //D("agentId:%s", conf->agentId.c_str());
    //D("sourceType:%d", conf->sourceType);


    return g_oLaunchApplet(a1, a2, conf);
}

DWORD WINAPI ThreadProc_WeChatAppEx(LPVOID lpThreadParameter)
{
    D("fuck WeChatAppEx[ver:%s]", __TIME__);

    if (true)
    {
        // Wait for debugger
        char* cmd = GetCommandLineA();
        if (strstr(cmd, "--type=") == NULL)
        {
            do
            {
                uint8_t* LaunchAppletAddr = (uint8_t*)GetModuleHandle(NULL) + 0x1B3FF3C;

                if (MH_Initialize() != MH_OK)
                {
                    D("Initialize err");
                    break;
                }
                if (MH_CreateHook(LaunchAppletAddr, &FakeLaunchApplet,
                    reinterpret_cast<LPVOID*>(&g_oLaunchApplet)) != MH_OK)
                {
                    D("MH_CreateHook err");
                    break;
                }
                if (MH_EnableHook(LaunchAppletAddr) != MH_OK)
                {
                    D("MH_EnableHook err");
                    break;
                }

            } while (false);

            D("hook LaunchApplet success!");

            if (ReadMemory<uint16_t>((uintptr_t)GetModuleHandle(NULL) + 0x2EC9FAB) == 0x0974)
            {
                D("patch devtool url!");
                WriteMemory<uint16_t>((uintptr_t)GetModuleHandle(NULL) + 0x2EC9FAB, 0x9090, true);
            }

            D("wait for debug %d %p", GetCurrentProcessId(), GetCurrentProcessId());

            while (!IsDebuggerPresent())
            {
                Sleep(1000);
            }

            D("attach debug");
        }
    }

    
    const bool kEnableWxLog = false;
    while (kEnableWxLog)
    {
        // 41 57 41 56 41 55 41 54 56 57 55 53 48 81 EC A8 02 00 00 0F 29 B4 24 90 02 00 00 4D 89 C5 49 89 D7 49 89 CE 48 8B 05 31 12 D6 06 48 31 E0 48 89 84 24 88 02 00 00 B9 02 00 00 00
        intptr_t pvDebugFlag = (intptr_t)GetModuleHandle(NULL) + 0x8994EE8;
        // debugFlag
        WriteMemory<int32_t>(pvDebugFlag, 0xFFF);
        // logLevel
        WriteMemory<int32_t>(pvDebugFlag + 0x10, 0);
        // logCallback
        WriteMemory<intptr_t>(pvDebugFlag + 0x18, NULL);

        Sleep(1);
    }

    return 0;
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, PVOID pvReserved)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hModule);

        if (Load() && Init())
        {
            TCHAR szAppNameEx[MAX_PATH] = TEXT("WeChatAppEx.exe");
            TCHAR szCurName[MAX_PATH];

            GetModuleFileName(NULL, szCurName, MAX_PATH);
            PathStripPath(szCurName);

            //是否判断宿主进程名
            if (StrCmpI(szCurName, szAppNameEx) == 0)
            {
                HANDLE hThread = CreateThread(NULL, NULL, ThreadProc_WeChatAppEx, NULL, NULL, NULL);
                if (hThread)
                {
                    CloseHandle(hThread);
                }
            }
        }
    }
    else if (dwReason == DLL_PROCESS_DETACH)
    {
        Free();
    }

    return TRUE;
}

