#pragma once
#include <Misc/MessageDialog.h>

#ifdef _MSC_VER
#define __func__ __FUNCTION__
#endif

#define DO_PRINT_LOG_AFTER_CHECK_FALSE 1
#define FUNC_NAME *FString(__func__)
#define AZ_LOG(Format, ...) \
{ \
    SET_WARN_COLOR(COLOR_CYAN);\
    const FString Msg = FString::Printf(TEXT(Format), ##__VA_ARGS__); \
    if (Msg == "") \
    { \
        UE_LOG(LogClass, Log, TEXT("%s() : %s"), FUNC_NAME, *GetNameSafe(this));\
    } \
    else \
    { \
        UE_LOG(LogClass, Log, TEXT("%s() : %s"), FUNC_NAME, *Msg);\
    } \
    CLEAR_WARN_COLOR();\
}
