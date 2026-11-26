#pragma once

#include <string_view>
#include <format>
#include <source_location>

#pragma warning(push)
#include "F4SE/F4SE.h"
#include "RE/Fallout.h"
#pragma warning(pop)

#include "Version.h"

#define DLLEXPORT __declspec(dllexport)

namespace logger
{
	template <class... Args>
	void info(std::format_string<Args...> fmt, Args&&... args) {
		REX::Impl::Log(std::source_location::current(), REX::ELogLevel::Info, fmt, std::forward<Args>(args)...);
	}
	inline void info(std::string_view msg) {
		REX::Impl::Log(std::source_location::current(), REX::ELogLevel::Info, msg);
	}

	template <class... Args>
	void warn(std::format_string<Args...> fmt, Args&&... args) {
		REX::Impl::Log(std::source_location::current(), REX::ELogLevel::Warning, fmt, std::forward<Args>(args)...);
	}
	inline void warn(std::string_view msg) {
		REX::Impl::Log(std::source_location::current(), REX::ELogLevel::Warning, msg);
	}

	template <class... Args>
	void error(std::format_string<Args...> fmt, Args&&... args) {
		REX::Impl::Log(std::source_location::current(), REX::ELogLevel::Error, fmt, std::forward<Args>(args)...);
	}
	inline void error(std::string_view msg) {
		REX::Impl::Log(std::source_location::current(), REX::ELogLevel::Error, msg);
	}

	template <class... Args>
	void critical(std::format_string<Args...> fmt, Args&&... args) {
		REX::Impl::Log(std::source_location::current(), REX::ELogLevel::Critical, fmt, std::forward<Args>(args)...);
	}
	inline void critical(std::string_view msg) {
		REX::Impl::Log(std::source_location::current(), REX::ELogLevel::Critical, msg);
	}
}

using namespace std::literals;
