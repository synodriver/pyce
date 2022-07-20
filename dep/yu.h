#pragma once
#include <stdio.h>
#include <windows.h>
#include <iostream>
#include <string.h>
#include <cstdlib>
#include <Tlhelp32.h>
#include <string>
#include "Getmode.h"
using namespace std;
//参数分别为：进程句柄、特征码、偏移、读取长度、开始扫描位地置、扫描结束位置
uintptr_t hanshu_dizhi; //记录特征码对应的地址
uintptr_t ScanAddress(HANDLE process, char *markCode, int nOffset, unsigned long dwReadLen = 4, uintptr_t StartAddr = 0x400000, uintptr_t EndAddr = 0x7FFFFFFF, int InstructionLen = 0)
{
	//************处理特征码，转化成字节*****************
	if (strlen(markCode) % 2 != 0) return 0;
	//特征码长度
	int len = strlen(markCode) / 2;  //获取代码的字节数

	//将特征码转换成byte型 保存在m_code 中
	BYTE *m_code = new BYTE[len];
	for (int i = 0; i < len; i++)
	{
		//定义可容纳单个字符的一种基本数据类型。
		char c[] = { markCode[i * 2], markCode[i * 2 + 1], '\0' };
		//将参数nptr字符串根据参数base来转换成长整型数
		m_code[i] = (BYTE)::strtol(c, NULL, 16);
	}
	//每次读取游戏内存数目的大小
	const DWORD pageSize = 4096;

	// 查找特征码 /
	//每页读取4096个字节
	BYTE *page = new BYTE[pageSize];
	uintptr_t tmpAddr = StartAddr;
	//定义和特征码一样长度的标识
	int compare_one = 0;

	while (tmpAddr <= EndAddr)
	{
		::ReadProcessMemory(process, (LPCVOID)tmpAddr, page, pageSize, 0); //读取0x400000的内存数据，保存在page，长度为pageSize

		//在该页中查找特征码
		for (int i = 0; i < pageSize; i++)
		{
			if (m_code[0] == page[i])//有一个字节与特征码的第一个字节相同，则搜索
			{
				for (int j = 0; j<len - 1; j++)
				{
					if (m_code[j + 1] == page[i + j + 1])//比较每一个字节的大小，不相同则退出
					{
						compare_one++;
					}
					else
					{
						compare_one = 0;
						break;
					}//如果下个对比的字节不相等，则退出，减少资源被利用
				}

				if ((compare_one + 1) == len)
				{
					// 找到特征码处理
					//赋值时要给初始值，避免冲突
					uintptr_t dwAddr = tmpAddr + i + nOffset;
					uintptr_t ullRet = 0;
					::ReadProcessMemory(process, (void*)dwAddr, &ullRet, dwReadLen, 0);
					//cout<<dwAddr<<endl;
					//这里的dwAddr已经对应的是搜索到的地址
					//地址输出的也是10进制    需要转化为16进制
					hanshu_dizhi = dwAddr;//记录地址
					if (InstructionLen)
					{
						ullRet += dwAddr + dwReadLen;
					}

					return ullRet;
				}
			}
		}

		tmpAddr = tmpAddr + pageSize - len;//下一页搜索要在前一页最后长度len 开始查找，避免两页交接中间有特征码搜索不出来
	}

	return 0;
}

//10到16 用法在下面
/*
int ret = 10;
string result = tentosixteen(ret);
cout << result << endl;
*/

int quzheng(unsigned int tendoc)
{
	int qian = tendoc / 16;
	return qian;
}

