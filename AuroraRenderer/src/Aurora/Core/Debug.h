#pragma once

#include "aurorapch.h"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/fmt/fmt.h"

namespace Aurora
{
	class Debug
	{
	public:
		static void Init();

		static void DisableCoreLogging();
		static void DisableClientLogging();

        static void SetLogLevel(spdlog::level::level_enum level);
        static void Flush();
        static void Shutdown();

		template<typename... Args>
		static void Log(const std::string& format, Args&&... args) {
			if (enableClientLog) {
				if constexpr (sizeof...(args) == 0) {
					ClientLogger->info(format);
				}
				else {
					ClientLogger->info(fmt::format(fmt::runtime(format), std::forward<Args>(args)...));
				}
			}
		}

		template<typename... Args>
		static void Warning(const std::string& format, Args&&... args) {
			if (enableClientLog) {
				if constexpr (sizeof...(args) == 0) {
					ClientLogger->warn(format);
				}
				else {
					ClientLogger->warn(fmt::format(fmt::runtime(format), std::forward<Args>(args)...));
				}
			}
		}

		template<typename... Args>
		static void Error(const std::string& format, Args&&... args) {
			if (enableClientLog) {
				if constexpr (sizeof...(args) == 0) {
					ClientLogger->error(format);
				}
				else {
					ClientLogger->error(fmt::format(fmt::runtime(format), std::forward<Args>(args)...));
				}
			}
		}

		template<typename... Args>
		static void Trace(const std::string& format, Args&&... args) {
			if (enableClientLog) {
				if constexpr (sizeof...(args) == 0) {
					ClientLogger->trace(format);
				}
				else {
					ClientLogger->trace(fmt::format(fmt::runtime(format), std::forward<Args>(args)...));
				}
			}
		}

		template<typename... Args>
		static void Critical(const std::string& format, Args&&... args) {
			if (enableClientLog) {
				if constexpr (sizeof...(args) == 0) {
					ClientLogger->critical(format);
				}
				else {
					ClientLogger->critical(fmt::format(fmt::runtime(format), std::forward<Args>(args)...));
				}
			}
		}


		template<typename... Args>
		static void CoreLog(const std::string& format, Args&&... args) {
			if (enableCoreLog) {
				if constexpr (sizeof...(args) == 0) {
					CoreLogger->info(format);
				}
				else {
					CoreLogger->info(fmt::format(fmt::runtime(format), std::forward<Args>(args)...));
				}
			}
		}

		template<typename... Args>
		static void CoreWarning(const std::string& format, Args&&... args) {
			if (enableCoreLog) {
				if constexpr (sizeof...(args) == 0) {
					CoreLogger->warn(format);
				}
				else {
					CoreLogger->warn(fmt::format(fmt::runtime(format), std::forward<Args>(args)...));
				}
			}
		}

		template<typename... Args>
		static void CoreError(const std::string& format, Args&&... args) {
			if (enableCoreLog) {
				if constexpr (sizeof...(args) == 0) {
					CoreLogger->error(format);
				}
				else {
					CoreLogger->error(fmt::format(fmt::runtime(format), std::forward<Args>(args)...));
				}
			}
		}

		template<typename... Args>
		static void CoreTrace(const std::string& format, Args&&... args) {
			if (enableCoreLog) {
				if constexpr (sizeof...(args) == 0) {
					CoreLogger->trace(format);
				}
				else {
					CoreLogger->trace(fmt::format(fmt::runtime(format), std::forward<Args>(args)...));
				}
			}
		}

		template<typename... Args>
		static void CoreCritical(const std::string& format, Args&&... args) {
			if (enableCoreLog) {
				if constexpr (sizeof...(args) == 0) {
					CoreLogger->critical(format);
				}
				else {
					CoreLogger->critical(fmt::format(fmt::runtime(format), std::forward<Args>(args)...));
				}
			}
		}

	private:

        template<typename T>
        static std::string ToString(T&& value) {
            if constexpr (std::is_same_v<std::decay_t<T>, std::string>) {
                return value;
            }
            else if constexpr (std::is_same_v<std::decay_t<T>, const char*>) {
                return std::string(value);
            }
            else if constexpr (std::is_arithmetic_v<std::decay_t<T>>) {
                return std::to_string(value);
            }
            else if constexpr (std::is_same_v<std::decay_t<T>, bool>) {
                return value ? "true" : "false";
            }
            else {
                std::ostringstream oss;
                oss << value;
                return oss.str();
            }
        }

        template<typename T>
        static std::string BuildMessage(T&& arg) {
            return ToString(std::forward<T>(arg));
        }

        template<typename T, typename... Args>
        static std::string BuildMessage(T&& first, Args&&... args) {
            return ToString(std::forward<T>(first)) + " " + BuildMessage(std::forward<Args>(args)...);
        }


		static std::shared_ptr<spdlog::logger> CoreLogger;
		static std::shared_ptr<spdlog::logger> ClientLogger;

		static bool enableCoreLog;
		static bool enableClientLog;



	};

}
