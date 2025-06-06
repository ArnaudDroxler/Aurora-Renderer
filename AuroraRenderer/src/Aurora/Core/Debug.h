#pragma once

#include "aurorapch.h"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

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
		static void Log(Args&&... args) {
			if (sizeof...(args) == 0) {
				ClientLogger->info("");
			}
			else {
				ClientLogger->info(BuildMessage(std::forward<Args>(args)...));
			}
		}

        template<typename... Args>
        static void Warning(Args&&... args) {
            if (sizeof...(args) == 0) {
                ClientLogger->warn("");
            }
            else {
                ClientLogger->warn(BuildMessage(std::forward<Args>(args)...));
            }
        }

        template<typename... Args>
        static void Error(Args&&... args) {
            if (sizeof...(args) == 0) {
                ClientLogger->error("");
            }
            else {
                ClientLogger->error(BuildMessage(std::forward<Args>(args)...));
            }
        }

        template<typename... Args>
        static void Trace(Args&&... args) {
            if (sizeof...(args) == 0) {
                ClientLogger->trace("");
            }
            else {
                ClientLogger->trace(BuildMessage(std::forward<Args>(args)...));
            }
        }

        template<typename... Args>
        static void Critical(Args&&... args) {
            if (sizeof...(args) == 0) {
                ClientLogger->critical("");
            }
            else {
                ClientLogger->critical(BuildMessage(std::forward<Args>(args)...));
            }
        }

        template<typename... Args>
        static void CoreLog(Args&&... args) {
            if (sizeof...(args) == 0) {
                CoreLogger->info("");
            }
            else {
                CoreLogger->info(BuildMessage(std::forward<Args>(args)...));
            }
        }

        template<typename... Args>
        static void CoreWarning(Args&&... args) {
            if (sizeof...(args) == 0) {
                CoreLogger->warn("");
            }
            else {
                CoreLogger->warn(BuildMessage(std::forward<Args>(args)...));
            }
        }

        template<typename... Args>
        static void CoreError(Args&&... args) {
            if (sizeof...(args) == 0) {
                CoreLogger->error("");
            }
            else {
                CoreLogger->error(BuildMessage(std::forward<Args>(args)...));
            }
        }

        template<typename... Args>
        static void CoreTrace(Args&&... args) {
            if (sizeof...(args) == 0) {
                CoreLogger->trace("");
            }
            else {
                CoreLogger->trace(BuildMessage(std::forward<Args>(args)...));
            }
        }

        template<typename... Args>
        static void CoreCritical(Args&&... args) {
            if (sizeof...(args) == 0) {
                CoreLogger->critical("");
            }
            else {
                CoreLogger->critical(BuildMessage(std::forward<Args>(args)...));
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
