//WindowsProject1.cpp: 定义应用程序的入口点。
//

#include "stdafx.h"
#include <fstream>
#include<iostream>
#include<Windows.h>
#include<Shlobj.h>
#include<tchar.h>
#include <cmath>
#include<Commctrl.h>
#include<atlstr.h>
#include"resource5.h"
#pragma comment(lib,"comctl32.lib")//链接comct132.lib这个库，用于选择文件路径
using namespace std;

/*全局变量*/

/*定义储存用于的类和结构体*/
struct node {
	char *GuanJianZi;
	int ChangDu;
	int CiShu;
	node *next;
};
struct table {
	node *nnext;
};
class XiangSi
{
public:
	~XiangSi();
	XiangSi() { flag = 0; };
	/*计算函数*/
	void ChuLi(char*, int);//详细处理
	void ChuLi();//初步处理
	void Qkong();//清空散列
	int Hax(int i) { return i%BiaoChang; }//哈希计算地址函数
	/*接口函数*/
	void ChuShiHua(char*);//初始化赋值函数
	char* YuanMa() { return DM; }
	table* HaXTable() { return Biao; }
	int HaXNum() { return BiaoChang; }
private:
	int flag;
	char *DM;//源代码储存
	table *Biao;//哈希表
	static int BiaoChang;//哈希表长
};

/*全局函数*/
char* peizhi3();
//int peizhi2();
TCHAR* lujing();//选择路径函数，返回路径
char* ConvertLPWSTRToLPSTR(LPWSTR lpwszStrIn);//TCHAR转化成char
char* DuQuWenJian();//读取文件函数，返回读到的内容
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);//窗口回调函数
float FenXi(table* b1, table *b2,int Bc1,int Bc2);//分析函数
int chazhao(table*b, int bc, int asc, char *a, int c);//哈希查找函数
int BiJiao(char *a, char *b, int c);//字符串比较函数
void XiangSiDu(float);//计算相似度
int BaoGao(table* b1, table *b2, int Bc1, int Bc2,float a);//生成实验报告


int XiangSi::BiaoChang = 31;
void XiangSi::Qkong() {
	if (flag == 1) {
		node *q, *f;
		int i;
		f = new node;
		for (i = 0; i < BiaoChang; i++) {
			f = Biao[i].nnext;
			while (f != NULL) {
				q = f;
				f = f->next;
				q->CiShu = 0;
			}
		}
		delete f, q;
	}
}
void XiangSi::ChuShiHua(char *a)
{   
	flag = 1;
	DM = a;
	Biao = new table[BiaoChang];
	for (int i = 0; i < BiaoChang; i++) {//初始化顶点
		Biao[i].nnext = NULL;
	}
	ChuLi();
}

XiangSi::~XiangSi()
{
	if(flag==1){
	node *q, *f;
	int i;
	f = new node;
	for (i = 0; i < BiaoChang; i++) {
		f = Biao[i].nnext;
		while (f != NULL) {
			q = f;
			f = f->next;
			delete q;
		}
	}
	delete f, q;
	delete[] Biao;
	delete[] DM;
	}
}



void XiangSi::ChuLi(char* a, int i) {
	if (!i)return;
	int asc = 0;
	for (int j = 0; j < i; j++)
	{
		asc += int(a[j]);
	}
	int k = Hax(asc);
	if (Biao[k].nnext == NULL) {
		node *s = new node;
		Biao[k].nnext = s;
		s->GuanJianZi = new char[i];
		for (int n = 0; n<i; n++)
			s->GuanJianZi[n] = a[n];
		s->ChangDu = i;
		s->CiShu = 1;
		s->next = NULL;
		return;
	}
	else {
		node *s;
		s = Biao[k].nnext;
		while (s != NULL) {
			if (i == s->ChangDu) {
				if (BiJiao(a, s->GuanJianZi,i)) {
					s->CiShu++;
					return;
				}
			}
			s = s->next;
		}
		node *p = new node;
		p->next = Biao[k].nnext;
		Biao[k].nnext = p;
		p->GuanJianZi = new char[i];
		for (int n = 0; n<i; n++)
			p->GuanJianZi[n] = a[n];
		p->ChangDu = i;
		p->CiShu = 1;
		return;
	}
}

