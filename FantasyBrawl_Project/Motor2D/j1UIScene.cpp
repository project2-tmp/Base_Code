#include "j1UIScene.h"
#include "j1Scene.h"
#include "j1App.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Audio.h"
#include "j1Window.h"
#include "j1ArenaInteractions.h"
#include "p2Log.h"
#include "j1Transition.h"
#include "j1Input.h"
#include "UI_element.h"
#include "UI_Button.h"
#include "UI_Image.h"
#include "UI_Slider.h"
#include "UI_Window.h"
#include "UI_Clock.h"
#include "j1EntityManager.h"
#include "j1Player.h"
#include "SDL_mixer\include\SDL_mixer.h"


j1UIScene::j1UIScene()
{
	
}

j1UIScene::~j1UIScene()
{
}

bool j1UIScene::Awake()
{
	name.assign("UIScene");
	return true;
}

bool j1UIScene::Start()
{

	
	 small_texts_font = App->fonts->Load("fonts/BMYEONSUNG.ttf", 80);
	
	_TTF_Font* big_texts_font = App->fonts->Load("fonts/finalf.ttf", 55);
	_TTF_Font* mid_texts_font = App->fonts->Load("fonts/finalf.ttf", 36);
	
	_TTF_Font* huge_texts_font = App->fonts->Load("fonts/finalf.ttf", 80);
	_TTF_Font* special_text_font = App->fonts->Load("fonts/finalf.ttf", 55);
	 big_buttons_font = App->fonts->Load("fonts/finalf.ttf", 80);
	 mid_buttons_font = App->fonts->Load("fonts/finalf.ttf", 50);
	

	SDL_Color yellow_color = { 229, 168, 61, 255 };
	SDL_Color white_color = { 255, 255, 255, 0 };
	SDL_Color grey_color = { 190, 177, 158, 191 };
	SDL_Color dark_yellow_color = { 146, 97, 45, 255 };
	SDL_Color black_color = { 0, 0, 0, 255 };
	SDL_Color brown_color = { 139,69,19 };
	


	

	

	menu* creditsMenu = new menu(CREDITS_MENU);
	{
		
	}

	menu* startMenu = new menu(START_MENU);
	{
		


		//PLAY
		UI_element* new_game = App->gui->createButton(372 * App->gui->UI_scale, 250 * App->gui->UI_scale, NULL, { 0,148,281,111 }, { 281,148,281,111 }, { 562,148,281,111 }, this);
		new_game->function = NEW_GAME;

		UI_element* new_text = App->gui->createText("PLAY", 200, 200, big_buttons_font, brown_color);
		new_text->setOutlined(true);
		new_game->appendChildAtCenter(new_text);

		//CHAMPIONS
		continueButton = App->gui->createButton(372 * App->gui->UI_scale, 400 * App->gui->UI_scale, NULL, { 0,148,281,111 }, { 0,148,281,111 }, { 0,148,281,111 }, this);
		continueButton->function = CONTINUE;


		UI_element* continue_text = App->gui->createText("CHAMPIONS", 200, 200, big_buttons_font, brown_color);
		continue_text->setOutlined(true);
		continueButton->appendChildAtCenter(continue_text);

		//EXIT GAME
		UI_element* exit_game = App->gui->createButton(372 * App->gui->UI_scale, 550 * App->gui->UI_scale, NULL, { 0,148,281,111 }, { 281,148,281,111 }, { 562,148,281,111 }, this);

		exit_game->function = EXIT;

		UI_element* exit_text = App->gui->createText("EXIT", 200, 200, big_buttons_font, brown_color);
		exit_text->setOutlined(true);
		exit_game->appendChildAtCenter(exit_text);

		//CREDITS
		UI_element* credits = App->gui->createButton(App->gui->UI_scale, 647 * App->gui->UI_scale, NULL, { 0,613,160,100 }, { 0,613,160,100 }, { 0,613,160,100 }, this);
		credits->function = WEBPAGE;



		//SETTINGS
		UI_element* settings_start_menu = App->gui->createButton(823 * App->gui->UI_scale, App->gui->UI_scale, NULL, { 250,613,170,150 }, { 250,613,170,150 }, { 250,613,170,150 }, this);

		settings_start_menu->function = SETTINGS;


		//BACKGROUND
		UI_element* background_image = App->gui->createImage(0, 0, App->tex->Load("gui/background.png"), this);

		startMenu->elements.push_back(background_image);
		startMenu->elements.push_back(new_game);
		startMenu->elements.push_back(new_text);
		startMenu->elements.push_back(continueButton);
		startMenu->elements.push_back(continue_text);
		startMenu->elements.push_back(exit_game);
		startMenu->elements.push_back(exit_text);
		startMenu->elements.push_back(credits);
		startMenu->elements.push_back(settings_start_menu);
		menus.push_back(startMenu);
	}

	menu* ingameMenu = new menu(INGAME_MENU);
	{
		UI_element* hp_bar_player1 = App->gui->createImageFromAtlas(App->scene->player1->Entityinfo.position.x, App->scene->player1->Entityinfo.position.y,  {424, 428, 209, 27}, this);
		hp_bar1 = hp_bar_player1;
		UI_element* sp_bar_player1 = App->gui->createImageFromAtlas(App->scene->player1->Entityinfo.position.x, App->scene->player1->Entityinfo.position.y, { 639, 428, 209, 27 }, this);
		sp_bar1 = sp_bar_player1;

		UI_element* hp_bar_player2 = App->gui->createImageFromAtlas(App->scene->player1->Entityinfo.position.x, App->scene->player1->Entityinfo.position.y, { 424, 428, 209, 27 }, this);
		hp_bar2 = hp_bar_player2;

		UI_element* hp_bar_player3 = App->gui->createImageFromAtlas(App->scene->player1->Entityinfo.position.x, App->scene->player1->Entityinfo.position.y, { 424, 428, 209, 27 }, this);
		hp_bar3 = hp_bar_player3;

		UI_element* hp_bar_player4 = App->gui->createImageFromAtlas(App->scene->player1->Entityinfo.position.x, App->scene->player1->Entityinfo.position.y, { 424, 428, 209, 27 }, this);
		hp_bar4 = hp_bar_player4;

		

		ingameMenu->elements.push_back(hp_bar_player1);
		/*ingameMenu->elements.push_back(hp_bar_player2);
		ingameMenu->elements.push_back(hp_bar_player3);
		ingameMenu->elements.push_back(hp_bar_player4);*/
		ingameMenu->elements.push_back(sp_bar_player1);
		menus.push_back(ingameMenu);

	}

	menu* championSelection = new menu(SELECTION_MENU);
	{
		UI_element* selection_image = App->gui->createImage(0, 0, App->tex->Load("gui/vs.png"), this);
		UI_element* selection_text = App->gui->createText("CHAMPION SELECTION", 275, 60, big_buttons_font, brown_color);
		selection_text->setOutlined(true);

		UI_element* champion_button1 = App->gui->createButton(300 * App->gui->UI_scale, 175 * App->gui->UI_scale, NULL, { 560,507,180,180 }, { 560,507,180,180 }, { 560,507,180,180 }, this);
		champion_button1->function = SELECTING;

		UI_element* champion_button2 = App->gui->createButton(500 * App->gui->UI_scale, 175* App->gui->UI_scale, NULL, { 740,507,180,180 }, { 740,507,180,180 }, { 740,507,180,180 }, this);
		UI_element* champion_button3 = App->gui->createButton(300 * App->gui->UI_scale, 375 * App->gui->UI_scale, NULL, { 560,696,180,180 }, { 560,696,180,180 }, { 560,696,180,180 }, this);
		UI_element* champion_button4 = App->gui->createButton(500 * App->gui->UI_scale, 375 * App->gui->UI_scale, NULL, { 740,696,180,180 }, { 740,696,180,180 }, { 740,696,180,180 }, this);

		//PLAYER QUADS
		UI_element* player1_quad = App->gui->createImageFromAtlas(20, 580, { 18, 904,200,180 }, this);
		UI_element* player2_quad = App->gui->createImageFromAtlas(280, 580, { 232, 904,200,180 }, this);
		UI_element* player3_quad = App->gui->createImageFromAtlas(540, 580, { 447, 904,200,180 }, this);
		UI_element* player4_quad = App->gui->createImageFromAtlas(800, 580, { 666, 904,200,180 }, this);

		//PLAYER IMAGE(?)
		mark1 = App->gui->createImageFromAtlas(35, 591, { 33, 1107, 169,122 }, this);
		mark2 = App->gui->createImageFromAtlas(293, 591, { 33, 1107, 169,122 }, this);
		mark3 = App->gui->createImageFromAtlas(552, 591, { 33, 1107, 169,122 }, this);
		mark4 = App->gui->createImageFromAtlas(812, 591, { 33, 1107, 169,122 }, this);

		//READY BUTTON
		ready = App->gui->createButton(700 * App->gui->UI_scale, 330 * App->gui->UI_scale, NULL, { 0,28,278,105 }, { 0,28,278,105 }, { 0,28,278,105 }, this);//{ 281,148,281,111 }, { 562,148,281,111 }
		ready_text = App->gui->createText("READY", 780, 340, big_buttons_font, black_color);

		championSelection->elements.push_back(selection_image);
		championSelection->elements.push_back(selection_text);
		championSelection->elements.push_back(champion_button1);
		championSelection->elements.push_back(champion_button2);
		championSelection->elements.push_back(champion_button3);
		championSelection->elements.push_back(champion_button4);
		championSelection->elements.push_back(player1_quad);
		championSelection->elements.push_back(player2_quad);
		championSelection->elements.push_back(player3_quad);
		championSelection->elements.push_back(player4_quad);
		championSelection->elements.push_back(mark1);
		championSelection->elements.push_back(mark2);
		championSelection->elements.push_back(mark3);
		championSelection->elements.push_back(mark4);
		championSelection->elements.push_back(ready);
		championSelection->elements.push_back(ready_text);
		menus.push_back(championSelection);
	}

	menu* settingsMenu = new menu(SETTINGS_MENU);
	{
		

		//WINDOW
		/*UI_element* settings_window = App->gui->createWindow(App->gui->UI_scale, App->gui->UI_scale, App->tex->Load("gui/big_parchment.png"), { 246,162,1000,718 }, this);*/
		UI_element* settings_image = App->gui->createImage(0, 0, App->tex->Load("gui/big_parchment.png"), this);
		UI_element* settings_text = App->gui->createText("OPTIONS", 425, 60, big_buttons_font, brown_color);
		settings_text->setOutlined(true);


		//BACK BUTTON
		UI_element* back_button = App->gui->createButton(375 * App->gui->UI_scale, 580 * App->gui->UI_scale, NULL, { 0,148,281,111 }, { 281,148,281,111 }, { 562,148,281,111 }, this);
		back_button->function = BACK;
		UI_element* back_text = App->gui->createText("BACK", 300, 300, mid_buttons_font, brown_color);
		back_text->setOutlined(true);
		back_button->appendChildAtCenter(back_text);

		//AUDIO
		Button* music_slider_butt = App->gui->createButton(240, 0, NULL, { 341, 287, 15, 40 }, { 341, 287, 15, 40 }, { 341, 287, 15, 40 }, this);
		Slider* music_slider = App->gui->createSlider(400, 255, NULL, { 0, 291, 288, 21 }, { 0, 318, 288, 21 }, music_slider_butt, mid_texts_font, brown_color);
		music_slider->modify = MUSIC;
		settings_image->appendChild(430 * App->gui->UI_scale, 160 * App->gui->UI_scale, music_slider);

		UI_element* audio_text = App->gui->createText("AUDIO", 280, 240, mid_buttons_font, brown_color);
		audio_text->setOutlined(true);

		//FULLSCREEN
		/*Button* full_switch = App->gui->createSwitch(600, 415, NULL, { 404, 291, 47, 22 }, { 404, 291, 47, 22 }, { 404, 314, 47, 22 }, { 404, 314, 47, 22 }, this);
		settings_image->appendChild(550 * App->gui->UI_scale, 325 * App->gui->UI_scale, full_switch);

		UI_element* fullscreen_text = App->gui->createText("FULLSCREEN", 280, 400, mid_buttons_font, brown_color);
		fullscreen_text->setOutlined(true);*/


		
		settingsMenu->elements.push_back(settings_image);
		settingsMenu->elements.push_back(settings_text);
		settingsMenu->elements.push_back(back_button);
		settingsMenu->elements.push_back(back_text);
		settingsMenu->elements.push_back(music_slider_butt);
		settingsMenu->elements.push_back(music_slider);
		settingsMenu->elements.push_back(audio_text);
		/*settingsMenu->elements.push_back(full_switch);
		settingsMenu->elements.push_back(fullscreen_text);*/
		menus.push_back(settingsMenu);
	}

	menu* pauseMenu = new menu(PAUSE_MENU);
	{
		//WINDOW
		UI_element* pause_window = App->gui->createWindow(140*App->gui->UI_scale, 50*App->gui->UI_scale, App->tex->Load("gui/medium_parchment.png"), { 225,250, 744, 703 }, this);
		UI_element* pause_text = App->gui->createText("PAUSE", 450, 100, big_buttons_font, brown_color);
		pause_text->setOutlined(true);


		//BACK BUTTON
		UI_element* back_button = App->gui->createButton(375 * App->gui->UI_scale, 580 * App->gui->UI_scale, NULL, { 0,148,281,111 }, { 281,148,281,111 }, { 562,148,281,111 }, this);
		back_button->function = BACK;
		UI_element* back_text = App->gui->createText("BACK", 300, 300, mid_buttons_font, brown_color);
		back_text->setOutlined(true);
		back_button->appendChildAtCenter(back_text);

		//AUDIO
		Button* music_slider_butt = App->gui->createButton(500, 0, NULL, { 341, 287, 15, 40 }, { 341, 287, 15, 40 }, { 341, 287, 15, 40 }, this);
		Slider* music_slider = App->gui->createSlider(420, 255, NULL, { 0, 291, 288, 21 }, { 0, 318, 288, 21 }, music_slider_butt, mid_texts_font, brown_color);
		music_slider->modify = MUSIC;
		

		UI_element* audio_text = App->gui->createText("AUDIO", 330, 240, mid_buttons_font, brown_color);
		audio_text->setOutlined(true);

		//FULLSCREEN
		/*Button* full_switch = App->gui->createSwitch(600, 415, NULL, { 404, 291, 47, 22 }, { 404, 291, 47, 22 }, { 404, 314, 47, 22 }, { 404, 314, 47, 22 }, this);
		

		UI_element* fullscreen_text = App->gui->createText("FULLSCREEN", 330, 400, mid_buttons_font, brown_color);
		fullscreen_text->setOutlined(true);*/



		pauseMenu->elements.push_back(pause_window);
		pauseMenu->elements.push_back(pause_text);
		pauseMenu->elements.push_back(back_button);
		pauseMenu->elements.push_back(back_text);
		pauseMenu->elements.push_back(music_slider_butt);
		pauseMenu->elements.push_back(music_slider);
		pauseMenu->elements.push_back(audio_text);
		/*pauseMenu->elements.push_back(full_switch);
		pauseMenu->elements.push_back(fullscreen_text);*/
		menus.push_back(pauseMenu);
	}

	finalMenu = new menu(FINAL_MENU);
	{

		/*CreateScoreBoard(2);*/

		/*player_winner = App->scene->GetWinner();

		if (player_winner != nullptr)
		{
			if(player_winner == App->scene->player1)
				CreateScoreBoard(1);
			else if(player_winner == App->scene->player2)
				CreateScoreBoard(2);
			else if (player_winner == App->scene->player3)
				CreateScoreBoard(3);
			else if (player_winner == App->scene->player4)
				CreateScoreBoard(4);
		}*/

	}

	
	current_menu = startMenu;

	App->audio->PlayMusic(App->audio->pathMainMenu1.data(), 0);

	return true;
}

