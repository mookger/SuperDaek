#include <arsa.h>

int score = 0;							//ไว้เช็ค score
int st = 0;								//ไว้เช็คให้ข้อความ score ปรากฏ
irr::gui::IGUIFont* font = 0;

void init()
{
	g_psd->load("menu.psd");							//โหลดหน้าเมนูขึ้นมา
	g_psd->loadFont("LayijiMahaniyomV1041.ttf", 40);			//โหลดฟอนต์คะแนน
	font = arsa_CreateFont(g_device, "LayijiMahaniyomV1041.ttf", 190);	//สร้างฟ้อนสำหรับ WIN
	g_snd->Load("yum.mp3"); // cache snd
	g_snd->Load("sound.mp3"); // cache snd
	g_snd->Play("sound.mp3", 0, true); // play bg snd
}

void update()
{
	//หน้าเมนู
	if (g_input->getTouchState(0) == irr::EKS_RELEASED)		//เมื่อกด tap และจะเช็คว่า tap ที่ตำแหน่งอะไร 
	{
		int x = g_input->getTouchX(0);
		int y = g_input->getTouchY(0);

		if (g_psd->hit(x, y, irr::core::stringc("start")))	//ถ้ากดปุ่ม Start
		{
			g_snd->Stop("sound.mp3");
			g_snd->Play("sound.mp3", 0, true);
			score = 0;
			g_psd->load("game.psd");
		}
		if (g_psd->hit(x, y, irr::core::stringc("credit")))	//ถ้ากด credit
		{
			g_psd->load("credit.psd");
		}
	}



	//GAME
	irr::s32 pac = g_psd->getPsdId("pac");				//ประกาศตัวแปรสำหรับตัว rabbit

	if (pac != -1)
	{
		st++;														//ไว้เช็คตัวขึ้น text score
		irr::f32 x, y;
		//เคลื่อนไหวของ pacman 
		irr::f32 speed = 500 * g_psd->getDeltaTime();				//ตั้งค่าความเร็วในการเคลื่อน
		g_psd->getPsdPos(pac, x, y);					//ส่งตำแหน่งที่แพคแมนอยู่
		if (g_input->isTouchLeft(0))
		{
			g_psd->setPsdDrawFlags(pac, irr::arsa::EDT_HFLIP | irr::arsa::EDT_TOP | irr::arsa::EDT_LEFT);	//สั่งให้ตัว rabbit กลับด้าน
			x -= speed;
		}
		if (g_input->isTouchRight(0))
		{
			g_psd->setPsdDrawFlags(pac, irr::arsa::EDT_DEFAULT);
			x += speed;
		}
		if (g_input->isTouchUp(0))
			y -= speed;
		if (g_input->isTouchDown(0))
			y += speed;
		g_psd->setPsdPos(pac, x, y);					//ส่งตำแหน่งที่แพคแมนอยู่


		//การกินขนม
		irr::s32 id = g_psd->hit(pac);			//กำหนดให้ เมื่อตัว pac โดน คือ เก็บค่าไว้ใน id
		if (id != -1 && g_psd->getPsdName(id).find("w") != -1)	//id โดนที่ตัวของ แครอทแล้ว
		{
			score++;											//score จะเพิ่มขึ้น
			g_psd->setPsdVisible(id, false);					//ให้แครอทตัวนั้นหายไป

			if (score == 50)									//ถ้า score เท่ากับ 50
				g_snd->Play("win.m4a");
			else
				g_snd->Play("yum.mp3");
		}
	}


	//play again
	if (g_input->getTouchState(0) == irr::EKS_RELEASED)
	{
		int x = g_input->getTouchX(0);
		int y = g_input->getTouchY(0);

		if (g_psd->hit(x, y, irr::core::stringc("play")))		//ถ้ากดปุ่ม play again
		{
			score = 0;											//รีเซ็คค่า score ใหม่
			g_psd->load("game.psd");							//จะโหลดไปที่หน้าเกม
		}
	}

	//backtomenu
	if (g_input->getTouchState(0) == irr::EKS_RELEASED)
	{
		int x = g_input->getTouchX(0);
		int y = g_input->getTouchY(0);

		if (g_psd->hit(x, y, irr::core::stringc("menu")))		//ถ้ากดปุ่ม menu
		{
			g_psd->load("menu.psd");							//จะโหลดไปยังหน้าเมนู
			st = 0;												//รีเซ็ตค่า st
		}
	}

	irr::s32 snd = g_psd->getPsdId("sound");
	irr::s32 nosnd = g_psd->getPsdId("nosound");


	//sound
	if (g_input->getTouchState(0) == irr::EKS_RELEASED)
	{
		int x = g_input->getTouchX(0);
		int y = g_input->getTouchY(0);

		if (g_psd->hit(x, y, irr::core::stringc("sound")))		//ถ้ากดปุ่ม menu
		{
			g_psd->setPsdVisible(snd, false);
			g_psd->setPsdVisible(nosnd, true);
			g_snd->Pause("sound.mp3");
		}
		else if (g_psd->hit(x, y, irr::core::stringc("nosound")))		//ถ้ากดปุ่ม menu
		{
			g_psd->setPsdVisible(snd, true);
			g_psd->setPsdVisible(nosnd, false);
			g_snd->Play("sound.mp3");
		}
	}


	//Credit and back
	irr::s32 back = g_psd->getPsdId("back");

	if (g_input->getTouchState(0) == irr::EKS_RELEASED)
	{
		int x = g_input->getTouchX(0);
		int y = g_input->getTouchY(0);

		if (g_psd->hit(x, y, irr::core::stringc("back")))
		{
			g_psd->load("menu.psd");
		}
	}

}


