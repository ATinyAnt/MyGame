

HeroAttr=
{
	WPTextture="..\\Hero\\weapon.pcx",
	WPMd2file="..\\Hero\\weapon.md2",
	Textture='..\\Hero\\act1.pcx',		--����
	Md2file='..\\Hero\\act1.md2',		--md2�ļ���
	Angle=180,					--��ʼ���Ƕ�
	HP=200,						--��ǰѪ��
	FullHP=200,					--��Ѫ
	Defense=8,					--����ֵ
	ShootRange=110,				--�����Χ
	Coord_X=0,					--Ӣ������
	Coord_Y=2,
	Coord_Z=830,
	Speed=0.2,
	--DesCoord_X=0,
	--DesCoord_Y=0,
	--DesCoord_Z=300,

};


function Hero(data)
	return HeroAttr[data];
end

function AllHeroData()
	return HeroAttr['Speed'],
			HeroAttr['WPTextture'],
			HeroAttr['WPMd2file'],
			HeroAttr['Textture'],
			HeroAttr['Md2file'],
			HeroAttr['Angle'],
			HeroAttr['HP'],
			HeroAttr['FullHP'],
			HeroAttr['Defense'],
			HeroAttr['ShootRange'],
			HeroAttr['Coord_X'],
			HeroAttr['Coord_Y'],
			HeroAttr['Coord_Z'];
end

