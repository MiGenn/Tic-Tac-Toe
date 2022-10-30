#include "PathUtilities.h"
#include "WinapiException.h"

std::wstring ExtractDirectory(const std::wstring& fullPath)
{
    static constexpr size_t directoryBufferSize{ 256ull };
    static constexpr size_t driveDesignatorSize{ 3ull };

    wchar_t directoryBuffer[directoryBufferSize];
    _wsplitpath_s(fullPath.c_str(), directoryBuffer, driveDesignatorSize, &directoryBuffer[driveDesignatorSize - 1], 
        directoryBufferSize - driveDesignatorSize - 1, nullptr, 0, nullptr, 0);

    return { directoryBuffer };
}

std::wstring ExtractName(const std::wstring& fullPath)
{
    static constexpr size_t nameBufferSize{ 64ull };

    wchar_t nameBuffer[nameBufferSize];
    _wsplitpath_s(fullPath.c_str(), nullptr, 0, nullptr, 0, nameBuffer, nameBufferSize, nullptr, 0);

    return { nameBuffer };
}

std::wstring ExtractExtension(const std::wstring& fullPath)
{
    static constexpr size_t extensionBufferSize{ 32ull };

    wchar_t extensionBuffer[extensionBufferSize];
    _wsplitpath_s(fullPath.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, extensionBuffer, extensionBufferSize);

    return { extensionBuffer };
}

std::wstring ExtractFullName(const std::wstring& fullPath)
{
    return ExtractName(fullPath) + ExtractExtension(fullPath);
}

bool IsPathValid(const std::wstring& directoryOrFullPath) noexcept
{
    struct _stat fileOrDirectoryInfo{};
    return !_wstat(directoryOrFullPath.c_str(), &fileOrDirectoryInfo);
}
