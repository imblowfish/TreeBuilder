#include "pch.h"
#include "MainMenu.h"

void MainMenu::InitMenu(){
	tViewer.LinkRenderer(renderer);
	tViewer.Setup(SCREEN_WIDTH, SCREEN_HEIGHT);
	int shiftY = 40;						//сдвиг каждой кнопки относительно предыдущей по Y
	int scrElX = 80, scrElY = 30;			//начальна€ пощици€ по x и y
	int fontSize = 12;
	for(unsigned int i = 0; i < buttonNames.size(); i++){
		buttons.push_back(Button());
		buttons[i].LinkRenderer(renderer);
		buttons[i].SetupMe(buttonNames[i], SCREEN_WIDTH - scrElX, scrElY+i*shiftY, fontSize, \
						   {BTN_COLOR}, {BTN_FRAME_COLOR}, {BTN_INPUT_COLOR}, {FONT_COLOR});
	}
	Redraw();
}

void MainMenu::MainLoop(){
	SDL_Event event;
	SDL_MouseButtonEvent bEvent;
	int mouseX, mouseY;
	bool running = true;
	while(running){
		while(SDL_PollEvent(&event)){
			switch(event.type){
				case SDL_KEYDOWN:
					keys[event.key.keysym.sym] = 1;
					if(keys[SDLK_ESCAPE]) running = false;
					InputInEditBox(event);
					if(keys[SDLK_RETURN]){
						int nowSelIdx = tViewer.GetNowSelected();
						if(nowSelIdx < 0) continue;
						tBuilder.SetData(tViewer.GetNowSelected(), tViewer.GetVal());
						tViewer.EndInput();
					}else
					if(keys[SDLK_e]){
						tViewer.StartEdit();
					}else
					//гор€чие клавиши
					if(keys[SDLK_LCTRL]){
						if(keys[SDLK_LALT] && keys[SDLK_c]) tBuilder.ClearTree();
						if(keys[SDLK_s]) tBuilder.SaveTree(pathToSave);
						if(keys[SDLK_l]) tBuilder.LoadTree(pathToSave);
						if(keys[SDLK_x]){
							if(tViewer.GetNowSelected() < 0) continue;
							tBuilder.DeleteNode(tViewer.GetNowSelected());
						}
					}else
					if(keys[SDLK_a]){
						if(tViewer.GetNowSelected() < 0){
							if(tBuilder.IsEmpty()) tBuilder.CreateTree();
						}else 
						if(tBuilder.IsEmpty()){
							tBuilder.CreateTree();
							return;
						}else
						tBuilder.AddNodeTo(tViewer.GetNowSelected());
					} 
					else if(keys[SDLK_q]){
						if(tBuilder.IsEmpty()) continue;
						tBuilder.SetFirstPlayer();
					} 
					else if(keys[SDLK_s])tViewer.SetViewID();
					Redraw();
				break;
				case SDL_KEYUP:
					keys[event.key.keysym.sym] = 0;
				break;
				case SDL_MOUSEBUTTONUP:
					SDL_GetMouseState(&mouseX, &mouseY);
					dragAndDrop = false;
					bEvent = event.button;
					if(bEvent.button != SDL_BUTTON_LEFT) continue;
					for(unsigned int i = 0; i < buttons.size(); i++)
						if(buttons[i].MouseEventListener(event, mouseX, mouseY)){
							ActionOnButton(buttons[i].GetStrValue());
							Redraw();
							continue;
						}
					if(tViewer.ClickedOnNode(mouseX, mouseY) < 0)
						if(tViewer.ClickedOnBottomNode(mouseX, mouseY) < 0) continue;
					Redraw();
				break;
				case SDL_MOUSEBUTTONDOWN:
					SDL_GetMouseState(&mouseX, &mouseY);
					bEvent = event.button;
					if(bEvent.button == SDL_BUTTON_LEFT){
						for(unsigned int i = 0; i < buttons.size(); i++)
							if(buttons[i].MouseEventListener(event, mouseX, mouseY)) Redraw();
					}
					if(bEvent.button != SDL_BUTTON_RIGHT) continue;
					if(tViewer.ClickedOnNode(mouseX, mouseY) < 0)
						if(tViewer.ClickedOnBottomNode(mouseX, mouseY) < 0) continue;
					dragAndDrop = true;
					Redraw();
				break;
				case SDL_MOUSEMOTION:
					if(!dragAndDrop) continue;
					SDL_GetMouseState(&mouseX, &mouseY);
					tBuilder.Edit(tViewer.GetNowSelected(), mouseX);
					Redraw();
				break;
				case SDL_QUIT:
					running = false;
				break;
			}
		}
	}
}

void MainMenu::Redraw(){
	ClearScreen();
	tViewer.Draw(tBuilder.GetTree(), tBuilder.GetLevelsInfo());
	for(unsigned int i = 0; i < buttons.size(); i++) buttons[i].Draw();
	SDL_RenderPresent(renderer);
}

void MainMenu::ActionOnButton(string val){
	if(val == buttonNames[ADD]){
		if(tViewer.GetNowSelected() < 0){
			if(tBuilder.IsEmpty()) tBuilder.CreateTree();
			return;
		}
		if(tBuilder.IsEmpty()){
			tBuilder.CreateTree();
			return;
		}
		tBuilder.AddNodeTo(tViewer.GetNowSelected());
	}else if(val == buttonNames[DELETE]){
		if(tViewer.GetNowSelected() < 0) return;
		tBuilder.DeleteNode(tViewer.GetNowSelected());
	} else if(val == buttonNames[SAVE]){
		if(tBuilder.IsEmpty()) return;
		tBuilder.SaveTree(pathToSave);
	} else if(val == buttonNames[LOAD]){
		tBuilder.ClearTree();
		tBuilder.LoadTree(pathToSave);
		tViewer.ResetSelected();
	} else if(val == buttonNames[SET_PLAYER]){
		if(tBuilder.IsEmpty()) return;
		tBuilder.SetFirstPlayer();
	} else if(val == buttonNames[SHOW_ID]){
		tViewer.SetViewID();
	}
}