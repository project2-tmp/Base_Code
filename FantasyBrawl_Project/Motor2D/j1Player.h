#ifndef __j1Player_H__
#define __j1Player_H__

#include "p2Point.h"
#include "j1Entity.h"
#include "j1ParticleSystem.h"
#include "j1Timer.h"
#include <list>

struct SDL_Texture;
struct Collider;
enum class PLAYER;
enum class fade_step;
enum class FADE_TEX;

#define JOYSTICK_DEAD_ZONE 8000

enum class CHARACTER
{
	WENDOLIN,
	MELIADOUL,
	SIMON,
	TRAKT,
	NONE
};

struct Playerdata {

	Animation idleRight;
	Animation idleLeft;
	Animation idleUp;
	Animation idleUpright;
	Animation idleUpleft;
	Animation idleDown;
	Animation idleDownright;
	Animation idleDownleft;

	Animation moveRight;
	Animation moveLeft;
	Animation moveUp;
	Animation moveUpright;
	Animation moveUpleft;
	Animation moveDown;
	Animation moveDownright;
	Animation moveDownleft;

	Animation attackRight;
	Animation attackLeft;
	Animation attackUp;
	Animation attackUpright;
	Animation attackUpleft;
	Animation attackDown;
	Animation attackDownright;
	Animation attackDownleft;

	std::string folder;
	std::string Texture;
	SDL_Texture* tex = nullptr;

	Particle basic_attack;
	uint basic_fx = 0;
	uint super_fx = 0;

	SDL_Rect playerrect = { 0,0,0,0 };
};

enum class MOVEMENT
{
	RIGHTWARDS,
	LEFTWARDS,
	UPWARDS,
	DOWNWARDS,

	STATIC
};

enum class AXISDIRECTION
{
	AXIS_X,
	AXIS_Y
};

enum class PSTATE
{
	MOVING,
	ATTACKING,
	IDLE,
	NONE
};

enum class RANK
{
	LOSER,
	CONTENDER
};

class j1Player :public j1Entity
{
public:

	j1Player(entity_info entityinfo, Playerdata * player_info);
	~j1Player();

	bool Start();
	bool Update(float dt);
	bool PostUpdate(float dt);
	bool CleanUp();

	// Called each loop iteration
	void FixedUpdate(float dt);
	// Called each logic iteration
	void LogicUpdate(float dt);

	// --- Entity Movement ---
	void MoveX(float dt);
	void MoveY(float dt);
	void HandleInput();

	// --- Entity Animations ---
	void HandleAnimations();
	void GetAttackAnimation();
	void GetMovementAnimation();
	void GetIdleAnimation();
	bool InRange(float axisX, float axisY, float range_start, float range_end);

	// --- Save & Load ---
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	// --- Character && Player ---
	void AssignCharacter();
	const fPoint GetNearestPlayerDirection();
	void ComputeDistance2players(fPoint pos = {-1,-1});
	bool AreOtherPlayersDead();

	// --- Visuals ---
	void BlitArrows();

	// --- Collisions Handling ---

	void OnCollision(Collider* c1, Collider* c2);

	void CheckCollision();

	void Right_Collision(Collider* entitycollider, const Collider* to_check);

	void Left_Collision(Collider* entitycollider, const Collider* to_check);

	void Up_Collision(Collider* entitycollider, const Collider* to_check);

	void Down_Collision(Collider* entitycollider, const Collider* to_check);

	void CheckParticleCollision(Collider * hitbox, const Collider* to_check);


	// --- Entity Attacks ---
	void HandleAttacks(float dt);
	void HandleShield();
	void HandleSuperAttacks();
	void HandleSpecialAttacks();
	void BlitSuperAimPaths(float dt);
	void BlitSPAimPaths(float dt);

	// --- Character Specific Super Attacks ---
	void Launch1stSuper();
	void Launch2ndSuper();
	void Launch3rdSuper();
	void Launch4thSuper();

	// --- Character Specific Special Abilities ---
	void Launch1stSP();
	void Launch2ndSP();
	void Launch3rdSP();
	void Launch4thSP();

public:

