/*!***************************************************************************
@file    main.cpp
@author  mc-w
@date    2/25/2016
@brief   Stack-based menu design for navigating in a console.

@copyright "Don't be a Jerk" (See LICENSE.md)
*****************************************************************************/
#include "console-utils.hpp"
#include <stack>
#include <map>
#include <string>
#include <vector>


//////////////////////////////////////////////////////
// Registered series of lookups for menus to interact. Each entry includes the menu in question
// represented as the name and a pointer to the container in question.
//////////////////////////////////////////////////////
enum Orientation { HORIZONTAL, VERTICAL };
class Container;
class MenuRegistry
{
public:
  // Register a specific key/containtainer association. Duplicate keys override eachother.
  static void Register(std::string str, Container *con) 
  { 
    registry_[str] = con;  
  }
  
  // Gets the container associted with the string key, returns null if it does not exist.
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

// Static init
std::map<std::string, Container *> MenuRegistry::registry_ = std::map<std::string, Container *>();



//////////////////////////////////////////////////////
// Menu Container and associated struct. The basic idea is that you create a label and a destination
// that allows menu navigation.
//////////////////////////////////////////////////////
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
  void AddItem(std::string label, std::string target) 
  { 
    lineItems_.push_back(Selectable(label, target)); 
  }
  
  // Setter
  void SetOrientation(Orientation o)   { orientation_ = o;  }
  void SetPosition(size_t x, size_t y) { x_ = x; y_ = y;    }

  // Accessors
  std::vector<Selectable> &GetAllItems() { return lineItems_; }
  Selectable GetSelected()               { return lineItems_[selected_]; }
  size_t GetSelectedLine()               { return selected_; }
  Orientation GetOrientation()           { return orientation_; }
  void SetSelectedLine(size_t line)      { selected_ = line; }
  size_t GetXPos() { return x_; }
  size_t GetYPos() { return y_; }


  // Changes to next selection, with wrapping.
  void Next() 
  { 
    ++selected_; 
    if(selected_ > lineItems_.size() - 1) 
      selected_ = 0; 
  }

  // Changes to previous selection, with wrapping.
  void Prev() 
  { 
    if(selected_ == 0) 
      selected_ = lineItems_.size() - 1; 
    else
      --selected_; 
  }

private:
  // Private ctor
    Container(std::string menuName) 
    : selected_(0)
    , name_(menuName)
    , lineItems_()
    , orientation_(Orientation::VERTICAL)
    , x_(0)
    , y_(0)
  {  }

  // Private variables
  size_t selected_;
  std::string name_;
  std::vector<Selectable> lineItems_;
  Orientation orientation_;
  size_t x_;
  size_t y_;
};



//////////////////////////////////////////////////////
// Stack-based menu system. Uses a stack of different menu containers
// to display the most recently navigated to on the top when draw is called.
//////////////////////////////////////////////////////
class MenuSystem
{
private:
  // Pushes a continer to the stack
  void pushContainer(Container *c)
  {
    if (c == nullptr)
    {
      if (stack_.top()->GetSelected().Target == "back")
        stack_.pop();
      else if (stack_.top()->GetSelected().Target == "exit")
        exit(0);
    }
    else
      stack_.push(c);
  }

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

  // Selects the currently highlighted line from the menu on the top of the stack
  void Select() 
  {
    pushContainer(MenuRegistry::GetContainer(stack_.top()->GetSelected().Target));
  }

  // Indicate a specific menu to push via name.
  void Select(std::string manualInput)
  {
    pushContainer(MenuRegistry::GetContainer(manualInput));

  }

  // Goes back. Returns if it did go back or not.
  bool Back() {
    if (stack_.size() > 0)
    {
      stack_.pop();
      return true;
    }
    
    return false;
  }

  // Draws the menu
  void Draw(size_t x = 3, size_t y = 2)
  {
    if (stack_.size() == 0)
      return;

    std::vector<Selectable> &v = stack_.top()->GetAllItems();
    Orientation o = stack_.top()->GetOrientation();
    size_t xPos = stack_.top()->GetXPos();
    size_t yPos = stack_.top()->GetYPos();
    if (o == VERTICAL)
    {
      for (size_t i = 0; i < v.size(); ++i)
      {
        if (i == stack_.top()->GetSelectedLine())
          RConsole::Canvas::DrawString(v[i].Label.c_str(), static_cast<float>(x + xPos), static_cast<float>(i + y + yPos), RConsole::LIGHTGREEN);
        else
          RConsole::Canvas::DrawString(v[i].Label.c_str(), static_cast<float>(x + xPos), static_cast<float>(i + y + yPos), RConsole::CYAN);
      }
    }
    else if (o == HORIZONTAL)
    {
      size_t xOffset = 0;
      for (size_t i = 0; i < v.size(); ++i)
      {
        if (i == stack_.top()->GetSelectedLine())
          RConsole::Canvas::DrawString(v[i].Label.c_str(), static_cast<float>(xOffset + x + xPos), static_cast<float>(y + yPos), RConsole::LIGHTGREEN);
        else
          RConsole::Canvas::DrawString(v[i].Label.c_str(), static_cast<float>(xOffset + x + xPos), static_cast<float>(y + yPos), RConsole::CYAN);
        xOffset += v[i].Label.size();
      }
    }
  }


  // Private variables
  std::stack<Container *> stack_;
};


