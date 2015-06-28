
#include "stdafx.h"
#include "GLFrame.h"													/**< ����GLFrame.hͷ�ļ� */
#include "resource.h"						
#include "luadata.h"

HINSTANCE GHinstance;

//bool isStartGame=false;


/** ��������� */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	int ret = -1;
	GHinstance=hInstance;
	GLApplication * appl = GLApplication::Create("OpenGL");			/**< ���������� */ 
	//����ж��Ƿ���������Ϸ
	if (strcmp(lpCmdLine,"STARTGAME")==0)
	{
		appl->SetGameType(true);
	}
	else
	{
		appl->SetGameType(false);
	}
	                   
	//ShowWindow(hwnd, SW_SHOW);
	
	if (appl != 0)
	{
		ret = appl->Main(hInstance, hPrevInstance, lpCmdLine, nCmdShow);/**< ִ�г�����ѭ�� */
		delete appl;													/**< ɾ�������ࣨ�ڼ̳����У�ʹ��GL_Example * example = new GL_Example(class_name);������һ���ڴ棩*/
	}
	else
	{																	/**< ����������� */
		MessageBox(HWND_DESKTOP, "�����������", "Error", MB_OK | MB_ICONEXCLAMATION);
	}

	return ret;
}																		

/** ��������Ϣ */
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LONG user_data = GetWindowLong(hWnd, GWL_USERDATA);					/**< �����û��Զ����32λ�ĳ��򸽼�ֵ */
	if (user_data == 0)
	{	
		/// ��������һ������
		if (uMsg == WM_CREATE)											/**< �����ڴ�����Ϣ */
		{																
			/// ���ش��ڽṹ��ָ�룬������մ����ĳ���ʵ������
			CREATESTRUCT * creation = reinterpret_cast<CREATESTRUCT *>(lParam);
			
			/// ��ó���ʵ����ָ��
			GLApplication * appl = reinterpret_cast<GLApplication *>(creation->lpCreateParams);
			
			/// �������ʵ����ָ��Ϊ�û��Զ���ĳ��򸽼�ֵ
			SetWindowLong(hWnd, GWL_USERDATA, reinterpret_cast<LONG>(appl));
			appl->m_IsVisible = true;									/**< ���ó���ɼ� */
			return 0;													/**< ���� */
		}
	}
	else
	{	
		/// ������ǵ�һ�δ��ڣ����س���ʵ����ָ��
		GLApplication * appl = reinterpret_cast<GLApplication *>(user_data);
		return appl->Message(hWnd, uMsg, wParam, lParam);				/**< ���ó���ʵ���Լ�����Ϣ������ */
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);					/**< ����Ĭ�ϵĴ�����Ϣ������ */
}


/** ���캯�� */
GLApplication::GLApplication(const char * class_name)					
{
	m_hWnd=m_Window.getWnd();
	m_Width=m_Window.m_WindowWidth;
	m_Height=m_Window.m_WindowHeight;
	m_IsRButtonDown=false;
	m_IsLButtonDown=false;
	m_IsMButtonMove=false;
	m_ClassName = class_name;											/**< �������� */
	m_IsProgramLooping = true;											/**< ���ó���ѭ��Ϊtrue */
	m_CreateFullScreen = true;											/**< ʹ��ȫ��ģʽ */
	m_IsVisible = false;												/**< ���ɼ� */
	m_ResizeDraw = false;												/**< �ڴ��ڸı��С��ʱ�򣬲��ɻ��� */
//	m_LastTickCount = 0;
}

void GLApplication::ToggleFullscreen()									/**< �л� ȫ��/����ģʽ */
{
	PostMessage(m_Window, WM_TOGGLEFULLSCREEN, 0, 0);					/**< �����Զ����л���Ϣ */
}

void GLApplication::TerminateApplication()								/**< �������� */
{
	PostMessage(m_Window, WM_QUIT, 0, 0);								/**< �����˳���Ϣ */
	m_IsProgramLooping = false;											/**< ֹͣ����ѭ�� */
}



