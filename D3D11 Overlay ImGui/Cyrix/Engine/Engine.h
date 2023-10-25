#pragma once
#include "../../pch.h"
#include "../Offsets/offsets.h"

class CS2 {
private:
	CEntity CreateEntity(uintptr_t*, uintptr_t*);
	void CreateLocalPlayer(uintptr_t*, uintptr_t*);
public:
	uintptr_t ClientBase;
	uintptr_t Engine2Base;
	void InitEntry();
	struct Utils {
		bool isVisible(CEntity entity);
	};
	struct Player {
		struct View {
			ViewMatrix GetViewMatrix();
		};
		CEntity LocalPlayer;
		View View;
	};
	struct Server {
		struct World {
			std::vector<CEntity> EntityList();
		};
		struct CSPlayerController {
			uintptr_t GetPawnAddress(DWORD);
		};
		CSPlayerController PlayerController;
		World World;
	};
	struct Screen {
		Resolution GetResolution();
	};
	Screen Screen;
	Player Player;
	Server Server;
	Utils Utils;
	GameEntry Entry;
	CGameEntitySystem EntitySystem;
};

extern CS2 engine;