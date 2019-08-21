#include <igl/readOFF.h>
#include <igl/opengl/glfw/Viewer.h>
#include <igl/jet.h>
#include <igl/viridis.h>
#include <igl/parula.h>
#include <igl/bwr.h>
#include <igl/colormap.h>
#include "tutorial_shared_path.h"

Eigen::MatrixXd V;
Eigen::MatrixXi F;
Eigen::VectorXd Z;
Eigen::MatrixXd C;

bool key_down(igl::opengl::glfw::Viewer& viewer, unsigned char key, int modifier)
{
    switch(key)
    {
        case '1':
        {
            igl::colormap(igl::COLOR_MAP_TYPE_INFERNO,Z,true,C);
            break;
        }
        case '2':
        {
            igl::jet(Z,true,C);
            break;
        }
        case '3':
        {
            igl::colormap(igl::COLOR_MAP_TYPE_MAGMA,Z,true,C);
            break;
        }
        case '4':
        {
            igl::parula(Z,true, C);
            break;
        }
        case '5':
        {
            igl::colormap(igl::COLOR_MAP_TYPE_PLASMA,Z,true,C);
            break;
        }
        case '6':
        {
            igl::viridis(Z,true, C);
            break;
        }
        case '7':
        {
            igl::bwr(Z,true,C);
            break;
        }
        case '8':
        {
            igl::colormap(igl::COLOR_MAP_TYPE_COOLWARM,Z,true,C);
            break;
        }
        case ' ':
        {
            // "clear"/ set to white
            C.setOnes();
            break;
        }
        default: break;

    }
    viewer.data().set_colors(C);
    return false;
}

int main(int argc, char *argv[])
{
  // Load a mesh in OFF format
  igl::readOFF(TUTORIAL_SHARED_PATH "/screwdriver.off", V, F);

  std::cout <<R"(Usage:
1  inferno colormap
2  jet colormap
3  magma colormap
4  parula colormap
5  plasma colormap
6  viridis colormap
7  bwr (blue-to-white-to-red) colormap
Press space set color to white
)";
  // Plot the mesh
  igl::opengl::glfw::Viewer viewer;
  viewer.data().set_mesh(V, F);

  // Use the z coordinate as a scalar field over the surface
  Z = V.col(2);

  // Compute per-vertex colors
  igl::jet(Z,true,C);

  viewer.callback_key_down = &key_down;

  // Add per-vertex colors
  viewer.data().set_colors(C);

  // Launch the viewer
  viewer.launch();
}