string huanzhi(int numtype)
{
	string us = std::to_string(numtype);
	if (us == "0")
	{
		string result = "0";
		return result;
	}
	else if (us == "1")
	{
		string result = "1";
		return result;
	}
	else if (us == "2")
	{
		string result = "2";
		return result;
	}
	else if (us == "3")
	{
		string result = "3";
		return result;
	}
	else if (us == "4")
	{
		string result = "4";
		return result;
	}
	else if (us == "5")
	{
		string result = "5";
		return result;
	}
	else if (us == "6")
	{
		string result = "6";
		return result;
	}
	else if (us == "7")
	{
		string result = "7";
		return result;
	}
	else if (us == "8")
	{
		string result = "8";
		return result;
	}
	else if (us == "9")
	{
		string result = "9";
		return result;
	}
	else if (us == "10")
	{
		string result = "A";
		return result;
	}
	else if (us == "11")
	{
		string result = "B";
		return result;
	}
	else if (us == "12")
	{
		string result = "C";
		return result;
	}
	else if (us == "13")
	{
		string result = "D";
		return result;
	}
	else if (us == "14")
	{
		string result = "E";
		return result;
	}
	else if (us == "15")
	{
		string result = "F";
		return result;
	}
	else if (us == "16")
	{
		string result = "10";
		return result;
	}
}

string daoxu(string sixteendoc)
{
	int length = strlen(sixteendoc.c_str());
	int i = 0;
	string ret;
	string result;
	while (i < length || i == length)
	{
		ret = sixteendoc.substr(length - i, 1);
		result = result + ret;
		//cout << ret << endl;
		i = i + 1;
	}
	return result;
}

string tentosixteen(unsigned int tendoc)
{
	//int tendoc = 426;
	int zhengshu = quzheng(tendoc);
	string sixteendoc = "";
	while (true)
	{
		if (zhengshu > 16)
		{
			int cha = tendoc - zhengshu * 16;
			tendoc = zhengshu;
			//cout << cha << endl;
			string result = huanzhi(cha);
			sixteendoc = sixteendoc + result;
			zhengshu = quzheng(tendoc);
		}
		else if (zhengshu == 16)
		{
			int cha = tendoc - zhengshu * 16;
			tendoc = zhengshu;
			//cout << cha << endl;
			string result = huanzhi(cha);
			sixteendoc = sixteendoc + result;
			zhengshu = quzheng(tendoc);
		}
		else
		{
			int cha = tendoc - zhengshu * 16;
			tendoc = zhengshu;
			//cout << cha << endl;
			//cout << zhengshu << endl;//最后一个整数也是差值
			string result1 = huanzhi(cha);
			string result2 = huanzhi(zhengshu);
			sixteendoc = sixteendoc + result1 + result2;
			break;
		}
	}
	string oversixdoc = daoxu(sixteendoc);//这个时候sixteendoc是反着的所以倒序倒为正了
	//cout << "oversixdoc:" << oversixdoc << endl;
	int length = strlen(oversixdoc.c_str());
	string tianchong;
	//cout <<"length:"  <<  length << endl;
	int i = 8 - length;
	while (true)
	{
		if (i > 0)
		{
			tianchong = tianchong + "0";
		}
		else
		{
			break;
		}
		i = i - 1;
	}
	//cout << "tianchong" << tianchong << endl;
	string result = tianchong + oversixdoc;
	//string result = tianchong + oversixdoc;
	return result;
}


//16到10 用法在下面
/*
string typesixteen = "ABCDEFAB";
unsigned int result =  sixteentoten(typesixteen);
cout << "结果:" << result << endl;
*/

int beishu(int length)
{
	int ji = 1;
	int length1 = 7 - length;
	int i = 0;
	while (true)
	{
		if (i < length1)
		{
			ji = ji * 16;
		}
		else
		{
			break;
		}
		i = i + 1;
	}
	//cout << "ji : " << ji << endl;
	return ji;
}

