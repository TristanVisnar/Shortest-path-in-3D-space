#include "oglwidget.h"

#include "input.h"
#include <QKeyEvent>
#include <QOpenGLShaderProgram>

OGLWidget::OGLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{

}

OGLWidget::~OGLWidget()
{

}

void OGLWidget::toStartStep()
{
    if(!result->isResultView())
        return;

    showSteps = true;
    if(showPath){
        showPath = false;
    }
    currentStep = 0;

    resetColorsToDefault();


}

void OGLWidget::toPreviousStep()
{
    if(!result->isResultView())
        return;

    showSteps = true;

    if(showPath){
        showPath = false;
        resetColorsToDefault();
        refreshColors();
    }
    else{
        if(currentStep == 0)
            currentStep = 0;
        else{
            result->scene.voxelSpace(result->steps[currentStep]).setColorToDefault();
            currentStep -= 1;
            result->scene.voxelSpace(result->steps[currentStep]).setColor(QVector4D(Voxel::ColorProcessNext));
        }
    }
}

void OGLWidget::toNextStep()
{
    if(!result->isResultView())
        return;

    showSteps = true;

    if(currentStep == result->steps.size()-1){
       showPath = true;
       refreshColors();
    }
    else if (showPath){
        showPath = false;
        resetColorsToDefault();
        refreshColors();
    }
    else{
        result->scene.voxelSpace(result->steps[currentStep]).setColor(QVector4D(Voxel::ColorProcessed));
        currentStep += 1;
        result->scene.voxelSpace(result->steps[currentStep]).setColor(QVector4D(Voxel::ColorProcessNext));
    }
}

void OGLWidget::toEndStep()
{
    if(!result->isResultView())
        return;

    showSteps = true;

    currentStep = result->steps.size()-1;
    showPath = true;

    refreshColors();
}

void OGLWidget::showOnlyEndPath()
{
    if(!result->isResultView())
        return;

    showSteps = false;
    showPath = true;

    resetColorsToDefault();
    refreshColors();
}

void OGLWidget::refresh()
{
    resetColorsToDefault();
    refreshColors();
}

void OGLWidget::firstDrawSetup()
{
    if(!result->scene.isSet())
        return;
    refresh();

    // camera setup
    int x = result->scene.voxelSpace.getXSize() / 2;
    int y = result->scene.voxelSpace.getYSize() / 2;
    int z = (x>y)? x:y;

    m_camera.setTranslation(QVector3D(x,y,z+2));
}

void OGLWidget::resetColorsToDefault()
{
    auto data = &result->scene.voxelSpace;
    for(unsigned int x=0; x < data->getXSize(); x++)
        for(unsigned int y=0; y < data->getYSize(); y++)
            for(unsigned int z=0; z < data->getZSize(); z++)
            {
                (*data)(x,y,z).setColorToDefault();
            }
    (*data)(result->scene.startPoint).setColor(QVector4D(Voxel::ColorStartPoint));
    (*data)(result->scene.endPoint).setColor(QVector4D(Voxel::ColorEndPoint));
}

void OGLWidget::refreshColors()
{
    if(!result->isResultView())
        return;

    auto data = &result->scene.voxelSpace;
    if(showSteps){
        for(auto i=0; i<currentStep; i++){
            (*data)(result->steps[i]).setColor(QVector4D(Voxel::ColorProcessed));
        }

        (*data)(result->steps[currentStep]).setColor(QVector4D(Voxel::ColorProcessNext));
    }

    if(showPath){
        for(auto i=0; i<result->paths.last().size(); i++){
            (*data)(result->paths.last()[i]).setColor(QVector4D(Voxel::ColorBestPath));
        }
    }

}


void OGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    //connect(this, SIGNAL(this->frameSwapped()), this, SLOT(update()));
    printContextInformation();


    // Set global information
    //glEnable(GL_CULL_FACE);
    glClearColor(0.53f, 0.81f, 0.92f, 1.0f);


    // Application-specific initialization
    {
      // Create Shader (Do not release until VAO is created)
      m_program = new QOpenGLShaderProgram();
      m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/main.vert");
      m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/main.frag");
      m_program->link();
      m_program->bind();

      // Cache Uniform Locations
      u_modelToWorld = m_program->uniformLocation("modelToWorld");
      u_worldToCamera = m_program->uniformLocation("worldToCamera");
      u_cameraToView = m_program->uniformLocation("cameraToView");
      u_color = m_program->uniformLocation("color");


      // Create Buffer (Do not release until VAO is created)
      m_vertex.create();
      m_vertex.bind();
      m_vertex.setUsagePattern(QOpenGLBuffer::StaticDraw);
      m_vertex.allocate(ObjectBuffers::CubeBuffer, sizeof(ObjectBuffers::CubeBuffer));

      // Create Vertex Array Object
      m_object.create();
      m_object.bind();
      m_program->enableAttributeArray(0);
      m_program->setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);


      // Release (unbind) all
      m_object.release();
      m_vertex.release();
      m_program->release();
    }

}

