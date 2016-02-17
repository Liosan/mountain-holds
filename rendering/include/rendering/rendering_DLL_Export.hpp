#pragma once

#if defined (_WIN32) 
  #if defined(rendering_EXPORTS)
    #define MH_RENDERING_EXPORT __declspec(dllexport)
  #else
    #define MH_RENDERING_EXPORT __declspec(dllimport)
  #endif /* rendering_EXPORTS */
#else /* defined (_WIN32) */
 #define MH_RENDERING_EXPORT
#endif
