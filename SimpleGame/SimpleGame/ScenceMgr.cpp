#include "stdafx.h"
#include "ScenceMgr.h"
#include "Renderer.h"
#include "Object.h"


ScenceMgr::ScenceMgr(int width,int height):characternum(0),bulletnum(0),time(0),bullettime(0),arrownum(0),team1_charactertime(0), character_frame(0), elaspedsecond(0)
{
	m_renderer = new Renderer(width, height);
	m_texbuilding1 = m_renderer->CreatePngTexture("./Resource/Building.png");
	m_texbuilding2 = m_renderer->CreatePngTexture("./Resource/agumon.png");
	m_texbackground = m_renderer->CreatePngTexture("./Resource/background.png");
	m_texcharacter = m_renderer->CreatePngTexture("./Resource/Eye_monster.png");
	m_texparticle = m_renderer->CreatePngTexture("./Resource/particle.png");

	Object* building1 = new Object(-150.f, 300.f, TEAM1_BUILDING);
	Object* building2 = new Object(0.f, 300.f, TEAM1_BUILDING);
	Object* building3 = new Object(150.f, 300.f, TEAM1_BUILDING);
	Object* building4 = new Object(-150.f, -300.f, TEAM2_BUILDING);
	Object* building5 = new Object(0.f, -300.f, TEAM2_BUILDING);
	Object* building6 = new Object(150.f, -300.f, TEAM2_BUILDING);
	soundBG = m_sound->CreateSound("./Resource/BGM.mp3");
	m_sound->PlaySoundW(soundBG, true, 20.f);

	
	building1->team = 1, building2->team = 1, building3->team = 1;
	building4->team = 2, building5->team = 2, building6->team = 2;

	Building[0] = building1;
	Building[1] = building2;
	Building[2] = building3; 
	Building[3] = building4;
	Building[4] = building5;
	Building[5] = building6;

	if (!m_renderer->IsInitialized())
	{
		cout << "Renderer could not be initialized.. " << endl;
	}

}

ScenceMgr::~ScenceMgr()
{	
	
}

//전체 오브젝트 업데이트 함수 
void ScenceMgr::Update_AllObject(float elaspedtime)
{   
	CollisionTest();

	if (arrownum >= MAX_OBJECT_COUNT) {
		arrownum = 0;
	}
	elaspedsecond = elaspedtime * 0.001f;
	
	MakeArrow(elaspedtime);

	MakeCharacter(elaspedtime);

	MakeBullet(elaspedtime);

	team2_charactertime += elaspedsecond;

	time += elaspedsecond;
	
	
	if (character_frame > 8.f) {
		character_frame = 0.f;
	}

	character_frame += elaspedsecond * 5.f;


	//캐릭터의 Life or Lifetime이 0이하가 되면 삭제해줌 
	for (int i = 0; i < MAX_OBJECT_COUNT; i++) {
		if (Characters[i] != NULL) {
			Characters[i]->Update(elaspedtime);
			if (Characters[i]->Lifetime <= 0.f || Characters[i]->Life <= 0.f) {
				delete Characters[i];
				Characters[i] = NULL;

			}
		}
	}
	
	// arrow의 Life or Lifetime이 0이하가 되면 삭제해줌  
	for (int i = 0; i < arrownum; i++) {
		if (Arrows[i] != NULL) {
			Arrows[i]->bullet_arrow_Update(elaspedtime);
			if (Arrows[i]->Lifetime <= 0.1f || Arrows[i]->Life <= 0.f || Arrows[i]->Getx() > 250 || Arrows[i]->Getx() < -250 || Arrows[i]->Gety() > 400 || Arrows[i]->Gety() < -400) {
				delete Arrows[i];
				Arrows[i] = NULL;
			}
		}
	}
		
	//총알의 Life or Lifetime이 0이하가 되면 삭제해줌 
	for (int i = 0; i < bulletnum; i++) {
		if (Bullets[i] != NULL) {
			Bullets[i]->bullet_arrow_Update(elaspedtime);
			if (Bullets[i]->Lifetime <= 0.f || Bullets[i]->Life <= 0.f || Bullets[i]->Getx() > 250 || Bullets[i]->Getx() < -250 || Bullets[i]->Gety() > 400 || Bullets[i]->Gety() < -400) {
				delete Bullets[i];
				Bullets[i] = NULL;
			}
		}
	}
	
	//빌딩 Life가 0이하가 되면 제거 
	for (int i = 0; i < 6; i++) {
		if (Building[i] != NULL) {
			if (Building[i]->Life <= 0) {
				delete Building[i];
				Building[i] = NULL;
			}
		}
	}

		
}
// character별로 3초마다 Arrow를 생성해주는 함수
void ScenceMgr::MakeArrow(float elaspedtime)
{	
	for (int i = 0; i < characternum; i++) {
		if (Characters[i] != NULL) {
			Characters[i]->arrow_time += (elaspedtime * 0.001f);
			if (Characters[i]->arrow_time > 3.f && Arrows[arrownum] == NULL) {
				if (Characters[i]->team == 1) {
					Object* Arrow = new Object(Characters[i]->Getx(), Characters[i]->Gety(), TEAM1_ARROW);
					Arrow->team = Characters[i]->team;
					Arrow->arrow_id = i; //arrow마다 id를 설정( arrow를 생성한 캐릭터와 충돌방지)
					Arrows[arrownum] = Arrow;
				}
				else if (Characters[i]->team == 2) {
					Object* Arrow = new Object(Characters[i]->Getx(), Characters[i]->Gety(), TEAM2_ARROW);
					Arrow->team = Characters[i]->team;
					Arrow->arrow_id = i;
					Arrows[arrownum] = Arrow;
				}				
				arrownum++;
				Characters[i]->arrow_time = 0.f;
			}
		}
	}
}

