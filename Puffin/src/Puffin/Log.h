#pragma once

#include <memory>

#include "Core.h"
#include "spdlog/spdlog.h"

namespace Puffin
{
	class PUFFIN_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
		
	};
}

// Core Logging
#define PF_CORE_TRACE(...)	::Puffin::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define PF_CORE_INFO(...)	::Puffin::Log::GetCoreLogger()->info(__VA_ARGS__)
#define PF_CORE_WARN(...)	::Puffin::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define PF_CORE_ERROR(...)	::Puffin::Log::GetCoreLogger()->error(__VA_ARGS__)
#define PF_CORE_FATAL(...)	::Puffin::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Client Logging
#define PF_TRACE(...)		::Puffin::Log::GetClientLogger()->trace(__VA_ARGS__)
#define PF_INFO(...)		::Puffin::Log::GetClientLogger()->info(__VA_ARGS__)
#define PF_WARN(...)		::Puffin::Log::GetClientLogger()->warn(__VA_ARGS__)
#define PF_ERROR(...)		::Puffin::Log::GetClientLogger()->error(__VA_ARGS__)
#define PF_FATAL(...)		::Puffin::Log::GetClientLogger()->fatal(__VA_ARGS__)
