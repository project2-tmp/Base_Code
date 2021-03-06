#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "j1Window.h"
#include "j1Audio.h"
#include "UI_element.h"
#include "UI_Image.h"
#include "UI_Text.h"
#include "UI_Button.h"
#include "UI_Window.h"
#include "j1UIScene.h"
#include "UI_Slider.h"
#include "UI_Clock.h"
#include "UI_Marker.h"
#include "j1Entity.h"
#include "j1EntityManager.h"
#include "j1Transition.h"
#include "j1FadeToBlack.h"
#include "j1ArenaInteractions.h"




j1Gui::j1Gui() : j1Module()
{
	name.assign("gui");
	
}

// Destructor
j1Gui::~j1Gui()
{}

// Called before render is available
bool j1Gui::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	atlas_file_name = conf.child("atlas").attribute("file").as_string("");

	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	atlas = App->tex->Load(atlas_file_name.data());
	UI_scale = App->win->GetScale();
	UI_scale = 1 / UI_scale;
	button_click_fx = App->audio->LoadFx("audio/fx/button_click.wav");

	current_step = fade_step::none;
	colorT = { 75,0,130,200 };

	return true;
}

// Update all guis
bool j1Gui::PreUpdate()
{
	bool ret = true;

	SDL_SetTextureAlphaMod(atlas, alpha_value);

	int x, y;
	App->input->GetMousePosition(x, y);
	int scale = App->win->GetScale();
	UI_element* element[MAX_GAMEPADS] = { nullptr };

	UI_element* mouse_focus = nullptr;

	for (int i = 0; i < MAX_GAMEPADS; ++i)
	{
		if (App->ui_scene->current_menu->gamepad_tabs[i].empty() == false)
		{
			//Get element to interact with
			if (draggingElement != nullptr)
				element[i] = draggingElement;
			else if (gamepad_last_focus[i] != nullptr && *App->ui_scene->current_menu->gamepads_focus[i] != nullptr && 
				gamepad_last_focus[i] != *App->ui_scene->current_menu->gamepads_focus[i])
			{
				gamepad_last_focus[i]->hovering = false;
				if (gamepad_last_focus[i]->callback != nullptr)
					gamepad_last_focus[i]->callback->OnUIEvent(gamepad_last_focus[i], MOUSE_LEAVE);
				gamepad_last_focus[i] = *App->ui_scene->current_menu->gamepads_focus[i];
			}
			else
			{
				if (App->ui_scene->current_menu != nullptr)
				{
					for (std::list <UI_element*>::reverse_iterator item = App->ui_scene->current_menu->elements.rbegin(); item != App->ui_scene->current_menu->elements.rend(); ++item)
					{

						if (*item == element[0] || *item == element[1] || *item == element[2] || *item == element[3])
							continue;

						iPoint globalPos = (*item)->calculateAbsolutePosition();
						if ((*item)->solid && (App->ui_scene->current_menu->gamepad_tabs[i].empty() == false && (*App->ui_scene->current_menu->gamepads_focus[i]) == (*item)))
						{
							element[i] = *item;
							if (x > globalPos.x && x < globalPos.x + (*item)->section.w / scale && y > globalPos.y && y < globalPos.y + (*item)->section.h / scale && mouse_focus == nullptr && (*item)->solid)
								mouse_focus = *item;
							break;
						}
						else if (x > globalPos.x && x < globalPos.x + (*item)->section.w / scale && y > globalPos.y && y < globalPos.y + (*item)->section.h / scale && mouse_focus == nullptr && (*item)->solid)
						{
							mouse_focus = *item;
						}

					}
				}
			}
		}
	}

	//Get element to interact with
	if (draggingElement != nullptr)
		mouse_focus = draggingElement;
	else
	{
		if (App->ui_scene->current_menu != nullptr)
		{
			for (std::list <UI_element*>::reverse_iterator item = App->ui_scene->current_menu->elements.rbegin(); item != App->ui_scene->current_menu->elements.rend(); ++item)
			{
				iPoint globalPos = (*item)->calculateAbsolutePosition();
				
				if (x > globalPos.x && x < globalPos.x + (*item)->section.w / scale && y > globalPos.y && y < globalPos.y + (*item)->section.h / scale && mouse_focus == nullptr && (*item)->solid)
				{
					mouse_focus = *item;
				}
				else if ((*item)->hovering)
				{
					if (last_mouse_focus != nullptr && *item == last_mouse_focus && last_mouse_focus != mouse_focus)
					{
						(*item)->hovering = false;
						last_mouse_focus = nullptr;
						if ((*item)->callback != nullptr)
							(*item)->callback->OnUIEvent(*item, MOUSE_LEAVE);
					}
				}
			}
		}
	}

	for (int i = 0; i < MAX_GAMEPADS; ++i)
	{
		//Send events related to UI elements
		if (element[i] != nullptr)
		{
			bool is_focused[MAX_GAMEPADS] = { false };

			if (App->ui_scene->current_menu->gamepad_tabs[i].empty() == false)
			{
				if ((*App->ui_scene->current_menu->gamepads_focus[i]) == element[i])
				{
					is_focused[i] = true;

					if ((*App->ui_scene->current_menu->gamepads_focus[i])->is_locked == false && App->input->ControllerIsConnected(PLAYER(i)) == true)
					{
						if (App->input->GetButton((PLAYER)i, SDL_CONTROLLER_BUTTON_DPAD_DOWN) == BUTTON_DOWN
							|| App->input->GetLRAxisState((PLAYER)i, SDL_CONTROLLER_AXIS_LEFTY) == GP_AXIS_STATE::AXIS_POSITIVE_DOWN)
						{
							gamepad_last_focus[i] = *App->ui_scene->current_menu->gamepads_focus[i];
							time_since_press[i].Start();
							automatic_traverse_margin[i].Start();
							if (App->ui_scene->current_menu->gamepads_focus[i] == --App->ui_scene->current_menu->gamepad_tabs[i].end())
								App->ui_scene->current_menu->gamepads_focus[i] = App->ui_scene->current_menu->gamepad_tabs[i].begin();
							else
								App->ui_scene->current_menu->gamepads_focus[i]++;
						}
						else if (App->input->GetButton((PLAYER)i, SDL_CONTROLLER_BUTTON_DPAD_UP) == BUTTON_DOWN
							|| App->input->GetLRAxisState((PLAYER)i, SDL_CONTROLLER_AXIS_LEFTY) == GP_AXIS_STATE::AXIS_NEGATIVE_DOWN)
						{
							gamepad_last_focus[i] = *App->ui_scene->current_menu->gamepads_focus[i];
							time_since_press[i].Start();
							automatic_traverse_margin[i].Start();
							if (App->ui_scene->current_menu->gamepads_focus[i] == App->ui_scene->current_menu->gamepad_tabs[i].begin())
								App->ui_scene->current_menu->gamepads_focus[i] = --App->ui_scene->current_menu->gamepad_tabs[i].end();
							else
								App->ui_scene->current_menu->gamepads_focus[i]--;
						}
						else if (App->input->GetButton((PLAYER)i, SDL_CONTROLLER_BUTTON_DPAD_DOWN) == BUTTON_REPEAT
							|| App->input->GetLRAxisState((PLAYER)i, SDL_CONTROLLER_AXIS_LEFTY) == GP_AXIS_STATE::AXIS_POSITIVE_REPEAT)
						{
							gamepad_last_focus[i] = *App->ui_scene->current_menu->gamepads_focus[i];
							if (ManageAutomaticTraverseTiming(i) == true)
							{
								if (App->ui_scene->current_menu->gamepads_focus[i] == --App->ui_scene->current_menu->gamepad_tabs[i].end())
									App->ui_scene->current_menu->gamepads_focus[i] = App->ui_scene->current_menu->gamepad_tabs[i].begin();
								else
									App->ui_scene->current_menu->gamepads_focus[i]++;
							}
						}
						else if (App->input->GetButton((PLAYER)i, SDL_CONTROLLER_BUTTON_DPAD_UP) == BUTTON_REPEAT
							|| App->input->GetLRAxisState((PLAYER)i, SDL_CONTROLLER_AXIS_LEFTY) == GP_AXIS_STATE::AXIS_NEGATIVE_REPEAT)
						{
							gamepad_last_focus[i] = *App->ui_scene->current_menu->gamepads_focus[i];
							if (ManageAutomaticTraverseTiming(i) == true)
							{
								if (App->ui_scene->current_menu->gamepads_focus[i] == App->ui_scene->current_menu->gamepad_tabs[i].begin())
									App->ui_scene->current_menu->gamepads_focus[i] = --App->ui_scene->current_menu->gamepad_tabs[i].end();
								else
									App->ui_scene->current_menu->gamepads_focus[i]--;
							}
						}
					}
					if ((*App->ui_scene->current_menu->gamepads_focus[i])->function == CREDITS)
					{
						if (App->input->GetButton((PLAYER)i, SDL_CONTROLLER_BUTTON_DPAD_RIGHT) == BUTTON_REPEAT
							|| App->input->GetLRAxisState((PLAYER)i, SDL_CONTROLLER_AXIS_LEFTX) == GP_AXIS_STATE::AXIS_POSITIVE_REPEAT)
						{
							App->ui_scene->current_menu->gamepads_focus[i]--;
						}
					}else if ((*App->ui_scene->current_menu->gamepads_focus[i])->function == EXIT)
					{
							if (App->input->GetButton((PLAYER)i, SDL_CONTROLLER_BUTTON_DPAD_LEFT) == BUTTON_REPEAT
								|| App->input->GetLRAxisState((PLAYER)i, SDL_CONTROLLER_AXIS_LEFTX) == GP_AXIS_STATE::AXIS_NEGATIVE_REPEAT)
							{
								App->ui_scene->current_menu->gamepads_focus[i]++;
							}
					}
					if (element[i]->element_type == IMAGE && element[i]->children.size() == 2)
					{
						if (App->ui_scene->champ_selected[i] == false)
						{
							if (App->input->GetButton((PLAYER)i, SDL_CONTROLLER_BUTTON_DPAD_RIGHT) == BUTTON_DOWN)
							{
								time_since_press[i].Start();
								automatic_traverse_margin[i].Start();
								element[i] = element[i]->children.front();
								element[i]->hovering = true;
								element[i]->callback->OnUIEvent(element[i], MOUSE_LEFT_CLICK);


							}
							else if (App->input->GetButton((PLAYER)i, SDL_CONTROLLER_BUTTON_DPAD_LEFT) == BUTTON_DOWN)
							{
								time_since_press[i].Start();
								automatic_traverse_margin[i].Start();
								element[i] = element[i]->children.back();
								element[i]->hovering = true;
								element[i]->callback->OnUIEvent(element[i], MOUSE_LEFT_CLICK);
							}
							else if (App->input->GetButton((PLAYER)i, SDL_CONTROLLER_BUTTON_DPAD_RIGHT) == BUTTON_REPEAT)
							{
								if (ManageAutomaticTraverseTiming(i,0.75f, 0.1f) == true)
								{
									element[i] = element[i]->children.front();
									element[i]->hovering = true;
									element[i]->callback->OnUIEvent(element[i], MOUSE_LEFT_CLICK);
								}
							}
							else if (App->input->GetButton((PLAYER)i, SDL_CONTROLLER_BUTTON_DPAD_LEFT) == BUTTON_REPEAT)
							{
								if (ManageAutomaticTraverseTiming(i,0.75f, 0.1f) == true)
								{
									element[i] = element[i]->children.back();
									element[i]->hovering = true;
									element[i]->callback->OnUIEvent(element[i], MOUSE_LEFT_CLICK);
								}
							}
						}
					}

					if (element[i]->parent != nullptr)
					{
						if (element[i]->parent->element_type == SLIDER)
						{
							if (App->input->GetButton((PLAYER)i, SDL_CONTROLLER_BUTTON_DPAD_RIGHT) == BUTTON_DOWN
								|| App->input->GetLRAxisState((PLAYER)i, SDL_CONTROLLER_AXIS_LEFTX) == GP_AXIS_STATE::AXIS_POSITIVE_DOWN)
							{
								time_since_press[i].Start();
								automatic_traverse_margin[i].Start();
								element[i]->localPosition += {4, 0};
								App->input->ForceButtonState((PLAYER)i, SDL_CONTROLLER_BUTTON_A, BUTTON_DOWN);
							}
							else if (App->input->GetButton((PLAYER)i, SDL_CONTROLLER_BUTTON_DPAD_LEFT) == BUTTON_DOWN
								|| App->input->GetLRAxisState((PLAYER)i, SDL_CONTROLLER_AXIS_LEFTX) == GP_AXIS_STATE::AXIS_NEGATIVE_DOWN)
							{
								time_since_press[i].Start();
								automatic_traverse_margin[i].Start();
								element[i]->localPosition -= {4, 0};
								App->input->ForceButtonState((PLAYER)i, SDL_CONTROLLER_BUTTON_A, BUTTON_DOWN);
							}
							else if (App->input->GetButton((PLAYER)i, SDL_CONTROLLER_BUTTON_DPAD_RIGHT) == BUTTON_REPEAT
								|| App->input->GetLRAxisState((PLAYER)i, SDL_CONTROLLER_AXIS_LEFTX) == GP_AXIS_STATE::AXIS_POSITIVE_REPEAT)
							{
								/*if (ManageAutomaticTraverseTiming(0.25f, 0.01f) == true)
								{*/
									
									element[i]->localPosition += {2, 0};
									App->input->ForceButtonState((PLAYER)i, SDL_CONTROLLER_BUTTON_A, BUTTON_DOWN);
								/*}*/
							}
							else if (App->input->GetButton((PLAYER)i, SDL_CONTROLLER_BUTTON_DPAD_LEFT) == BUTTON_REPEAT
								|| App->input->GetLRAxisState((PLAYER)i, SDL_CONTROLLER_AXIS_LEFTX) == GP_AXIS_STATE::AXIS_NEGATIVE_REPEAT)
							{
								/*if (ManageAutomaticTraverseTiming(0.25f, 0.01f) == true)
								{*/
									element[i]->localPosition -= {2, 0};
									App->input->ForceButtonState((PLAYER)i, SDL_CONTROLLER_BUTTON_A, BUTTON_DOWN);
								/*}*/
							}
						}
					}
				}


				if (!element[i]->hovering)
				{
					element[i]->hovering = true;
					gamepad_last_focus[i] = element[i];
					if (element[i]->callback != nullptr)
						element[i]->callback->OnUIEvent(element[i], MOUSE_ENTER);
				}
				else if (IN_RANGE(element[i]->element_type, CUSTOMIZING_BUTTON_BASIC,CUSTOMIZING_BUTTON_SHIELD) && element[i]->function == CUSTOMIZE)
				{
					if (App->input->AnyButtonDown((PLAYER)i))
						element[i]->callback->OnUIEvent(element[i], BUTTON_ANY,i);
					else if (App->input->AnyTriggerDown((PLAYER)i))
						element[i]->callback->OnUIEvent(element[i], TRIGGER_ANY, i);
				}
				else if ((is_focused[0] == true && App->input->GetButton(PLAYER::P1, SDL_CONTROLLER_BUTTON_A) == BUTTON_DOWN)
					|| (is_focused[1] == true && App->input->GetButton(PLAYER::P2, SDL_CONTROLLER_BUTTON_A) == BUTTON_DOWN)
					|| (is_focused[2] == true && App->input->GetButton(PLAYER::P3, SDL_CONTROLLER_BUTTON_A) == BUTTON_DOWN)
					|| (is_focused[3] == true && App->input->GetButton(PLAYER::P4, SDL_CONTROLLER_BUTTON_A) == BUTTON_DOWN))
				{
					if (element[i]->callback != nullptr)
					{
						ret = element[i]->callback->OnUIEvent(element[i], MOUSE_LEFT_CLICK);
					}
					if (element[i]->dragable)
					{
						element[i]->Start_Drag();
						draggingElement = element[i];
					}
					if (element[i]->element_type == BUTTON || element[i]->element_type == SWITCH)
						App->audio->PlayFx(button_click_fx, 0);
				}
				else if ((is_focused[0] == true && App->input->GetButton(PLAYER::P1, SDL_CONTROLLER_BUTTON_A) == BUTTON_UP)
					|| (is_focused[1] == true && App->input->GetButton(PLAYER::P2, SDL_CONTROLLER_BUTTON_A) == BUTTON_UP)
					|| (is_focused[2] == true && App->input->GetButton(PLAYER::P3, SDL_CONTROLLER_BUTTON_A) == BUTTON_UP)
					|| (is_focused[3] == true && App->input->GetButton(PLAYER::P4, SDL_CONTROLLER_BUTTON_A) == BUTTON_UP))
				{
					if (element[i]->callback != nullptr)
					{
						element[i]->callback->OnUIEvent(element[i], MOUSE_LEFT_RELEASE);
					}
					if (element[i]->dragable)
					{
						element[i]->End_Drag();
						draggingElement = nullptr;
					}
				}
				//Character selection and somebody presses A to lock their character selection or B to unlock the selection
				if (element[i]->element_type == IMAGE && element[i]->children.size() == 2)
				{
					if (App->ui_scene->champ_selected[i] == false && App->input->GetButton((PLAYER)i, SDL_CONTROLLER_BUTTON_A) == BUTTON_DOWN)
					{
					App->ui_scene->champ_selected[i] = true;
					App->audio->PlayFx(App->audio->fxConfirmChamp);

					if (i == 0 && App->ui_scene->marks_reset == true)
						App->ui_scene->player1_quad->section = { 484, 518, 170,191 };
					if (i == 1 && App->ui_scene->marks_reset == true)
						App->ui_scene->player2_quad->section = { 484, 518, 170,191 };
					if (i == 2 && App->ui_scene->marks_reset == true)
						App->ui_scene->player3_quad->section = { 484, 518, 170,191 };
					if (i == 3 && App->ui_scene->marks_reset == true)
						App->ui_scene->player4_quad->section = { 484, 518, 170,191 };
					
					
					}
					else if (App->ui_scene->champ_selected[i] == true && App->input->GetButton((PLAYER)i, SDL_CONTROLLER_BUTTON_B) == BUTTON_DOWN)
					{
						App->ui_scene->champ_selected[i] = false;
						App->audio->PlayFx(App->audio->fxCancelChamp);

						if (i == 0)
							App->ui_scene->player1_quad->section = { 288, 518, 170,191 };
						if (i == 1)
							App->ui_scene->player2_quad->section = { 288, 518, 170,191 };
						if (i == 2)
							App->ui_scene->player3_quad->section = { 288, 518, 170,191 };
						if (i == 3)
							App->ui_scene->player4_quad->section = { 288, 518, 170,191 };
						

						
					}
				}
			}
		}
	}

	//Evaluate what to do with the mouse focus
	if (mouse_focus != nullptr)
	{
		if (mouse_focus->hovering == false)
		{
			mouse_focus->hovering = true;
			last_mouse_focus = mouse_focus;
			if (mouse_focus->callback != nullptr)
				mouse_focus->callback->OnUIEvent(mouse_focus, MOUSE_ENTER);
		}
		else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
		{
			 if (mouse_focus->callback != nullptr)
			{
				ret = mouse_focus->callback->OnUIEvent(mouse_focus, MOUSE_LEFT_CLICK);
			}
			if (mouse_focus->dragable)
			{
				mouse_focus->Start_Drag();
				draggingElement = mouse_focus;
			}
			if (mouse_focus->element_type == BUTTON || mouse_focus->element_type == SWITCH)
				App->audio->PlayFx(button_click_fx, 0);
		}
		else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
		{
			if (mouse_focus->callback != nullptr)
			{
				mouse_focus->callback->OnUIEvent(mouse_focus, MOUSE_LEFT_RELEASE);
			}
			if (mouse_focus->dragable)
			{
				mouse_focus->End_Drag();
				draggingElement = nullptr;
			}
		}
		else if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN)
		{
			if (mouse_focus->callback != nullptr)
				ret = mouse_focus->callback->OnUIEvent(mouse_focus, MOUSE_RIGHT_CLICK);
		}
		else if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_UP)
		{
			if (mouse_focus->callback != nullptr)
				mouse_focus->callback->OnUIEvent(mouse_focus, MOUSE_RIGHT_RELEASE);
		}
	}
	return ret;
}

