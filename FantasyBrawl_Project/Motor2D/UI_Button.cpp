#include "UI_Button.h"
#include "j1App.h"
#include "j1Render.h"
#include "Brofiler\Brofiler.h"

void Button::setOutlined(bool isOutlined)
{
	if (text != nullptr)
		text->setOutlined(isOutlined);
}

void Button::appendChild(int x, int y, UI_element * child)
{
	child->localPosition = { x, y };
	child->setOriginalPos(x, y);
	child->parent = this;
	text = (Text*)child;
}

void Button::appendChildAtCenter(UI_element * child)
{
	iPoint child_pos(section.w / (2 / App->gui->UI_scale), section.h / (2 / App->gui->UI_scale));
	child_pos.x -= child->section.w / (2 / App->gui->UI_scale);
	child_pos.y -= child->section.h / (2 / App->gui->UI_scale);
	child->localPosition = { child_pos.x, child_pos.y };
	child->setOriginalPos(child_pos.x, child_pos.y);
	child->parent = this;
	text = (Text*)child;
}

void Button::BlitElement()
{
	BROFILER_CATEGORY("Button Blit", Profiler::Color::Beige);

	iPoint globalPos = calculateAbsolutePosition();
	switch (state)
	{
	case STANDBY:
		if (!active)
			App->render->Blit(texture, globalPos.x, globalPos.y, &section, 1.0f, (0,0),0,0,size);
		else
			App->render->Blit(texture, globalPos.x, globalPos.y, &sectionActive, 1.0f, (0, 0), 0, 0, size);
		break;
	case MOUSEOVER:
		if (!active)
			App->render->Blit(texture, globalPos.x, globalPos.y, &OnMouse, 1.0f, (0, 0), 0, 0, size);
		else
			App->render->Blit(texture, globalPos.x, globalPos.y, &OnMouseActive, 1.0f, (0, 0), 0, 0, size);
		break;
	case CLICKED:
		App->render->Blit(texture, globalPos.x, globalPos.y, &OnClick, 1.0f, (0, 0), 0, 0, size);
		break;
	}

	if (text != nullptr)
	{
		text->BlitElement();
	}

	if (parent != nullptr && parent->element_type == IMAGE && parent->children.size() == 2)
	{
		state = STANDBY;
	}

	
}