bool j1UIScene::PreUpdate()
{
	return true;
}

bool j1UIScene::Update(float dt)
{
	bool ret = true;

	App->audio->PlayMusic(App->audio->pathMainMenu1.data(), 0);

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		if (actual_menu == START_MENU)
		{
			ret = false;
		}
		else if (actual_menu == INGAME_MENU)
		{
			App->on_GamePause = true;
			actual_menu = PAUSE_MENU;

			Mix_PauseMusic();
			App->transition->menuTransition(PAUSE_MENU, 0.3f);
			App->arena_interactions->PauseStorm();
			ret = true;
		}
		else if (actual_menu == PAUSE_MENU)
		{
			App->on_GamePause = false;
			actual_menu = INGAME_MENU;

			Mix_ResumeMusic();
			App->transition->menuTransition(INGAME_MENU, 0.3f);
			App->arena_interactions->ContinueStorm();
			ret = true;

		}
		else if (actual_menu == SELECTION_MENU)
		{
			actual_menu = START_MENU;
			App->transition->menuTransition(START_MENU, 0.3f);
			ret = true;
		}
	}


	//PAUSING GAME WHILE ON MENUS
	if (actual_menu == START_MENU)
	{
		
		App->on_GamePause = true;
		
	}
	else if (actual_menu == SETTINGS_MENU)
	{
		
		App->on_GamePause = true;
	}
	else if (actual_menu == SELECTION_MENU)
	{
		App->on_GamePause = true;
	}
	else if (actual_menu == INGAME_MENU)
	{
		App->on_GamePause = false;
	}
	else if (actual_menu == FINAL_MENU)
	{
		App->on_GamePause = true;
	}
		
	//UPDATING HP BARS POSITION
	hp_bar1->localPosition.x = App->scene->player1->Entityinfo.position.x - 25;
	hp_bar1->localPosition.y = App->scene->player1->Entityinfo.position.y - 125;
	/*hp_bar2->localPosition.x = App->scene->player2->Entityinfo.position.x - 25;
	hp_bar2->localPosition.y = App->scene->player2->Entityinfo.position.y - 125;*/
	/*hp_bar3->localPosition.x = App->scene->player3->Entityinfo.position.x - 25;
	hp_bar3->localPosition.y = App->scene->player3->Entityinfo.position.y - 100;
	hp_bar4->localPosition.x = App->scene->player4->Entityinfo.position.x - 25;
	hp_bar4->localPosition.y = App->scene->player4->Entityinfo.position.y - 100;*/

	//UPDATING SP BARS POSITION
	sp_bar1->localPosition.x = App->scene->player1->Entityinfo.position.x - 25;
	sp_bar1->localPosition.y = App->scene->player1->Entityinfo.position.y - 100;
	
	if (player1_select == true)
	{
		mark1->section = { 243, 1107, 169,122 };
	}
	else if (player1_select == false)
	{
		mark1->section = { 33, 1107, 169,122 };
	}

	if (player2_select == true)
	{
		mark2->section = { 243, 1107, 169,122 };
	}
	else if (player2_select == false)
	{
		mark2->section = { 33, 1107, 169,122 };
	}

	if (player3_select == true)
	{
		mark3->section = { 243, 1107, 169,122 };
	}
	else if (player3_select == false)
	{
		mark3->section = { 33, 1107, 169,122 };
	}

	if (player4_select == true)
	{
		mark4->section = { 243, 1107, 169,122 };
	}
	else if (player4_select == false)
	{
		mark4->section = { 33, 1107, 169,122 };
	}

	if (player1_select == true && player2_select == true && player3_select == true && player4_select == true)
	{
		ready->section = { 281,148,281,111 };
		ready->function = INGAME;
		
	}
	else
	{
		ready->section = { 0,28,278,105 };
	}


	//GET TO SCOREBOARD SCREEN

	player_winner = App->scene->GetWinner();

	if (player_winner != nullptr && scoreboard == false)
	{
		scoreboard = true;
		actual_menu = FINAL_MENU;
		App->transition->menuTransition(FINAL_MENU, 0.3);
		if (player_winner == App->scene->player1)
			CreateScoreBoard(1);
		else if (player_winner == App->scene->player2)
			CreateScoreBoard(2);
		else if (player_winner == App->scene->player3)
			CreateScoreBoard(3);
		else if (player_winner == App->scene->player4)
			CreateScoreBoard(4);
	}

	


	return ret;
}

