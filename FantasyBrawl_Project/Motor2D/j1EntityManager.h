// ----------------------------------------------------
// Controls all logic entities (enemies/player/etc.) --
// ----------------------------------------------------

#ifndef __J1ENTITYMANAGER_H__
#define __J1ENTITYMANAGER_H__

#include "j1Module.h"
#include "j1Entity.h"
#include "j1Player.h"
#include <list>
#include <string>


class j1Entity;

struct AnimationRanges
{
	float AnimationRangeRight_start = 0.0f;
	float AnimationRangeRight_end = 0.0f;
	float AnimationRangeLeft_start = 0.0f;
	float AnimationRangeLeft_end = 0.0f;
	float AnimationRangeLeft_start2 = 0.0f;
	float AnimationRangeLeft_end2 = 0.0f;

	float AnimationRangeUp_start = 0.0f;
	float AnimationRangeUp_end = 0.0f;
	float AnimationRangeUpright_start = 0.0f;
	float AnimationRangeUpright_end = 0.0f;
	float AnimationRangeUpleft_start = 0.0f;
	float AnimationRangeUpleft_end = 0.0f;

	float AnimationRangeDown_start = 0.0f;
	float AnimationRangeDown_end = 0.0f;
	float AnimationRangeDownright_start = 0.0f;
	float AnimationRangeDownright_end = 0.0f;
	float AnimationRangeDownleft_start = 0.0f;
	float AnimationRangeDownleft_end = 0.0f;
};

class j1EntityManager : public j1Module
{
public:

	j1EntityManager();

	virtual ~j1EntityManager();

	// --- Called before render is available ---
	bool Awake(pugi::xml_node&);

	// --- Called before the first frame ---
	bool Start();

	// --- Called each loop iteration ---
	bool PreUpdate();
	bool Update(float dt);
	void UpdateEntity(float dt);
	bool PostUpdate(float dt);

	// --- Called before quitting ---
	bool CleanUp();

	// --- Entities management ---
	j1Entity * const CreateEntity(entity_type entitytype, entity_info entityinfo, Playerdata * player_info);
	void OnCollision(Collider* c1, Collider* c2);
	Animation* LoadAnimation(const char* animationPath, const char* animationName);

	// --- Save & Load ---
	bool Load(pugi::xml_node&);

	bool Save(pugi::xml_node&) const;

public:

	std::list<j1Entity*>	entities;
	float				update_ms_cycle = 0;
	uint playerid = 0;
	// --- Player ---
	Playerdata Wendolin;
	Playerdata Simon;
	Playerdata Trakt;
	Playerdata Meliadoul;
	AnimationRanges animranges;

	// --- Shield ---
	SDL_Texture* shield_texture = nullptr;
	std::string shield_texturepath;
	Animation shield_anim;
	Animation shieldEnd_anim;

	// --- Aim paths ---
	SDL_Texture * aimpath = nullptr;
	SDL_Texture * WendolinSuper_aimpath = nullptr;
	SDL_Texture * SimonSuper_aimpath = nullptr;
	SDL_Texture * TraktSuper_aimpath = nullptr;
	SDL_Texture * MeliadoulSuper_aimpath = nullptr;
	SDL_Texture * target_tex = nullptr;

	Animation targetanim;

	// --- Particles ---
	SDL_Texture * Dagger_texture = nullptr;
	SDL_Texture * budu_texture = nullptr;
	SDL_Texture * axe_texture = nullptr;
	SDL_Texture * inkball_texture = nullptr;
	SDL_Texture* particle_hittex = nullptr;

	SDL_Texture * inkshot_texture = nullptr;
	SDL_Texture * inksplash_texture = nullptr;
	SDL_Texture * trakttentacles_texture = nullptr;
	SDL_Texture * fallenaxe_texture = nullptr;
	SDL_Texture * wendolin_ultismoke = nullptr;
	SDL_Texture * parry_texture = nullptr;
	SDL_Texture* wendolin_superdaggertex = nullptr;
	SDL_Texture* simonteleport_tex = nullptr;

	SDL_Rect parrytex_rect;
	SDL_Rect inkshot_rect;
	SDL_Rect inksplash_rect;
	SDL_Rect trakttentacles_rect;

	Animation Wendolinsmokeanim;
	Animation particle_hitanim;
	Animation simonteleport_anim;

	// --- ID Circle animations ---
	std::string circle_texturepath;
	SDL_Texture* circlesprites = nullptr;
	Animation IDCircle_red;
	Animation IDCirclesuper_red;
	Animation IDCircleshield_red;
	Animation IDCircleboth_red;

	Animation IDCircle_green;
	Animation IDCirclesuper_green;
	Animation IDCircleshield_green;
	Animation IDCircleboth_green;

	Animation IDCircle_blue;
	Animation IDCirclesuper_blue;
	Animation IDCircleshield_blue;
	Animation IDCircleboth_blue;

	Animation IDCircle_yellow;
	Animation IDCirclesuper_yellow;
	Animation IDCircleshield_yellow;
	Animation IDCircleboth_yellow;

	// --- Arrows ---
	SDL_Texture * arrows_tex = nullptr;
	SDL_Rect red_arrow = { 0,0,11,16 };
	SDL_Rect green_arrow = { 11,0,11,16 };
	SDL_Rect yellow_arrow = { 23,0,11,16 };
	SDL_Rect blue_arrow = { 35,0,11,16 };

	// --- Death ---
	SDL_Texture * death_tex = nullptr;
	Animation death_anim;
	SDL_Texture * deathbubbles_tex = nullptr;

	// --- Player Key values ---
	uint SuperCooldown = 0;
	uint ShieldCooldown = 0;

	uint WendolinSP_NumDaggers = 0;
	float WendolinSP_DaggerAngle = 0.0f;
	float Wendolin_ghostTime = 0.0f;
	uint Wendolin_ghostalpha = 0.0f;
	float WendolinSP_speedmultiplier = 0.0f;

	uint parryPlife = 0;
};

#endif // __J1ENTITYMANAGER_H__