//Building별로 10초당 Bullet을 생성하는 함수 
void ScenceMgr::MakeBullet(float elaspedtime) 
{
	bullettime += (elaspedtime * 0.001f);
	if (bullettime >= 5.f) {
		for (int i = 0; i < 6; i++) {
			if (Building[i] != NULL) {
				if (i < 3) {
					Object* bullet = new Object(Building[i]->Getx(), Building[i]->Gety(), TEAM1_BULLET);
					Bullets[bulletnum] = bullet;
					Bullets[bulletnum]->team = 1;
					bulletnum++;
				}
				else {
					Object* bullet = new Object(Building[i]->Getx(), Building[i]->Gety(), TEAM2_BULLET);
					Bullets[bulletnum] = bullet;
					Bullets[bulletnum]->team = 2;					
					bulletnum++;
				}
			}
		}
		bullettime = 0;
	}
	

}
	
//team1 캐릭터 5초마다 생성해주는 함수 
void ScenceMgr::MakeCharacter(float elaspedtime)
{
	team1_charactertime += elaspedtime * 0.001f;
	if (team1_charactertime > 5.f) {
		Object *team1_character = new Object(rand()%500-250,rand()%400, TEAM1_CHARACTER);
		team1_character->team = 1;
		Characters[characternum] = team1_character;
		characternum++;
		team1_charactertime = 0.f;
		}
		
}

