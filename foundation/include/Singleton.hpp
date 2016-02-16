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
				auto& instance = Singleton<Contained>::InstanceImpl();
				if (!instance.singletonInitializeCalled_)
				{
					MH_ASSERT(false, "Illegal call to Instance() before Initialize()");
					throw std::logic_error("Illegal call to Instance() before Initialize()");
				}
				return instance;
			}

			static void Initialize()
			{
				auto& instance = Singleton<Contained>::InstanceImpl();
				if (instance.singletonInitializeCalled_)
				{
					MH_ASSERT(false, "Illegal double call to Initialize()");
					throw std::logic_error("Illegal double call to Initialize()");
				}
				instance.singletonInitializeCalled_ = true;
			}
		protected:
			Singleton() :
				singletonInitializeCalled_(false)
			{}

			virtual ~Singleton()
			{}
		private:
			static Contained& InstanceImpl()
			{
				static Contained instance;
				return instance;
			}

			bool singletonInitializeCalled_;
		};
	}
}