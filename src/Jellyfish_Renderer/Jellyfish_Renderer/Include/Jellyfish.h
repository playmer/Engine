#pragma once
#include <string>

namespace Jellyfish
{
	//basic linker test
	class Derp
	{
	public:
		void DoAThing();
	private:
		std::string str = "hello";
	};

	class RenderableObject
	{

	};


	class Renderer
	{
		void Draw();
	};
}
