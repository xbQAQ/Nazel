#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Nazel {
	// �������������Ǿ�̬�ģ�����cpp�ﶨ�壬��Ȼ���޷��������ⲿ���Ŵ���
	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_EditorLogger;
	void Log::Init()
	{
		// �Զ�����־��ʽ��%^��ɫ��ʼ�㡢%Tʱ���(HH:MM:SS)��%n��־����%@��־���ļ������� %vʵ�ʵ��ı���%$��ɫ������
		spdlog::set_pattern("%^[%T] %n %@: %v%$");
		s_CoreLogger = spdlog::stdout_color_mt("Nazel Core");// Nazel ��Ӧ�Զ����ʽ��%n
		s_CoreLogger->set_level(spdlog::level::trace);

		s_EditorLogger = spdlog::stdout_color_mt("Nazel Editor");// APP ��Ӧ�Զ����ʽ��%n
		s_EditorLogger->set_level(spdlog::level::trace);
	}
}
