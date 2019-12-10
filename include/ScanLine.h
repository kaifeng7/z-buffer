#ifndef __SCANLINE_H
#define __SCANLINE_H

#include <stdlib.h>
#include <GL/glut.h>

#include "Object.h"


struct ClassifiedPolygon
{
    int id;            //多边形的编号
    int y_min;         
    ClassifiedPolygon *next;
};

struct ClassifiedEdge
{
    double x; //边的上端点的x坐标

    int y_min; //边的下端点的y坐标
    int id;    //边所属多边形的编号
    double dx; //相邻两条扫描线交点的x坐标差(-1/k)
    double z;
    double dzx; //dzx = -a/c
    double dzy; //dzy = b/c
    ClassifiedEdge *next;
};

struct Polygon
{
    double a,b,c,d;//多边形所在平面的方程系数
    int y_min;  //多边形跨越的最后一条扫描线的y
    bool flag;    
    Color color;       //多边形颜色
};

class SectionScanLine
{
  public:
    ClassifiedPolygon *head_classified_polygon[MAX_POLYGON_Y];
    ClassifiedEdge *head_classified_edge[MAX_POLYGON_Y];
    ClassifiedPolygon *head_active_polygon;
    ClassifiedEdge *head_active_edge;
    std::vector<Polygon> polygons;


    void init_classified(Object ob);
    void update_active(int y);
    ClassifiedEdge *sort(ClassifiedEdge *head, int y);
    int PolygonLong(ClassifiedPolygon *head);
    int EdgeLong(ClassifiedEdge *head);

    void SectionScanLineStart();
    SectionScanLine();
    ~SectionScanLine();
};

extern int buffer[MAX_POLYGON_Y][MAX_POLYGON_X];
#endif