bool j1Gui::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
		UI_Debug = !UI_Debug;

	if (App->ui_scene->actual_menu == INGAME_MENU && !App->transition->doingMenuTransition)
	{
		if (App->arena_interactions->UI_storm_countdown < 6 && current_step == fade_step::none)
		{
			App->fade->FadeCustom(colorT.r, colorT.g, colorT.b, alphaT, 0.5f, start_time, total_time, current_step, colorT);
		}
	}

	return true;
}

// Called after all Updates
bool j1Gui::PostUpdate(float dt)
{

	if (App->ui_scene->current_menu != nullptr)
	{
		for (std::list <UI_element*>::const_iterator item = App->ui_scene->current_menu->elements.begin(); item != App->ui_scene->current_menu->elements.end(); ++item)
		{
			if ((*item)->moving)
			{
				(*item)->Mouse_Drag();
				if ((*item)->parent != nullptr && (*item)->parent->element_type == SLIDER)
				{
					Slider* parent = (Slider*)(*item)->parent;
					parent->setProgress((parent->button->localPosition.x + parent->button->section.w / (2 / App->gui->UI_scale)) / ((parent->bar_length) * App->gui->UI_scale));
				}
				(*item)->state = CLICKED;
			}

			/*if ((*item)->parent == nullptr)*/
			(*item)->BlitElement();
		}
	}
	if (UI_Debug)
		UIDebugDraw();

	if (App->ui_scene->actual_menu == INGAME_MENU && !App->transition->doingMenuTransition)
	{
		SDL_Rect screen = { 0,0,0,0 };
		uint width, height = 0;
		App->win->GetWindowSize(width, height);
		screen.w = width;
		screen.h = height;
		screen.x = App->ui_scene->timer->localPosition.x;
		screen.y = App->ui_scene->timer->localPosition.y;

		App->fade->PostUpdate(screen, start_time, total_time, current_step, colorT, alphaT, App->ui_scene->timer->texture);

		if(current_step == fade_step::none)
		App->ui_scene->timer->BlitElement();
	}

		
	return true;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");

	std::list <UI_element*>::iterator item = UI_elements.begin();

	while (item != UI_elements.end())
	{
		if(*item)
		delete *item;
		item++;
	}
	UI_elements.clear();

	return true;
}