//클릭하는 위치에 team2캐릭터를 만들어주는 함수 
void ScenceMgr::Clickmake(int x, int y)
{	
	if (Characters[characternum] == NULL && team2_charactertime > 7.0f) {
		if ((float)-(y - 400) < 0) {
			Object* team2_character = new Object((float)x - 250, (float)-(y - 400), TEAM2_CHARACTER);
			team2_character->team = 2;
			Characters[characternum] = team2_character;
			characternum++;
			team2_charactertime = 0;
		}
	}
	
}
//렌더러 함수 
void ScenceMgr::RenderObject()
{
	//배경화면 렌더 
	m_renderer->DrawTexturedRect(0.f,0.f,0.f,800.f,1.f,1.f,1.f,1.f,m_texbackground, 0.4f);
	//캐릭터 렌더 
	for (int i = 0; i < MAX_OBJECT_COUNT; i++)
	{
		if (Characters[i] != NULL)
		{
			m_renderer->DrawTexturedRectSeq(Characters[i]->Getx(), Characters[i]->Gety(),
				Characters[i]->Getz(), Characters[i]->Getsize(),
				Characters[i]->Getr(), Characters[i]->Getg(),
				Characters[i]->Getb(), Characters[i]->Geta(),m_texcharacter,(int)character_frame,0,8,1, Characters[i]->level);
		}
	}

	//빌딩 렌더 
	for (int i = 0; i < MAX_OBJECT_COUNT; i++) {
		if (Building[i] != NULL) {
			if (Building[i]->team == 1) {
				m_renderer->DrawTexturedRect(Building[i]->Getx(), Building[i]->Gety(), Building[i]->Getz(), Building[i]->Getsize(),
					Building[i]->Getr(), Building[i]->Getg(), Building[i]->Getb(), Building[i]->Geta(), m_texbuilding1,Building[i]->level);
			}
			else if (Building[i]->team == 2) {
				m_renderer->DrawTexturedRect(Building[i]->Getx(), Building[i]->Gety(), Building[i]->Getz(), Building[i]->Getsize(),
					Building[i]->Getr(), Building[i]->Getg(), Building[i]->Getb(), Building[i]->Geta(), m_texbuilding2, Building[i]->level);
			}
		}
	}

	//총알 렌더 
	for (int i = 0; i < MAX_OBJECT_COUNT; i++) {
		if (Bullets[i] != NULL) {
			m_renderer->DrawParticle(Bullets[i]->Getx(), Bullets[i]->Gety(), Bullets[i]->Getz(),
				Bullets[i]->Getsize(), Bullets[i]->Getr(), Bullets[i]->Getg(), Bullets[i]->Getb(), Bullets[i]->Geta(), 1, 0, m_texparticle, time);
		}
	}
	//Arrow 렌더 
	for (int i = 0; i < MAX_OBJECT_COUNT; i++) {
		if (Arrows[i] != NULL) {
			m_renderer->DrawSolidRect(Arrows[i]->Getx(), Arrows[i]->Gety(), Arrows[i]->Getz(),
				Arrows[i]->Getsize(), Arrows[i]->Getr(), Arrows[i]->Getg(), Arrows[i]->Getb(), Arrows[i]->Geta(), Arrows[i]->level);
		}
	}
	//Building Gauge 렌더
	for (int i = 0; i < 6; i++) {
		if (Building[i] != NULL) {
			if (Building[i]->team == 1)
				m_renderer->DrawSolidRectGauge(Building[i]->Getx(), Building[i]->Gety() + 60.f, 0.f, Building[i]->Getsize(), Building[i]->Getsize() / 10.f, 1.0f, 0.f, 0.f, 1.f, Building[i]->Life / 500.f, 0.1f);
			else if(Building[i]->team == 2)
				m_renderer->DrawSolidRectGauge(Building[i]->Getx(), Building[i]->Gety() + 60.f, 0.f, Building[i]->Getsize(), Building[i]->Getsize() / 10.f, 0.0f, 0.f, 1.f, 1.f, Building[i]->Life / 500.f, 0.1f);
		}
	}
	//Character Gauge 렌더 
	for (int i = 0; i < characternum; i++) {
		if (Characters[i] != NULL) {
			if (Characters[i]->team == 1)
				m_renderer->DrawSolidRectGauge(Characters[i]->Getx(), Characters[i]->Gety() + 25.f, Characters[i]->Getz(),Characters[i]->Getsize(), Characters[i]->Getsize() / 10.f, 1.0f, 0.f, 0.f, 1.f, Characters[i]->Life / 100.f, 0.1f);
			else if (Characters[i]->team == 2)
				m_renderer->DrawSolidRectGauge(Characters[i]->Getx(), Characters[i]->Gety() + 25.f, Characters[i]->Getz(),Characters[i]->Getsize(), Characters[i]->Getsize() / 10.f, 0.0f, 0.f, 1.f, 1.f, Characters[i]->Life / 100.f, 0.1f);
		}
	}

	//Text 렌더 
	m_renderer->DrawTextW(-250.f, 380.f, GLUT_BITMAP_TIMES_ROMAN_24, 1.f, 1.f, 0.f, "Dohyun");
}

