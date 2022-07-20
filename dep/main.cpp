#include "yu.h"

void retisone()
{
	cin.ignore();
	cout << "请输入窗口标题：";
	string title;
	getline(cin, title);
	cout << "请输入内存地址16位：";
	string straddress;
	cin >> straddress;
	string name = "0x" + straddress;
	DWORD address = std::strtoul(name.c_str(), NULL, 16);//转换为DWORD类型变量
	HWND hWnd;
	LPCWSTR  newititle = ConvertCharToLPWSTR(title.c_str());
	hWnd = FindWindow(NULL, newititle);//根据窗口标题获取窗口句柄
	DWORD PID;
	GetWindowThreadProcessId(hWnd, &PID);//存入PID
	HANDLE hprocess;
	hprocess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);//获取进程
	DWORD value = 0;//用来存原值是多少
	DWORD size = 0;
	if (false == ReadProcessMemory(hprocess, (void*)address, &value, sizeof(DWORD), &size))
	{
		cout << "读取失败" << endl;
	}
	else
	{
		cout << "当前的值(10进制)：" << value << endl;
	}
}

void retistwo()
{
	cin.ignore();
	cout << "请输入窗口标题：";
	string title;
	getline(cin, title);
	cout << "请输入内存地址16位：";
	string straddress;
	cin >> straddress;
	cout << "请输入修改为多少值(10进制)：";
	string newthing;
	cin >> newthing;
	string name = "0x" + straddress;
	DWORD address = std::strtoul(name.c_str(), NULL, 16);//转换为DWORD类型变量
	HWND hWnd;
	LPCWSTR  newititle = ConvertCharToLPWSTR(title.c_str());
	hWnd = FindWindow(NULL, newititle);//根据窗口标题获取窗口句柄
	DWORD PID;
	GetWindowThreadProcessId(hWnd, &PID);//存入PID
	HANDLE hprocess;
	hprocess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);//获取进程
	DWORD value = 0;//用来存原值是多少
	DWORD newvalue = std::strtoul(newthing.c_str(), NULL, 10);//转换为DWORD类型变量
	DWORD size = 0;
	if (false == WriteProcessMemory(hprocess, (void*)address, &newvalue, sizeof(DWORD), &size))
	{
		cout << "修改失败" << endl;
	}
	else
	{
		cout << "修改成功" << endl;
	}
}

void retisthree()
{
	cin.ignore();
	cout << "请输入窗口标题：";
	string title;
	getline(cin, title);
	cout << "请输入特征码(如果不行可以试试切换大小写)：";
	string special;
	cin >> special;
	char * specialcode = const_cast<char*>(special.c_str());
	cout << "请输入偏移量，如果没有请输入0：";
	int move;
	cin >> move;

	HWND hWnd;
	LPCWSTR  newititle = ConvertCharToLPWSTR(title.c_str());
	hWnd = FindWindow(NULL, newititle);//根据窗口标题获取窗口句柄

	DWORD PID;
	GetWindowThreadProcessId(hWnd, &PID);//存入PID

	HANDLE hprocess;
	hprocess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);//获取进程

	ScanAddress(hprocess, specialcode, 0);//"获取地址值为多少"  如果输出的地址不对，可以试试特征码的大写和小写的替换
	//cout << hanshu_dizhi << endl;//输出地址(10进制)


	unsigned int num = hanshu_dizhi;
	num = num + move;//相对内存地址偏移量(10进制)
	string oversixdoc = tentosixteen(num);//这里已经转换为了16进制的地址
	cout << "地址为：" << oversixdoc << endl;//输出地址(10进制)
}

void retisfour()
{
	cin.ignore();
	cout << "请输入窗口标题：";
	string title;
	getline(cin, title);
	cout << "请输入DLL路径：";
	string path;
	cin >> path;
	HWND hWnd;
	LPCWSTR  newititle = ConvertCharToLPWSTR(title.c_str());
	hWnd = FindWindow(NULL, newititle);//根据窗口标题获取窗口句柄

	DWORD PID;
	GetWindowThreadProcessId(hWnd, &PID);//存入PID
	LPCWSTR  newipath = ConvertCharToLPWSTR(path.c_str());
	InjectDLL(newipath, PID);//这个dll你所要注入的dll文件，这个"数字"是你想注入的进程的PID号
}