bool j1UIScene::PostUpdate(float dt)
{
	return true;
}

bool j1UIScene::OnUIEvent(UI_element* element, event_type event_type)
{
	bool ret = true;

	
	
	if (event_type == MOUSE_ENTER)
	{
		element->state = MOUSEOVER;

	}
	else if (event_type == MOUSE_LEAVE)
	{
		element->state = STANDBY;

	}
	else if (event_type == MOUSE_LEFT_CLICK)
	{
		
		element->state = CLICKED;

		if (element->element_type == SWITCH)
		{
			Button* tmp = (Button*)element;
			tmp->active = !tmp->active;
			newValues.fullscreen = tmp->active;
		}
		
	
		switch (element->function)
		{
		case NEW_GAME:
		{
			
			//RESET SELECTION BOOLS && COUNTER
			counter = 0;
			player1_select = false;
			player2_select = false;
			player3_select = false;
			player4_select = false;

			// --- Reset everything ---
			App->scene->ResetAll();
			
			scoreboard = false;

			actual_menu = SELECTION_MENU;
			App->transition->menuTransition(SELECTION_MENU, 0.3);

			App->audio->PlayMusic(App->audio->pathChampSelect.data(), 0);
			break;
		}
		case INGAME:
		{			
			actual_menu = INGAME_MENU;
			App->transition->menuTransition(INGAME_MENU, 0.3);
			App->audio->PlayMusic(App->audio->pathMap1.data(), 0);
			break;
		}
		case SELECTING:
		{		

			counter++;

			if (counter == 1)
			{
				player1_select = true;
			}
			if (counter == 2)
			{
				player2_select = true;
			}
			if (counter == 3)
			{
				player3_select = true;
			}
			if (counter == 4)
			{
				player4_select = true;
			}



			//App->arena_interactions->StartStorm();
			break;
		}
		case SCORES:
		{
			actual_menu = FINAL_MENU;
			App->transition->menuTransition(FINAL_MENU, 0.3);
			break;
			
		}
		case RESTART:
		{
			App->scene->ResetAll();
			actual_menu = START_MENU;
			App->transition->menuTransition(START_MENU, 0.3);
			break;
		}
		break;
		case CONTINUE:
		{

		}
		break;
		case SETTINGS:
			actual_menu = SETTINGS_MENU;
			App->transition->menuTransition(SETTINGS_MENU, 0.3);
			App->audio->PlayMusic(App->audio->pathOptions.data(), 0);

			break;
		case CREDITS:

			break;
		case EXIT:
			ret = false;
			break;
		case PAUSE:
			
			break;
		case APPLY:

			break;
		case CANCEL:

			break;
		case BACK:
			if (actual_menu == SETTINGS_MENU)
			{
				App->transition->menuTransition(previous_menu, 0.3);
				actual_menu = START_MENU;
				App->audio->PlayMusic(App->audio->pathMainMenu1.data(),0);
			}
			if (actual_menu == PAUSE_MENU)
			{
				App->on_GamePause = false;
				//App->arena_interactions->DestroyStorm();
				App->transition->menuTransition(START_MENU, 0.3);
				actual_menu = START_MENU;
				App->audio->PlayMusic(App->audio->pathMainMenu1.data(), 0);
			}
			break;
		case RESTORE:

			break;
		case HOME:

			break;
		case WEBPAGE:

			App->RequestBrowser("https://github.com/CheckTheDog/Fantasy-Brawl");

			break;
		}
	}


	else if (event_type == MOUSE_LEFT_RELEASE)
	{
		if (element->parent != nullptr && element->parent->element_type == SLIDER)
		{
			Slider* tmp = (Slider*)element->parent;
			switch (tmp->modify)
			{
			case MUSIC:
				newValues.music = tmp->progress;
				break;
			case FX:
				newValues.fx = tmp->progress;
				break;
			}
		}
		if (element->state == CLICKED)
			element->state = MOUSEOVER;
	}
	else if (event_type == MOUSE_RIGHT_CLICK)
	{
	}
	else if (event_type == MOUSE_RIGHT_RELEASE)
	{
	}
	else if (event_type == TIMER_ZERO)
	{
		LOG("Clock reached zero");
	}
	else if (event_type == STOPWATCH_ALARM)
	{
		Clock* clock = (Clock*)element;
		LOG("Clock alarm at: %d", clock->time);
	}

	return ret;
}