void j1Gui::UIDebugDraw()
{
	for (std::list <UI_element*>::const_iterator item = App->ui_scene->current_menu->elements.begin(); item != App->ui_scene->current_menu->elements.end(); ++item)
	{
		SDL_Rect box;
		int scale = App->win->GetScale();
		box.x = (*item)->calculateAbsolutePosition().x * scale;
		box.y = (*item)->calculateAbsolutePosition().y * scale;
		box.w = (*item)->section.w;
		box.h = (*item)->section.h;
		App->render->DrawQuad(box, 255, 0, 0, 255, false, false);
	}

}
// const getter for atlas
const SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}

Text* j1Gui::createText(const char* text, int x, int y, _TTF_Font* font, SDL_Color color, j1Module* callback)
{
	Text* ret = new Text(text, x, y, font, color, callback);
	ret->solid = false;
	UI_elements.push_back(ret);

	return ret;
}

Image* j1Gui::createImage(int x, int y, SDL_Texture* texture, j1Module* callback)
{
	uint tex_width, tex_height;
	App->tex->GetSize(texture, tex_width, tex_height);
	Image* ret = new Image(texture, x, y, { 0, 0, (int)tex_width, (int)tex_height }, callback);
	UI_elements.push_back(ret);

	return ret;
}

Image* j1Gui::createImageFromAtlas(int x, int y, SDL_Rect section, j1Module* callback)
{
	Image* ret = new Image(atlas, x, y, section, callback);
	UI_elements.push_back(ret);

	return ret;
}

