#pragma once
#include "Application.hpp"
#include "GuiManager.hpp"

namespace av
{
	class EditorApplication : public Application 
	{
	private:
		GuiManager gui;
	public:
		void AppInit() override;

		void HandleEvents() override;

		void Process() override;

		void Draw() override;
	};
}