bool j1UIScene::CleanUp()
{
	std::list <menu*>::iterator item = menus.begin();

	while (item != menus.end())
	{
		delete *item;
		item++;
	}

	menus.clear();
	current_menu = nullptr;
	

	return true;
}


bool j1UIScene::Load(pugi::xml_node& data)
{

	return true;
}

bool j1UIScene::Save(pugi::xml_node& data) const
{

	return true;
}

bool j1UIScene::loadMenu(menu_id id)
{
	bool ret = false;
	
	previous_menu = current_menu->id;
	pauseClock();
	for (std::list <menu*>::const_iterator item = menus.begin(); item != menus.end(); item++)
	{
		if ((*item)->id == id)
		{
			current_menu = *item;
			playClock();
			ret = true;
			if (id == SETTINGS_MENU)
			{
				for (std::list <UI_element*>::const_iterator item2 = current_menu->elements.begin(); item2 != current_menu->elements.end(); ++item2)
				{
					if ((*item2)->element_type == SWITCH)
					{
						Button* full_switch = (Button*)*item2;
						startValues.fullscreen = full_switch->active;
					}
					if ((*item2)->element_type == SLIDER)
					{
						Slider* slider = (Slider*)*item2;
						switch (slider->modify)
						{
						case MUSIC:
							startValues.music = slider->getProgress();
							break;
						case FX:
							startValues.fx = slider->getProgress();
							break;
						}
					}
				}
			}
			break;
		}
	}

	
	return ret;
}



