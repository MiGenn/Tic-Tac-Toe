#include "WindowUtilities.h"
#include "commctrl.h"
#include <stdexcept>
#include <unordered_map>

LRESULT HandleMouseEnterOrLeave(HWND handle, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR, DWORD_PTR);
void TrackMouseForWindow(HWND window);
void StopTrackMouseForWindow(HWND window);

using EventHandler = std::function<void(bool)>;

std::unordered_map<HWND, std::pair<bool, EventHandler>> hoveringStateAndEventHandlerByWindow;

void SubscribeOnMouseLeaveOrEnterEvent(HWND targetWindow, std::function<void(bool)>&& eventHandler)
{
	if (!SetWindowSubclass(targetWindow, HandleMouseEnterOrLeave, NULL, NULL))
		throw std::runtime_error("Cannot subscribe on mouse leave or enter event");

	hoveringStateAndEventHandlerByWindow.insert_or_assign(targetWindow, 
		std::pair<bool, EventHandler>(false, std::move(eventHandler)));
}

void UnsubscribeFromMouseLeaveOrEnterEvent(HWND targetWindow)
{
	if (!RemoveWindowSubclass(targetWindow, HandleMouseEnterOrLeave, NULL))
		throw std::runtime_error("Cannot unsubscribe from mouse leave or enter event");

	hoveringStateAndEventHandlerByWindow.erase(targetWindow);
}

LRESULT HandleMouseEnterOrLeave(HWND window, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR, DWORD_PTR)
{
	if (message == WM_MOUSEMOVE)
	{
		TrackMouseForWindow(window);

		auto& [isMouseEnteredWindow, eventHandler] { hoveringStateAndEventHandlerByWindow.find(window)->second };
		if (!isMouseEnteredWindow)
		{
			isMouseEnteredWindow = true;
			eventHandler(isMouseEnteredWindow);
		}
	}
	else if (message == WM_MOUSELEAVE)
	{
		StopTrackMouseForWindow(window);

		auto& [isMouseEnteredWindow, eventHandler] { hoveringStateAndEventHandlerByWindow.find(window)->second };
		isMouseEnteredWindow = false;
		eventHandler(isMouseEnteredWindow);
	}

	return DefSubclassProc(window, message, wParam, lParam);
}

void TrackMouseForWindow(HWND window)
{
	TRACKMOUSEEVENT trackMouseEvent{ sizeof(TRACKMOUSEEVENT) };
	trackMouseEvent.dwFlags = TME_LEAVE;
	trackMouseEvent.hwndTrack = window;
	TrackMouseEvent(&trackMouseEvent);
}

void StopTrackMouseForWindow(HWND window)
{
	TRACKMOUSEEVENT trackMouseEvent{ sizeof(TRACKMOUSEEVENT) };
	trackMouseEvent.dwFlags = TME_LEAVE | TME_CANCEL;
	trackMouseEvent.hwndTrack = window;
	TrackMouseEvent(&trackMouseEvent);
}
