#pragma once
#include <memory>
#include "Core.h"
#include "spdlog/spdlog.h"

namespace Nazel {
	class NAZEL_API Log {
	public:
		static void Init();// ��ʼ��
		// �����ṩ��ȡ��־����
		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetEditorLogger() { return s_EditorLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;// Nazel��Ŀ����־����
		static std::shared_ptr<spdlog::logger> s_EditorLogger;// Sandbox��Ŀ����־����
	};
}

// Core log macros
// ...�ǽ��ܺ�����������__VA_ARGS__ת������������
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
