#pragma once

#include <atomic>

#include "debug.h"

namespace mh
{
	namespace foundation
	{
		/// Singleton enforcing explicit initialization. 
		/// Calling Instance() before Initialize() will cause an error.
		template<class Contained>
		class Singleton
		{
		public:
			Singleton(Singleton const&) = delete;
			Singleton(Singleton&&) = delete;
			Singleton& operator=(Singleton const&) = delete;
			Singleton& operator=(Singleton&&) = delete;

			static Contained& Instance()
			{
				auto& instance = Singleton<Contained>::InstancePtr();
				if (!instance)
				{
					MH_ASSERT(false, "Illegal call to Instance() before Initialize()");
					throw std::logic_error("Illegal call to Instance() before Initialize()");
				}
				return *instance;
			}

			template<typename... Args>
			static void Initialize(const Args&... params)
			{
				auto& instance = Singleton<Contained>::InstancePtr();
				if (instance)
				{
					MH_ASSERT(false, "Illegal double call to Initialize()");
					throw std::logic_error("Illegal double call to Initialize()");
				}
				instance.reset(new Contained(params...));
			}

			static void Destroy()
			{
				auto& instance = Singleton<Contained>::InstancePtr();
				if (!instance)
				{
					MH_ASSERT(false, "Illegal call to Destroy() on uninitialized singleton");
					throw std::logic_error("Illegal call to Destroy() on uninitialized singleton");
				}
				instance.reset(nullptr);
			}
		protected:
			Singleton()
			{}
			virtual ~Singleton()
			{}
		private:
			static std::unique_ptr<Contained>& InstancePtr()
			{
				static std::unique_ptr<Contained> instance;
				return instance;
			}
		};
	}
}