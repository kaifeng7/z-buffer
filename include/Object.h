
#ifndef __OBJECT_H
#define __OBJECT_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "Vector.h"

class Object
{
  public:
    int v_num;
    int f_num;
    std::vector<Vector3D> v_array;
    std::vector<std::vector<int> > f_array;
    std::string file_path;

    Object();
    Object(std::string path);

    ~Object();    
    void Rotate(double** RotateMat, std::vector<Vector3D>& v_array);

};



#endif