/** ��Ϣѭ�� */
LRESULT GLApplication::Message(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//int xPos,yPos;
	switch (uMsg)														/**< ����ͬ��Windows��Ϣ */
	{
		case WM_SYSCOMMAND:												/**< �ػ�ϵͳ���� */
			switch (wParam)												
			{
				case SC_SCREENSAVE:										/**< �ػ���Ļ������������ */
				case SC_MONITORPOWER:									/**< �ػ���ʾ��ʡ��ģʽ�������� */
					return 0;											/**< ���������������� */
				break;
			}
		break;															/**< �˳� */

		case WM_CLOSE:													/**< �رմ��� */
			TerminateApplication();										/**< ����TerminateApplication���� */
			return 0;													
		break;

		case WM_EXITMENULOOP:
		case WM_EXITSIZEMOVE:
//			m_LastTickCount = GetTickCount();							/**< ���¼�������ֵ */
			return 0;
		break;

		case WM_MOVE:
			m_Window.SetPosX(LOWORD(lParam));							/**< ������������ */
			m_Window.SetPosY(HIWORD(lParam));							
			return 0;
		break;

		case WM_PAINT:
			if (m_ResizeDraw == true)									/**< �����Ҫ�ػ� */
			{
				m_Window.ReshapeGL();									/**< �������ô��ڵĴ�С */
				Draw();													/**< ���»��� */
				m_Window.SwapBuffers();									/**< ����ǰ��֡���� */
			}
		break;

		case WM_SIZING:													/**< �������ڸı��С */
		{
			RECT * rect = (RECT *)lParam;
			m_Window.SetWidth(rect->right - rect->left);				/**< ���ô��ڿ�� */
			m_Window.SetHeight(rect->bottom - rect->top);				/**< ���ô��ڸ߶� */
			return TRUE;
		}
		break;

		case WM_SIZE:													/**< ���ڸı��С�� */
			switch (wParam)												/**< ����ͬ�Ĵ���״̬ */
			{
				case SIZE_MINIMIZED:									/**< �Ƿ���С��? */
					m_IsVisible = false;								/**< ����ǣ������ò��ɼ� */
					return 0;											
				break;

				case SIZE_MAXIMIZED:									/**< �����Ƿ����? */
				case SIZE_RESTORED:										/**< ���ڱ���ԭ? */
					m_IsVisible = true;									/**< ����Ϊ�ɼ� */
					m_Window.SetWidth(LOWORD(lParam));					/**< ���ô��ڿ�� */
					m_Window.SetHeight(HIWORD(lParam));					/**< ���ô��ڸ߶� */
					m_Window.ReshapeGL();								/**< �ı䴰�ڴ�С */
					//m_LastTickCount = GetTickCount();					/**< ���¼�������ֵ */
					return 0;											
				break;
			}
		break;															

		case WM_KEYDOWN:												/**< ���°�����Ϣ */
			m_Keys.SetPressed(wParam);									
			return 0;													
		break;

		case WM_KEYUP:													/**< �����ͷż���Ϣ */
			
			m_Keys.SetReleased(wParam);									
			return 0;													
		break;

		case WM_LBUTTONDOWN:	//Ӣ�۷���ת��
			m_IsLButtonDown=true;
			m_LX = LOWORD(lParam); 
			m_LY = HIWORD(lParam);
			break;
		
		case WM_RBUTTONDOWN:	//Ӣ�����ߵķ���
			m_IsRButtonDown=true;
			m_RX = LOWORD(lParam); 
			m_RY = HIWORD(lParam);
			break;
 		case  WM_MOUSEMOVE:
			//MessageBox(NULL,"jj","fsd",MB_OK);
			m_MX = LOWORD(lParam); 
			m_MY = HIWORD(lParam);
 			break;
		case WM_MOUSEWHEEL:
			//char temp[65];
			//sprintf(temp,"X=%f",(float)(short)HIWORD(wParam));
			m_IsMButtonMove=true;
			m_MM=(short)HIWORD(wParam);
			//MessageBox(NULL,temp,temp,MB_OK);
			break;
		case WM_TOGGLEFULLSCREEN:										/**< �л� ȫ��/����ģʽ */
			m_CreateFullScreen = !m_CreateFullScreen;
			PostMessage(hWnd, WM_QUIT, 0, 0);
			break;

	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);					/**< ����Ĭ�ϵĴ�����Ϣ������ */
}



void GLApplication::SetScreen()
{
	LuaData lua;
	lua.InitLuaEnv();
	//lua->SetLuaFile("Lua\\baseinfo.lua");
	if (!lua.LoadLuaFile("baseinfo.lua"))
	{
		MessageBox(NULL,"���������ļ�����","GLApplication ����",MB_OK);
		exit(0);
	}
	lua.getGlobalProc("getIsFullScreen");
	lua_pcall(lua.GetLuaEnv(), 0, 1, 0);
	if (lua_tonumber(lua.GetLuaEnv(),-1))
	{
		m_Width=GetSystemMetrics(SM_CXSCREEN);
		m_Height=GetSystemMetrics(SM_CYSCREEN);

		m_CreateFullScreen = true;										/**< m_CreateFullScreen��¼��ǰ����ʾģʽΪ���� */
	}
	else
	{
		lua.getGlobalProc("getScreenSize");
		lua_pcall(lua.GetLuaEnv(), 0, 2, 0);

		m_Height=lua_tonumber(lua.GetLuaEnv(),-1);
		m_Width=lua_tonumber(lua.GetLuaEnv(),-2);
		m_CreateFullScreen = false;
	}


	m_Window.m_WindowWidth=m_Width;
	m_Window.m_WindowHeight=m_Height;
}