	// --- Basic Stuff ---
	PLAYER ID;
	Playerdata playerinfo;
	PSTATE PlayerState;
	CHARACTER character = CHARACTER::NONE;
	Animation Pdeath_anim;

	// --- MOVEMENT VARIABLES ---
	fPoint Future_position = { 0,0 };
	MOVEMENT EntityMovement;
	AXISDIRECTION direction;

	// --- Gamepad Input ---
	float multipliermin = 0.1f;
	bool RJinverted = false;

	// Left Joystick LJ 
	float LJAxisx_value = 0.0f;
	float LJAxisy_value = 0.0f;
	float LJdirection_x = 0.0f;
	float LJdirection_y = 0.0f;

	// Right Joystick RJ
	float RJAxisx_value = 0.0f;
	float RJAxisy_value = 0.0f;
	float RJdirection_x = 0.0f;
	float RJdirection_y = 0.0f;
	
	bool PlayerPrintOnTop = false;

	// --- Collisions ---
	SDL_Rect Intersection = { 0,0,0,0 };

	// --- Timers ---
	j1Timer superTimer;
	j1Timer shieldTimer;
	j1Timer shieldDuration;
	j1Timer basicTimer;
	j1Timer attackanimTimer;
	uint SuperCooldown = 6;

	bool shieldON = false;
	bool superON = false;
	bool specialON = false;
	bool shield_available = false;
	bool super_available = false;
	bool launched_super = false;

	// --- Score ---
	uint kills = 0;
	RANK P_rank = RANK::CONTENDER;

	// --- Auto aim ---
	float Aim_Radius = 300.0f;
	Animation Target_anim;
	bool auto_aimON = false;
	fPoint targetP_pos = { 0.0f,0.0f };

	// --- Particles ---
	Particle * last_particle = nullptr;
	fPoint static_pos = { 0.0f,0.0f };
	fPoint static_posend = { 0.0f,0.0f };

	// --- Distances to players ---
	float absoluteDistanceP1 = 0.0f;
	float absoluteDistanceP2 = 0.0f;
	float absoluteDistanceP3 = 0.0f;
	float absoluteDistanceP4 = 0.0f;

	fPoint directionP1 = { 0.0f,0.0f };
	fPoint directionP2 = { 0.0f,0.0f };
	fPoint directionP3 = { 0.0f,0.0f };
	fPoint directionP4 = { 0.0f,0.0f };

	// --- IDCircle ---
	Animation * CurrentIDCircleAnimation = nullptr;

	// --- Shield ---
	Animation * CurrentShieldAnimation = nullptr;
	Animation shieldAnim;
	Animation shieldendAnim;

	// --- Character Specific ---
	bool teleported = false;
	j1Timer RJinversion;
	j1Timer Traktpulsation;
	bool ghost = false;
	j1Timer ghostTimer;
	uint alpha = 255;
	Particle parryP;
	Particle inkshot;
	std::list<Particle*> MeliadoulAXES;
	float TraktSPradius = 0.0f;
	float traktSPAngle = 0.0f;
	Animation WendolinsmokeANIM;
	Animation simonteleport_anim;

	// --- Fade ---
	fade_step current_step;
	Uint32 start_time = 0;
	Uint32 total_time = 0;
	SDL_Color colA;
	float alphaA = 0;

	fade_step current_stepD;
	Uint32 start_timeD = 0;
	Uint32 total_timeD = 0;
	SDL_Color colB;
	float alphaB = 0;

	fade_step current_stepHP;
	Uint32 start_timeHP = 0;
	Uint32 total_timeHP = 0;
	SDL_Color colC;
	float alphaC = 0;

	bool damage_received = false;
	j1Player* last_hitP = nullptr;

	//Killcounter
	int killcounter = 0;
	bool startdisplaying = false;
	bool Has_to_blit_kills = false;
	j1Timer timerkill;
	SDL_Texture* killstexture = nullptr;
};

#endif // __j1Player_H__
