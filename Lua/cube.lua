CubeNum=7

Cube=
{
	--中1
	bx1= -96.0,
	bz1= -60.0,
	ex1=  96.0,
	ez1=  60.0,

	--前2
	bx2= -288.0,
	bz2=  210.0,
	ex2= -96.0,
	ez2=  330.0,

	bx3=  96.0,
	bz3=  210.0,
	ex3=  288.0,
	ez3=  330.0,

	--中2
	bx4= -480.0,
	bz4= -60.0,
	ex4= -288.0,
	ez4=  60.0,

	bx5=  288.0,
	bz5= -60.0,
	ex5=  480.0,
	ez5=  60.0,

	--后2
	bx6= -288.0,
	bz6= -330.0,
	ex6= -96.0,
	ez6= -210.0,

	bx7=  96.0,
	bz7= -330.0,
	ex7=  288.0,
	ez7= -210.0,
}

function GetCubeNum()
	return CubeNum;
end

function AllCubeData()
	return Cube['bx1'],
			Cube['bz1'],
			Cube['ex1'],
			Cube['ez1'],

			Cube['bx2'],
			Cube['bz2'],
			Cube['ex2'],
			Cube['ez2'],

			Cube['bx3'],
			Cube['bz3'],
			Cube['ex3'],
			Cube['ez3'],

			Cube['bx4'],
			Cube['bz4'],
			Cube['ex4'],
			Cube['ez4'],

			Cube['bx5'],
			Cube['bz5'],
			Cube['ex5'],
			Cube['ez5'],

			Cube['bx6'],
			Cube['bz6'],
			Cube['ex6'],
			Cube['ez6'],

			Cube['bx7'],
			Cube['bz7'],
			Cube['ex7'],
			Cube['ez7'];
end

