/*!***************************************************************************
@file    main.cpp
@author  mc-w
@date    2/25/2016
@brief   Stack-based menu design for navigating in a console.

@copyright See LICENSE.md
*****************************************************************************/
#include "menu-system.hpp"
#include "conio.h"
#include "console-input.h"


// Application entry point. Note the order of events for menu initialization:
// It requires you to establish a base menu along with a series of different
// smaller sub-menus, or containers. 
int main()
{
  // Pre-menu init
  RConsole::Canvas::ReInit(60, 20);
  RConsole::Canvas::SetCursorVisible(false);

  // ====== Start menu init section ======
  Container *mainMenu = Container::Create("mainMenu");
  mainMenu->AddItem("> Mode select", "gamemode");
  mainMenu->AddItem("> Shopping List", "shopping");
  mainMenu->AddItem("exit", "exit");

  Container *gamemodeMenu = Container::Create("gamemode");
  gamemodeMenu->AddItem("> Mode1", "gamemode");
  gamemodeMenu->AddItem("> Mode2", "");
  gamemodeMenu->AddItem("back", "back");
  
  Container *shoppingMenu = Container::Create("shopping");
  shoppingMenu->AddItem("> Carrots", "");
  shoppingMenu->AddItem("> Spam", "");
  shoppingMenu->AddItem("> Chips", "");
  shoppingMenu->AddItem("> Lettuce", "");
  shoppingMenu->AddItem("> Oatmeal", "");
  shoppingMenu->AddItem("back", "back");

  MenuSystem testBlock("mainMenu");
  // ====== End menu init system ======

  while(1)
  {
    if(KeyHit())
    {
      int c = GetChar();
      switch (c)
      {
      case KEY_ESCAPE:
        if (!testBlock.Back())
          testBlock.Select("mainMenu");
        break;
      case 'S':
      case 's':
        testBlock.Down();
        break;
      case 'W':
      case 'w':
        testBlock.Up();
        break;
      case KEY_SPACE:
      case KEY_ENTER:
        testBlock.Select();
        break;
      }
    }

    testBlock.Draw();
    RConsole::Canvas::Update();
  }


  delete mainMenu;
  delete gamemodeMenu;
  delete shoppingMenu;
  return 0;
}
