#pragma once

#if defined (_WIN32) 
  #if defined(map_EXPORTS)
    #define MH_MAP_EXPORT __declspec(dllexport)
  #else
    #define MH_MAP_EXPORT __declspec(dllimport)
  #endif /* map_EXPORTS */
#else /* defined (_WIN32) */
 #define MH_MAP_EXPORT
#endif
