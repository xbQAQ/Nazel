#pragma once

#include <memory>

#ifdef NZ_PLATFORM_WINDOWS
	#ifdef NZ_DYNAMIC_LINK
		#ifdef NZ_BUILD_DLL
			#define NAZEL_API __declspec(dllexport)
		#else
			#define NAZEL_API __declspec(dllimport)
		#endif
	#else
		#define NAZEL_API
	#endif
#else
	#error Nazel only supports Windows!
#endif

#ifdef NZ_DEBUG
#define NZ_ENABLE_ASSERTS
#endif // NZ_DEBUG

#ifdef NZ_ENABLE_ASSERTS
#define NZ_ASSERT(x, ...) { if(!(x)) { LOG_EDITOR_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#define NZ_CORE_ASSERT(x, ...) { if(!(x)) { LOG_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
#define NZ_ASSERT(x, ...)
#define NZ_CORE_ASSERT(x, ...)
#endif


#define BIT(x)(1 << x)
#define BIND_FUNCTION(X) std::bind(&X, this, std::placeholders::_1)

namespace Nazel {
template <typename T>
using Scope = std::unique_ptr<T>;

template <typename T>
using Ref = std::shared_ptr<T>;
}