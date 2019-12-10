#include "include/Render.h"

int main(int argc, char *argv[])
{
    std::string path = "Example02.obj";
    Object ob(path);
    std::cout << ob.v_num << " " << ob.f_num;
    // for (int i = 0; i < ob.v_num; i++)
    // {
    //     std::cout << ob.f_array.at(i).at(0) << " " << ob.f_array.at(i).at(1) << " " << ob.f_array.at(i).at(2) << std::endl;
    // }

    SectionScanLine section_scan_line;    
    section_scan_line.init_classified(ob);
    section_scan_line.SectionScanLineStart();
 
    glutInit(&argc, argv);
    Render render(&ob, &section_scan_line);
    render.display();   

    return 0;
}
