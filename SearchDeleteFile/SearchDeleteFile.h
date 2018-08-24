// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 SEARCHDELETEFILE_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// SEARCHDELETEFILE_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef SEARCHDELETEFILE_EXPORTS
#define SEARCHDELETEFILE_API EXTERN_C __declspec(dllexport)
#else
#define SEARCHDELETEFILE_API EXTERN_C __declspec(dllimport)
#endif


#include "../Common/Funcs.h"
#include "DeletedFile.h"
#include "FileRecovery.h"

#define VL_MAGIC_NUMBER (0x23E72DAC)

SEARCHDELETEFILE_API char* SearchDeletedFileInfo(const int magic, const char* diskPartition, const char* fileType);

SEARCHDELETEFILE_API void DestroyBuffer(char* buff);

SEARCHDELETEFILE_API int DeletedFileRecovery(const int magic, const char* fileName, const char* fileAddr, const char* diskPartition, const char* recoveryPath);


