#include "aurorapch.h"
#include "Debug.h"


namespace Aurora
{
	std::shared_ptr<spdlog::logger> Debug::CoreLogger;
	std::shared_ptr<spdlog::logger> Debug::ClientLogger;

	bool Debug::enableCoreLog;
	bool Debug::enableClientLog;


	void Debug::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		CoreLogger = spdlog::stdout_color_mt("Aurora");
		CoreLogger->set_level(spdlog::level::trace);

		ClientLogger = spdlog::stdout_color_mt("Application");
		ClientLogger->set_level(spdlog::level::trace);

		enableCoreLog = true;
		enableClientLog = true;
	}

	void Debug::DisableCoreLogging()
	{
		enableCoreLog = false;
	}

	void Debug::DisableClientLogging()
	{
		enableClientLog = false;
	}

	void Debug::CoreLog(const char * str)
	{
		if (enableCoreLog) {
			CoreLogger->trace(str);
		}
	}

	void Debug::CoreInfo(const char * str)
	{
		if (enableCoreLog) {
			CoreLogger->info(str);
		}
	}

	void Debug::CoreWarning(const char * str)
	{
		if (enableCoreLog) {
			CoreLogger->warn(str);
		}
	}

	void Debug::CoreError(const char * str)
	{
		if (enableCoreLog) {
			CoreLogger->error(str);
		}
	}

	void Debug::CoreCritical(const char * str)
	{
		if (enableCoreLog) {
			CoreLogger->critical(str);
		}
	}

	void Debug::Log(const char * str)
	{
		if (enableClientLog) {
			ClientLogger->trace(str);
		}
	}

	void Debug::Info(const char * str)
	{
		if (enableClientLog) {
			ClientLogger->info(str);
		}
	}

	void Debug::Warning(const char * str)
	{
		if (enableClientLog) {
			ClientLogger->warn(str);
		}
	}

	void Debug::Error(const char * str)
	{
		if(enableClientLog) {
			ClientLogger->error(str);
		}
	}

	void Debug::Critical(const char * str)
	{
		if (enableClientLog) {
			ClientLogger->critical(str);
		}
	}
}
