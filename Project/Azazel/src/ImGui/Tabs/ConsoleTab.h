#pragma once

class ConsoleTab
{
public:
	ConsoleTab() = default;
	~ConsoleTab() = default;

	void Update();
	void OpenTab();

private:
	bool myTabOpen = true;
};

