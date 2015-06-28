
//#include "stdafx.h"
#include "gameworld.h"						    
//#include "Monster.h"
extern CHero *myHero;
extern HINSTANCE GHinstance;
//extern bool isStartGame;

Camera   m_Camera;
CFightSocket *F_S;

/** ����һ�������ʵ�� */
GLApplication * GLApplication::Create(const char * class_name)
{
	GameWorld * test = new GameWorld(class_name);
	return reinterpret_cast<GLApplication *>(test);
}


/** ���캯�� */
GameWorld::GameWorld(const char * class_name) : GLApplication(class_name)
{
  /// ��ʼ���û��Զ���ĳ������
	m_Fps = 0;
	m_RenderMode = true;
	sp = false;
	mp = false;
//    m_Hero.m_angle=0.0;
	m_Camera.move_angle=0.0;
// 	onfire=false;
// 	onlaser=false;
	showlaser_step=8;
	m_str="ת��";
	myHero=&m_Hero;
	SceneNum=0;
	CurrentScene=1;
	//IsNextScene=false;
	m_is_open_menu=true;
	
	m_IsLButtonDown=false;
	AllScene=3;
	IsWin=false;
	IsFight=false;
	begin_game=false;
	firstin=1;
	goon=false;
	F_S=&FS;
	CurrentScene=1;
//	isTransmit=false;
	
}


/** ��ʼ��OpenGL */
bool GameWorld::Init()									
{
/** �û��Զ���ĳ�ʼ������ */
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);						
	glClearDepth(1.0f);											
	glDepthFunc(GL_LEQUAL);										
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);	
	
	glShadeModel(GL_SMOOTH);									
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			
	ResizeDraw(true);	                             /**< �ı�OpenGL���ڴ�С��ֱ�ӵ�������ĺ��� */
	
	//ParallelView();
	m_initmenu.loadTexture();
	cur=LoadCursor(GHinstance,MAKEINTRESOURCE(IDC_GAME_CURSOR));
	//isConnectGame=true;

	m_showmenu.LoadTexture();
	if(!m_Terrain.init() || !m_camp.Init())
	{
		MessageBox(NULL,"��ʼ������ʧ��!","����",MB_OK);
		exit(0);
	}
	//m_laser.Init(1500);
	/** ��ʼ������ */
	if(!m_font.InitFont(-16) || !m_infofont.InitFont(-64))
		return false;

	LoadCubeCoord();
	/** ��ʼ��MD2�ļ� */


	m_Hero.Init(isConnectGame);

	m_ball.LoadTexture("..\\data\\bomb.bmp");
	if (isConnectGame)
	{
		FS.InitSocket("192.168.1.102",6000);
		m_is_open_menu=false;
		SceneNum=2;									//����������ս��ֱ�Ӵӵڶ�������ʼ
	}
	else
	{
		m_Monster.Init(1);			//��ʼ��ʼ����һ���ؿ�
		Vector3 m_CameraPosition=m_Camera.setCameraPosition(m_Hero.m_RoleCoord.x, m_Hero.m_RoleCoord.y, m_Hero.m_RoleCoord.z);
		/** ��������� */
		m_Camera.setCamera( m_CameraPosition.x,m_CameraPosition.y, m_CameraPosition.z,  m_Hero.m_RoleCoord.x, m_Hero.m_RoleCoord.y, m_Hero.m_RoleCoord.z,  0, 1, 0,false);
	}
	return true;                                        /**< �ɹ����� */
}


/** �û��Զ����ж�غ��� */
void GameWorld::Uninit()									
{
}





