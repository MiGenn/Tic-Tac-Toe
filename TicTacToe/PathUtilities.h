#pragma once
#include <string>

std::wstring ExtractDirectory(const std::wstring& fullPath);
std::wstring ExtractName(const std::wstring& fullPath);
std::wstring ExtractExtension(const std::wstring& fullPath);
std::wstring ExtractFullName(const std::wstring& fullPath);

bool IsPathValid(const std::wstring& directoryOrFullPath) noexcept;
