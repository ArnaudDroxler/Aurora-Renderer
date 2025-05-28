#pragma once

#include "aurorapch.h"

#include "spdlog\spdlog.h"
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Aurora
{
	class Debug
	{
	public:
		static void Init();

		static void DisableCoreLogging();
		static void DisableClientLogging();

		static void CoreLog(const char* str);
		static void CoreInfo(const char* str);
		static void CoreWarning(const char* str);
		static void CoreError(const char* str);
		static void CoreCritical(const char* str);

		static void Log(const char* str);
		static void Info(const char* str);
		static void Warning(const char* str);
		static void Error(const char* str);
		static void Critical(const char* str);

	private:
		static std::shared_ptr<spdlog::logger> CoreLogger;
		static std::shared_ptr<spdlog::logger> ClientLogger;

		static bool enableCoreLog;
		static bool enableClientLog;

	};

}
