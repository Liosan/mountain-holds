#pragma once

#if defined (_WIN32) 
  #if defined(foundation_EXPORTS)
    #define MH_FOUNDATION_EXPORT __declspec(dllexport)
  #else
    #define MH_FOUNDATION_EXPORT __declspec(dllimport)
  #endif /* MyLibrary_EXPORTS */
#else /* defined (_WIN32) */
 #define MH_FOUNDATION_EXPORT
#endif
