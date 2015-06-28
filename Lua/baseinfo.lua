TRUE=1;
FALSE=0;
Base=
{
	platform_path='D:\\TEST\\Platform\\Release\\Platform.exe';		--对战平台路径
	server_ip='192.168.1.102';
	server_port='6000';
	width=1200;
	height=800;
	isFullScreen=FALSE;
	scene0=
	{
		Textture0="Monster\\goblin\\goblin.pcx",
		Md2file0="Monster\\goblin\\tris.md2",
		WPTextture0="Monster\\goblin\\weapon.pcx",
		WPMd2file0="Monster\\goblin\\weapon.md2",
		Coord0_x=130,
		Coord0_y=24,
		Coord0_z=900,

		Textture1="Monster\\pknight\\evil.pcx",
		Md2file1="Monster\\pknight\\tris.md2",
		WPTextture1="Monster\\pknight\\w_blaster.pcx",
		WPMd2file1="Monster\\pknight\\weapon.md2",
		Coord1_x=-130,
		Coord1_y=24,
		Coord1_z=900,

		Textture2="Monster\\phantom\\phantom_white.bmp",
		Md2file2="Monster\\phantom\\tris.md2",
		WPTextture2="Monster\\phantom\\weapon.pcx",
		WPMd2file2="Monster\\phantom\\weapon.md2",
		Coord2_x=0,
		Coord2_y=24,
		Coord2_z=1000,

	};

	scene1=
	{
		wall="data\\wall.bmp";
		terrain="";
	};
}

function getBaseScene0Data()
	return Base['scene0']['Textture1'],
			Base['scene0']['Md2file1'],
			Base['scene0']['WPTextture1'],
			Base['scene0']['WPMd2file1'],
			Base['scene0']['Coord1_x'],
			Base['scene0']['Coord1_y'],
			Base['scene0']['Coord1_z'],

			Base['scene0']['Textture0'],
			Base['scene0']['Md2file0'],
			Base['scene0']['WPTextture0'],
			Base['scene0']['WPMd2file0'],
			Base['scene0']['Coord0_x'],
			Base['scene0']['Coord0_y'],
			Base['scene0']['Coord0_z'],

			Base['scene0']['Textture2'],
			Base['scene0']['Md2file2'],
			Base['scene0']['WPTextture2'],
			Base['scene0']['WPMd2file2'],
			Base['scene0']['Coord2_x'],
			Base['scene0']['Coord2_y'],
			Base['scene0']['Coord2_z'];


end

function getIsFullScreen()
	return Base['isFullScreen'];
end

function getScreenSize()
	return Base['width'],
			Base['height'];
end
