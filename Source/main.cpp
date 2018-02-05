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

void Shutdown() 
{
  exit(0);
}

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
  mainMenu->SetOrientation(ASCIIMenus::HORIZONTAL);
  mainMenu->AddItem("|  Mode select  |", "gamemode");
  mainMenu->AddItem("| Shopping List |", "shopping");
  mainMenu->AddItem("|     Exit      |", "exit", Shutdown);

  Container *gamemodeMenu = Container::Create("gamemode");
  gamemodeMenu->SetOrientation(ASCIIMenus::VERTICAL);
  gamemodeMenu->SetPosition(0, 1);
  gamemodeMenu->AddItem("| Mode1 |", "");
  gamemodeMenu->AddItem("| Mode2 |", "");
  gamemodeMenu->AddItem("| Back  |", "back");
  
  Container *shoppingMenu = Container::Create("shopping");
  shoppingMenu->SetOrientation(ASCIIMenus::VERTICAL);
  shoppingMenu->SetPosition(17, 1);
  shoppingMenu->AddItem("| Carrots |", "");
  shoppingMenu->AddItem("|  Spam   |", "");
  shoppingMenu->AddItem("|  Chips  |", "");
  shoppingMenu->AddItem("| Lettuce |", "");
  shoppingMenu->AddItem("| Oatmeal |", "");
  shoppingMenu->AddItem("|  Back   |", "back");

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
      case 'd':
      case 'D':
      case KEY_DOWN:
      case KEY_NUM_2:
      case KEY_RIGHT:
      case KEY_NUM_6:
        testBlock.Down();
        break;
      case 'W':
      case 'w':
      case 'a':
      case 'A':
      case KEY_UP:
      case KEY_NUM_8:
      case KEY_LEFT:
      case KEY_NUM_4:
        testBlock.Up();
        break;
      case KEY_SPACE:
      case KEY_ENTER:
        testBlock.Select();
        break;
      }
    }

    testBlock.Draw(0,0, true);
    RConsole::Canvas::Update();
  }


  delete mainMenu;
  delete gamemodeMenu;
  delete shoppingMenu;
  return 0;
}
