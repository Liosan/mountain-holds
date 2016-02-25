#pragma once

#if defined (_WIN32) 
  #if defined(scripting_EXPORTS)
    #define MH_SCRIPTING_EXPORT __declspec(dllexport)
  #else
    #define MH_SCRIPTING_EXPORT __declspec(dllimport)
  #endif /* scripting_EXPORTS */
#else /* defined (_WIN32) */
 #define MH_SCRIPTING_EXPORT
#endif