void XiangSi::ChuLi() {
	int i = 0;
	int j = 0;
	char buf[15];
	while (DM[i] != '\1') {//关键字初步统计
		if (DM[i] >= 'A'&&DM[i] <= 'Z') {
			buf[j] = DM[i];
			j++;
		}
		else if (DM[i] >= 'a'&&DM[i] <= 'z') {
			buf[j] = DM[i];
			j++;
		}
		else if (DM[i] >= 48 && DM[i] <= 57) {
			buf[j] = DM[i];
			j++;
		}
		else
		{
			ChuLi(buf, j);
			j = 0;
		}
		i++;
	}
}

char* DuWenJian(char *a) {
	ifstream in(a);
	if (!in) {
		cout << "open file error!" << endl;
		return NULL;
	}
	int j = 0;
	while (!in.eof()) {
		in.get();
		j++;//计算文件长度
	}
	in.close();
	cout << "文件大小为" << j << endl;
	char ch;
	ifstream fin(a);
	if (!fin) {
		cout << "open file error!" << endl;
		return NULL;
	}
	char *str = new char[j + 1];
	int i = 0;
	while (!fin.eof()) {
		ch = fin.get();
		str[i] = ch;
		i++;
	}
	str[i] = '\1';
	fin.close();
	return str;
}

//主函数
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR IpCmdLine, int nCmdShow) {

	//设计窗口类
	WNDCLASS wnd; //定义窗口类
	wnd.cbClsExtra = 0;//储存指定分配给窗口类结构的附加字节数，系统将该成员初始化为零
	wnd.cbWndExtra = 0;//储存指定分配给窗口类的附加字节数
	wnd.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);//将背景刷成白色
	wnd.hCursor = LoadCursor(NULL, IDC_ARROW);//加载光标
	wnd.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON));//加载图标
	wnd.hInstance = hInstance;//当前应用程序实例
	wnd.lpfnWndProc = WindowProc;//窗口回调函数
	wnd.lpszClassName = L"cppBiJiao";//窗口类型名
	wnd.lpszMenuName = NULL;//菜单名称
	wnd.style = CS_VREDRAW | CS_HREDRAW;//风格

	//注册窗口
	RegisterClass(&wnd);

	//保存实例句柄 并创建窗口
	HWND hWnd = CreateWindow(L"cppBiJiao",     //窗口类 已经注册好的
		L"程序源代码的相似性",                 //窗口的标题
		WS_SYSMENU | WS_CAPTION,               //窗口的风格:左上角有菜单、带标题栏的窗口
		300,                                   //窗口的左上角X
		300,                                   //窗口的右上角Y
		500,                                   //窗口的宽
		300,                                   //窗口的高
		NULL,                                  //父窗口句柄
		NULL,                                  //菜单句柄
		hInstance,                             //当前应用程序实例
		0                                      //附加信息
	);

	//显示窗口
	ShowWindow(hWnd, SW_SHOW);

	//更新
	UpdateWindow(hWnd);

	//消息循环
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

