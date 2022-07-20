#pragma once
// 尝试 将 delphi 翻译成 c++
#include<Windows.h>
#include<Tlhelp32.h>
class TToolhelp
{
public:
	TToolhelp(DWORD dwFlags = 0, DWORD dwProcessID = 0);
	~TToolhelp();
	BOOL CreateSnapshot(DWORD dwFlags, DWORD dwProcessID = 0);
	BOOL ProcessFirst(PPROCESSENTRY32 ppe);
	BOOL ProcessNext(PPROCESSENTRY32 ppe);
	BOOL ProcessFind(DWORD dwProcessId, PPROCESSENTRY32 ppe);
	BOOL ModuleFirst(PMODULEENTRY32 pme);
	BOOL ModuleNext(PMODULEENTRY32 pme);
	BOOL ModuleFind_BaseAddr(void *pvBaseAddr, PMODULEENTRY32 pme);
	BOOL ModuleFind_ModName(WCHAR * pszModName, PMODULEENTRY32 pme);
	BOOL ThreadFirst(PTHREADENTRY32  pte);
	BOOL ThreadNext(PTHREADENTRY32 pte);
	BOOL HeapListFirst(PHEAPLIST32 phl);
	BOOL HeapListNext(PHEAPLIST32 phl);
	int HowManyHeaps();
	BOOL HeapFirst(PHEAPENTRY32 phe, DWORD dwProcessID, DWORD dwHeapID);
	BOOL HeapNext(PHEAPENTRY32 phe);
	int HowManyBlocksInHeap(DWORD dwProcessID, DWORD dwHeapId);
	BOOL IsAHeap(HANDLE hProcess, void * pvBlock, DWORD * pdwFlags);
	BOOL EnableDebugPrivilege(BOOL fEnable = TRUE);
	BOOL ReadProcessMemory(DWORD dwProcessID, void* pvBaseAddress, void* pvBuffer,
		DWORD cbRead, DWORD *pdwNumberOfBytesRead = NULL);
private:
	HANDLE m_hSnapshot;
};
//构造函数
TToolhelp::TToolhelp(DWORD dwFlags, DWORD dwProcessID)
{
	m_hSnapshot = INVALID_HANDLE_VALUE;
	CreateSnapshot(dwFlags, dwProcessID);
}
//析构函数
TToolhelp::~TToolhelp()
{
	if (m_hSnapshot != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hSnapshot);
	}
}
// 建立快照
BOOL TToolhelp::CreateSnapshot(DWORD dwFlags, DWORD dwProcessID)
{
	if (m_hSnapshot != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hSnapshot);
	}
	if (dwFlags == 0)
		m_hSnapshot = INVALID_HANDLE_VALUE;
	else
		m_hSnapshot = CreateToolhelp32Snapshot(dwFlags, dwProcessID);
	return m_hSnapshot != INVALID_HANDLE_VALUE;
}
// 进程枚举
BOOL TToolhelp::ProcessFirst(PPROCESSENTRY32 ppe)
{
	if (Process32First(m_hSnapshot, ppe) && ppe->th32ProcessID == 0)
	{
		return ProcessNext(ppe);
	}
}
BOOL TToolhelp::ProcessNext(PPROCESSENTRY32 ppe)
{
	if (Process32First(m_hSnapshot, ppe) && ppe->th32ProcessID == 0)
	{
		return ProcessNext(ppe);
	}
}
BOOL TToolhelp::ProcessFind(DWORD dwProcessId, PPROCESSENTRY32 ppe)
{
	BOOL R = ProcessFirst(ppe);
	while (R)
	{
		if (ppe->th32ProcessID == dwProcessId)
			break;
		R = ProcessNext(ppe);
	}
	return R;
}
// 模块枚举
BOOL TToolhelp::ModuleFirst(PMODULEENTRY32 pme)
{
	return Module32First(m_hSnapshot, pme);
}
BOOL TToolhelp::ModuleNext(PMODULEENTRY32 pme)
{
	return Module32Next(m_hSnapshot, pme);
}
BOOL TToolhelp::ModuleFind_BaseAddr(void *pvBaseAddr, PMODULEENTRY32 pme)
{
	BOOL R = ModuleFirst(pme);
	while (R)
	{
		if (pme->modBaseAddr == pvBaseAddr)
			break;
		R = ModuleNext(pme);
	}
	return R;
}
BOOL TToolhelp::ModuleFind_ModName(WCHAR *pszModName, PMODULEENTRY32 pme)
{
	BOOL R = ModuleFirst(pme);
	while (R)
	{
		if (lstrcmpi(pme->szModule, (LPCSTR)pszModName) == 0 ||
			lstrcmpi(pme->szExePath, (LPCSTR)pszModName) == 0) break;
		R = ModuleNext(pme);
	}
	return R;
}
// 线程枚举
BOOL TToolhelp::ThreadFirst(PTHREADENTRY32 pte)
{
	return Thread32First(m_hSnapshot, pte);
}
BOOL TToolhelp::ThreadNext(PTHREADENTRY32 pte)
{
	return Thread32Next(m_hSnapshot, pte);
}
// 内存枚举
int TToolhelp::HowManyHeaps()
{
	HEAPLIST32 hl;
	BOOL fOK;
	int R = 0;
	hl.dwSize = sizeof(HEAPLIST32);
	fOK = HeapListFirst(&hl);
	while (fOK)
	{
		R++;
		fOK = HeapListNext(&hl);
	}
	return R;
}
int TToolhelp::HowManyBlocksInHeap(DWORD dwProcessID, DWORD dwHeapId)
{
	HEAPENTRY32 he;
	BOOL fOK;
	int	R = 0;
	he.dwSize = sizeof(he);
	fOK = HeapFirst(&he, dwProcessID, dwHeapId);
	while (fOK)
	{
		R++;
		fOK = HeapNext(&he);
	}
	return R;
}
BOOL TToolhelp::HeapListFirst(PHEAPLIST32 phl)
{
	return Heap32ListFirst(m_hSnapshot, phl);
}
BOOL TToolhelp::HeapListNext(PHEAPLIST32 phl)
{
	return Heap32ListNext(m_hSnapshot, phl);
}
BOOL TToolhelp::HeapFirst(PHEAPENTRY32 phe, DWORD dwProcessID, DWORD dwHeapID)
{
	return Heap32First(phe, dwProcessID, dwHeapID);
};
BOOL TToolhelp::HeapNext(PHEAPENTRY32 phe)
{
	return Heap32Next(phe);
}
BOOL TToolhelp::IsAHeap(HANDLE hProcess, void *pvBlock, DWORD *pdwFlags)
{
	HEAPLIST32 hl;
	HEAPENTRY32 he;
	MEMORY_BASIC_INFORMATION  mbi;
	BOOL fOkHL, fOkHE;
	BOOL R = FALSE;
	hl.dwSize = sizeof(HEAPLIST32);
	he.dwSize = sizeof(HEAPENTRY32);
	fOkHL = HeapListFirst(&hl);
	while (fOkHL)
	{
		fOkHE = HeapFirst(&he, hl.th32ProcessID, hl.th32HeapID);
		while (fOkHE)
		{
			VirtualQueryEx(hProcess, &he.dwAddress, &mbi, sizeof(MEMORY_BASIC_INFORMATION));
			if (DWORD(mbi.AllocationBase) <= DWORD(pvBlock) &&
				DWORD(pvBlock) <= DWORD(mbi.AllocationBase) + mbi.RegionSize)
			{
				*pdwFlags = hl.dwFlags;
				R = TRUE;
				return R;
			};
			fOkHE = HeapNext(&he);
		}
		fOkHL = HeapListNext(&hl);
	}
}
// 提升权限
BOOL TToolhelp::EnableDebugPrivilege(BOOL fEnable)
{
	HANDLE	hToken;
	TOKEN_PRIVILEGES  tp;
	BOOL R = FALSE;
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken))
	{
		tp.PrivilegeCount = 1;
		LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tp.Privileges[0].Luid);
		if (fEnable)
			tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		else
			tp.Privileges[0].Attributes = 0;
		AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), NULL, NULL);
		R = GetLastError() == ERROR_SUCCESS;
		CloseHandle(hToken);
	}
	return R;
}
// 内存读取
BOOL TToolhelp::ReadProcessMemory(DWORD dwProcessID, void * pvBaseAddress, void * pvBuffer,
	DWORD cbRead, DWORD *pdwNumberOfBytesRead)
{
	return Toolhelp32ReadProcessMemory(dwProcessID, pvBaseAddress, pvBuffer, cbRead, (SIZE_T *)pdwNumberOfBytesRead);
}