int callback(string ret, int i)
{
	unsigned int callback = 1;
	//cout << "i:" << i << "ret" << ret << endl;
	//cout << "beishu : " << beilv << endl;
	if (ret == "0")
	{
		int beilv = beishu(i);
		callback = beilv * 0;
	}
	else if (ret == "1")
	{
		int beilv = beishu(i);
		callback = beilv * 1;
	}
	else if (ret == "2")
	{
		int beilv = beishu(i);
		callback = beilv * 2;
	}
	else if (ret == "3")
	{
		int beilv = beishu(i);
		callback = beilv * 3;
	}
	else if (ret == "4")
	{
		int beilv = beishu(i);
		callback = beilv * 4;
	}
	else if (ret == "5")
	{
		int beilv = beishu(i);
		callback = beilv * 5;
	}
	else if (ret == "6")
	{
		int beilv = beishu(i);
		callback = beilv * 6;
	}
	else if (ret == "7")
	{
		int beilv = beishu(i);
		callback = beilv * 7;
	}
	else if (ret == "8")
	{
		int beilv = beishu(i);
		callback = beilv * 8;
	}
	else if (ret == "9")
	{
		int beilv = beishu(i);
		callback = beilv * 9;
	}
	else if (ret == "A" || ret == "a")
	{
		int beilv = beishu(i);
		callback = beilv * 10;
	}
	else if (ret == "B" || ret == "b")
	{
		int beilv = beishu(i);
		callback = beilv * 11;
	}
	else if (ret == "C" || ret == "c")
	{
		int beilv = beishu(i);
		callback = beilv * 12;
	}
	else if (ret == "D" || ret == "d")
	{
		int beilv = beishu(i);
		callback = beilv * 13;
	}
	else if (ret == "E" || ret == "e")
	{
		int beilv = beishu(i);
		callback = beilv * 14;
	}
	else if (ret == "F" || ret == "f")
	{
		int beilv = beishu(i);
		callback = beilv * 15;
	}
	return callback;
}

int sixteentoten(string ten)
{
	int length = strlen(ten.c_str());
	int i = 0;
	int answer = 0;
	int reback;
	string ret;
	while (true)
	{
		if (i < length)
		{
			ret = ten.substr(i, 1);//获取到了每一个位数上的具体值
			reback = callback(ret, i);
			answer = answer + reback;
		}
		else
		{
			break;
		}
		i = i + 1;
	}



	string result = "1";
	return answer;
}




/*Char*转TCHAR*/
LPWSTR ConvertCharToLPWSTR(const char* szString)

{

	int dwLen = strlen(szString) + 1;

	int nwLen = MultiByteToWideChar(CP_ACP, 0, szString, dwLen, NULL, 0);//算出合适的长度

	LPWSTR lpszPath = new WCHAR[dwLen];

	MultiByteToWideChar(CP_ACP, 0, szString, dwLen, lpszPath, nwLen);

	return lpszPath;

}

/*string转LPCWSTR*/
LPCWSTR stringToLPCWSTR(std::string orig)
{
	size_t origsize = orig.length() + 1;
	const size_t newsize = 100;
	size_t convertedChars = 0;
	wchar_t *wcstring = (wchar_t *)malloc(sizeof(wchar_t)*(orig.length() - 1));
	mbstowcs_s(&convertedChars, wcstring, origsize, orig.c_str(), _TRUNCATE);
	return wcstring;
}


