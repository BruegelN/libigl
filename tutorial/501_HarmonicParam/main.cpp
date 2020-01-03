#include <igl/boundary_loop.h>
#include <igl/harmonic.h>
#include <igl/map_vertices_to_circle.h>
#include <igl/readOFF.h>
#include <igl/opengl/glfw/Viewer.h>
#include <igl/png/writePNG.h>

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
  // Load a mesh in OFF format
  igl::readOFF(TUTORIAL_SHARED_PATH "/camelhead.off", V, F);

  std::vector<std::vector<int>> boundary_loop_list;
  igl::boundary_loop(F, boundary_loop_list);

  int loop_length = 0;
  std::vector<int> largest_boundary_loop;
  for (auto &boundary_loop : boundary_loop_list)
  {
    // assume that more vertices along a boundary correspond to longer boundary
    if (loop_length < boundary_loop.size())
    {
      loop_length = boundary_loop.size();
      largest_boundary_loop = boundary_loop;
    }
  }

  Eigen::Map<Eigen::VectorXi> bnd_temp(largest_boundary_loop.data(), largest_boundary_loop.size());

  Eigen::VectorXi bnd = bnd_temp;
  // Map the boundary to a circle, preserving edge proportions
  Eigen::MatrixXd bnd_uv;
  igl::map_vertices_to_circle(V,bnd,bnd_uv);

  // Harmonic parametrization for the internal vertices
  igl::harmonic(V,F,bnd,bnd_uv,1,V_uv);

  // Scale UV to make the texture more clear
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