/** ������º��� */
void GameWorld::Update(/*DWORD milliseconds*/)						
{
	
	if (m_is_open_menu == true)
	{
		ParallelView();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		::ShowCursor(true);
		switch(m_initmenu.render(0,0,m_Width,m_Height,m_MX,m_MY,m_IsLButtonDown))
		{
			case BEGINGAME:
				m_is_open_menu=false;
//				InitGame();
				ProjectionView();
				
				//m_IsLButtonDown=false;			//��Ϊ�����ؼ�ʱ��m_IsLButtonDown�ᱻ���ó�true
				break;

			case CONNECTGAME:
				
				ConnectGame();
				break;

			case EXITGAME:
				TerminateApplication();
				
				break;


		}
		m_IsLButtonDown=false;
	}
	if (m_Keys.IsPressed(VK_ESCAPE) == true)
	{		
		m_is_open_menu=true;
	}
	else if (!m_is_open_menu)		//������˲˵�����ִ������Ķ���
	{
		if(m_Keys.IsPressed(VK_LEFT) || m_Keys.IsPressed('A')) /**< ���������'A'������ */
		{
			m_Camera.move_angle=m_Camera.move_angle-2.0f;
			if(m_Camera.moveCamera(m_Camera.getSpeed(),m_Camera.move_angle)==false)          /**< �ƶ������ */
			{
				m_Camera.move_angle=m_Camera.move_angle+2.0f;
			}
		}

		if(m_Keys.IsPressed(VK_RIGHT) || m_Keys.IsPressed('D')) /**< ���ҷ������'D'������ */
		{
			m_Camera.move_angle=m_Camera.move_angle+2.0f;
			if(m_Camera.moveCamera(m_Camera.getSpeed(),m_Camera.move_angle)==false)          /**< �ƶ������ */
			{
				m_Camera.move_angle=m_Camera.move_angle-2.0f;
			}          /**< �ƶ������ */
		
			//MessageBox(NULL,"sdfsd","sdfsd",MB_OK);
		}

		if(m_Keys.IsPressed('Q') && !m_Hero.isRush && !m_Hero.isJumpBit)
		{
			m_Hero.isLightning ? m_Hero.isLightning=false : m_Hero.isLightning=true;
			m_Keys.SetReleased('Q');
			
			if(m_Hero.isLightning)
			{
				::ShowCursor(false);
			}
			else
			{
				::ShowCursor(true);
			}
		}
		if(m_Keys.IsPressed('F') && !m_Hero.isRush && !m_Hero.isJumpBit)   //������'F',Ӣ�ۿ���
		{
			FS.sendPro.skill='F';
			m_Hero.OpenFire();
//			FS.SendFinghtInfo();
			
		}
		if (m_Keys.IsPressed('E') && !m_Hero.isRush)		//�������������ܳ����ܣ�ͬ�����˿��ܣ����ܳ�����
		{
			FS.sendPro.skill='E';
			m_Hero.JumpBit();
			//IsHitMonster(m_Hero.m_RoleDesCoord.x,m_Hero.m_RoleDesCoord.z,m_Hero.m_RoleDesCoord.x,m_Hero.m_RoleDesCoord.z);
		}
		if (m_Keys.IsPressed('R') && !m_Hero.isJumpBit)		//������'Q',Ӣ�����������ܼ���
		{
			FS.sendPro.skill='R';
			m_Hero.Rush();
		}

		//��
		if (m_IsRButtonDown && !m_Hero.isRush  && !m_Hero.isJumpBit && !m_Hero.isLightning)		//�������Ҽ� ,����Ӣ�۵�״̬
		{
			FS.sendPro.skill='W';
			setHeroStatus(RBUTTON);
			m_IsRButtonDown=false;

		}

		if (m_Keys.IsPressed('N'))		//�����¡�N����������һ�����������ڲ���	
		{
			if (keytime.ReachTime(500))
			{
				keytime.SetTimenow();
				SceneNum++;
				CurrentScene=2;
				if(SceneNum>2) SceneNum=2;
				//SceneNum=SceneNum%2;
			}
			
		}
		if (m_Keys.IsPressed('S'))
		{
			goon=true;
		}
		if (m_Keys.IsPressed('T') && !isConnectGame)//������������Ϸ��ʱ��ſ���ʹ�ô��ͼ���
		{
			//isTransmit=true;
			if (m_Monster.getMonsterNum()==0 && !IsWin)
			{
				SceneNum++;
				CurrentScene=2;
				//m_Monster.ReInit();
				m_Monster.Init(2);
			}
			m_Hero.Transmit(SceneNum,CurrentScene);
			
			m_Keys.SetReleased('T');
		}
		

		
		if (m_IsLButtonDown && !m_Hero.isRush  && !m_Hero.isJumpBit && !m_Hero.onlaser)
		{
			setHeroStatus(LBUTTON);				//���ﲻ�ܽ�m_IsLButtonDown���ó�false����Ϊ���滹Ҫ�õ�
			m_Hero.m_RoleDesCoord.x=m_Hero.m_RoleCoord.x;
			m_Hero.m_RoleDesCoord.z=m_Hero.m_RoleCoord.z;
			
		}
		if (m_IsMButtonMove)
		{
			//MessageBox();
			ChangeScreen();
			m_IsMButtonMove=false;
		}
	}	    
}


