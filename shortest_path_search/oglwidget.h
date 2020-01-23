#ifndef OGLWIDGET_H
#define OGLWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include "objectbuffers.h"
#include "vector3d.h"
#include "camera3d.h"
#include "result.h"
#include "transform3d.h"

class QOpenGLShaderProgram;

class OGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    public:
        OGLWidget(QWidget *parent = 0);
        ~OGLWidget();

        Result* result;

        void toStartStep();
        void toPreviousStep();
        void toNextStep();
        void toEndStep();
        void showOnlyEndPath();

        void refresh();
        void firstDrawSetup();

        void keyPressEvent(QKeyEvent *event);
        void keyReleaseEvent(QKeyEvent *event);
        void mousePressEvent(QMouseEvent *event);
        void mouseReleaseEvent(QMouseEvent *event);

        unsigned int currentStep = 0;
    private:
        bool showSteps = true;
        bool showPath = false;
        bool isColorInitialized = false;

        void resetBooleans();

        void resetColorsToDefault();
        void refreshColors();

        void initializeGL();
        void resizeGL(int w, int h);
        void paintGL();



        // OpenGL State Information
        QOpenGLBuffer m_vertex;
        QOpenGLVertexArrayObject m_object;
        QOpenGLShaderProgram *m_program;

        // Shader Information
        int u_modelToWorld;
        int u_worldToCamera;
        int u_cameraToView;
        int u_color;
        QMatrix4x4 m_projection;
        Camera3D m_camera;
        Transform3D m_transform;

        // Prints out OpenGL Context for debugging purposes
        void printContextInformation();

    protected slots:
        void update();

};



#endif // OGLWIDGET_H
