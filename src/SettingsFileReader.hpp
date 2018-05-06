#pragma once
#include <Utils/include/Resource.hpp>
#include "Settings/WindowSettings.h"
#include "Settings/ResourceSettings.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

class SettingsFile : public Resource {
public:
	SettingsFile(const std::string &name) : Resource(name) { this->Load(IOType::text);	}

	~SettingsFile() { this->Unload(); }

	unsigned ID() const { return m_ID; }

	virtual bool Reloadable() const override { return true; }

	virtual std::string Directory() const override { return "../"; }

	void ApplyParsedSettings(const std::vector<std::string>& strings,
		const std::vector<float>& f)
	{
		if (strings.size() == 0) return;
		std::string c = strings[0];

		if (c == "screen_size") {
			// syntax: screen_size width height
			g_InitialWindowWidth = f[1];
			g_InitialWindowHeight = f[2];
		}

		else if (c == "start_fullscreen") {
			// syntax: fullscreen <0 = no, 1 = yes>
			g_StartFullscreen = bool(f[1]);
		}

		//TODO:
		else if (c == "camera") {
			// scene file syntax: camera x y z   ry   <orientation spec>

			// Parameters: (Rx, Ry, eye, orient) where
			// frustum height ratio Ry = f[4]
			// frustum width ratio  Rx = Ry * W / H
			// Eye position = ([1],f[2],f[3])
			// View orientation =  Orientation(5,strings,f) 

			//scene_camera = new RT_Camera(
			//	(f[4] * width / height),
			//	f[4],
			//	Vector3f(f[1], f[2], f[3]),
			//	Orientation(5, strings, f));
		}


		else {
			fprintf(stderr, "\n*********************************************\n");
			fprintf(stderr, "* Unknown command in ini parser: %s\n", c.c_str());
			fprintf(stderr, "*********************************************\n\n");
		}
	}

	bool LoadSettingsINI(void)
	{
		std::stringstream input(Data());
		if (input.fail()) {
			std::cout << "ERROR in LoadSettingsINI: Bad input data. " << std::endl;
			//fflush(stderr);
			return false;
		}

		// For each line in file
		for (std::string line; getline(input, line); ) {
			std::vector<std::string> strings;
			std::vector<float> floats;

			// Parse as parallel lists of strings and floats
			std::stringstream lineStream(line);
			for (std::string s; lineStream >> s; ) { // Parses space-separated strings until EOL
				float f;
				double d;
				//std::stringstream(s) >> f; // Parses an initial float into f, or zero if illegal
				if (!(std::stringstream(s) >> d)) d = nan(""); // An alternate that produced NANs
				f = (float)d;

				floats.push_back(f);
				strings.push_back(s);
			}

			if (strings.size() == 0) continue; // Skip blanks lines
			if (strings[0][0] == '#') continue; // Skip comment lines

												// Pass the line's data to the apply function
			ApplyParsedSettings(strings, floats);
		}

		return true;
	}

protected:
	virtual bool LoadImpl() { return LoadSettingsINI(); }
	virtual void UnloadImpl() { return; }
private:
	unsigned m_ID{ 0 };
};
