

HeroAttr=
{
	WPTextture="..\\Hero\\weapon.pcx",
	WPMd2file="..\\Hero\\weapon.md2",
	Textture='..\\Hero\\act1.pcx',		--纹理
	Md2file='..\\Hero\\act1.md2',		--md2文件名
	Angle=180,					--初始化角度
	HP=200,						--当前血量
	FullHP=200,					--满血
	Defense=8,					--防御值
	ShootRange=110,				--射击范围
	Coord_X=0,					--英雄坐标
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

