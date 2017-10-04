#define _WIN32_DCOM
#define _DLL_DECL
#include <comdef.h>
#include <Wbemidl.h>
#include "sysinfo.h"

int sys_info_init(IWbemLocator **locator, IWbemServices **services){
	HRESULT hres;
	hres =  CoInitialize(0); 
	if (FAILED(hres)){		
		return -1;
	}

	hres = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID *) locator);
	if (FAILED(hres)){		
		CoUninitialize();
		return -1;
	}

	hres = (*locator)->ConnectServer(_bstr_t(L"ROOT\\CIMV2"), NULL, NULL, 0, NULL, 0, 0, services);
	if (FAILED(hres)){		
		(*locator)->Release();    
		CoUninitialize();	
		return -1;
	}

	hres = CoSetProxyBlanket(*services, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);
	if (FAILED(hres)){
		(*services)->Release();
		(*locator)->Release();     
		CoUninitialize();
		return -1;
	}

	return 0;	
}

int sys_info_uninit(IWbemLocator **locator, IWbemServices **services){
	(*services)->Release();
	(*locator)->Release();	
	CoUninitialize();
	return 0;
}

int sys_info_exec_wql(IWbemServices **services, char *wql, char *prop, char *value){
	IEnumWbemClassObject *enumerator = NULL;
	IWbemClassObject *object = NULL;

	HRESULT hres;	
	hres = (*services)->ExecQuery(bstr_t("WQL"), bstr_t(wql), WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL, &enumerator);
	if (FAILED(hres)){
		return -1;
	}
	ULONG uReturn = 0;
	while (enumerator){
		HRESULT hr = enumerator->Next(WBEM_INFINITE, 1, &object, &uReturn);
		if(0 == uReturn){
			break;
		}
		VARIANT variant;
		hr = object->Get(bstr_t(prop), 0, &variant, 0, 0);
		strcat(value, (char*)bstr_t(variant.bstrVal));
		VariantClear(&variant);
	}
	enumerator->Release();
	object->Release();	
	return 0;
}

int sys_info_exec(char *value){
	IWbemLocator *locator = NULL;
	IWbemServices *services = NULL;	

	int res;
	res = sys_info_init(&locator, &services);
	if(res != 0){
		return res;
	}

	res = sys_info_exec_wql(&services, "SELECT ProcessorId FROM Win32_Processor", "ProcessorId", value);
	if(res != 0){
		return res;
	}

	res = sys_info_exec_wql(&services, "SELECT SerialNumber FROM Win32_BIOS", "SerialNumber", value);
	if(res != 0){
		return res;
	}

	res = sys_info_exec_wql(&services, "SELECT SerialNumber FROM Win32_BaseBoard", "SerialNumber", value);
	if(res != 0){
		return res;
	}

	res = sys_info_exec_wql(&services, "SELECT Product FROM Win32_BaseBoard", "Product", value);
	if(res != 0){
		return res;
	}

	res = sys_info_uninit(&locator, &services);
	if(res != 0){
		return res;
	}

	return 0;
}

//int main(int argc, char **argv){
//	char value[1024] = "\0";
//	int res = -99;
//	res = sys_info_exec(value);
//	if(res == 0){
//		printf("%s\n",value);
//	}else{
//		printf("ERROR: %d\n", res);
//	}		
//	return 0; 
//}