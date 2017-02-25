/*!***************************************************************************
@file    main.cpp
@author  Reverie Wisp
@date    2/25/2016
@brief   Stack-based menu design for navigating in a console.

@copyright "Don't be a Jerk" (See LICENSE.md)
*****************************************************************************/
#include "console-utils.hpp"
#include "conio.h"
#include <stack>
#include <map>
#include <string>
#include <vector>



// Registered series of lookups for menus to interact. Each entry includes the menu in question
// represented as the name and a pointer to the container in question.
class Container;
class MenuRegistry
{
public:
  // Static member funcs
  static void Register(std::string str, Container *con) { registry_[str] = con;  }
  static Container *GetContainer(std::string str)       
  { 
    auto iter = registry_.find(str); 
    if(iter != registry_.end())
      return registry_[str];
    return nullptr;
  }

private:
  // Private variables
  static std::map<std::string, Container *> registry_;
};
std::map<std::string, Container *> MenuRegistry::registry_ = std::map<std::string, Container *>();



// Menu Container and associated struct. The basic idea is that you create a label and a destination
// that allows menu navigation.
struct Selectable
{
  Selectable(std::string label, std::string target)
    : Label(label)
    , Target(target)
  {  }

  std::string Label;
  std::string Target;
};
class Container
{
public:
  // Effective ctor, registers the name for you. Deallocation needed after.
  static Container *Create(std::string menuName) 
  { 
    Container *c = new Container(menuName);
    MenuRegistry::Register(menuName, c); 
    return c;
  }

  // Member functions
  void AddItem(std::string label, std::string target) { lineItems_.push_back(Selectable(label, target)); }
  std::vector<Selectable> &GetAllItems() { return lineItems_; }
  Selectable GetSelected() { return lineItems_[selected_]; }
  int IndexSelectionValue() { return selected_; }
  void Next() { ++selected_; if(selected_ > lineItems_.size() - 1) selected_ = 0; }
  void Prev() { --selected_; if(selected_ < 0) selected_ = lineItems_.size() - 1; }

private:
  // Private ctor
    Container(std::string menuName) 
    : selected_(0)
    , name_(menuName)
    , lineItems_()
  {  }

  // Private variables
  int selected_;
  std::string name_;
  std::vector<Selectable> lineItems_;
};



// Stack-based menu system.
class MenuSystem
{
public:
  // Ctor
  MenuSystem(std::string initial)
    : stack_()
  { 
    Container *c = MenuRegistry::GetContainer(initial);
    if(c == nullptr)
      throw "Verify the strings used for the intial menu choice!";
    stack_.push(c); 
  }
  
  // Member functions
  void Down() { stack_.top()->Next(); }
  void Up()   { stack_.top()->Prev(); }
  void Select() 
  {
    Container *c = MenuRegistry::GetContainer(stack_.top()->GetSelected().Target);
    if(c == nullptr)
    {
      if(stack_.top()->GetSelected().Target == "back")
        stack_.pop();
      else if(stack_.top()->GetSelected().Target == "exit")
        exit(0);
    }
    else
      stack_.push(c);
  }
  void Draw()
  {
    std::vector<Selectable> &v = stack_.top()->GetAllItems();
    for(int i = 0; i < v.size(); ++i)
    {
      if(i == stack_.top()->IndexSelectionValue())
        RConsole::Canvas::DrawString(v[i].Label.c_str(), 3, i + 2, RConsole::LIGHTGREEN);
      else
        RConsole::Canvas::DrawString(v[i].Label.c_str(), 3, i + 2, RConsole::CYAN);
    }
  }

private:
  // Private variables
  std::stack<Container *> stack_;
};



// Application entry point. Note the order of events for menu initialization:
// It requires you to establish a base menu along with a series of different
// 
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
    if(_kbhit())
    {
      char c = _getch();
      switch(c)
      {
        case 's':
          testBlock.Down();
          break;
        case 'w':
          testBlock.Up();
          break;
        case ' ':
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
