#include "operation_handler.h"
#include <iostream>
#include "static_view.h"

using namespace std;
using namespace pcl::visualization;
using namespace std::chrono;

namespace ce30_pcviz {
OperationHandler::OperationHandler(shared_ptr<PCLVisualizer> viz)
  : viz_(viz)
{
  viz_->registerKeyboardCallback(
        boost::bind(&OperationHandler::HandleKeyboardEvent, this, _1));
  viz_->registerMouseCallback(
        boost::bind(&OperationHandler::HandleMouseEvent, this, _1));
  aerial_view_.reset(
      new StaticView(viz_, -10.0f, 0.0f, 5.0f, 10.0f, 0.0f, 0.0f));
  vertical_view_.reset(
      new StaticView(viz_, 10.0f, 0.0f, 30.0f, 11.0f, 0.0f, 0.0f));
  AddShortcut(
      {"1", [this](){aerial_view_->Change();}, "Switch to Aerial View"});
  AddShortcut(
      {"2", [this](){vertical_view_->Change();}, "Switch to Vertical View"});
}

void OperationHandler::HandleMouseEvent(const MouseEvent &event) {

}

void OperationHandler::HandleKeyboardEvent(const KeyboardEvent &event) {
  auto this_tap = high_resolution_clock::now();
  if (duration_cast<milliseconds>(this_tap - last_tap_time_).count() < 100) {
    last_tap_time_ = this_tap;
    return;
  }
  last_tap_time_ = this_tap;

  for (auto& shortcut : ctrl_shortcuts_) {
    if (event.isCtrlPressed() && event.getKeySym() == shortcut.key) {
      shortcut.callback();
    }
  }
}

void OperationHandler::AddShortcut(const CtrlShortcut &shortcut) {
  ctrl_shortcuts_.push_back(shortcut);
}

vector<pair<string, string>> OperationHandler::CtrlShortcutMap() {
  vector<pair<string, string>> map;
  for (auto& shortcut : ctrl_shortcuts_) {
    map.push_back({shortcut.key, shortcut.description});
  }
  return map;
}

void OperationHandler::PrintShortcuts() {
  cout << "Shortcuts:" << endl;
  for (auto& shortcut : ctrl_shortcuts_) {
    cout
        << "  * Ctrl+'" << shortcut.key
        << "' -- " << shortcut.description << endl;
  }
}

void OperationHandler::UseAerialView() {
  aerial_view_->Change();
}

void OperationHandler::UseVerticalView() {
  vertical_view_->Change();
}
}
