#pragma once
#include "WinapiException.h"
#include <functional>

void SubscribeOnMouseLeaveOrEnterEvent(HWND targetWindow, std::function<void(bool)>&& eventHandler);
void UnsubscribeFromMouseLeaveOrEnterEvent(HWND targetWindow);
