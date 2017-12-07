#include <glad/glad.h> // MUST be included BEFORE glfw3.h
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#include <array>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GLProgram.hpp"
#include "Texture.hpp"
#include "Camera.hpp"
#include "GLFWWindow.hpp"
#include "Renderer.hpp"
#include "Model.hpp"
Camera cam;
double dt{ 0.f };
double lastFrame{ 0.f };

void UnrecoverableError( const char* msg ) {
  std::cout << "An unrecoverable error has occurred:\n";
  std::cout << msg << std::endl;
  system( "pause" );
  exit( -1 );
}

void ProcessInput( GLFWWindow &window ) {
  if( glfwGetKey( window, GLFW_KEY_ESCAPE ) == GLFW_PRESS ) {
    glfwSetWindowShouldClose( window, true );
  }
  if( glfwGetKey( window, GLFW_KEY_1 ) == GLFW_PRESS ) {
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
  }
  if( glfwGetKey( window, GLFW_KEY_2 ) == GLFW_PRESS ) {
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    glLineWidth( 1.5f );
  }

  if( glfwGetKey( window, GLFW_KEY_3 ) == GLFW_PRESS ) {
    glPolygonMode( GL_FRONT_AND_BACK, GL_POINT );
  }
  float camSpeed{ 5.f * ( float )dt };
  if( glfwGetKey( window, GLFW_KEY_W ) == GLFW_PRESS )
    cam.position += camSpeed * cam.Front();
  if( glfwGetKey( window, GLFW_KEY_S ) == GLFW_PRESS )
    cam.position -= camSpeed * cam.Front();
  if( glfwGetKey( window, GLFW_KEY_A ) == GLFW_PRESS )
    cam.position -= cam.Right() * camSpeed;
  if( glfwGetKey( window, GLFW_KEY_D ) == GLFW_PRESS )
    cam.position += cam.Right() * camSpeed;
  if( glfwGetKey( window, GLFW_KEY_SPACE ) == GLFW_PRESS ) {
    cam.LookAt( { 0.f,0.f,0.f } );
  }

}

float vertices[ ] = {
  -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
  0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
  0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
  0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
  -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
  -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

  -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
  0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
  0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
  0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
  -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
  -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

  -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
  -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
  -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
  -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
  -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
  -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

  0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
  0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
  0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
  0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
  0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
  0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

  -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
  0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
  0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
  0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
  -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
  -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

  -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
  0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
  0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
  0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
  -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
  -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};
glm::vec3 cubePositions[ ] = {
  glm::vec3( 0.0f,  0.0f,  0.0f ),
  glm::vec3( 2.0f,  5.0f, -15.0f ),
  glm::vec3( -1.5f, -2.2f, -2.5f ),
  glm::vec3( -3.8f, -2.0f, -12.3f ),
  glm::vec3( 2.4f, -0.4f, -3.5f ),
  glm::vec3( -1.7f,  3.0f, -7.5f ),
  glm::vec3( 1.3f, -2.0f, -2.5f ),
  glm::vec3( 1.5f,  2.0f, -2.5f ),
  glm::vec3( 1.5f,  0.2f, -1.5f ),
  glm::vec3( -1.3f,  1.0f, -1.5f )
};

std::string ReadFile( const std::string& path ) {
  std::ifstream infile( path );
  std::string result;
  result.reserve( 1024 );
  std::array<char, 256> line;
  while( infile.good( ) && !infile.eof( ) ) {
    infile.getline( line.data( ), 256 );
    result += line.data( );
    result += "\n";
  }
  return result;
}

std::string RelativePath( ) {
  static std::string path{ "../" };
  return path;
}
std::string ShaderPath( ) {
  static std::string path{ RelativePath( ) + "Resources/Shaders/" };
  return path;
}

void MouseCallback( const GLFWWindow::EMouseMoved &mme ) {
  float sensitivity{ .05f };
  const float xoff{ sensitivity * ( float )( mme.newPosition.x - mme.oldPosition.x ) };
  const float yoff{ sensitivity * ( float )( mme.newPosition.y - mme.oldPosition.y ) };
  cam.yaw += xoff;
  cam.pitch -= yoff;
  if( cam.pitch > 89.9f ) cam.pitch = 89.9f;
  if( cam.pitch < -89.9f ) cam.pitch = -89.9f;
}

void ScrollCallback( const GLFWWindow::EMouseScrolled &mse) {
  if( cam.fov >= 1.0f && cam.fov <= 70.0f )
    cam.fov -= ( float )mse.offset.y;
  if( cam.fov <= 1.0f )
    cam.fov = 1.0f;
  if( cam.fov >= 70.0f )
    cam.fov = 70.0f;
}

