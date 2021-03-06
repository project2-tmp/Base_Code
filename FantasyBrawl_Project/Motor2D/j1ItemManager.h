#ifndef __J1ITEMMANAGER_H__
#define __J1ITEMMANAGER_H__

#include "j1Module.h"
#include "Animation.h"
#include "j1Timer.h"
#include "p2Point.h"
#include <list>

struct SDL_Texture;
struct Collider;

enum class ItemType
{
	NONE = -1,
	LIFE = 0,
	SUPER_CD,
	SPEED,
	ITEM_MAX
};

struct Item
{
	bool spawned = false;
	iPoint Pos = { 0,0 };
	ItemType type = ItemType::NONE;
	Collider* col = nullptr;

	j1Timer time_inactive;
	Animation* animation[3] = { nullptr };

	Item(ItemType type, iPoint Pos) : type(type), Pos(Pos)
	{}
};

class j1ItemManager : public j1Module
{
public:
	j1ItemManager();
	~j1ItemManager();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	//Start the logic of the item manager
	bool StartItemManager();

	//To be called each time we leave the game either to go to map Preview or back to main menu
	bool CloseItemManager();

	// Called each loop iteration
	bool Update(float dt);

	// Called each loop iteration
	bool PostUpdate(float dt); 

	// Called before quitting
	bool CleanUp();

	//Create & initialize an Item and add it to the items list 
	Item* CreateItem(ItemType type, iPoint position);

	void DeSpawnItem(Item* item);

	void ReSpawnItem(Item* item);

	ItemType RandomItemType();

	//Get the pointer to the item that has this collider in it
	Item* GetItemWithCollider(const Collider* c ) const;

	// Pause & Continue The items timers
	void PauseItemManager();
	void ContinueItemManager();


	//Receive a list with the positions in Map Coordinates of the spawners
	void ReceiveSpawnersPositions( const std::list<iPoint> to_copy);
	
private:
	Animation* LoadAnimation(const char* animationPath, const char* animationName);
	void GetSpawnersFromMap();

private:
	std::list<Item*> items;
	std::list<iPoint> spawners_pos;

	float respawn_time = 0.0f;

	SDL_Texture* items_tex = nullptr;
};
#endif
