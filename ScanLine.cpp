#include "include/ScanLine.h"
int buffer[MAX_POLYGON_Y][MAX_POLYGON_X];
SectionScanLine::SectionScanLine()
{
    for (int i = 0; i < MAX_POLYGON_Y; i++)
    {
        head_classified_polygon[i] = new ClassifiedPolygon;
        head_classified_polygon[i]->next = nullptr;
        head_classified_edge[i] = new ClassifiedEdge;
        head_classified_edge[i]->next = nullptr;
    }
    head_active_polygon = new ClassifiedPolygon;
    head_active_polygon->next = nullptr;
    head_active_edge = new ClassifiedEdge;
    head_active_edge->next = nullptr;
}
SectionScanLine::~SectionScanLine()
{
    for (int i = 0; i < MAX_POLYGON_Y; i++)
    {
        ClassifiedPolygon *pCP = head_classified_polygon[i]->next;
        ClassifiedPolygon *ptemp;

        while (pCP)
        {
            ptemp = pCP->next;
            delete pCP;
            pCP = ptemp;
        }
        head_classified_polygon[i]->next = nullptr;

        ClassifiedEdge *pCE = head_classified_edge[i]->next;
        ClassifiedEdge *etemp;

        while (pCE)
        {
            etemp = pCE->next;
            delete pCE;
            pCE = etemp;
        }
        head_classified_edge[i]->next = nullptr;
    }

    ClassifiedPolygon *pAP = head_active_polygon->next;
    ClassifiedPolygon *ptemp;

    while (pAP)
    {
        ptemp = pAP->next;
        delete pAP;
        pAP = ptemp;
    }
    head_active_polygon->next = nullptr;

    ClassifiedEdge *pAE = head_active_edge->next;
    ClassifiedEdge *etemp;

    while (pAE)
    {
        etemp = pAE->next;
        delete pAE;
        pAE = etemp;
    }
    head_active_edge->next = nullptr;
}

void SectionScanLine::init_classified(Object ob)
{
    for (int i = 0; i < ob.f_num; ++i)
    {

        ClassifiedPolygon *cp = new ClassifiedPolygon;
        Polygon polygon;
        Vector3D v[3];
        Vector3D v1(ob.v_array.at(ob.f_array.at(i).at(0)));
        Vector3D v2(ob.v_array.at(ob.f_array.at(i).at(1)));
        Vector3D v3(ob.v_array.at(ob.f_array.at(i).at(2)));
        v[0] = v1;
        v[1] = v2;
        v[2] = v3;
        Vector3D sub1 = v[1] - v[0];
        Vector3D sub2 = v[2] - v[0];

        polygon.a = sub1.y * sub2.z + sub2.y - sub1.z;
        polygon.b = 0.0 - (sub1.x * sub2.z - sub2.x * sub1.z);
        polygon.c = sub1.x * sub2.y - sub2.x * sub1.y;
        polygon.a = polygon.a / (polygon.a + polygon.b + polygon.c);
        polygon.b = polygon.b / (polygon.a + polygon.b + polygon.c);
        polygon.c = polygon.c / (polygon.a + polygon.b + polygon.c);

        polygon.d = 0.0 - polygon.a * v[0].x - polygon.b * v[0].y - polygon.c * v[0].z;

        int min_y, max_y;
        max_y = round(max(max(v[0].y, v[1].y), v[2].y));
        min_y = round(min(min(v[0].y, v[1].y), v[2].y));
        polygon.y_min = min_y;
        polygon.flag = false;
        polygons.push_back(polygon);

        cp->y_min = min_y;
        cp->id = i;
        Color white;
        white.r = polygon.a;
        white.g = polygon.b;
        white.b = polygon.c;

        polygon.color = white;
        cp->next = nullptr;
        ClassifiedPolygon *pCP = head_classified_polygon[max_y];
        // std::cout << std::endl
        //           << v[0].y << " " << v[1].y << " " << v[2].y << " " << max_y;
        while (pCP->next)
        {
            pCP = pCP->next;
            //printf("*");
        }
        pCP->next = cp;

        for (int j = 0; j < 3; j++)
        {
            int max_edge_y, min_edge_y;
            double max_z;
            ClassifiedEdge *ce = new ClassifiedEdge;
            if (v[j].y > v[(j + 1) % 3].y)
            {
                ce->x = v[j].x;
                max_edge_y = round(v[j].y);
                min_edge_y = round(v[(j + 1) % 3].y);
                max_z = v[j].z;
            }
            else
            {
                ce->x = v[(j + 1) % 3].x;
                max_edge_y = round(v[(j + 1) % 3].y);
                min_edge_y = round(v[j].y);
                max_z = v[(j + 1) % 3].z;
            }
            if (isEquald(v[j].y, v[(j + 1) % 3].y))
            {
                continue;
            }
            else
            {
                ce->dx = -(v[j].x - v[(j + 1) % 3].x) / (v[j].y - v[(j + 1) % 3].y);
            }

            if (min_edge_y >= max_edge_y)
                continue;

            ce->y_min = min_edge_y;
            ce->id = cp->id;

            if (isEquald(polygon.c, 0))
            {
                ce->z = 0;
                ce->dzx = 0;
                ce->dzy = 0;
            }
            else
            {
                ce->z = -(polygon.d + polygon.a * ce->x + polygon.b * max_edge_y) / polygon.c;
                ce->dzx = -(polygon.a / polygon.c);
                ce->dzy = polygon.b / polygon.c;
            }
            ce->next = nullptr;

            ClassifiedEdge *pCE = head_classified_edge[max_edge_y];
            // std::cout << std::endl
            //           << max_edge_y;
            while (pCE->next)
            {
                pCE = pCE->next;
                //printf("*");
            }
            pCE->next = ce;
        }
    }
}

