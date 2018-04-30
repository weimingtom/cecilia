//20171119
//20180109
#include <windows.h>
#include <direct.h>
#include <ctype.h>

#include "python.h"
#include "importdl.h"

const struct filedescr _PyImport_DynLoadFiletab[] = {
#ifdef _DEBUG
	{"_d.pyd", "rb", C_EXTENSION},
	{"_d.dll", "rb", C_EXTENSION},
#else
	{".pyd", "rb", C_EXTENSION},
	{".dll", "rb", C_EXTENSION},
#endif
	{0, 0}
};


static int strcasecmp(char *string1, char *string2)
{ 
	int first, second;
	do 
	{
		first  = tolower(*string1);
		second = tolower(*string2);
		string1++;
		string2++;
	} while (first && first == second);

	return (first - second);
} 

#define DWORD_AT(mem) (*(DWORD *)(mem))
#define WORD_AT(mem)  (*(WORD *)(mem))

static char *GetPythonImport (HINSTANCE hModule)
{
	unsigned char *dllbase, *import_data, *import_name;
	DWORD pe_offset, opt_offset;
	WORD opt_magic;
	int num_dict_off, import_off;

	if (hModule == NULL) 
	{
		return NULL;
	}

	dllbase = (unsigned char *)hModule;
	pe_offset = DWORD_AT(dllbase + 0x3C);

	if (memcmp(dllbase + pe_offset, "PE\0\0", 4)) 
	{
		return NULL;
	}
	
	opt_offset = pe_offset + 4 + 20;
	opt_magic = WORD_AT(dllbase + opt_offset);
	if (opt_magic == 0x10B) 
	{
		num_dict_off = 92;
		import_off = 104;
	} 
	else if (opt_magic == 0x20B) 
	{
		num_dict_off = 108;
		import_off = 120;
	} 
	else 
	{
		return NULL;
	}

	if (DWORD_AT(dllbase + opt_offset + num_dict_off) >= 2) 
	{
		import_data = dllbase + DWORD_AT(dllbase +
						 opt_offset +
						 import_off);
		while (DWORD_AT(import_data)) 
		{
			import_name = dllbase + DWORD_AT(import_data + 12);
			if (strlen(import_name) >= 6 &&
			    !strncmp(import_name,"python", 6)) 
			{
				char *pch;
				pch = import_name + 6;
				while (*pch && *pch != '.') 
				{
					if (*pch >= '0' && *pch <= '9') 
					{
						pch++;
					} 
					else 
					{
						pch = NULL;
						break;
					}
				}
				if (pch) 
				{
					return import_name;
				}
			}
			import_data += 20;
		}
	}
	return NULL;
}


dl_funcptr _PyImport_GetDynLoadFunc(const char *fqname, const char *shortname,
				    const char *pathname, FILE *fp)
{
	dl_funcptr p;
	char funcname[258], *import_python;

	PyOS_snprintf(funcname, sizeof(funcname), "init%.200s", shortname);

	{
		HINSTANCE hDLL = NULL;
		char pathbuf[260];
		LPTSTR dummy;
		if (GetFullPathName(pathname,
				    sizeof(pathbuf),
				    pathbuf,
				    &dummy))
		{
			hDLL = LoadLibraryEx(pathname, NULL,
					     LOAD_WITH_ALTERED_SEARCH_PATH);
		}
		if (hDLL == NULL)
		{
			char errBuf[256];
			unsigned int errorCode;

			char theInfo[256];
			int theLength;

			errorCode = GetLastError();

			theLength = FormatMessage(
				FORMAT_MESSAGE_FROM_SYSTEM,
				NULL,
				errorCode,
				0,
				(LPTSTR) theInfo,
				sizeof(theInfo),
				NULL);
			if (theLength == 0) 
			{
				PyOS_snprintf(errBuf, sizeof(errBuf),
				      "DLL load failed with error code %d",
					      errorCode);
			} 
			else 
			{
				size_t len;
				if (theLength >= 2 &&
				    theInfo[theLength - 2] == '\r' &&
				    theInfo[theLength - 1] == '\n') 
				{
					theLength -= 2;
					theInfo[theLength] = '\0';
				}
				strcpy(errBuf, "DLL load failed: ");
				len = strlen(errBuf);
				strncpy(errBuf + len, theInfo,
					sizeof(errBuf) - len);
				errBuf[sizeof(errBuf) - 1] = '\0';
			}
			PyErr_SetString(PyExc_ImportError, errBuf);
			return NULL;
		} 
		else 
		{
			char buffer[256];

			PyOS_snprintf(buffer, sizeof(buffer), "python%d%d.dll",
				PY_MAJOR_VERSION, PY_MINOR_VERSION);
			import_python = GetPythonImport(hDLL);

			if (import_python &&
			    strcasecmp(buffer,import_python)) 
			{
				PyOS_snprintf(buffer, sizeof(buffer),
					      "Module use of %.150s conflicts "
					      "with this version of Python.",
					      import_python);
				PyErr_SetString(PyExc_ImportError, buffer);
				FreeLibrary(hDLL);
				return NULL;
			}
		}
		p = GetProcAddress(hDLL, funcname);
	}

	return p;
}