void retisfive()
{
	cin.ignore();
	cout << "请输入窗口标题：";
	string title;
	getline(cin, title);

	HWND hWnd;
	LPCWSTR  newititle = ConvertCharToLPWSTR(title.c_str());
	hWnd = FindWindow(NULL, newititle);//根据窗口标题获取窗口句柄
	DWORD PID;
	GetWindowThreadProcessId(hWnd, &PID);//存入PID
	setlocale(LC_ALL, "chs");
	MODULEENTRY32 meme;
	meme.dwSize = sizeof(MODULEENTRY32);
	BOOL fmOk;
	TToolhelp(NULL).EnableDebugPrivilege(TRUE);
	TToolhelp thModules = TToolhelp(TH32CS_SNAPALL, PID);//进程号
	fmOk = thModules.ModuleFirst(&meme);
	// 模块
	while (true)
	{
		if (fmOk == 1)
		{
			//cout << fmOk << endl;
			//模块
			wprintf(L"模块基址:十进制 %d ,十六进制 0x%x ,模块大小：%d,模块名称:%s\n", meme.modBaseAddr, meme.modBaseAddr, meme.modBaseSize, meme.szModule);
			fmOk = thModules.ModuleNext(&meme);
		}
		else
		{
			cout << "over" << endl;
			break;
		}

	}


	TToolhelp(NULL).EnableDebugPrivilege(FALSE);

}

void retissix()
{
	cin.ignore();
	cout << "请输入窗口标题：";
	string title;
	getline(cin, title);
	cout << "请输入模块名字：";
	string dllname;
	getline(cin, dllname);
	HWND hWnd;
	LPCWSTR  newititle = ConvertCharToLPWSTR(title.c_str());
	hWnd = FindWindow(NULL, newititle);//根据窗口标题获取窗口句柄
	DWORD PID;
	GetWindowThreadProcessId(hWnd, &PID);//存入PID
	//cout << PID << endl;
	setlocale(LC_ALL, "chs");
	MODULEENTRY32 meme;
	meme.dwSize = sizeof(MODULEENTRY32);
	BOOL fmOk;
	TToolhelp(NULL).EnableDebugPrivilege(TRUE);
	TToolhelp thModules = TToolhelp(TH32CS_SNAPALL, PID);//进程号
	fmOk = thModules.ModuleFirst(&meme);

	while (true)
	{
		if (fmOk == 1)
		{
			//cout << fmOk << endl;
			//模块
			//wprintf(L"模块基址:十进制 %d ,十六进制 0x%x ,模块大小：%d,模块名称:%s\n", meme.modBaseAddr, meme.modBaseAddr, meme.modBaseSize, meme.szModule);
			const wchar_t * havedll;
			havedll = meme.szModule;
			char * chhacedll = WcharToChar(havedll);
			string strhavedll = chhacedll;
			if (strhavedll == dllname)
			{
				unsigned char * sixaddr = meme.modBaseAddr;

				wprintf(L"该模块地址：%x\n", meme.modBaseAddr);
				break;
			}

			// 下一模块
			fmOk = thModules.ModuleNext(&meme);
		}
		else
		{
			cout << "over" << endl;
			break;
		}

	}



	TToolhelp(NULL).EnableDebugPrivilege(FALSE);

}

void niceui()
{
	cout << "---------Ece程序介绍---------" << endl;
	cout << "----1.读取某进程某地址数据---" << endl;
	cout << "----2.修改某进程某地址数据---" << endl;
	cout << "----3.特征码搜索地址---------" << endl;
	cout << "----4.DLL注入----------------" << endl;
	cout << "----5.获取程序所有模块地址---" << endl;
	cout << "----6.获取程序特定模块地址---" << endl;
	cout << "----7.刷新屏幕内容-----------" << endl;
	cout << "----8.加入QQ群:614815597-----\n\n" << endl;
}

int main()
{
	niceui();
	int ret;
	while (true)
	{
		cout << "你的指令是：";
		cin >> ret;
		if (ret == 1)//根据窗口标题读取某进程的某个地址
		{
			retisone();
		}
		else if (ret == 2)//根据窗口标题修改某个进程的某个地址
		{
			retistwo();
		}
		else if (ret == 3)//特征码搜索
		{
			retisthree();
		}
		else if (ret == 4)//dll注入
		{
			retisfour();
		}
		else if (ret == 5)//获取程序所有模块地址
		{
			retisfive();
		}

		else if (ret == 6)//获取程序特定模块地址
		{
			retissix();
		}
		else if (ret == 7)//清屏
		{
			system("cls");
			niceui();
		}
		else if (ret == 8)//加入Q群
		{
			cout << "请你手动加群啦哈哈哈。官方Q群：614815597" << endl;
		}
		else
		{
			cout << "这是一个无法辨析命令" << endl;
		}
	}

	while (true);
}