int iK = 0;//计数器记录读取了几次文件
XiangSi s, z;
//窗口回调函数
LRESULT CALLBACK WindowProc(
	HWND hWnd,          //窗口句柄
	UINT uMsg,          //消息编号 WM_LBUTTONDOWM
	WPARAM wParam,      //附加参数  键盘
	LPARAM lParam) {    //附加参数  点击的坐标
						//测试代码，在输出位置输出对应操作
						/*wchar_t str[255];
						wsprintfW(str,L"uMsg=%d;wParam=%d;lParam=%d",uMsg,wParam,lParam);
						OutputDebugString(str);
						*/

	switch (uMsg) {//分别处理消息
	case WM_CREATE://创建窗口消息,如果创建了窗口就会有此消息
		CreateWindow(
			TEXT("BUTTON"), //控件类名称
			TEXT("添加测试文件"),//按钮上的字及快捷
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,//按钮样式
			10,
			10,
			150,
			50,
			hWnd,
			(HMENU)1000,     //控件ID
			((LPCREATESTRUCT)lParam)->hInstance,//实例句柄
			NULL
		);
		CreateWindow(
			TEXT("BUTTON"), //控件类名称
			TEXT("开始分析"),
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			170,
			10,
			150,
			50,
			hWnd,
			(HMENU)1001,     //控件ID
			((LPCREATESTRUCT)lParam)->hInstance,//实例句柄
			NULL
		);
		CreateWindow(
			TEXT("BUTTON"), //控件类名称
			TEXT("修改关键字表"),//按钮上的字及快捷
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,//按钮样式
			330,
			10,
			150,
			50,
			hWnd,
			(HMENU)1003,     //控件ID
			((LPCREATESTRUCT)lParam)->hInstance,//实例句柄
			NULL
		);
		CreateWindow(
			TEXT("BUTTON"), //控件类名称
			TEXT("清空载入"),//按钮上的字及快捷
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,//按钮样式
			10,
			70,
			150,
			50,
			hWnd,
			(HMENU)1002,     //控件ID
			((LPCREATESTRUCT)lParam)->hInstance,//实例句柄
			NULL
		);
		CreateWindow(
			TEXT("BUTTON"), //控件类名称
			TEXT("关于"),//按钮上的字及快捷
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,//按钮样式
			170,
			70,
			150,
			50,
			hWnd,
			(HMENU)1004,     //控件ID
			((LPCREATESTRUCT)lParam)->hInstance,//实例句柄
			NULL
		);
		break;
	case WM_COMMAND://判断点击按钮
		if (LOWORD(wParam) == 1000)//点击添加测试文件按钮
		{
				if(iK==0){
					char *str = DuQuWenJian();//读取文件一存入str中
					if(str!=NULL){
						s.ChuShiHua(str);//str中的字符串存入开散列表中
						MessageBoxA(hWnd, "文件一载入成功", "选择文件成功", MB_OK | MB_ICONEXCLAMATION | MB_ICONWARNING | MB_DEFBUTTON1);//输出
						iK++;//文件个数加一
					}
				}else if(iK==1){
					char *str = DuQuWenJian();//读取文件二存入str中
					if (str != NULL) {
						z.ChuShiHua(str);//str中的字符串存入开散列表中
						MessageBoxA(hWnd, "文件二载入成功", "选择文件成功", MB_OK | MB_ICONEXCLAMATION | MB_ICONWARNING | MB_DEFBUTTON1 );//输出
						iK++;//文件个数加一
					}
				}
				else
					MessageBox(hWnd, TEXT("文件只能选择两次"), TEXT("消息"), MB_OK | MB_ICONEXCLAMATION | MB_ICONWARNING | MB_DEFBUTTON1);
		}
		if (LOWORD(wParam) == 1001)//点击开始分析按钮
			if(iK==2){
				//MessageBox(0, TEXT("开始分析"), TEXT("分析"), MB_OK | MB_ICONEXCLAMATION);
				table *B1 = s.HaXTable(); //获得文件一处理过后哈希表
				table *B2 = z.HaXTable(); //获得文件一处理过后哈希表
				int Bc1 = s.HaXNum();//获得文件一的哈希表长
				int Bc2 = z.HaXNum();//获得文件二的哈希表长
				float aa=FenXi(B1, B2, Bc1, Bc2);//分析两个表中关键字的相对距离
				XiangSiDu(aa);//输出测试结果
				//MessageBox(0, TEXT("分析完成"), TEXT("分析"), MB_OK | MB_ICONEXCLAMATION);
			}
			else
				MessageBox(0, TEXT("文件选择错误"), TEXT("消息"), MB_OK | MB_ICONEXCLAMATION);
		if (LOWORD(wParam) == 1002)//点击清空载入按钮
		{
			iK = 0;//文件个数等于零
			s.Qkong();//清空载入文件
			z.Qkong();//清空载入文件
		}
		if (LOWORD(wParam) == 1003){//点击修改配置文件按钮
			system("notepad.exe 配置.txt");//用记事本打开测试文件 并且修改
		}
		if (LOWORD(wParam) == 1004) //点击关于按钮
			MessageBox(hWnd, TEXT("本程序由网络161单东琳开发"), TEXT("关于"), MB_OK | MB_ICONEXCLAMATION | MB_ICONWARNING | MB_DEFBUTTON1);
		break;
	case WM_DESTROY://窗口结束消息，如果窗口结束了就会有此消息
		PostQuitMessage(0);//退出消息处理函数
		return 0;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//获取路径函数
TCHAR* lujing() {
	LPITEMIDLIST pil = NULL;
	INITCOMMONCONTROLSEX InitCtrls = { 0 };
	TCHAR szBuf[4096] = { 0 };
	BROWSEINFO bi = { 0 };
	bi.hwndOwner = NULL;
	bi.iImage = 0;
	bi.lParam = NULL;
	bi.lpfn = NULL;
	bi.lpszTitle = _T("请选择测试代码(cpp)");
	bi.pszDisplayName = szBuf;
	bi.ulFlags = BIF_BROWSEINCLUDEFILES;
	InitCommonControlsEx(&InitCtrls);//在调用函数SHBrowseForFolder之前需要调用该函数初始化相关环境
	pil = SHBrowseForFolder(&bi);
	if (NULL != pil) {//若函数执行成功，并且用户选择文件路径并点击确定
		SHGetPathFromIDList(pil, szBuf);//获取选择到的文件路径
		return szBuf;
	}
	return NULL;
}

//读取文件函数，返回读到的内容
char* DuQuWenJian() {
	TCHAR *a = lujing();
	if (a == NULL) {
		//MessageBox(0, TEXT("您取消了打开文件！"), TEXT("提示"), MB_OK | MB_ICONEXCLAMATION);//输出
		return NULL;
	}
	ifstream fin(a);
	if (!fin) {
		//MessageBox(0, TEXT("文件选择错误！"), TEXT("提示"), MB_OK | MB_ICONEXCLAMATION);//输出
		return NULL;
	}
	int len = _tcslen(a);
	//MessageBox(0, TEXT("文件打开成功！"), TEXT("提示"), MB_OK | MB_ICONEXCLAMATION);//输出
	TCHAR str[20000] = { 0 };
	TCHAR ch;
	int i = 0;
	while (!fin.eof()) {
		ch = fin.get();
		str[i] = ch;
		i++;
	}
	//MessageBox(0, str, TEXT("提示"), MB_OK | MB_ICONEXCLAMATION);
	fin.close();
	char*st;
	st = ConvertLPWSTRToLPSTR(str);
	return st;
}
//字符串比较函数
int BiJiao(char *a, char *b, int c) {
	for (int i = 0; i < c; i++)
		if (a[i] != b[i])
			return 0;
	return 1;
}

//TCHAR转化成char
char* ConvertLPWSTRToLPSTR(LPWSTR lpwszStrIn)
{
	LPSTR pszOut = NULL;
	if (lpwszStrIn != NULL)
	{
		int nInputStrLen = wcslen(lpwszStrIn);
		int nOutputStrLen = WideCharToMultiByte(CP_ACP, 0, lpwszStrIn, nInputStrLen, NULL, 0, 0, 0) + 2;
		pszOut = new char[nOutputStrLen];
		if (pszOut)
		{
			memset(pszOut, 0x00, nOutputStrLen);
			WideCharToMultiByte(CP_ACP, 0, lpwszStrIn, nInputStrLen, pszOut, nOutputStrLen, 0, 0);
		}
	}
	return pszOut;
}

int chazhao(table*b, int bc, int asc, char *a, int c) {
	int j = asc%bc;//得到地址关键字应该在的地址
	node *q, *f;
	if (b[j].nnext == NULL) {//如果不存在就返回0，这个关键字一个 也没有出现
		return 0;
	}
	else {
		f = b[j].nnext;//得到头指针
		while (f != NULL) {
			q = f;
			if (BiJiao(q->GuanJianZi, a, c)) {//如果找到就直接返回出现次数
				return q->CiShu;
			}
			f = f->next;//地址后移
		}
	}
	return 0;//没找到返回0
}

float FenXi(table* b1, table *b2, int Bc1, int Bc2) {
	char *str = peizhi3();
	int i = 0;
	int j = 0;
	int m = 0;
	char buf[20];
	float JiGuo = 0;
	int asc;
	for (int n = 0; n < 20; n++)
		buf[j] = '\1';
	while (str[i] != '\1') {
		if (str[i] >= 'A'&&str[i] <= 'Z') {
			buf[j] = str[i];
			j++;
		}
		else if (str[i] >= 'a'&&str[i] <= 'z') {
			buf[j] = str[i];
			j++;
		}
		else if (str[i] >= 48 && str[i] <= 57) {
			buf[j] = str[i];
			j++;
		}
		else
		{//从文件中读入关键字储存到buf中
			asc = 0;
			for (int k = 0; k < j; k++)
				asc += int(buf[k]);//计算关键字的ASCII码和
			int aa = chazhao(b1, Bc1, asc, buf, j);//查找关键字在代码一中出现的次数
			int bb = chazhao(b2, Bc2, asc, buf, j);//查找关键字二在代码二中出现的次数
			bb = bb - aa;//计算差
			JiGuo += pow(bb, 2);//平方求和
			for (int n = 0; n < j; n++)//重新给buf初始化
				buf[j] = '\1';
			j = 0;
		}
		i++;
	}
	JiGuo = sqrt(JiGuo);//开平方
	BaoGao(b1, b2, Bc1, Bc2, JiGuo);
	return JiGuo;
}

void XiangSiDu(float a) {
	if (a < 2){
		CString str;
		str.Format(_T("经计算两代码几何距离为%f，代码相似度极高"), a);
		MessageBox(0, str, TEXT("分析结果"), MB_OK | MB_ICONEXCLAMATION);
		}
	else if (a < 3) {
		CString str;
		str.Format(_T("经计算两代码几何距离为%f，代码相似度较高"), a);
		MessageBox(0, str, TEXT("分析结果"), MB_OK | MB_ICONEXCLAMATION);
	}
	else if (a < 4) {
		CString str;
		str.Format(_T("经计算两代码几何距离为%f，代码相似度较低"), a);
		MessageBox(0, str, TEXT("分析结果"), MB_OK | MB_ICONEXCLAMATION);
	}
	else {
		CString str;
		str.Format(_T("经计算两代码几何距离为%f，代码相似度极低"), a);
		MessageBox(0, str, TEXT("分析结果"), MB_OK | MB_ICONEXCLAMATION);
	}
}

//生成实验报告
int BaoGao(table* b1, table *b2, int Bc1, int Bc2,float a) {
	ofstream fout("D:/报告.txt");	//只写形式打开文件
	if (!fout)
	{
		MessageBox(0, TEXT("测试报告生成失败"), TEXT("分析结果"), MB_OK | MB_ICONEXCLAMATION);
	}
	node *q, *f;
	int j;
	f = new node;
	fout << "代码一字符统计" << endl;
	fout << "-----------------------------" << endl;
	for (j = 0; j < Bc1; j++) {
		f = b1[j].nnext;
		while (f != NULL) {
			q = f;
			f = f->next;
			for (int m = 0; m < q->ChangDu; m++)
			{
				fout << q->GuanJianZi[m];
			}
			fout << ":" << q->CiShu << endl;
		}
	}
	fout << endl;
	fout << endl;
	fout << "代码二字符统计" << endl;
	fout << "-----------------------------" << endl;
	for (j = 0; j < Bc2; j++) {
		f = b2[j].nnext;
		while (f != NULL) {
			q = f;
			f = f->next;
			for (int m = 0; m < q->ChangDu; m++)
			{
				fout << q->GuanJianZi[m];
			}
			fout << ":" << q->CiShu << endl;
		}
	}
	fout << "-----------------------------" << endl;
	fout << "两个代码几何距离为" << a;
	delete f, q;
	fout.close();
	MessageBox(0, TEXT("在D盘根目录，详细测试报告生成成功"), TEXT("分析结果"), MB_OK | MB_ICONEXCLAMATION);
	return 0;
}

char* peizhi3() {
	ifstream in("配置.txt");
	if (!in) {
		cout << "open file error!" << endl;
		return NULL;
	}
	int j = 0;
	while (!in.eof()) {
		in.get();
		j++;
	}
	in.close();
	cout << "文件大小为" << j << endl;
	char ch;
	ifstream fin("配置.txt");
	if (!fin) {
		cout << "open file error!" << endl;
		return NULL;
	}
	char *str = new char[j + 1];
	int i = 0;
	while (!fin.eof()) {
		ch = fin.get();
		str[i] = ch;
		i++;
	}
	str[i] = '\1';
	fin.close();
	return str;
}