void game01(int finish)
{

	chk_state = 1;
	//ปุ่ม
	if (g_input->getTouchState(0) == irr::EKS_RELEASED)		//เมื่อกด tap และจะเช็คว่า tap ที่ตำแหน่งอะไร 			
	{
		int x = g_input->getTouchX(0);
		int y = g_input->getTouchY(0);

		if (g_psd->hit(x, y, irr::core::stringc("btn_reset")))
		{
			state = STATE_STATE;
		}
	}

	//GAME
	irr::s32 ku1 = g_psd->getPsdId("ku1");
	irr::s32 bg = g_psd->getPsdId("bg");
	irr::s32 bg_summer = g_psd->getPsdId("bg_summer");

	//heart
	irr::s32 heart = g_psd->getPsdId("heart");
	irr::s32 heart0 = g_psd->getPsdId("heart0");
	irr::s32 heart1 = g_psd->getPsdId("heart1");
	irr::s32 heart2 = g_psd->getPsdId("heart2");
	irr::s32 heart3 = g_psd->getPsdId("heart3");
	irr::s32 heart4 = g_psd->getPsdId("heart4");

	//summer
	irr::s32 summer_s = g_psd->getPsdId("summer_s");
	irr::s32 summer_u = g_psd->getPsdId("summer_u");
	irr::s32 summer_m1 = g_psd->getPsdId("summer_m1");
	irr::s32 summer_m2 = g_psd->getPsdId("summer_m2");
	irr::s32 summer_e = g_psd->getPsdId("summer_e");
	irr::s32 summer_r = g_psd->getPsdId("summer_r");

	irr::s32 s_r = g_psd->getPsdId("s_r");
	irr::s32 s_e = g_psd->getPsdId("s_u");
	irr::s32 s_m1 = g_psd->getPsdId("s_m1");
	irr::s32 s_m2 = g_psd->getPsdId("s_m2");
	irr::s32 s_u = g_psd->getPsdId("s_u");
	irr::s32 s_s = g_psd->getPsdId("s_s");
	irr::s32 s_b1 = g_psd->getPsdId("s_b1");
	irr::s32 s_b2 = g_psd->getPsdId("s_b2");
	irr::s32 s_b3 = g_psd->getPsdId("s_b3");
	irr::s32 s_c1 = g_psd->getPsdId("s_c1");
	irr::s32 s_c2 = g_psd->getPsdId("s_c2");

	//btn
	irr::s32 btn_back = g_psd->getPsdId("btn_back");
	irr::s32 btn_forward = g_psd->getPsdId("btn_forward");
	irr::s32 btn_reset = g_psd->getPsdId("btn_reset");
	irr::s32 btn_jump = g_psd->getPsdId("btn_jump");

	if (ku1 != -1)
	{
		st++;
		irr::f32 x1, y1;

		//เคลื่อนไหวของ ku
		irr::f32 speed = 500 * g_psd->getDeltaTime();				//ตั้งค่าความเร็วในการเคลื่อน
		g_psd->getPsdPos(ku1, x1, y1);					//ส่งตำแหน่งที่แพคแมนอยู่


		/*
		if (g_input->getTouchState(0) == irr::EKS_DOWN)		//เมื่อกด tap และจะเช็คว่า tap ที่ตำแหน่งอะไร
		{
		int x = g_input->getTouchX(0);
		int y = g_input->getTouchY(0);

		if (g_psd->hit(x, y, irr::core::stringc("btn_back")))
		{
		x1 -= speed;
		}
		if (g_psd->hit(x, y, irr::core::stringc("btn_forward")))
		{
		x1 += speed;
		}
		if (g_psd->hit(x, y, irr::core::stringc("btn_jump")))
		{
		y1 -= speed;
		irr::s32 id = g_psd->hit(ku);
		if (g_psd->getPsdName(id).find("ground") != -1){}
		else
		y1 += speed;
		}
		}
		*/

		if (count == 5)
		{
		}					//ถ้าหัวใจหมดก็เดินไม่ได้
		else
		{
			if (g_input->isTouchLeft(0))
			{
				x1 -= speed;
			}
			if (g_input->isTouchRight(0))
			{
				x1 += speed;
			}
			if (g_input->isTouchUp(0))
			{
				y1 -= speed;

			}
			if (g_input->isTouchDown(0))
			{
				irr::s32 id = g_psd->hit(ku1);
				if (g_psd->getPsdName(id).find("ground") != -1){}
				else
					y1 += speed;
			}

			g_psd->setPsdPos(ku1, x1, y1);					//ส่งตำแหน่งที่แพคแมนอยู่
		}

		//พื้นหลังเลื่อนตามตัวละคร
		g_psd->setPosition(irr::core::vector3df(300 - x1, 0, 0));
		g_psd->setPsdPos(bg_summer, x1 - 40, 11);

		//หัวใจก็เลื่อนตามตัวละคร
		g_psd->setPsdPos(heart, x1 - 285, 20);
		g_psd->setPsdPos(heart0, x1 - 285, 20);
		g_psd->setPsdPos(heart1, x1 - 247, 20);
		g_psd->setPsdPos(heart2, x1 - 211, 20);
		g_psd->setPsdPos(heart3, x1 - 174, 20);
		g_psd->setPsdPos(heart4, x1 - 137, 20);

		//ตัวหนังสือก็เคลื่อนตามตัวละคร
		g_psd->setPsdPos(summer_s, x1, 45);
		g_psd->setPsdPos(summer_u, x1 + 54, 45);
		g_psd->setPsdPos(summer_m1, x1 + 110, 45);
		g_psd->setPsdPos(summer_m2, x1 + 172, 45);
		g_psd->setPsdPos(summer_e, x1 + 231, 45);
		g_psd->setPsdPos(summer_r, x1 + 286, 45);

		//ปุ่มเลื่อนตามตัวละคร
		g_psd->setPsdPos(btn_back, x1 - 280, 540);
		g_psd->setPsdPos(btn_forward, x1 - 170, 540);
		g_psd->setPsdPos(btn_reset, x1 + 550, 30);
		g_psd->setPsdPos(btn_jump, x1 + 540, 520);

		//SUMMER
		irr::s32 id = g_psd->hit(ku1);
		//ถูกตัว
		if (id != -1 && g_psd->getPsdName(id).find("s_s") != -1)	//ถ้าเก็บตัว b -> ให้ b หายไป 
		{
			g_psd->setPsdVisible(id, false);
			g_psd->setPsdVisible(summer_s, true);
			score++;
		}
		if (id != -1 && g_psd->getPsdName(id).find("s_u") != -1)	//ถ้าเก็บตัว b -> ให้ b หายไป 
		{
			g_psd->setPsdVisible(id, false);
			g_psd->setPsdVisible(summer_u, true);
			score++;
		}
		if (id != -1 && g_psd->getPsdName(id).find("s_m1") != -1)	//ถ้าเก็บตัว b -> ให้ b หายไป 
		{
			g_psd->setPsdVisible(id, false);
			g_psd->setPsdVisible(summer_m1, true);
			score++;
		}
		if (id != -1 && g_psd->getPsdName(id).find("s_m2") != -1)	//ถ้าเก็บตัว b -> ให้ b หายไป 
		{
			g_psd->setPsdVisible(id, false);
			g_psd->setPsdVisible(summer_m2, true);
			score++;
		}
		if (id != -1 && g_psd->getPsdName(id).find("s_e") != -1)	//ถ้าเก็บตัว b -> ให้ b หายไป 
		{
			g_psd->setPsdVisible(id, false);
			g_psd->setPsdVisible(summer_e, true);
			score++;
		}
		if (id != -1 && g_psd->getPsdName(id).find("s_r") != -1)	//ถ้าเก็บตัว b -> ให้ b หายไป 
		{
			g_psd->setPsdVisible(id, false);
			g_psd->setPsdVisible(summer_r, true);
			score++;
		}

		//ผิดตัว
		if (id != -1 && g_psd->getPsdName(id).find("s_b1") != -1)	//ถ้าเก็บตัว a -> ให้ตัว a หายไป หัวใจลด	
		{
			g_psd->setPsdVisible(id, false);
			count++;
		}
		if (id != -1 && g_psd->getPsdName(id).find("s_b2") != -1)	//ถ้าเก็บตัว b -> ให้ b หายไป 
		{
			g_psd->setPsdVisible(id, false);
			count++;
		}
		if (id != -1 && g_psd->getPsdName(id).find("s_b3") != -1)	//ถ้าเก็บตัว b -> ให้ b หายไป 
		{
			g_psd->setPsdVisible(id, false);
			count++;
		}
		if (id != -1 && g_psd->getPsdName(id).find("s_c1") != -1)	//ถ้าเก็บตัว b -> ให้ b หายไป 
		{
			g_psd->setPsdVisible(id, false);
			count++;
		}
		if (id != -1 && g_psd->getPsdName(id).find("s_c2") != -1)	//ถ้าเก็บตัว b -> ให้ b หายไป 
		{
			g_psd->setPsdVisible(id, false);
			count++;
		}


		//heart
		if (count == 1)
		{
			g_psd->setPsdVisible(heart4, false);
		}
		if (count == 2)
		{
			g_psd->setPsdVisible(heart3, false);
		}
		if (count == 3)
		{
			g_psd->setPsdVisible(heart2, false);
		}
		if (count == 4)
		{
			g_psd->setPsdVisible(heart1, false);
		}
		if (count == 5)
		{
			g_psd->setPsdVisible(heart0, false);
		}

	}
}

