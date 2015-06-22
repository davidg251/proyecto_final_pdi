#include "controlador.cpp"

using namespace cv;

int main()
{
  Controlador con;
  con.listarArchivos();
  con.ejecutarMejora();

  return 0;
}
