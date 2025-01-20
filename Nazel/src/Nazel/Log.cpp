#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Nazel {
	// 由于这两属性是静态的，得在cpp里定义，不然报无法解析的外部符号错误
	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_EditorLogger;
	void Log::Init()
	{
		// 自定义日志格式：%^颜色起始点、%T时间戳(HH:MM:SS)、%n日志名、%@日志的文件及行数 %v实际的文本、%$颜色结束点
		spdlog::set_pattern("%^[%T] %n %@: %v%$");
		s_CoreLogger = spdlog::stdout_color_mt("Nazel Core");// Nazel 对应自定义格式的%n
		s_CoreLogger->set_level(spdlog::level::trace);

		s_EditorLogger = spdlog::stdout_color_mt("Nazel Editor");// APP 对应自定义格式的%n
		s_EditorLogger->set_level(spdlog::level::trace);
	}
}
