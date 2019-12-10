#include "include/Render.h"

SectionScanLine *Render::section_scan_line = nullptr;
Object *Render::object = nullptr;
double **Render::RotateMat = nullptr;

Render::Render(Object *ob, SectionScanLine *sectionscanline)
{
    section_scan_line = sectionscanline;
    object = ob;

    RotateMat = new double *[4];
    for (int i = 0; i < 4; ++i)
    {
        RotateMat[i] = new double[4];
        for (int j = 0; j < 4; ++j)
        {
            RotateMat[i][j] = 0.0f;
        }
    }
}

Render::~Render()
{
    for (int i = 0; i < 4; ++i)
    {
        delete[] RotateMat[i];
    }
    delete[] RotateMat;
    RotateMat = nullptr;
}

void Render::loop()
{
    int width = MAX_POLYGON_X, height = MAX_POLYGON_Y;
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width, 0, height);

    section_scan_line->init_classified(*object);
    section_scan_line->SectionScanLineStart();

    glBegin(GL_POINTS);
	
	#pragma omp parallel for
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {

            Color rgb(0.1f, 0.1f, 0.1f);
            int tmp = buffer[y][x];
            if (tmp > 0)
            {
                rgb = section_scan_line->polygons.at(tmp).color;
                //std::cout <<tmp<<" "<< "RGB:" << rgb.r << " " << rgb.g << " " << rgb.b << std::endl;
            }
            glColor3f(rgb.r, rgb.g, rgb.b);
            glVertex2i(x, y);
            buffer[y][x] = 0;
        }
    }
    glEnd();

    glFinish();
}

void Render::display()
{
    int width = MAX_POLYGON_X, height = MAX_POLYGON_Y;
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("ScanLine");
    glutDisplayFunc(loop);
    glutSpecialFunc(keyboard);

    glutMainLoop();
}

void Render::keyboard(int key, int x, int y)
{
    double theta = 0.0;
    double dtheta = M_PI / 12;
    double n1, n2;
    switch (key)
    {
    case GLUT_KEY_LEFT:
        theta = dtheta;
        n1 = 0.0;
        n2 = 0.0;
        break;
    case GLUT_KEY_RIGHT:
        theta = -dtheta;
        n1 = 0.0;
        n2 = 0.0;
        break;
    case GLUT_KEY_UP:
        theta = 0;
        n1 = 1.5;
        n2 = 0.0;
        break;
    case GLUT_KEY_DOWN:
        theta = 0;
        n1 = -0.8;
        n2 = 0.0;
        break;

    default:
        break;
    }

    double cos_theta = cos(theta), sin_theta = sin(theta);
    if (!isEquald(n1,0))
    {
        RotateMat[0][0] = n1;
        RotateMat[0][1] = 0;
        RotateMat[0][2] = 0;
        RotateMat[0][3] = 0;

        RotateMat[1][0] = 0;
        RotateMat[1][1] = n1;
        RotateMat[1][2] = 0;
        RotateMat[1][3] = 0;

        RotateMat[2][0] = 0;
        RotateMat[2][1] = 0;
        RotateMat[2][2] = n1;
        RotateMat[2][3] = 0;

        RotateMat[3][0] = 0;
        RotateMat[3][1] = 0;
        RotateMat[3][2] = 0;
        RotateMat[3][3] = 1;
    }
    else
    {
        RotateMat[0][0] = cos_theta;
        RotateMat[0][1] = 0;
        RotateMat[0][2] = sin_theta;
        RotateMat[0][3] = 0;

        RotateMat[1][0] = 0;
        RotateMat[1][1] = 1;
        RotateMat[1][2] = 0;
        RotateMat[1][3] = 0;

        RotateMat[2][0] = -sin_theta;
        RotateMat[2][1] = 0;
        RotateMat[2][2] = cos_theta;
        RotateMat[2][3] = 0;

        RotateMat[3][0] = 0;
        RotateMat[3][1] = 0;
        RotateMat[3][2] = 0;
        RotateMat[3][3] = 1;
    }
    object->Rotate(RotateMat, object->v_array);
    //section_scan_line->init_classified(*object);
    //section_scan_line->SectionScanLineStart();
    glutPostRedisplay();
}
