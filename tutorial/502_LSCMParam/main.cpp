#include <igl/boundary_loop.h>
#include <igl/readOFF.h>
#include <igl/opengl/glfw/Viewer.h>
#include <igl/png/writePNG.h>

#include <igl/lscm.h>

#include "tutorial_shared_path.h"

Eigen::MatrixXd V;
Eigen::MatrixXi F;
Eigen::MatrixXd V_uv;

bool key_down(igl::opengl::glfw::Viewer& viewer, unsigned char key, int modifier)
{

  if (key == '1')
  {
    // Plot the 3D mesh
    viewer.data().set_mesh(V,F);
    viewer.core().align_camera_center(V,F);
  }
  else if (key == '2')
  {
    // Plot the mesh in 2D using the UV coordinates as vertex coordinates
    viewer.data().set_mesh(V_uv,F);
    viewer.core().align_camera_center(V_uv,F);
  }
  if (key == ' ')
  {
    // Allocate temporary buffers
    Eigen::Matrix<unsigned char,Eigen::Dynamic,Eigen::Dynamic> R(2560,1600);
    Eigen::Matrix<unsigned char,Eigen::Dynamic,Eigen::Dynamic> G(2560,1600);
    Eigen::Matrix<unsigned char,Eigen::Dynamic,Eigen::Dynamic> B(2560,1600);
    Eigen::Matrix<unsigned char,Eigen::Dynamic,Eigen::Dynamic> A(2560,1600);

    // Draw the scene in the buffers
    viewer.core().draw_buffer(
      viewer.data(),false,R,G,B,A);

    // Save it to a PNG
    igl::png::writePNG(R,G,B,A,"out.png");
  }

  viewer.data().compute_normals();

  return false;
}

int main(int argc, char *argv[])
{
  using namespace Eigen;
  using namespace std;

  // Load a mesh in OFF format
  igl::readOFF(TUTORIAL_SHARED_PATH "/camelhead.off", V, F);

  // Fix two points on the boundary
  VectorXi bnd,b(2,1);
  igl::boundary_loop(F,bnd);
  b(0) = bnd(0);
  b(1) = bnd(round(bnd.size()/2));
  MatrixXd bc(2,2);
  bc<<0,0,1,0;

  // LSCM parametrization
  igl::lscm(V,F,b,bc,V_uv);

  // Scale the uv
  V_uv *= 20;

  // Plot the mesh
  igl::opengl::glfw::Viewer viewer;
  viewer.data().set_mesh(V, F);
  viewer.data().set_uv(V_uv);
  viewer.callback_key_down = &key_down;

  // Disable wireframe
  viewer.data().show_lines = false;

  // Draw checkerboard texture
  viewer.data().show_texture = true;

  // Launch the viewer
  viewer.launch();
}
