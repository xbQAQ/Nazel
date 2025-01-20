#pragma once
#include <memory>
#include "Core.h"
#include "spdlog/spdlog.h"

namespace Nazel {
	class NAZEL_API Log {
	public:
		static void Init();// 初始化
		// 向外提供获取日志对象
		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetEditorLogger() { return s_EditorLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;// Nazel项目的日志对象
		static std::shared_ptr<spdlog::logger> s_EditorLogger;// Sandbox项目的日志对象
	};
}

// Core log macros
// ...是接受函数参数包，__VA_ARGS__转发函数参数包
#define LOG_CORE_TRACE(...)    ::Nazel::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define LOG_CORE_INFO(...)     ::Nazel::Log::GetCoreLogger()->info(__VA_ARGS__)
#define LOG_CORE_WARN(...)     ::Nazel::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define LOG_CORE_ERROR(...)    ::Nazel::Log::GetCoreLogger()->error(__VA_ARGS__)
#define LOG_CORE_FATAL(...)    ::Nazel::Log::GetCoreLogger()->fatal(__VA_ARGS__) 

// Editor log macros
#define LOG_EDITOR_TRACE(...)	      ::Nazel::Log::GetEditorLogger()->trace(__VA_ARGS__)
#define LOG_EDITOR_INFO(...)	      ::Nazel::Log::GetEditorLogger()->info(__VA_ARGS__)
#define LOG_EDITOR_WARN(...)	      ::Nazel::Log::GetEditorLogger()->warn(__VA_ARGS__)
#define LOG_EDITOR_ERROR(...)	      ::Nazel::Log::GetEditorLogger()->error(__VA_ARGS__)
#define LOG_EDITOR_FATAL(...)	      ::Nazel::Log::GetEditorLogger()->fatal(__VA_ARGS__)
