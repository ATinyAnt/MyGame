
#ifndef	__GLFRAME_H__
#define	__GLFRAME_H__


#include "GLWindow.h"											/**< ����GLWindow.hͷ�ļ� */
#include "Vector.h"
#include "GameTime.h"



/** ��������� */
class Keys														 
{
public:
	
	/** ���캯�� */
	Keys() { Clear(); }
	
	/** ������еİ�����Ϣ */
	void Clear() { ZeroMemory(&m_KeyDown, sizeof(m_KeyDown)); }
	
	/** �ж�ĳ�����Ƿ��� */
	bool IsPressed(unsigned int key)
	{
		if (key < MAX_KEYS)
		{
			return m_KeyDown[key];
		}
		return false;
	}
	
	/** ����ĳ���������� */
	void SetPressed(unsigned int key)	{ if (key < MAX_KEYS) m_KeyDown[key] = true; }
	
	/** ����ĳ�������ͷ� */
	void SetReleased(unsigned int key)	{ if (key < MAX_KEYS) m_KeyDown[key] = false; }

private:
	static const unsigned int MAX_KEYS = 256;
	bool m_KeyDown[MAX_KEYS];									/**< ����256��������״̬ */
};


/** �����ĳ����࣬�̳�����������OpenGL���� */
class GLApplication											
{
public:
	
	/** ����һ��ȫ�ֵ�Create����������������뱻�̳���ʵ�� */
	static GLApplication * Create(const char * class_name);	/**< �������Լ������� */

	void SetGameType(bool type){isConnectGame=type;};
	/** ���������� */
	virtual ~GLApplication() {};

	//friend class GLWindow;

protected:
	
	/** ����ĺ������뱻�̳���ʵ�֣���ɻ�����OpenGL��Ⱦ���� */
	virtual bool	Init() = 0;							/**< OpenGL�ĳ�ʼ�� */ 
	virtual void	Uninit() = 0;							/**< OpenGL��ж�� */
	virtual void	Update(/*DWORD milliseconds*/) = 0;				/**< ִ��OpenGL����ĸ��� */
	virtual void	Draw() = 0;									/**< ����OpenGL���� */

	
	
	
	/** ͨ�õĺ��� */
	void	ToggleFullscreen();									/**< �л� ȫ��/����ģʽ */
	void	TerminateApplication();								/**< �������� */
	void	ResizeDraw(bool enable) { m_ResizeDraw = enable; }	/**< �����ڴ��ڸı��С��ʱ�򣬿��Ի��� */
	
	Keys	m_Keys;												/**< ������ */
	bool    m_IsRButtonDown;
	bool	m_IsLButtonDown;
	bool	m_IsMButtonMove;									//�м���Ƿ������
	int m_Width;		//��Ļ�Ŀ�
	int m_Height;		//��Ļ�ĸ�

	int m_RX;	//����Ҽ����µ�λ��X
	int m_RY;	//����Ҽ����µ�λ��Y
	int m_LX;	//���������µ�λ��X
	int m_LY;	//���������µ�λ��Y
	int m_MX;
	int m_MY;	//��굱ǰλ��
	int m_MM;	//����м�ĵĹ���ֵ
	/** ���캯�� */
	GLApplication(const char * class_name);
	HWND m_hWnd;
	bool isConnectGame;
	//HINSTANCE m_hInstance;
	
private:
	
	/** �������ѭ�� */
	friend int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
	int		Main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
	
	/** ��Ϣ����ص����� */
	friend LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT	Message(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void SetScreen();
	static const UINT WM_TOGGLEFULLSCREEN = (WM_USER + 1);		/**< �Զ�����Ϣ�����л�����ģʽ��ʱ���� */

	GLWindow	m_Window;										/**< Window�� */
	const char* m_ClassName;									/**< ������ */
	bool		m_IsProgramLooping;								/**< ����ѭ����ǣ����Ϊfalse�����˳����� */
	bool        m_CreateFullScreen;                             /**< ��Ϊtrue���򴴽�ȫ��ģʽ */
	bool		m_IsVisible;									/**< �����Ƿ�ɼ� */
	bool		m_ResizeDraw;									/**< �Ƿ��ڸı��Сʱ�������˻��ƺ��� */
	CGameTime   m_Time;											//��ʱ��
	//DWORD		m_LastTickCount;								/**< ��һ�μ�ʱ����ֵ */
	
};

#endif	// __GLFRAMEWORK_H__