void GameWorld::ConnectGame()
{
	//ָ���½��̵���������ֵ���ʽ
	STARTUPINFO sui;
	//���ڽ��մ����½��̺��½��̵�һЩ��Ϣ
	PROCESS_INFORMATION pi;
	ZeroMemory(&sui,sizeof(STARTUPINFO));
	ZeroMemory( &pi, sizeof (pi) );
	if(!CreateProcess("Platform.exe",NULL,NULL,NULL,true,0,NULL,NULL,&sui,&pi))
	{
		MessageBox(NULL,"�����ӽ���ʧ�ܣ�","����",MB_OK);
		return;
	}
	else
	{
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
	ExitProcess(0);

}


//��Ļ���� ת ��������
Vector3 GameWorld::From2DTo3D(float x, float y)
{
//	char ts[256];
	float depth[2];
	GLdouble glx = 0.0;
	GLdouble gly = 0.0;
	GLdouble glz = 0.0;
	y=m_Height-y;//��תy����
	glReadPixels(x,y,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,depth);
	GLdouble model[16],project[16];
	glGetDoublev(GL_MODELVIEW_MATRIX,model);
	glGetDoublev(GL_PROJECTION_MATRIX,project);
	GLint viewport[4]={0,0,m_Width,m_Height};
	gluUnProject(x,y,depth[0],model,project,viewport,&glx,&gly,&glz);


	return Vector3((float)glx,(float)gly,(float)glz);	
}


//����Ӣ��״̬
void GameWorld::setHeroStatus(int flag)
{
	Vector3 ThreeDCoord;
    if (flag == RBUTTON)
    {
		ThreeDCoord=From2DTo3D(m_RX,m_RY);

		m_Hero.m_RoleDesCoord.x=ThreeDCoord.x;
		m_Hero.m_RoleDesCoord.z=ThreeDCoord.z;
		m_Hero.setAngle(m_Hero.getAngle(m_Hero.m_RoleDesCoord.x,m_Hero.m_RoleDesCoord.z));
    }
	else if (flag == LBUTTON)
	{
		Vector3 temp=From2DTo3D(m_LX,m_LY);
		m_Hero.setAngle(m_Hero.getAngle(temp.x,temp.z));
	}
	float angle=m_Hero.getAngle();
	if (angle<0)
	{
		angle+=360;
		m_Hero.setAngle(angle);
	}
	
}

//����������λ��(��ʼ���������λ��)
// Vector3 MyGame::setCameraPosition(float x,float y,float z)
// {
// 	float len=70.0;
// 	m_Camera.setLen(len);
// 	return Vector3(x,y+30.0,z+len);
// }


void GameWorld::ParallelView()
{
	glViewport(0, 0, m_Width, m_Height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, m_Width, 0, m_Height, -1.0,1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


void GameWorld::ProjectionView()
{
	glViewport(0, 0, m_Width, m_Height);									/**< ���������ӿ� */
	glMatrixMode(GL_PROJECTION);									
	glLoadIdentity();													
	gluPerspective(45.0f, (GLfloat) m_Width / (GLfloat)m_Height, 1.0f, 4000.0f);	
	glMatrixMode(GL_MODELVIEW);											
	glLoadIdentity();
}


//�ı���Ļ���ӽ�
/*
	�ͣ�20 50 30
	�У�30 70 45
	�ߣ�40 90 60
	��� 50 110 75
*/
void GameWorld::ChangeScreen()
{
	if((m_MM>0) && (m_Camera.getLen()>=110.0))
	{
		return;
	}
	else if ((m_MM<0) && (m_Camera.getLen()<=50.0))
	{
		return;
	}
	Vector3 cp=m_Camera.getPosition();
	Vector3 cv=m_Camera.getView();
	Vector3 cu=m_Camera.getUpVector();
	float len;
	if (m_MM>0)
	{
		len=m_Camera.getLen()+20.0;
		cp.y+=10.0;
	}
	else
	{
		len=m_Camera.getLen()-20.0;
		cp.y-=10.0;
	}
	m_Camera.setLen(len);
	m_Camera.setCamera(cp.x,cp.y,cp.z,cv.x,cv.y,cv.z,cu.x,cu.y,cu.z,false);
	m_Camera.moveCamera(0,m_Camera.move_angle);
}


void GameWorld::HeroEvent()
{
	if (m_Hero.IsDead())
	{
		m_Hero.m_RoleCoord.x=-99999;
		m_Hero.m_RoleCoord.z=-99999;
		return;
	}

	m_Hero.Animate(SceneNum);

	if (m_Hero.isLightning || m_Hero.showlightning)
	{
		Vector3 V_temp;
		if(!m_Hero.showlightning)
		{
			V_temp=From2DTo3D(m_MX,m_MY);
		}
		CoordOf2D *P_temp;
		P_temp=m_Hero.Lightning(V_temp.x,V_temp.z,isConnectGame,m_IsLButtonDown);
		if (isConnectGame)
		{
			if (P_temp->x >= -480.0 && P_temp->x<=480.0 && P_temp->z >= -480.0 && P_temp->z <= 480.0)
			{
				FS.sendPro.skill='Z';
				FS.P_Coord.x=P_temp->x;
				FS.P_Coord.z=P_temp->z;

			}
		}
		
		//m_Hero.Lightning(V_temp.x,V_temp.z,m_IsLButtonDown);
		
	}
	m_Hero.ShowLife();	
}



void GameWorld::MonsterEvent()
{
	glPushAttrib(GL_CURRENT_BIT);
	glPushMatrix();
	//m_Monster.ChaseHero(m_Hero.m_RoleCoord.x,m_Hero.m_RoleCoord.z);
	//m_Monster.ShowMonsterLife();
	m_Monster.Animate();
	if (m_Monster.getMonsterNum()==0)
	{
		if (SceneNum==AllScene-1)
		{
			IsWin=true;
		}
		if (SceneNum<AllScene-1)
		{
			m_infofont.PrintText("FINISH",-1,3);

		}
	}
	glPopMatrix();
	glPopAttrib();
}

void GameWorld::MenuEvent()
{
	if (menutime.ReachTime(20))
	{
		menutime.SetTimenow();
		glPushAttrib(GL_CURRENT_BIT);
		glPushMatrix();
		ParallelView();
		m_showmenu.renderbar(m_Width,204);
		float size =m_Width * 0.0475;
		float size2=m_Width * 0.0495;
		//������
		if (m_Hero.lasertime.ReachTime(500))
		{
			//m_showmenu.render(931.0,6.0,57,57,0,1.0,1.0,1.0);
			m_showmenu.render(0.776*m_Width,6.0,size,57,0,1.0,1.0,1.0);
		}
		else
		{
			m_showmenu.render(0.776*m_Width,6.0,size2,59,0,1.0,0.0,0.0);
		}
		
		//����
		if (m_Hero.jumpbittime.ReachTime(3000))
		{
			//m_showmenu.render(998.0,6.0,57,57,1,1.0,1.0,1.0);
			m_showmenu.render(0.832*m_Width,6.0,size,57,1,1.0,1.0,1.0);
		}
		else
		{
			m_showmenu.render(0.832*m_Width,6.0,size2,59,1,1.0,0.0,0.0);
		}

		//���
		if (m_Hero.rushtime.ReachTime(1000))
		{
			//m_showmenu.render(1064.0,6.0,57,57,2,1.0,1.0,1.0);
			m_showmenu.render(0.887*m_Width,6.0,size,57,2,1.0,1.0,1.0);
		}
		else
		{
			m_showmenu.render(0.887*m_Width,6.0,size2,59,2,1.0,0.0,0.0);
		}
		//��
		if (m_Hero.m_RoleCoord.x == m_Hero.m_RoleDesCoord.x)
		{
			m_showmenu.render(0.776*m_Width,136.0,size,57,5,1.0,1.0,1.0);
		}
		else
		{
			m_showmenu.render(0.776*m_Width,136.0,size2,59,5,1.0,0.0,0.0);
		}
		//����
		if (!isConnectGame/* && isTransmit*/)
		{
			if (m_Hero.transmittime.ReachTime(5000))
			{
				//m_showmenu.render(1129.0,70.0,57,57,7,1.0,1.0,1.0);
				m_showmenu.render(0.941*m_Width,70.0,size,57,7,1.0,1.0,1.0);
			}
			else
			{
				m_showmenu.render(0.941*m_Width,70.0,size2,59,7,1.0,0.0,0.0);
			}
		}
		
		//����
		if (m_Hero.isLightning==false/* && */)
		{
			if (m_Hero.lightningtime.ReachTime(1000))
			{
				m_showmenu.render(0.941*m_Width,6.0,size,57,4,1.0,1.0,1.0);
			}
			else 
			{
				m_showmenu.render(0.941*m_Width,6.0,size2,59,4,1.0,0.0,0.0);
			}
			
		}
		else 
		{
			m_showmenu.render(0.941*m_Width,6.0,size2,59,4,1.0,0.0,0.0);
		}
		//Сͼ��Ӣ��
		m_showmenu.render(14.0+(101.0*m_Hero.m_RoleCoord.x/480.0)+101.0,6.0+(92.0*(-m_Hero.m_RoleCoord.z)/480.0)+92.0,12,12,6,1.0,1.0,1.0);
		//glPushMatrix();
		
		//glPopMatrix();

		ProjectionView();
		glPopMatrix();
		glPopAttrib();
	}
	
}


void GameWorld::HandleRecvMessage(char msg[])
{
	for(int i=0;i<FS.heroNum;++i)
	{
		if ((FS.heroid[i]!=m_Hero.ownid)/* && (FS.pro.hero_id == FS.heroid[i])*/)
		{
			if(FS.recvPro.hero_id == FS.heroid[i])
			{

				FS.fightHero[i]->m_RoleCoord.x=FS.recvPro.x;
				FS.fightHero[i]->m_RoleCoord.z=FS.recvPro.z;

				FS.fightHero[i]->m_RoleDesCoord.x=FS.recvPro.dx;
				FS.fightHero[i]->m_RoleDesCoord.z=FS.recvPro.dz;
				if(FS.fightHero[i]->HP != FS.recvPro.HP)
				{
					FS.fightHero[i]->HP=FS.recvPro.HP;
					FS.fightHero[i]->SetBeHit(true);
					FS.fightHero[i]->ShowLife(0.3F,-1.0f,OTHER);
					
				}				
				
				FS.fightHero[i]->setAngle(FS.recvPro.angle);

				switch(FS.recvPro.skill)
				{
				case 'F':
					FS.fightHero[i]->OpenFire();
					break;
				case 'E':
					FS.fightHero[i]->JumpBit();
					break;
				case 'R':
					FS.fightHero[i]->Rush();
					break;
				case 'Z':
					memcpy(&FS.P_Coord,msg,sizeof(FS.P_Coord));
					FS.fightHero[i]->showlightning=true;
					FS.fightHero[i]->Lightning(FS.P_Coord.x,FS.P_Coord.z,true,true,OTHER);
					break;
				}
				break;
			}
			
		}
	}
}


void GameWorld::HandleSendMessage()
{


	FS.sendPro.hero_id=m_Hero.ownid;
	FS.sendPro.angle=m_Hero.getAngle();
	FS.sendPro.x=m_Hero.m_RoleCoord.x;
	FS.sendPro.y=m_Hero.m_RoleCoord.y;
	FS.sendPro.z=m_Hero.m_RoleCoord.z;

	FS.sendPro.dx=m_Hero.m_RoleDesCoord.x;
	FS.sendPro.dy=m_Hero.m_RoleDesCoord.y;
	FS.sendPro.dz=m_Hero.m_RoleDesCoord.z;

	FS.sendPro.HP=m_Hero.GetHP();
	
	
	FS.SendFinghtInfo();
// 	if (FS.sendPro.skill=='Z')
// 	{
// 		//char temp[1024];
// 		//sprintf(temp,"send:%1024s",FS.sendMsg);
// 		m_log.WriteLog("F_Send.log",FS.sendMsg);
// 	}
 	//������Э�飬�����³�ʼ��
 	FS.InitProtocal();


}


void GameWorld::F_HeroEvent()
{
	m_Hero.Animate(SceneNum);
	for(int i=0;i<FS.heroNum;++i)
	{
		if(FS.heroid[i]!=m_Hero.ownid)
		{

			FS.fightHero[i]->Animate(1,OTHER);
			FS.fightHero[i]->Lightning(FS.P_Coord.x,FS.P_Coord.z,true,true,OTHER);
			FS.fightHero[i]->ShowLife(0.3f,-1.0f,OTHER);

		}
	}
	if (m_Hero.isLightning || m_Hero.showlightning)
	{
		Vector3 V_temp;
		if(!m_Hero.showlightning)
		{
			V_temp=From2DTo3D(m_MX,m_MY);
		}
		CoordOf2D *P_temp;
		P_temp=m_Hero.Lightning(V_temp.x,V_temp.z,isConnectGame,m_IsLButtonDown);
		if (isConnectGame)
		{
			if (P_temp->x >= -480.0 && P_temp->x<=480.0 && P_temp->z >= -480.0 && P_temp->z <= 480.0)
			{
				FS.sendPro.skill='Z';
				FS.P_Coord.x=P_temp->x;
				FS.P_Coord.z=P_temp->z;
				

			}
		}

		//m_Hero.Lightning(V_temp.x,V_temp.z,m_IsLButtonDown);

	}
	m_Hero.ShowLife();
}


void GameWorld::FightEvent()
{
	if (firstin==1)
	{
		startgametime.SetTimenow();
		firstin=2;
	}
	else if (firstin==2  && startgametime.ReachTime(200) /*&& goon*/)
	{
		//memcpy(FS.sendMsg,"WEREADY\0",sizeof("WEREADY\0"));
		FS.SendMsg("WEREADY\0");
		firstin=0;
	}
	else if (firstin==0)
	{
		F_HeroEvent();
		HandleSendMessage();
		
	}

	int len=FS.RecvMsg(FS.recvMsg);
	static Ptotocal pro;
	memcpy(&pro,FS.recvMsg,sizeof(Ptotocal));
	if (LOSECONNECT==len)
	{
		TerminateApplication();
	}
	else if (len > 0)
	{
		//MessageBox(NULL,pro.flag,pro.flag,MB_OK);
		if (strcmp(pro.flag,"FIGHTINFO")==0 )
		{
			memcpy(&FS.recvPro, pro.msg, sizeof(FS.recvPro));
			HandleRecvMessage(FS.recvMsg+sizeof(pro));
		}
		else if (strcmp(pro.flag,"OWNID")==0)
		{
			memcpy(&m_Hero.ownid,pro.msg,sizeof(m_Hero.ownid));
		}
		else if (strcmp(pro.flag,"HEROINFO")==0)
		{
			firstin=0;
			HeroInfo info;
			
			memcpy(&info.hero_num,pro.msg,sizeof(info.hero_num));
			FS.heroNum=info.hero_num;
			char *addr=pro.msg+sizeof(info.hero_num);
			for (int i=0;i<info.hero_num;i++)
			{
				memcpy(&info.base_info,addr,sizeof(info.base_info));
				addr=addr+sizeof(info.base_info);
				FS.fightHero[i]=new CHero;

				FS.heroid[i]=info.base_info.hero_id;
				if (info.base_info.hero_id != m_Hero.ownid)
				{

					FS.fightHero[i]->setHeroID(info.base_info.hero_id);
					FS.fightHero[i]->Init(isConnectGame);
					FS.fightHero[i]->setHeroCoord(info.base_info.x,info.base_info.z);
					FS.fightHero[i]->m_Color.x=1.0-0.15*(float)i;
					FS.fightHero[i]->m_Color.y=0.5;
					FS.fightHero[i]->m_Color.z=0.25+0.10*(float)i;
				}
				else
				{
					FS.fightHero[i]->setHeroID(info.base_info.hero_id);
					m_Hero.setHeroCoord(info.base_info.x,info.base_info.z);
					Vector3 m_CameraPosition=m_Camera.setCameraPosition(m_Hero.m_RoleCoord.x, m_Hero.m_RoleCoord.y, m_Hero.m_RoleCoord.z);
					/** ��������� */
					m_Camera.setCamera( m_CameraPosition.x,m_CameraPosition.y, m_CameraPosition.z,  m_Hero.m_RoleCoord.x, m_Hero.m_RoleCoord.y, m_Hero.m_RoleCoord.z,  0, 1, 0,false);
				}
			}
		}
	}
	//return ;	
}


void GameWorld::SetFalse()
{
	m_IsLButtonDown=false;
}


/** ���ƺ��� */
void GameWorld::Draw()											
{
// 	if (!m_Hero.isLightning)
// 	{
// 		SetCursor(cur);
// 	}
	SetCursor(cur);
	if (m_is_open_menu==false)
	{
		if (isConnectGame)
		{
//			InitGame();

			glClearColor(0.5, 0.5, 0.5, 0.0);//���óɻ�ɫ
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			
			glLoadIdentity();

			//���������
			m_Camera.setLook(SceneNum);
			//��Ⱦ����
			m_Terrain.render(2);
			FightEvent();	
			//HeroEvent();
			MenuEvent();
		}
		else
		{
			glClearColor(0.5, 0.5, 0.5, 0.0);//���óɻ�ɫ
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			
			glLoadIdentity();
			//���������
			m_Camera.setLook(SceneNum);
			//��Ⱦ����
			if (SceneNum==0)
			{
				m_camp.Render();
			}
			else
			{
				m_Terrain.render(SceneNum);
				MonsterEvent();
			}
			
			
			//glPushMatrix();
			HeroEvent();
			//glPopMatrix();
			MenuEvent();
			
		}

		glPushMatrix();
		m_font.PrintText("��л�����ˣ��ر��л���Լ�",-1.0f,-3.0f);
		m_font.PrintText("				--  ������",-1.0f,-3.4f);
//  		char temp[64];
//  		sprintf(temp,"x=%f,z=%f",FS.heroCoord.x,FS.heroCoord.z);
//  		m_font.PrintText(temp,-1,3.4);
		glPopMatrix();

		if(IsWin)
		{
			glPushMatrix();
			char *temp="WIN";
			m_infofont.PrintText(temp,-1,1);
			glPopMatrix();
		}
		if(m_Hero.IsDead())
		{
			glPushMatrix();
			char *temp="LOSE";
			m_infofont.PrintText(temp,-1,1);
			glPopMatrix();
		}
	}
	SetFalse();
	glFlush();	                 /**< ǿ��ִ�����е�OpenGL���� */
	Sleep(20);
	//MessageBox(NULL,"sdf","sdf",MB_OK);
}


