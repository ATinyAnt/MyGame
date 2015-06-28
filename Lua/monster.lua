MapRange=470;
function setCoordRange(range)
	MapRange=range;
end


--���һ��Ҫ���ó�ȫ�ֱ���

--math.randomseed(tostring(os.time()):reverse():sub(1, 6));
--function getCoord()

	--math.randomseed(os.time());
--	return 940*math.random()-470;
--end


Monster1Attr={
	Textture="..\\Monster\\ogro\\ogrobase.pcx",
	Md2file="..\\Monster\\ogro\\tris.md2",
	WPTextture="..\\Monster\\ogro\\weapon.pcx",
	WPMd2file="..\\Monster\\ogro\\weapon.md2",
	HP=200,
	FullHP=200,
	Angle=0,
	Defense=2,							--����ķ���
	Coord_X=0,
	Coord_Y=10,
	Coord_Z=0,
	ChaseRange=150,
	ShootRange=100,
	Scale=0.10,							--�Ŵ���
	Speed=0.25,							--���������ٶ�
	Distance=0.5;						--����һ���ƶ��ľ���
	Skillcooltime=3000,					--������ȴʱ��
	Monsternum=1,						--����1��5ֻ
	Hasweapon=1,
};


Monster2Attr={
	Textture="..\\Monster\\act3\\act3.pcx",
	Md2file="..\\Monster\\act3\\act3.md2",
	WPTextture="..\\Monster\\act3\\weapon.pcx",
	WPMd2file="..\\Monster\\act3\\weapon.md2",
	HP=200,
	FullHP=200,
	Angle=0,
	Defense=2,							--����ķ���
	Coord_X=0,
	Coord_Y=10,
	Coord_Z=0,
	ChaseRange=160,
	ShootRange=100,
	Scale=0.10,							--�Ŵ���
	Speed=0.45,							--���������ٶ�
	Distance=1.0;						--����һ���ƶ��ľ���
	Skillcooltime=1000,					--������ȴʱ��
	Monsternum=6,						--����1��5ֻ
	Hasweapon=1,
};

--function Monster1(data)
--	return Monster1Attr[data];
--end


--�����Ĺ�������
function BaseMonster1Data()
	return	Monster1Attr['Distance'],
			Monster1Attr['Textture'],
			Monster1Attr['Md2file'],
			Monster1Attr['WPTextture'],
			Monster1Attr['WPMd2file'],
			Monster1Attr['HP'],
			Monster1Attr['FullHP'],
			Monster1Attr['Angle'],
			Monster1Attr['Defense'],
			--Monster1Attr['Coord_X'],
			Monster1Attr['Coord_Y'],
			--Monster1Attr['Coord_Z'],
			Monster1Attr['ChaseRange'],
			Monster1Attr['ShootRange'],
			Monster1Attr['Scale'],
			Monster1Attr['Speed'],
			Monster1Attr['Skillcooltime'],
			Monster1Attr['Monsternum'],

			Monster1Attr['Hasweapon'];

end

--�����Ĺ�������
function BaseMonster2Data()
	return	Monster2Attr['Distance'],
			Monster2Attr['Textture'],
			Monster2Attr['Md2file'],
			Monster2Attr['WPTextture'],
			Monster2Attr['WPMd2file'],
			Monster2Attr['HP'],
			Monster2Attr['FullHP'],
			Monster2Attr['Angle'],
			Monster2Attr['Defense'],
			--Monster1Attr['Coord_X'],
			Monster2Attr['Coord_Y'],
			--Monster1Attr['Coord_Z'],
			Monster2Attr['ChaseRange'],
			Monster2Attr['ShootRange'],
			Monster2Attr['Scale'],
			Monster2Attr['Speed'],
			Monster2Attr['Skillcooltime'],
			Monster2Attr['Monsternum'],

			Monster2Attr['Hasweapon'];

end


--��ͬ�Ĺ�������
function DiffMonster1Data()
	return  Monster1Attr['Coord_X'],
			Monster1Attr['Coord_Z'];
end


--return 0 ��ʾ���ֶ����ù������꣬�������ֱ�ʾҪ��������Ĺ�����
function IsSetCoord(num)
	return 0;
end

function GetCoord()
	Coord1_X=200;
	Coord1_Z=200;
	Coord2_X=-130;
	Coord2_Z=-130;
	return Coord1_X,
			Coord1_Z,
			Coord2_X,
			Coord2_Z;

end












