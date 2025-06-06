#include "aurorapch.h"
#include "Debug.h"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace Aurora
{
	std::shared_ptr<spdlog::logger> Debug::CoreLogger;
	std::shared_ptr<spdlog::logger> Debug::ClientLogger;

	bool Debug::enableCoreLog;
	bool Debug::enableClientLog;


	void Debug::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");

		auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("debug.log", true);
		auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		CoreLogger = std::make_shared<spdlog::logger>("Aurora",
			spdlog::sinks_init_list{ console_sink, file_sink });


		ClientLogger = std::make_shared<spdlog::logger>("Application",
			spdlog::sinks_init_list{ console_sink, file_sink });

		SetLogLevel(spdlog::level::trace);

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

	void Debug::SetLogLevel(spdlog::level::level_enum level) {
		CoreLogger->set_level(level);
		ClientLogger->set_level(level);
	}
	void Debug::Flush()
	{
		CoreLogger->flush();
		ClientLogger->flush();
	}
	void Debug::Shutdown()
	{
		Flush();

		spdlog::drop_all();
	}
}
