#include <iostream> 
#include <sstream> 

#include "p2Defs.h"
#include "p2Log.h"

#include "j1Window.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Viewport.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Scene.h"
#include "j1Map.h"
#include "j1Collision.h"
#include "j1EntityManager.h"
#include "j1ArenaInteractions.h"
#include "j1ParticleSystem.h"
#include "j1Fonts.h"
#include "j1Gui.h"
#include "j1Transition.h"
#include "j1UIScene.h"
#include "j1BuffManager.h"
#include "j1App.h"
#include "j1FadeToBlack.h"
#include "j1ItemManager.h"

#include "Brofiler/Brofiler.h"


// Constructor
j1App::j1App(int argc, char* args[]) : argc(argc), args(args)
{
	PERF_START(ptimer);

	input = new j1Input();
	win = new j1Window();
	render = new j1Render();
	tex = new j1Textures();
	audio = new j1Audio();
	scene = new j1Scene();
	map = new j1Map();
	coll = new j1Collision();
	entities = new j1EntityManager();
	arena_interactions = new j1ArenaInteractions();
	buff = new j1BuffManager();
	particlesys = new j1ParticleSystem();
	view = new j1Viewport();
	fonts = new j1Fonts();
	gui = new j1Gui();
	transition = new j1Transition();
	ui_scene = new j1UIScene();
	fade = new j1FadeToBlack();
	item_manager = new j1ItemManager();

	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(input);
	AddModule(win);
	AddModule(tex);
	AddModule(audio);
	AddModule(entities);
	AddModule(buff);
	AddModule(coll);
	AddModule(map);
	AddModule(scene);
	AddModule(arena_interactions);
	AddModule(item_manager);

	AddModule(particlesys);
	

	AddModule(fonts);
	AddModule(gui);
	AddModule(transition);
	AddModule(view);
	AddModule(ui_scene);
	AddModule(fade);


	// render last to swap buffer
	AddModule(render);

	PERF_PEEK(ptimer);
}

// Destructor
j1App::~j1App()
{
	// release modules
	std::list<j1Module*>::reverse_iterator item = modules.rbegin();

	while(item != modules.rend())
	{
		delete *item;
		item++;
	}

	modules.clear();
}

void j1App::AddModule(j1Module* module)
{
	module->Init();
	modules.push_back(module);
}

// Called before render is available
bool j1App::Awake()
{
	PERF_START(ptimer);

	pugi::xml_document	config_file;
	pugi::xml_node		config;
	pugi::xml_node		app_config;

	bool ret = false;
		
	config = LoadConfig(config_file);

	if(config.empty() == false)
	{
		// self-config
		ret = true;
		app_config = config.child("app");
		title.assign(app_config.child("title").child_value());
		organization.assign(app_config.child("organization").child_value());

		framerate_cap = app_config.attribute("framerate_cap").as_uint();

		//Read from config file your framerate cap

		if (framerate_cap > 0)
		{
			capped_ms = 1000 / framerate_cap;
		}
	}

	if(ret == true)
	{
		std::list<j1Module*>::const_iterator item = modules.begin();

		while (item != modules.end() && ret)
		{
			ret = (*item)->Awake(config.child((*item)->name.data()));
			item++;
		}
	}

	PERF_PEEK(ptimer);

	return ret;
}

// Called before the first frame
bool j1App::Start()
{
	PERF_START(ptimer);
	bool ret = true;

	std::list<j1Module*>::const_iterator item = modules.begin();

	while (item != modules.end() && ret)
	{
		if((*item)->active)
		ret = (*item)->Start();

		item++;
	}

	startup_time.Start();

	PERF_PEEK(ptimer);

	return ret;
}

// Called each loop iteration
bool j1App::Update()
{

	BROFILER_CATEGORY("App_Update", Profiler::Color::AntiqueWhite);

	bool ret = true;
	PrepareUpdate();

	if(input->GetWindowEvent(WE_QUIT) == true)
		ret = false;

	if(ret == true)
		ret = PreUpdate();

	if(ret == true)
		ret = DoUpdate();

	if(ret == true)
		ret = PostUpdate();

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug_mode = !debug_mode;

	FinishUpdate();
	return ret;
}

// ---------------------------------------------
pugi::xml_node j1App::LoadConfig(pugi::xml_document& config_file) const
{
	pugi::xml_node ret;

	pugi::xml_parse_result result = config_file.load_file("config.xml");

	if(result == NULL)
		LOG("Could not load map xml file config.xml. pugi error: %s", result.description());
	else
		ret = config_file.child("config");

	return ret;
}

// ---------------------------------------------
void j1App::PrepareUpdate()
{
	frame_count++;
	last_sec_frame_count++;

	dt = frame_time.ReadSec();

	if (on_GamePause == true && App->ui_scene->actual_menu != SELECTION_MENU && App->ui_scene->actual_menu != START_MENU)
	{
		if(!App->transition->doingMenuTransition)
		dt = 0.0f;
	}
	
	frame_time.Start();
}