/** �������ѭ�� */
int GLApplication::Main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	/// ע��һ������
	WNDCLASSEX windowClass;												/**< ������ */
	ZeroMemory(&windowClass, sizeof(WNDCLASSEX));						/**< ��սṹΪ0 */
	windowClass.cbSize			= sizeof(WNDCLASSEX);					/**< ���ڽṹ�Ĵ�С */
	windowClass.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC |CS_NOCLOSE ;	/**< ���ô�������Ϊ���ƶ�ʱ�ػ�����Ϊ����ȡ��DC */
	windowClass.lpfnWndProc		= (WNDPROC)(WindowProc);				/**< WndProc������Ϣ */
	windowClass.hInstance		= hInstance;							/**< ����ʵ�� */
	windowClass.hbrBackground	= (HBRUSH)(COLOR_APPWORKSPACE);			/**< ���ñ��� */
	windowClass.hCursor			= LoadCursor(NULL, IDC_ARROW);			/**< ������ */
	windowClass.lpszClassName	= m_ClassName;							/**< �������� */
	//windowClass.style		    = windowClass.style &~CS_NOCLOSE;
	if (RegisterClassEx(&windowClass) == 0)								/**< ����ע�ᴰ���� */
	{																	/**< NOTE: Failure, Should Never Happen */
		MessageBox(HWND_DESKTOP, "ע�ᴰ��ʧ��!", "Error", MB_OK | MB_ICONEXCLAMATION);
		return -1;														/**< �˳�������FALSE */
	}

	
	SetScreen();

	while (m_IsProgramLooping)											/**< ѭ��ֱ��WM_QUIT�˳����� */
	{	
		
		/// ����һ������
		if (m_Window.Create("ת��", m_ClassName,m_CreateFullScreen, hInstance, this) == true)
		{	
			
			/// �����ʼ��ʧ�ܣ����˳�
			if (Init() == false)									    /**< �����Զ���ĳ�ʼ������ */
			{		
				/**< ʧ�� */
				TerminateApplication();									/**< �رմ����˳����� */
			}
			else														/**< �ɹ���ʼ��Ϣѭ�� */
			{															 
				MSG msg;												/**< Window��Ϣ�ṹ */ 
				bool isMessagePumpActive = true;						/**< ����Ϣ��Ϊ��ʱ��������Ϣѭ�� */
//				m_LastTickCount = GetTickCount();						/**< ���ص�ǰ�ļ�ʱ����ֵ */
				m_Keys.Clear();											/**< ������еİ�����Ϣ */
				
				while (isMessagePumpActive == true)						/**< ����Ϣ��Ϊ��ʱ��������Ϣѭ�� */
				{														/**< �ɹ��������ڣ���ⴰ����Ϣ */
					if (PeekMessage(&msg, m_Window, 0, 0, PM_REMOVE) != 0)
					{													
						/// ����Ƿ�ΪWM_QUIT��Ϣ
						if (msg.message != WM_QUIT)						
						{
							DispatchMessage(&msg);						/**< ������Ƿ�����Ϣ����Ϣ�ص������д��� */
						}
						else											
						{
							isMessagePumpActive = false;				/**< ����ǣ����˳� */
						}
					}
					else		/// ���û����Ϣ										
					{
						if (m_IsVisible == false)						/**< ������ڲ��ɼ� */
						{
							WaitMessage();								/**< ��ͣ���򣬵ȴ���Ϣ */
						}
						else											/**< ������ڿɼ� */
						{												/**< ִ����Ϣѭ�� */
							//DWORD tickCount = GetTickCount();			/**< ���ؼ�ʱ���ĵ�ǰֵ */
							//Update(tickCount - m_LastTickCount);		/**< �����û��Զ���ĸ��º��� */
							
							Update();
							Draw();	
							m_Window.SwapBuffers();						/**< ����ǰ��֡���� */
								//m_Time.SetTimenow();
							//}	
						}
					}
				}														/**< ���isMessagePumpActive == true����ѭ�� */
			}															
																		/**< ������� */
			Uninit();												/**< �û��Զ����ж�غ��� */
			m_Window.Destroy();											/**< ɾ������ */
		}
		else															/**< �����������ʧ�� */
		{																
			MessageBox(HWND_DESKTOP, "����OpenGL���ڴ���", "Error", MB_OK | MB_ICONEXCLAMATION);
			m_IsProgramLooping = false;									/**< ֹͣ����ѭ�� */
		}
	}																	

	UnregisterClass(m_ClassName, hInstance);							/**< ȡ��ע��Ĵ��� */
	return 0;
}