//DLL注入
/// <summary>
/// 注入DLL文件
/// </summary>
/// <param name="DllFullPath">DLL文件的全路径</param>
/// <param name="dwRemoteProcessId">要注入的程序的PID</param>
/// <returns></returns>
BOOL InjectDLL(const wchar_t* DllFullPath, const DWORD dwRemoteProcessId)
{
	// 计算路径的字节数
	int pathSize = (wcslen(DllFullPath) + 1) * sizeof(wchar_t);
	// 获取句柄后，可以完全控制进程
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwRemoteProcessId);
	if (hProcess == NULL) {
		//cout << "获取句柄失败" << endl;
		return FALSE;
	}
	// TerminateProcess(hProcess, 0);//关闭句柄对象
	// 实现注入
	// 1.首先要提升权限，打开进程的访问令牌
	// 【参数1】当前程序
	// 【参数2】权限，可添加的权限|可查询的权限
	HANDLE hToken;
	if (FALSE == OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES |
		TOKEN_QUERY, &hToken)) {
		// 权限修改失败
		//cout << "权限修改失败" << endl;
		return FALSE;
	}
	//2.查看与进程相关的特权信息
	LUID luid;
	if (FALSE == LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid)) {
		// 特权信息查询失败
		//cout << "特权信息查询失败" << endl;
		return FALSE;
	};
	//3.调节进程的访问令牌的特权属性
	// 这几行代码固定不变
	TOKEN_PRIVILEGES tkp;
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Luid = luid;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; // 打开特权
	// 【参数1】访问令牌
	// 【参数2】是否禁用特权
	// 【参数3】新特权所占的字节数
	// 【参数4】原来的特权是否需要保存
	// 【参数5】原特权的长度
	if (FALSE == AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(tkp), NULL, NULL))
	{
		// 提升特权失败
		//cout << "提升特权失败" << endl;
		return FALSE;
	};

	//在远程进程中申请内存空间
	// 【参数1】程序的句柄对象
	// 【参数2】申请的内存地址，由系统分配，所以为NULL
	// 【参数3】申请的内存长度
	// 【参数4】调用物理存储器
	// 【参数5】这块内存可读可写，可执行
	// 【返回】申请到的地址
	LPVOID lpAddr = VirtualAllocEx(hProcess, NULL, pathSize, MEM_COMMIT,
		PAGE_EXECUTE_READWRITE);
	if (lpAddr == NULL) {
		// 在远程进程中申请内存失败
		//cout << "在远程进程中申请内存失败" << endl;
		return FALSE;
	}
	// 把DLL路径写入到远程进程中
	// 强行修改程序的内存
	// 【参数1】程序的句柄
	// 【参数2】申请到的内存首地址
	// 【参数3】写入的内容
	// 【参数4】要写入的字节数
	// 【参数5】
	if (FALSE == WriteProcessMemory(hProcess, lpAddr, DllFullPath,
		pathSize, NULL)) {
		// 在远程进程中写入数据失败
		//cout << "在远程进程中写入数据失败" << endl;
		return FALSE;
	};


	// 调用Kernel32.dll中的LoadLibraryW方法用以加载DLL文件
	PTHREAD_START_ROUTINE pfnStartAssr =
		(PTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle((LPCSTR)L"Kernel32.dll"),
		"LoadLibraryW");

	// 在远程进程中开辟线程
	// 【参数1】远程线程的句柄
	// 【参数2】线程属性。NULL表示使用默认属性
	// 【参数3】堆栈大小。0代表默认
	// 【参数4】加载DLL文件的对象
	// 【参数5】加载文件的路径
	// 【参数6】延迟时间。0代表立即启动
	// 【参数7】线程ID。为NULL就行了
	HANDLE hRemoteThread = CreateRemoteThread(hProcess, NULL, 0, pfnStartAssr, lpAddr, 0,
		NULL);
	if (hRemoteThread == NULL) {
		// 创建远程线程失败
		//cout << "创建远程线程失败" << endl;
		// 释放内存
		VirtualFreeEx(hProcess, lpAddr, 0, MEM_FREE);
		return FALSE;
	}
	cout << "注入成功" << endl;
	// 等待线程结束
	WaitForSingleObject(hRemoteThread, -1);
	// 关闭线程
	CloseHandle(hRemoteThread);
	// 释放内存
	VirtualFreeEx(hProcess, lpAddr, 0, MEM_FREE);
}

//const wchar_t* 转 char*
char* WcharToChar(const wchar_t* wc)
{
	char* m_char = NULL;
	delete m_char;
	m_char = NULL;
	int len = WideCharToMultiByte(CP_ACP, 0, wc, wcslen(wc), NULL, 0, NULL, NULL);
	m_char = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, wc, wcslen(wc), m_char, len, NULL, NULL);
	m_char[len] = '\0';
	return m_char;
}