Button* j1Gui::createSwitch(int x, int y, SDL_Texture* texture, SDL_Rect standbyUnactive, SDL_Rect OnMouseUnactive, SDL_Rect standbyActive, SDL_Rect OnMouseActive, j1Module* callback)
{
	SDL_Texture* usingTexture = (texture) ? texture : atlas;

	Button* ret = new Button(x, y, usingTexture, standbyUnactive, OnMouseUnactive, standbyActive, OnMouseActive, callback);
	UI_elements.push_back(ret);

	return ret;
}


Window* j1Gui::createWindow(int x, int y, SDL_Texture * texture, SDL_Rect section, j1Module * callback)
{
	SDL_Texture* usingTexture = (texture) ? texture : atlas;

	Window* ret = new Window(usingTexture, x, y, section, callback);
	UI_elements.push_back(ret);

	return ret;
}

Slider * j1Gui::createSlider(int x, int y, SDL_Texture * texture, SDL_Rect empty, SDL_Rect full, Button* button, _TTF_Font* text_font, SDL_Color text_color, float default_progress, j1Module * callback, char* text)
{
	SDL_Texture* usingTexture = (texture) ? texture : atlas;

	Slider* ret = new Slider(x, y, usingTexture, empty, full, default_progress, callback);
	ret->solid = false;

	if (full.w > full.h)
	{
		button->setDragable(true, false);
		button->setLimits(empty.w / (2 / UI_scale), empty.w / (2 / UI_scale), -1, -1);
	}
	else
	{
		button->setDragable(false, true);
		button->setLimits(-1, -1, empty.h / (2 / UI_scale), empty.h / (2 / UI_scale));
	}

	ret->appendChild(((empty.w * UI_scale) - 5 - button->section.w / (2 / UI_scale)) * default_progress, y, button);
	button->setOriginalPos(((empty.w * UI_scale) - 7 - button->section.w / (2 / UI_scale)) * 0.5f, y);

	ret->appendChild(x, y, createText(text, x, y, text_font, text_color));

	UI_elements.push_back(ret);

	return ret;
}