// ---------------------------------------------
void j1App::FinishUpdate()
{
	if(want_to_save == true)
		SavegameNow();

	if(want_to_load == true)
		LoadGameNow();

	// Framerate calculations --

	// Framerate calculations --

	if (last_sec_frame_time.Read() > 1000)
	{
		last_sec_frame_time.Start();
		prev_last_sec_frame_count = last_sec_frame_count;
		last_sec_frame_count = 0;
	}

	float avg_fps = float(frame_count) / startup_time.ReadSec();

	float seconds_since_startup = startup_time.ReadSec();
	uint32 last_frame_ms = frame_time.Read();
	uint32 frames_on_last_update = prev_last_sec_frame_count;

	static char title[256];
	sprintf_s(title, 256, "Av.FPS: %.2f Last Frame Ms: %u Last sec frames: %i Last dt: %.3f Time since startup: %.3f Frame Count: %lu",
		avg_fps, last_frame_ms, frames_on_last_update, dt, seconds_since_startup, frame_count);
	App->win->SetTitle(title);

	if (capped_ms > 0 && last_frame_ms < capped_ms)
	{
		j1PerfTimer t;
		SDL_Delay(capped_ms - last_frame_ms);
		//LOG("We waited for %d milliseconds and got back in %f", capped_ms - last_frame_ms, t.ReadMs());
	}
}

// Call modules before each loop iteration
bool j1App::PreUpdate()
{
	bool ret = true;

	std::list<j1Module*>::const_iterator item = modules.begin();
	j1Module* pModule = NULL;

	for (;item != modules.end() && ret; ++item)
	{
		pModule = *item;

		if (!pModule->active)
		{
			continue;
		}

		ret = (*item)->PreUpdate();
	}

	return ret;
}

// Call modules on each loop iteration
bool j1App::DoUpdate()
{
	bool ret = true;

	std::list<j1Module*>::const_iterator item = modules.begin();
	j1Module* pModule = NULL;

	for (; item != modules.end() && ret; ++item)
	{
		pModule = *item;

		if (!pModule->active)
		{
			continue;
		}

		ret = (*item)->Update(dt);
	}

	return ret;
}

// Call modules after each loop iteration
bool j1App::PostUpdate()
{
	bool ret = true;
	std::list<j1Module*>::const_iterator item = modules.begin();
	j1Module* pModule = NULL;

	for (; item != modules.end() && ret; ++item)
	{
		pModule = *item;

		if (!pModule->active)
		{
			continue;
		}

		ret = (*item)->PostUpdate(dt);
	}

	return ret;
}

// Called before quitting
bool j1App::CleanUp()
{
	PERF_START(ptimer);
	bool ret = true;
	std::list<j1Module*>::reverse_iterator item = modules.rbegin();

	while(item != modules.rend() && ret == true)
	{
		ret = (*item)->CleanUp();
		item++;
	}

	PERF_PEEK(ptimer);
	return ret;
}

// ---------------------------------------
int j1App::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* j1App::GetArgv(int index) const
{
	if(index < argc)
		return args[index];
	else
		return NULL;
}

// ---------------------------------------
const char* j1App::GetTitle() const
{
	return title.data();
}

// ---------------------------------------
const char* j1App::GetOrganization() const
{
	return organization.data();
}

// Load / Save
void j1App::LoadGame(const char* file)
{
	// we should be checking if that file actually exist
	// from the "GetSaveGames" list
	want_to_load = true;
	//load_game.create("%s%s", fs->GetSaveDirectory(), file);
}

// ---------------------------------------
void j1App::SaveGame(const char* file) const
{
	// we should be checking if that file actually exist
	// from the "GetSaveGames" list ... should we overwrite ?

	want_to_save = true;
	save_game.assign(file);
}

// ---------------------------------------
void j1App::GetSaveGames(std::list<std::string>& list_to_fill) const
{
	// need to add functionality to file_system module for this to work
}

bool j1App::LoadGameNow()
{
	bool ret = false;

	load_game.assign("save_game.xml");

	pugi::xml_document data;
	pugi::xml_node root;

	pugi::xml_parse_result result = data.load_file(load_game.data());

	if(result != NULL)
	{
		LOG("Loading new Game State from %s...", load_game.data());

		root = data.child("game_state");

		std::list<j1Module*>::const_iterator item = modules.begin();
		ret = true;

		while (item != modules.end() && ret)
		{
			ret = (*item)->Load(root.child((*item)->name.data()));
			item++;
		} 

		data.reset();
		if(ret == true)
			LOG("...finished loading");
		else
			LOG("...loading process interrupted with error on module %s", ((*item) != NULL) ? (*item)->name.data() : "unknown");
	}
	else
		LOG("Could not parse game state xml file %s. pugi error: %s", load_game.data(), result.description());

	want_to_load = false;
	return ret;
}

bool j1App::SavegameNow() const
{
	bool ret = true;

	save_game.assign("save_game.xml");

	LOG("Saving Game State to %s...", save_game.data());

	// xml object were we will store all data
	pugi::xml_document data;
	pugi::xml_node root;
	
	root = data.append_child("game_state");

	std::list<j1Module*>::const_iterator item = modules.begin();

	while(item != modules.end() && ret == true)
	{
		ret = (*item)->Save(root.append_child((*item)->name.data()));
		item++;
	}

	if (ret == true)
	{
		data.save_file(save_game.data());
		LOG("... finished saving", save_game.data());
	}
	else
		LOG("Save process halted from an error in module %s", ((*item) != NULL) ? (*item)->name.data() : "unknown");

	data.reset();
	want_to_save = false;
	return ret;
}

void j1App::RequestBrowser(const char * url) const
{
	ShellExecuteA(NULL, "open", url, NULL, NULL, SW_SHOWNORMAL);
}