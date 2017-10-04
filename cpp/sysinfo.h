#ifndef SYSINFO_H
#define SYSINFO_H

#ifdef _DLL_DECL
	#define DLL_DECL __declspec(dllexport)
#else
	#define DLL_DECL __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C"{
#endif

int sys_info_init(IWbemLocator **locator, IWbemServices **services);
int sys_info_uninit(IWbemLocator **locator, IWbemServices **services);
int sys_info_exec_wql(IWbemServices **services, char *wql, char *prop, char *value);
int DLL_DECL sys_info_exec(char *value);

#ifdef __cplusplus
}
#endif

#endif