void j1UIScene::applySettings(settings_values values)
{
	Uint32 flag = 0;
	if (values.fullscreen)
		flag = SDL_WINDOW_FULLSCREEN;
	SDL_SetWindowFullscreen(App->win->window, flag);



	for (std::list <UI_element*>::const_iterator item = current_menu->elements.begin(); item != current_menu->elements.end(); ++item)
	{
		if ((*item)->element_type == SWITCH)
		{
			Button* full_switch = (Button*)*item;
			full_switch->active = values.fullscreen;
		}
		if ((*item)->element_type == SLIDER)
		{
			Slider* slider = (Slider*)*item;
			switch (slider->modify)
			{
			case MUSIC:
				slider->setProgress(values.music);
				break;
			case FX:
				slider->setProgress(values.fx);
				break;
			}
			slider->button->localPosition.x = ((slider->section.w * App->gui->UI_scale) - 5 - slider->button->section.w / (2 / App->gui->UI_scale)) * slider->progress;
		}
	}
}

void j1UIScene::pauseClock()
{
	for (std::list <UI_element*>::const_iterator item = current_menu->elements.begin(); item != current_menu->elements.end(); ++item)
	{
		if ((*item)->element_type == CLOCK)
		{
			Clock* clock = (Clock*)*item;
			/*if (!clock->counter.isPaused())
				clock->counter.Pause();*/
		}
	}
}