void OGLWidget::paintGL()
{
    if(!result->isSettingsSet() && !result->scene.isSet())
        return;
    if(!isColorInitialized){
        firstDrawSetup();
        isColorInitialized = true;
    }
    auto data = &result->scene.voxelSpace;
    glClear(GL_COLOR_BUFFER_BIT);


    // Render using our shader
    m_program->bind();
    m_program->setUniformValue(u_worldToCamera, m_camera.toMatrix());
    m_program->setUniformValue(u_cameraToView, m_projection);
    for(unsigned int x=0; x < data->getXSize(); x++)
        for(unsigned int y=0; y < data->getYSize(); y++)
            for(unsigned int z=0; z < data->getZSize(); z++)
                {
                  m_object.bind();
                  QMatrix4x4 model;
                  model.translate(static_cast<float>(x),static_cast<float>(y), static_cast<float>(z));
                  model.scale(0.2);
                  m_program->setUniformValue(u_modelToWorld, model);
                  Voxel* vx = &(*data)(x,y,z);
                  m_program->setUniformValue(u_color, vx->getColor());
                  //glDrawArrays(GL_LINE,0,36);
                  glDrawArrays(GL_TRIANGLES, 0, 36);
                  m_object.release();
                }
    m_program->release();

}


void OGLWidget::update()
{
  // Update input
  Input::update();

  static const float transSpeed = 0.5f;
  static const float rotSpeed   = 0.5f;
  // Camera Transformation
  if (Input::buttonPressed(Qt::RightButton))
  {
    // Handle rotations
    m_camera.rotate(-rotSpeed * Input::mouseDelta().x(), Camera3D::LocalUp);
    m_camera.rotate(-rotSpeed * Input::mouseDelta().y(), m_camera.right());
  }
  // Handle translations
  QVector3D translation;
  if (Input::keyPressed(Qt::Key_W))
  {
    translation += m_camera.forward();
  }
  if (Input::keyPressed(Qt::Key_S))
  {
    translation -= m_camera.forward();
  }
  if (Input::keyPressed(Qt::Key_A))
  {
    translation -= m_camera.right();
  }
  if (Input::keyPressed(Qt::Key_D))
  {
    translation += m_camera.right();
  }
  if (Input::keyPressed(Qt::Key_Q))
  {
    translation -= m_camera.up();
  }
  if (Input::keyPressed(Qt::Key_E))
  {
    translation += m_camera.up();
  }
  m_camera.translate(transSpeed * translation);

  // Update instance information
  m_transform.rotate(1.0f, QVector3D(0.4f, 0.3f, 0.3f));

  // Schedule a redraw
  QOpenGLWidget::update();
}

void OGLWidget::keyPressEvent(QKeyEvent *event)
{
  if (event->isAutoRepeat())
  {
    event->ignore();
  }
  else
  {
    Input::registerKeyPress(event->key());
  }
}

void OGLWidget::keyReleaseEvent(QKeyEvent *event)
{
  if (event->isAutoRepeat())
  {
    event->ignore();
  }
  else
  {
    Input::registerKeyRelease(event->key());
  }
}

void OGLWidget::mousePressEvent(QMouseEvent *event)
{
  Input::registerMousePress(event->button());
}

void OGLWidget::mouseReleaseEvent(QMouseEvent *event)
{
  Input::registerMouseRelease(event->button());
}



void OGLWidget::printContextInformation()
{

}

void OGLWidget::resizeGL(int w, int h)
{
    m_projection.setToIdentity();
    m_projection.perspective(60.0f, w / float(h), 0.1f, 1000.0f);
    //m_projection.ortho(-1.0, 1.0,-1.0, 1.0, 0.0, 1.0);
}
