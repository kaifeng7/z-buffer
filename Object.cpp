#include "include/Object.h"
Object::Object()
{
    v_num = 0;
    f_num = 0;
    file_path = "";
}

Object::Object(std::string path)
{
    file_path = path;
    v_num = 0;
    f_num = 0;
    std::ifstream ifs(path.c_str());
    if (!ifs.is_open())
    {
        std::cout << "fail to open map\n";
        exit(-1);
    }
    std::string line;

    while (getline(ifs, line))
    {
        if (line[0] == 'v' && line[1] != 'n' && line[1] != 't')
        {
            std::istringstream in(line);
            std::string s1;
            double f2, f3, f4;
            in >> s1 >> f2 >> f3 >> f4;
            Vector3D v;
            v.x = f2*50 + 500;
            v.y = f3*50 + 500; //TODO
            v.z = f4*50;
            v_array.push_back(v);
            v_num++;
        }
        else if (line[0] == 'f')
        {
            std::istringstream in(line);
            std::istringstream in2(line);
            std::string s1;
            int i2, i3, i4;
            int d;      //丢弃
            char c, cc; //丢弃
            in >> s1 >> i2 >> c >> cc;
            if (cc == '/')
            {
                in >> d;
                in >> i3 >> c >> cc >> d;
                in >> i4 >> c >> cc >> d;
            }
            else
            {
                in2 >> s1 >> i2 >> c >> d >> c >> d;
                in2 >> i3 >> c >> d >> c >> d;
                in2 >> i4 >> c >> d >> c >> d;
            }

            std::vector<int> vi;
            vi.push_back(i2 - 1);
            vi.push_back(i3 - 1);
            vi.push_back(i4 - 1);

            f_array.push_back(vi);
            f_num++;
        }
    }
}

Object::~Object() {}

void Object::Rotate(double **RotateMat, std::vector<Vector3D> &v_array)
{
	#pragma omp parallel for
    for (int i = 0; i < v_num; ++i)
    {
        //Point3f* point = &vertexes[i].point;
        Vector3D point; 
        
        v_array[i].x-=500;
        v_array[i].y-=500;
        Vector3D tmp_point = v_array[i];
       
        v_array[i].x = RotateMat[0][0] * tmp_point.x + RotateMat[0][1] * tmp_point.y + RotateMat[0][2] * tmp_point.z+RotateMat[0][3];
        v_array[i].y = RotateMat[1][0] * tmp_point.x + RotateMat[1][1] * tmp_point.y + RotateMat[1][2] * tmp_point.z+RotateMat[1][3];
        v_array[i].z = RotateMat[2][0] * tmp_point.x + RotateMat[2][1] * tmp_point.y + RotateMat[2][2] * tmp_point.z+RotateMat[2][3];

        v_array[i].x+=500;
        v_array[i].y+=500;
    }

    // 顶点法向量
    // int normal_num = normals.size();
    // for (int i = 0; i < normal_num; ++i)
    // {
    // 	Point3f tmp_point = normals[i];
    // 	normals[i].x = RotateMat[0][0] * tmp_point.x + RotateMat[0][1] * tmp_point.y + RotateMat[0][2] * tmp_point.z;
    // 	normals[i].y = RotateMat[1][0] * tmp_point.x + RotateMat[1][1] * tmp_point.y + RotateMat[1][2] * tmp_point.z;
    // 	normals[i].z = RotateMat[2][0] * tmp_point.x + RotateMat[2][1] * tmp_point.y + RotateMat[2][2] * tmp_point.z;

    // }
    // 面的法向量
    // for (int i = 0; i < f_num; ++i)
    // {
    // 	Vector3D tmp_point = faces[i].normal;
    // 	faces[i].normal.x = RotateMat[0][0] * tmp_point.x + RotateMat[0][1] * tmp_point.y + RotateMat[0][2] * tmp_point.z;
    // 	faces[i].normal.y = RotateMat[1][0] * tmp_point.x + RotateMat[1][1] * tmp_point.y + RotateMat[1][2] * tmp_point.z;
    // 	faces[i].normal.z = RotateMat[2][0] * tmp_point.x + RotateMat[2][1] * tmp_point.y + RotateMat[2][2] * tmp_point.z;
    // }
}