Marker* j1Gui::createMarker(int x, int y, iPoint margin, std::list<UI_element*>::iterator* target, SDL_Texture* texture, SDL_Rect rect, j1Module* callback, marker_custom_button_rects rects, Marker_anim_data anim_data, bool is_support_marker)
{
	Marker* ret = nullptr;

	ret = new Marker(texture,x,y,margin,target,rect,callback, is_support_marker, rects, anim_data);
	UI_elements.push_back(ret);

	return ret;
}

bool j1Gui::ManageAutomaticTraverseTiming(int player_timer, float time_to_start, float time_margin)
{
	bool ret = false;
	
	if (time_since_press[player_timer].ReadSec() >= time_to_start && automatic_traverse_margin[player_timer].ReadSec() >= time_margin)
	{
		automatic_traverse_margin[player_timer].Start();
		ret = true;
	}

	return ret;
}

Button* j1Gui::createButton(int x, int y, SDL_Texture* texture, SDL_Rect standby, SDL_Rect OnMouse, SDL_Rect OnClick, j1Module* callback)
{
	SDL_Texture* usingTexture = (texture) ? texture : atlas;

	Button* ret = new Button(x, y, usingTexture, standby, OnMouse, OnClick, callback);
	UI_elements.push_back(ret);

	return ret;
}

