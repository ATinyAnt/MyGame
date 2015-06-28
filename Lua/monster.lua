MapRange=470;
function setCoordRange(range)
	MapRange=range;
end


--这个一定要设置成全局变量

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
	Defense=2,							--怪物的防御
	Coord_X=0,
	Coord_Y=10,
	Coord_Z=0,
	ChaseRange=150,
	ShootRange=100,
	Scale=0.10,							--放大倍数
	Speed=0.25,							--动画播放速度
	Distance=0.5;						--怪物一步移动的距离
	Skillcooltime=3000,					--技能冷却时间
	Monsternum=1,						--怪物1有5只
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
	Defense=2,							--怪物的防御
	Coord_X=0,
	Coord_Y=10,
	Coord_Z=0,
	ChaseRange=160,
	ShootRange=100,
	Scale=0.10,							--放大倍数
	Speed=0.45,							--动画播放速度
	Distance=1.0;						--怪物一步移动的距离
	Skillcooltime=1000,					--技能冷却时间
	Monsternum=6,						--怪物1有5只
	Hasweapon=1,
};

--function Monster1(data)
--	return Monster1Attr[data];
--end


--基本的怪物属性
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

--基本的怪物属性
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


--不同的怪物属性
function DiffMonster1Data()
	return  Monster1Attr['Coord_X'],
			Monster1Attr['Coord_Z'];
end


--return 0 表示不手动设置怪物坐标，其它数字表示要设置坐标的怪物数
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