//충돌테스트 함수 
void ScenceMgr::CollisionTest()
{	
	//빌딩과 캐릭터의 충돌	
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < characternum; j++) {
			if (Building[i] != NULL && Characters[j] != NULL) {
				if (CollisionCheck(Building[i], Characters[j]) && Building[i]->team != Characters[j]->team) {
					Building[i]->Life -= Characters[j]->Life;
					Characters[j]->Life = 0;
					cout << "빌딩" << i + 1 << "의 life=" << Building[i]->Life << endl;
				}
			}

		}
	}

	//Building과 bullet의 충돌 체크 
	for (int i = 0; i < 6; i++) {
		if (Building[i] != NULL) {
			for (int j = 0; j < bulletnum; j++) {
				if (Bullets[j] != NULL) {
					if ( CollisionCheck(Building[i],Bullets[j]) && (Building[i]->team != Bullets[j]->team)) {
						Building[i]->Life -= Bullets[j]->Life;
						Bullets[j]->Life = 0;
						cout << "빌딩" << i+1 << "의 life=" << Building[i]->Life << endl;
					}
				}
			}
		}
	}
	//총알과 캐릭터의 충돌 
	for (int i = 0; i < characternum; i++)
	{
		if (Characters[i] != NULL) {
			for (int j = 0; j < bulletnum; j++)
			{				
				if (Bullets[j] != NULL) {
					if (CollisionCheck(Characters[i], Bullets[j]) && Characters[i]->team != Bullets[j]->team) {
						Characters[i]->Life -= Bullets[j]->Life;
						Bullets[j]->Life = 0;
					}
				}
			}	
		}
	 }
	//character와 arrow의 충돌 
	for (int i = 0; i < characternum; i++)
	{
		if (Characters[i] != NULL) {
			for (int j = 0; j < arrownum; j++)
			{
				if (Arrows[j] != NULL) {
					if (CollisionCheck(Characters[i],Arrows[j]) && Characters[i]->team != Arrows[j]->team)
					{
						Characters[i]->Life -= Arrows[j]->Life;
						Arrows[j]->Life = 0;
					}
				
				}
			}
		}
	}
	
	//Building과 arrow의 충돌 
	for (int i = 0; i < 6; i++)
	{
		if (Building[i] != NULL) {
			for (int j = 0; j < arrownum; j++) {
				if (Arrows[j] != NULL) {
					if (CollisionCheck(Building[i], Arrows[j]) && Building[i]->team != Arrows[j]->team) {
						Building[i]->Life -= Arrows[j]->Life;
						Arrows[j]->Life = 0;
						cout << "빌딩" << i + 1 << "의 life=" << Building[i]->Life << endl;
					}
				}
			}
		}
	}
	
}

//충돌체크 함수 
bool ScenceMgr::CollisionCheck(Object *a, Object *b) {
	float left_a = a->Getx() - (a->Getsize() / 2.f);
	float right_a = a->Getx() + (a->Getsize() / 2.f);
	float top_a = a->Gety() + (a->Getsize() / 2.f);
	float bottom_a = a->Gety() - (a->Getsize() / 2.f);
	float left_b = b->Getx() - (b->Getsize() / 2.f);
	float right_b = b->Getx() + (b->Getsize() / 2.f);
	float top_b = b->Gety() + (b->Getsize() / 2.f);
	float bottom_b = b->Gety() - (b->Getsize() / 2.f);

	if (left_a > right_b) return false;
	if (right_a < left_b) return false;
	if (top_a < bottom_b) return false; 
	if (bottom_a > top_b) return false;

	return true;
}