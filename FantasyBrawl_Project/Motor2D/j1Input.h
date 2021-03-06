#ifndef __j1INPUT_H__
#define __j1INPUT_H__

#include "j1Module.h"
#include "p2Defs.h"

//#define NUM_KEYS 352
#define NUM_MOUSE_BUTTONS 5
//#define LAST_KEYS_PRESSED_BUFFER 50
#define NUM_GAMEPAD_BUTTONS 15
// This macro controls how many gamepads at max will be processed 
#define MAX_GAMEPADS 4
#define AXISMAX 32767

struct _SDL_Rect;
struct _SDL_GameController;
struct _SDL_Haptic;
struct SDL_GameControllerButtonBind;

enum j1EventWindow
{
	WE_QUIT = 0,
	WE_HIDE = 1,
	WE_SHOW = 2,
	WE_COUNT
};

// Keyboard input states 
enum j1KeyState
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

// Gamepad input state (just for better legibility)
enum GP_BUTTON_STATE
{
	BUTTON_IDLE = 0,
	BUTTON_DOWN,
	BUTTON_REPEAT,
	BUTTON_UP
};


enum class BUTTON_BIND
{
	BASIC_ATTACK = 0,
	SPECIAL_ATTACK,
	SUPER_ATTACK,
	SHIELD,
	MAX_BUTTON_BIND
};
enum class GP_AXIS_STATE
{
	AXIS_IDLE = 0,
	AXIS_POSITIVE_DOWN,
	AXIS_POSITIVE_REPEAT,
	AXIS_POSITIVE_RELEASE,
	AXIS_NEGATIVE_DOWN,
	AXIS_NEGATIVE_REPEAT,
	AXIS_NEGATIVE_RELEASE
};

enum class PLAYER
{
	P1 = 0,
	P2,
	P3,
	P4
};

// Gamepad custom struct 
struct Gamepad
{
	// Id's
	_SDL_GameController* id_ptr = nullptr; //SDL_GameController of the Gamepad
	_SDL_Haptic* haptic_ptr = nullptr;

	// Data
	GP_BUTTON_STATE* buttons = nullptr;
	int* axis = nullptr;
	GP_BUTTON_STATE* triggers_state = nullptr;
	GP_AXIS_STATE*   multidirection_axis_state = nullptr;
	int index = -1;

	SDL_GameControllerButtonBind* binded_buttons = nullptr;

	bool any_button_down = false;
	bool any_axis_down = false;
	int last_button_pressed = -1;
	int last_axis_pressed = -1;
};

class j1Input : public j1Module
{

public:

	j1Input();

	// Destructor
	virtual ~j1Input();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();

	// Called before quitting
	bool CleanUp();

	// Gather relevant win events
	bool GetWindowEvent(j1EventWindow ev);

	//UI_EVENTS
	bool OnUIEvent(UI_element* element, event_type event_type);
	bool OnUIEvent(UI_element* element, event_type event_type, int p);

	// Check key states (includes mouse and joy buttons)
	j1KeyState GetKey(int id) const
	{
		return keyboard[id];
	}

	void ForceKeyboardKeyState(int id, j1KeyState state);

	j1KeyState GetMouseButtonDown(int id) const
	{
		return mouse_buttons[id - 1];
	}

	// Check gamepad button states. Works jut like GetKey
	GP_BUTTON_STATE GetButton(PLAYER p,int id) const
	{
		if (controllers[(int)p].id_ptr != nullptr)
			return controllers[(int)p].buttons[id];
		else
			return BUTTON_IDLE;
	}

	// Check gamepad axis & triggers. returns the integer value of the trigger or axis
	int GetAxis(PLAYER p, int id) const
	{
		if (controllers[(int)p].id_ptr != nullptr)
			return controllers[(int)p].axis[id];
		else
			return 0;
	}


	// Check gamepad button bind. 
	GP_BUTTON_STATE GetButton(PLAYER p, BUTTON_BIND id) const;

	//Bind a button to an action
	void BindButton(PLAYER p, BUTTON_BIND bind, int button_to_bind, int bind_type);

	// PASS A TRIGGER FROM THE ENUM!
	GP_BUTTON_STATE GetTriggerState(PLAYER p, int id) const;

	GP_AXIS_STATE GetLRAxisState(PLAYER p, int id) const;

	//Introduce the controller (linked to the player), the intensity from 0.0 to 1.0 and the duration in miliseconds
	void ShakeController(PLAYER p, float intensity, uint32 length);

	//This function allows you to force a state into a button from a controller, use carefully and only under controlled situations
	void ForceButtonState(PLAYER p, int id, GP_BUTTON_STATE state);

	//Stop the vibration of a controller
	void StopControllerShake(PLAYER p);

	//Button pressed down
	bool AnyButtonDown(PLAYER p) { return controllers[(int)p].any_button_down; }

	//Trigger pressed down
	bool AnyTriggerDown(PLAYER p) { return controllers[(int)p].any_axis_down; }

	// Check if a certain window event happened
	bool GetWindowEvent(int code);

	// Get mouse / axis position
	void GetMousePosition(int &x, int &y);
	void GetMouseMotion(int& x, int& y);

	// Get the actual binding of a BUTTON_BIND
	int GetBindRealButton(PLAYER p, BUTTON_BIND bind) const;

	//SDL_Rect GetButtonRect(SDL_GameControllerButton button) { return { 0,(int)button * 72, 72,72 }; }

	bool ControllerIsConnected(PLAYER p);


private: // Functions
	void LoadConfigBinding(PLAYER p);
	void AddBindingToConfig(PLAYER p);

private: // Variables
	bool		windowEvents[WE_COUNT];
	j1KeyState*	keyboard;
	j1KeyState	mouse_buttons[NUM_MOUSE_BUTTONS];
	int			mouse_motion_x;
	int			mouse_motion_y;
	int			mouse_x;
	int			mouse_y;

	Gamepad controllers[MAX_GAMEPADS] = {nullptr};
	uint index_addition_controllers = 0;
};

#endif // __j1INPUT_H__