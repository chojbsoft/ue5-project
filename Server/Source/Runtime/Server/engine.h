#pragma once

#include "NetworkManager.h"

class Engine
{

public:
	int32 Init();
	void Tick(const float deltaTime);
	void Exit();

public:
	void RequestExit(const string& reason);

public:
	bool _isExitRequested = false;

private:
	NetworkManager* _networkManager = nullptr;
	URL _url;
};

extern Engine engine;