void SectionScanLine::update_active(int y)
{

    if (head_classified_polygon[y]->next)
    {
        ClassifiedPolygon *pAP = head_active_polygon;
        while (pAP->next)
            pAP = pAP->next;
        pAP->next = head_classified_polygon[y]->next;
        head_classified_polygon[y]->next = nullptr;
    }
    ClassifiedPolygon *pAP = head_active_polygon;
    while (pAP->next)
    {
        if (pAP->next->y_min >= y)
        {
            ClassifiedPolygon *pnAP = pAP->next;
            pAP->next = pnAP->next;
            delete pnAP;
            pnAP = nullptr;
        }
        else
            pAP = pAP->next;
    }

    if (head_classified_edge[y]->next)
    {
        ClassifiedEdge *pAE = head_active_edge;
        while (pAE->next)
            pAE = pAE->next;
        pAE->next = head_classified_edge[y]->next;
        head_classified_edge[y]->next = nullptr;
    }
    ClassifiedEdge *pAE = head_active_edge;

    while (pAE->next)
    {
        pAE->next->x += pAE->next->dx;
        //std::cout << y << " " << pAE->next->x <<" "<<pAE->next->id<<std::endl;
        pAE->next->z += pAE->next->dx * pAE->next->dzx + pAE->next->dzy;
        if (pAE->next->y_min >= y)
        {
            ClassifiedEdge *pnAE = pAE->next;
            pAE->next = pnAE->next;
            delete pnAE;
            pnAE = nullptr;
        }
        else
            pAE = pAE->next;
    }

    head_active_edge = sort(head_active_edge, y);
    pAE = head_active_edge;
}

ClassifiedEdge *SectionScanLine::sort(ClassifiedEdge *head, int y)
{
    if (head == nullptr || head->next == nullptr)
        return head;

    ClassifiedEdge *pAE = head->next->next;
    ClassifiedEdge *plAE = head->next;
    ClassifiedEdge *pheadAE = head;
    while (pAE)
    {
        pheadAE = head;
        while (pAE != pheadAE->next)
        {
            //std::cout << y << " " << pAE->x << " " << pheadAE->next->x << std::endl;
            if (pAE->x < pheadAE->next->x)
            {
                plAE->next = pAE->next;
                pAE->next = pheadAE->next;
                pheadAE->next = pAE;
            }
            else if (round(pAE->x) == round(pheadAE->next->x) && round(pAE->dx) > round(pheadAE->next->dx))
            {
                plAE->next = pAE->next;
                pAE->next = pheadAE->next;
                pheadAE->next = pAE;
            }
            else
            {
                pheadAE = pheadAE->next;
            }
        }
        if (plAE->next == pAE)
        {
            pAE = pAE->next;
            plAE = plAE->next;
        }
        else
            pAE = plAE->next;
    }
    return head;
}

int SectionScanLine::PolygonLong(ClassifiedPolygon *head)
{
    int sum = 0;
    ClassifiedPolygon *p = head;
    while (p->next)
    {
        sum++;
        p = p->next;
    }
    return sum;
}

int SectionScanLine::EdgeLong(ClassifiedEdge *head)
{
    int sum = 0;
    ClassifiedEdge *p = head;
    while (p->next)
    {
        sum++;
        p = p->next;
    }
    return sum;
}

void SectionScanLine::SectionScanLineStart()
{
    for (int y = MAX_POLYGON_Y - 1; y >= 0; y--)
    {
        update_active(y);
        //std::cout<<PolygonLong(head_active_polygon)<<" "<<EdgeLong(head_active_edge)<<std::endl;
        ClassifiedPolygon *pAP = head_active_polygon;
        // while (pAP->next)
        // {
        //     //std::cout<<y<<" plon "<<std::endl;
        //     polygons.at(pAP->next->id).flag = false;
        //     pAP = pAP->next;
        // }

        ClassifiedEdge *pAE = head_active_edge;

        while (pAE->next)
        {
            ClassifiedEdge *pnAE = pAE->next;
            pAP = head_active_polygon;
            double max = -999999;
            int max_i = -1;
            //std::cout << y << " " << pAE->next->id << std::endl;
            if (pnAE->next)
            {

                //std::cout<<y<<" "<<pAE->next->id<<" "<<pAP->next->id<<std::endl;
                polygons.at(pAP->next->id).flag = ~polygons.at(pAP->next->id).flag;

                if (polygons.at(pAP->next->id).flag == true)
                {
                    double zz;
                    if (isEquald(polygons.at(pAP->next->id).c, 0))
                    {
                        zz = 0;
                    }
                    else
                    {
                        zz = -(polygons.at(pAP->next->id).d + polygons.at(pAP->next->id).a * ((pAE->next->x + pnAE->next->x) / 2) +
                               polygons.at(pAP->next->id).b * y) /
                             polygons.at(pAP->next->id).c;
                    }
                    if (zz > max)
                    {
                        max = zz;
                        max_i = pAE->next->id;
                    }
                }
                pAP = pAP->next;
                for (int i = round(pAE->next->x); i < round(pnAE->next->x); i++)
                {
                    buffer[y][i] = max_i;
                    //std::cout<<y<<" "<<i<<" "<<max_i<<std::endl;
                }
            }
            else
            {
                polygons.at(pAP->next->id).flag == false;
            }
            pAE = pAE->next;
        }
    }
}