void game02()
{
	chk_state = 2;
	//GAME
	irr::s32 ku2 = g_psd->getPsdId("ku2");
	irr::s32 bg = g_psd->getPsdId("bg");
	irr::s32 bg_ghost = g_psd->getPsdId("bg_ghost");

	//heart
	irr::s32 heart = g_psd->getPsdId("heart");
	irr::s32 heart0 = g_psd->getPsdId("heart0");
	irr::s32 heart1 = g_psd->getPsdId("heart1");
	irr::s32 heart2 = g_psd->getPsdId("heart2");
	irr::s32 heart3 = g_psd->getPsdId("heart3");
	irr::s32 heart4 = g_psd->getPsdId("heart4");

	//summer
	irr::s32 ghost_g = g_psd->getPsdId("ghost-g");
	irr::s32 ghost_h = g_psd->getPsdId("ghost-h");
	irr::s32 ghost_o = g_psd->getPsdId("ghost-o");
	irr::s32 ghost_s = g_psd->getPsdId("ghost-s");
	irr::s32 ghost_t = g_psd->getPsdId("ghost-t");

	irr::s32 g_c = g_psd->getPsdId("g-c");
	irr::s32 g_m = g_psd->getPsdId("g-m");
	irr::s32 g_h = g_psd->getPsdId("g-h");
	irr::s32 g_s = g_psd->getPsdId("g-s");
	irr::s32 g_t = g_psd->getPsdId("g-t");
	irr::s32 g_a = g_psd->getPsdId("g-a");
	irr::s32 g_b = g_psd->getPsdId("g-b");
	irr::s32 g_g = g_psd->getPsdId("g-g");
	irr::s32 g_o = g_psd->getPsdId("g-o");
	irr::s32 g_i = g_psd->getPsdId("g_i");

	//btn
	irr::s32 btn_back = g_psd->getPsdId("btn_back");
	irr::s32 btn_forward = g_psd->getPsdId("btn_forward");
	irr::s32 btn_reset = g_psd->getPsdId("btn_reset");
	irr::s32 btn_jump = g_psd->getPsdId("btn_jump");

	if (ku2 != -1)
	{
		st++;
		irr::f32 x2, y2;

		//เคลื่อนไหวของ ku
		irr::f32 speed = 500 * g_psd->getDeltaTime();				//ตั้งค่าความเร็วในการเคลื่อน
		g_psd->getPsdPos(ku2, x2, y2);					//ส่งตำแหน่งที่แพคแมนอยู่

		if (count == 5)
		{
		}					//ถ้าหัวใจหมดก็เดินไม่ได้
		else
		{
			if (g_input->isTouchLeft(0))
			{
				x2 -= speed;
			}
			if (g_input->isTouchRight(0))
			{
				x2 += speed;
			}
			if (g_input->isTouchUp(0))
			{
				y2 -= speed;

			}
			if (g_input->isTouchDown(0))
			{
				irr::s32 id = g_psd->hit(ku2);
				if (g_psd->getPsdName(id).find("ground") != -1){}
				else
					y2 += speed;
			}

			g_psd->setPsdPos(ku2, x2, y2);					//ส่งตำแหน่งที่แพคแมนอยู่
		}


		//พื้นหลังเลื่อนตามตัวละคร
		g_psd->setPosition(irr::core::vector3df(300 - x2, 0, 0));
		g_psd->setPsdPos(bg_ghost, x2 - 40, 11);

		//หัวใจก็เลื่อนตามตัวละคร
		g_psd->setPsdPos(heart, x2 - 285, 20);
		g_psd->setPsdPos(heart0, x2 - 285, 20);
		g_psd->setPsdPos(heart1, x2 - 247, 20);
		g_psd->setPsdPos(heart2, x2 - 211, 20);
		g_psd->setPsdPos(heart3, x2 - 174, 20);
		g_psd->setPsdPos(heart4, x2 - 137, 20);

		//ตัวหนังสือก็เคลื่อนตามตัวละคร
		g_psd->setPsdPos(ghost_g, x2, 45);
		g_psd->setPsdPos(ghost_h, x2 + 54, 45);
		g_psd->setPsdPos(ghost_o, x2 + 110, 45);
		g_psd->setPsdPos(ghost_s, x2 + 172, 45);
		g_psd->setPsdPos(ghost_t, x2 + 231, 45);

		//ปุ่มเลื่อนตามตัวละคร
		g_psd->setPsdPos(btn_back, x2 - 280, 540);
		g_psd->setPsdPos(btn_forward, x2 - 170, 540);
		g_psd->setPsdPos(btn_reset, x2 + 550, 30);
		g_psd->setPsdPos(btn_jump, x2 + 540, 520);

		//GHOST
		irr::s32 id = g_psd->hit(ku2);
		//ถูกตัว
		if (id != -1 && g_psd->getPsdName(id).find("g-g") != -1)
		{
			g_psd->setPsdVisible(id, false);
			g_psd->setPsdVisible(ghost_g, true);
			score++;
		}
		if (id != -1 && g_psd->getPsdName(id).find("g-h") != -1)
		{
			g_psd->setPsdVisible(id, false);
			g_psd->setPsdVisible(ghost_h, true);
			score++;
		}
		if (id != -1 && g_psd->getPsdName(id).find("g-o") != -1)
		{
			g_psd->setPsdVisible(id, false);
			g_psd->setPsdVisible(ghost_o, true);
			score++;
		}
		if (id != -1 && g_psd->getPsdName(id).find("g-s") != -1)
		{
			g_psd->setPsdVisible(id, false);
			g_psd->setPsdVisible(ghost_s, true);
			score++;
		}
		if (id != -1 && g_psd->getPsdName(id).find("g-t") != -1)
		{
			g_psd->setPsdVisible(id, false);
			g_psd->setPsdVisible(ghost_t, true);
			score++;
		}

		//ผิดตัว
		if (id != -1 && g_psd->getPsdName(id).find("g-c") != -1)	//ถ้าเก็บตัว a -> ให้ตัว a หายไป หัวใจลด	
		{
			g_psd->setPsdVisible(id, false);
			count++;
		}
		if (id != -1 && g_psd->getPsdName(id).find("g-m") != -1)	//ถ้าเก็บตัว b -> ให้ b หายไป 
		{
			g_psd->setPsdVisible(id, false);
			count++;
		}
		if (id != -1 && g_psd->getPsdName(id).find("g-a") != -1)	//ถ้าเก็บตัว b -> ให้ b หายไป 
		{
			g_psd->setPsdVisible(id, false);
			count++;
		}
		if (id != -1 && g_psd->getPsdName(id).find("g-b") != -1)	//ถ้าเก็บตัว b -> ให้ b หายไป 
		{
			g_psd->setPsdVisible(id, false);
			count++;
		}
		if (id != -1 && g_psd->getPsdName(id).find("g_i") != -1)	//ถ้าเก็บตัว b -> ให้ b หายไป 
		{
			g_psd->setPsdVisible(id, false);
			count++;
		}


		//heart
		if (count == 1)
		{
			g_psd->setPsdVisible(heart4, false);
		}
		if (count == 2)
		{
			g_psd->setPsdVisible(heart3, false);
		}
		if (count == 3)
		{
			g_psd->setPsdVisible(heart2, false);
		}
		if (count == 4)
		{
			g_psd->setPsdVisible(heart1, false);
		}
		if (count == 5)
		{
			g_psd->setPsdVisible(heart0, false);
		}


	}
}

void hud()
{
	if (st>1)
	{
		irr::core::stringw str = "Score: ";
		str += score;
		g_psd->drawFont(727, 42, str, irr::arsa::EDT_DEFAULT, irr::video::SColor(255, 255, 255, 255));
		if (score == 50)
		{
			font->draw("YOU WIN!", irr::core::recti(0, 0, 960, 640), irr::video::SColor(255, 98, 51, 20), true, true);
		}
	}
}


void deinit()
{
	font->drop();
}

int main(int argc, char* args[])
{
	arsa_SetFuncInit(init);
	arsa_SetFuncUpdate(update);
	arsa_SetFuncHud(hud);
	arsa_SetFuncDeInit(deinit);
	arsa_EasyStart();

	return 0;
}
