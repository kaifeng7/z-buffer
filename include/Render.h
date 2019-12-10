#ifndef __RENDER_H
#define __RENDER_H

#include "ScanLine.h"

class Render
{
public:
	
	Render(Object* object,SectionScanLine* section_scan_line);
	~Render();
	void display();

	static SectionScanLine* section_scan_line;
    static Object* object;
	static double** RotateMat;
	
	static void loop();
	static void keyboard(int key, int x, int y);

};

#endif