int PAINT_X;
int PAINT_Y;
double PAINT_SZX, PAINT_SZY;
double mouse_wasX, mouse_wasY;
const double PAINT_SZ = 1e5;
Image PAINT_im;
int PAINT_PAINT;
double PAINT_mouseX = 0, PAINT_mouseY = 0;

void PAINT_printInfo() {
    glColor3f(1, 0, 0);
    glRasterPos2f(3, 3);
    string s = to_string(PAINT_mouseX) + "   " + to_string(PAINT_mouseY);
    for (int i = 0; i < int(s.size()); ++i) {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, s[i]);
    }
}

void PAINT_Draw() {
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glRotatef(-90, 0, 0, 1);
    glTranslatef(-PAINT_X, 0, 0);

    for (int i = 0; i < PAINT_X; ++i) {
        for (int j = 0; j < PAINT_Y; ++j) {
            glBegin(GL_POLYGON);
                glColor3f(double(PAINT_im[i][j][0]) / 255, double(PAINT_im[i][j][1]) / 255, double(PAINT_im[i][j][2]) / 255);
                glVertex2f(i, j);
                glVertex2f(i + 1, j);
                glVertex2f(i + 1, j + 1);
                glVertex2f(i, j + 1);
            glEnd();
        }
    }
    glBegin(GL_POLYGON);
        glColor3f(0, 0, 0);
        glVertex2f(PAINT_mouseX - 0.3, PAINT_mouseY      );
        glVertex2f(PAINT_mouseX      , PAINT_mouseY - 0.3);
        glVertex2f(PAINT_mouseX + 0.3, PAINT_mouseY      );
        glVertex2f(PAINT_mouseX      , PAINT_mouseY + 0.3);
    glEnd();

    // PAINT_printInfo();
    glFlush();
    glutSwapBuffers();
}

void PAINT_Timer(int value) {
    PAINT_Draw();
    glutTimerFunc(0, PAINT_Timer, value);
}

void PAINT_mouseMove(int x, int y) {
	PAINT_mouseX = min(PAINT_X - 0.1, max(0.1, double(y) / PAINT_SZX * PAINT_X));
    PAINT_mouseY = min(PAINT_Y - 0.1, max(0.1, double(x) / PAINT_SZY * PAINT_Y));
    if (PAINT_PAINT) {
        double plusX = (PAINT_mouseX - mouse_wasX) / 10;
        double plusY = (PAINT_mouseY - mouse_wasY) / 10;
        for (int i = 0; i <= 10; ++i) {
            PAINT_im[int(PAINT_mouseX - plusX * i)][int(PAINT_mouseY - plusY * i)] = vector<int>(3, 255 * int(PAINT_PAINT == 2));
        }
    }
    mouse_wasX = PAINT_mouseX;
    mouse_wasY = PAINT_mouseY;
}

void PAINT_mouseButton(int button, int state, int x, int y) {
    PAINT_mouseX = min(PAINT_X - 0.1, max(0.1, double(y) / PAINT_SZX * PAINT_X));
    PAINT_mouseY = min(PAINT_Y - 0.1, max(0.1, double(x) / PAINT_SZY * PAINT_Y));
    if (button == GLUT_RIGHT_BUTTON) {
        if (state == GLUT_DOWN) {
            PAINT_PAINT = 2;
            mouse_wasX = PAINT_mouseX;
            mouse_wasY = PAINT_mouseY;
        } else {
            PAINT_PAINT = 0;
        }
    } else if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            PAINT_PAINT = 1;
            mouse_wasX = PAINT_mouseX;
            mouse_wasY = PAINT_mouseY;
        } else {
            PAINT_PAINT = 0;
        }
    }
    if (PAINT_PAINT) {
        PAINT_im[int(PAINT_mouseX)][int(PAINT_mouseY)] = vector<int>(3, 255 * int(PAINT_PAINT == 2));
    }
}

void PAINT_Initialize() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, PAINT_Y, 0, PAINT_X, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

void Image::paint(int d) {
    PAINT_X = d;
    PAINT_Y = d;
    PAINT_SZX = sqrtl((PAINT_SZ) / (PAINT_X * PAINT_Y)) * PAINT_X;
    PAINT_SZY = sqrtl((PAINT_SZ) / (PAINT_X * PAINT_Y)) * PAINT_Y;
    int c = 0;
    glutInit(&c, nullptr);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(PAINT_SZX, PAINT_SZY);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("PAINT TOOL");
    glutDisplayFunc(PAINT_Draw);
    glutTimerFunc(50, PAINT_Timer, 0);

    glutMouseFunc(PAINT_mouseButton);
    glutMotionFunc(PAINT_mouseMove);
    glutPassiveMotionFunc(PAINT_mouseMove);
    
    PAINT_Initialize();

    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

    PAINT_im = Image(PAINT_X, PAINT_Y, 3, vector<int>(3, 255));

    glutMainLoop();

    (*this) = PAINT_im;
}

void Image::show() {
    PAINT_X = n();
    PAINT_Y = m();
    PAINT_SZX = sqrtl((PAINT_SZ) / (PAINT_X * PAINT_Y)) * PAINT_X;
    PAINT_SZY = sqrtl((PAINT_SZ) / (PAINT_X * PAINT_Y)) * PAINT_Y;
    int c = 0;
    glutInit(&c, nullptr);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(PAINT_SZX, PAINT_SZY);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("PAINT TOOL");
    glutDisplayFunc(PAINT_Draw);
    glutTimerFunc(50, PAINT_Timer, 0);

    PAINT_Initialize();

    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

    PAINT_im = (*this);
    glutMainLoop();
}