Clock * j1Gui::createTimer(int x, int y, int initial_value, _TTF_Font * font, SDL_Color color, j1Module * callback)
{
	Clock* ret = new Clock(x, y, TIMER, font, color, callback);
	ret->setStartValue(initial_value);
	ret->solid = false;
	UI_elements.push_back(ret);

	return ret;
}

Clock * j1Gui::createStopWatch(int x, int y, _TTF_Font * font, SDL_Color color, j1Module * callback)
{
	Clock* ret = new Clock(x, y, STOPWATCH, font, color, callback);
	ret->solid = false;
	UI_elements.push_back(ret);

	return ret;
}

Button* j1Gui::createInteractiveButton(int x, int y, SDL_Texture* texture, SDL_Rect standby, SDL_Rect OnMouse, SDL_Rect OnClick, uint id, j1Module* callback)
{
	id = App->entities->playerid;
	if (id == 0)
	{
		SDL_Rect box;
		int scale = App->win->GetScale();
		box.x = standby.x * scale;
		box.y = standby.y * scale;
		box.w = standby.w * scale;
		box.h = standby.h * scale;
		App->render->DrawQuad(box, 255, 0, 0, 255, false, false);
	}

	SDL_Texture* usingTexture = (texture) ? texture : atlas;

	Button* ret = new Button(x, y, usingTexture, standby, OnMouse, OnClick, callback);
	UI_elements.push_back(ret);

	return ret;
}