void UpdateTime( ) {
  double currentFrame = glfwGetTime( );
  dt = currentFrame - lastFrame;
  unsigned FPS{ static_cast< unsigned >( 1. / dt ) };
  lastFrame = currentFrame;
}

int NanosuitDemo( ) {
  
  GLFWWindow window;
  window.MouseMode( CursorMode::disabled );
  window.On<GLWindow::EMouseMoved>( MouseCallback );
  window.On<GLWindow::EMouseScrolled>( ScrollCallback );
  glm::mat4 projection;

  glEnable( GL_DEPTH_TEST );
  GLTextRenderer gltr( "Text.sprog" );
  GLProgram modelShader( "Model.sprog" );
  Model nanosuit( "nanosuit.obj" );

  while( window.State( ) != WindowState::closed ) {
    double currentFrame = glfwGetTime( );
    dt = currentFrame - lastFrame;
    unsigned FPS{ static_cast< unsigned >( 1. / dt ) };
    lastFrame = currentFrame;

    ProcessInput( window );
    if( window.KeyPressed( GLFW_KEY_F5 ) == GLFW_PRESS ) {
      modelShader.Reload( );
      nanosuit.Reload( );
    }
    // Handle Rendering
    glClearColor( .3f, 0.2f, .3f, 1.f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    // Drawing code (belongs in render loop)
    projection = glm::perspective( glm::radians( cam.fov ), 800.f / 600.f,cam.nearplane, cam.farplane );
    modelShader.SetUniform( "projection", projection );
    modelShader.SetUniform( "view", cam.View() );
    
    glm::mat4 model;
    model = glm::translate( model, glm::vec3( .0f, -1.75f, .0f ) );
    model = glm::scale( model, { .2f, .2f, .2f } );
    modelShader.SetUniform( "model", model );
    nanosuit.Draw( modelShader );

    gltr.Render("FPS: " + std::to_string( FPS ), { 0.f,0.f }, { .5f,.8f,.2f } );

    // check and call events and swap the buffers
    glfwSwapBuffers( window );
    glfwPollEvents( );
  }


  glfwTerminate( );
  return 0;
}

int SaturnDemo( ) {
  GLFWWindow window;
  window.MouseMode( CursorMode::disabled );
  window.On<GLWindow::EMouseMoved>( MouseCallback );
  window.On<GLWindow::EMouseScrolled>( ScrollCallback );
  glm::mat4 projection;

  glEnable( GL_DEPTH_TEST );
  GLTextRenderer gltr( "Text.sprog" );

  while( window.State( ) != WindowState::closed ) {
    UpdateTime( );

    ProcessInput( window );
    if( window.KeyPressed( GLFW_KEY_F5 ) ) {
    }
    // Handle Rendering
    glClearColor( .3f, 0.2f, .3f, 1.f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    // Drawing code (belongs in render loop)

    glm::mat4 model;



    gltr.Render( "FPS: " + std::to_string( (int)(1./dt) ), { 0.f,0.f }, { .5f,.8f,.2f } );

    // check and call events and swap the buffers
    glfwSwapBuffers( window );
    glfwPollEvents( );
  }


  glfwTerminate( );
  return 0;
}

#include "Detectors.hpp"

#include "EntitiesWith.hpp"

struct somegameplayshit {
  EntitiesWith<int, bool, float> Entities;
  void Update( ) { }

};
struct vec3 {
  float x, y, z;
};
struct Transform {
  vec3 pos, rot, scale;
};
#include "Simulation.hpp"
#include "World.hpp"
void ECSDemo( ) {
  Simulation Sim;
  World &TestWorld{ Sim.CreateWorld( "Test World" ) };
  TestWorld.AddSystem<somegameplayshit>( "Some gameplay shit" );
  static_assert( IsEntitiesWith_v<decltype( somegameplayshit::Entities )> );
  static_assert( HasEntities_v<somegameplayshit> );
  static_assert( has_Update_memfn_v<somegameplayshit> );
  ArchetypeRef enemy{ Sim.CreateArchetype( "Enemy" ) };
  enemy.Add<Transform>( vec3{ 0.f,0.f,0.f }, vec3{ 0.f,0.f,0.f }, vec3{ 1.f,1.f,1.f } );
  TestWorld.Spawn( enemy );
}


int main( ) {
  ECSDemo( );
  return NanosuitDemo( );
}