void j1UIScene::playClock()
{
	for (std::list <UI_element*>::const_iterator item = current_menu->elements.begin(); item != current_menu->elements.end(); ++item)
	{
		if ((*item)->element_type == CLOCK)
		{
			Clock* clock = (Clock*)*item;
			/*if (clock->counter.isPaused())
				clock->counter.Play();*/
		}
	}
}

void j1UIScene::CreateScoreBoard(int num)
{
	
	SDL_Color brown_color = { 139,69,19 };

	//WINDOW
	
	if (final_image != nullptr)
	{
		App->tex->UnLoad(final_image->texture);
	}
	final_image = App->gui->createImage(0, 0, App->tex->Load("gui/big_parchment.png"), this);

	UI_element* final_text = App->gui->createText("SCOREBOARD", 400, 60, big_buttons_font, brown_color);
	final_text->setOutlined(true);

	//SCOREBOARD INFO
	UI_element* winner_text = App->gui->createText("WINNER:", 260, 230, big_buttons_font, brown_color);
	winner_text->setOutlined(true);


	switch (num)
	{
		case 1:
		{
			win_text = App->gui->createText("PLAYER 1", 500, 230, small_texts_font, brown_color);
			win_text->setOutlined(true);
			break;
		}
		case 2:
		{
			win_text = App->gui->createText("PLAYER 2", 500, 230, small_texts_font, brown_color);
			win_text->setOutlined(true);
			break;
		}
		case 3:
		{
			win_text = App->gui->createText("PLAYER 3", 500, 230, small_texts_font, brown_color);
			win_text->setOutlined(true);
			break;
		}
		case 4:
		{
			win_text = App->gui->createText("PLAYER 4", 500, 230, small_texts_font, brown_color);
			win_text->setOutlined(true);
			break;
		}
	}
	
	

	//END BUTTON
	UI_element* end_button = App->gui->createButton(375 * App->gui->UI_scale, 580 * App->gui->UI_scale, NULL, { 0,148,281,111 }, { 281,148,281,111 }, { 562,148,281,111 }, this);
	end_button->function = RESTART;
	UI_element* end_text = App->gui->createText("END", 300, 300, mid_buttons_font, brown_color);
	end_text->setOutlined(true);
	end_button->appendChildAtCenter(end_text);




	finalMenu->elements.push_back(final_image);
	finalMenu->elements.push_back(final_text);
	finalMenu->elements.push_back(winner_text);
	finalMenu->elements.push_back(win_text);
	finalMenu->elements.push_back(end_button);
	finalMenu->elements.push_back(end_text);
	menus.push_back(finalMenu);


}