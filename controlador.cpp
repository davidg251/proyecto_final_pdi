#include <iostream>
#include <stdlib.h>
#include <string>
#include <opencv2/opencv.hpp>
#include "controlador.h"

using namespace cv;
using namespace std;

void Controlador::listarArchivos()
{
  /*
  Ejecuta el comando ls en la carpeta imagenes para
  asi obtener la lista de imagenes en la carpeta, se
  guardan en el vector imagenes
  */

  FILE *tuberia;
  char *comando = (char *)"ls imagenes/";
  char line[256];

  if( !(tuberia = (FILE*)popen(comando,"r")) )
  {
    perror("Problemas al ejecutar el comando");
    exit(1);
  }

  while( fgets(line, sizeof line, tuberia) )
  {
    this->imagenes.push_back(line);
  }

  pclose(tuberia);

}



void Controlador::ejecutarMejora()
{

  int numeroImagenes = this->imagenes.size();
  int tamanioCadena = 0;
  int suma,filas, columnas = 0;
  int y0,yz = 0;//diametro interno
  int x0,xz =0;//diametro externo
  bool bandera = false;
  bool bandera2 = false;
  bool bandera3 = false;
  string nombreImagen;
  Mat imagensrc, imagenGris;
  Mat bordes,destino;

  for(int i=0;i<numeroImagenes;i++)
  {

    nombreImagen = (string)imagenes[i];
    tamanioCadena = nombreImagen.size();
    nombreImagen.erase(tamanioCadena-1,tamanioCadena);

    imagensrc = imread("/home/david/pdi_c++/imagenes/"+nombreImagen);
    destino.create( imagensrc.size(), imagensrc.type() );
    cvtColor( imagensrc, imagenGris, CV_BGR2GRAY );

    medianBlur(imagenGris, bordes, 11);
    Canny(bordes, bordes ,0,200,3);
    destino = Scalar::all(0);
    imagensrc.copyTo( destino, bordes);
    suma = 0;
    bandera = false;
    bandera2 = false;
    bandera3 = false;
    filas = destino.rows;
    columnas = destino.cols;

    for(int y=0;y<filas;y++)
    {
        suma = 0;

        for(int x=0;x<columnas;x++)
        {

            Vec3b color = destino.at<Vec3b>(Point(x,y));
            suma += (color[0] + color[1]+ color[2]) ;
            //este trozo de codigo produce error

            if((color[0] + color[1]+ color[2] > 0) && !bandera2)
            {

              //line( destino, Point( x, y ), Point( x, y+100), Scalar( 255, 0, 0 ),  1, 1 );

              bandera2 = true;
              bandera3 = true;
              x0 = x;
            }else if((color[0] + color[1]+ color[2] == 0) && bandera3)
            {

              //line( destino, Point( x, y ), Point( x, y+400), Scalar( 255, 0, 0 ),  1, 1 );
              bandera3 = false;
              bandera2 = true;
              xz = x;
            }

        }

        if( suma > 0 && !bandera)
        {
           line( destino, Point( 0, y-1 ), Point( columnas, y-1), Scalar( 255, 0, 0 ),  1, 1 );
           bandera = true;
           y0 = y-1;
        }
        else if( suma == 0 && bandera )
        {
          line( destino, Point( 0, y-1 ), Point( columnas, y-1), Scalar( 255, 0, 0 ),  1, 1 );
          bandera = false;
          yz = y-1;
        }

    }


    int temp = (int) ((x0 + xz)/2);
    cout<<"#################"<<endl;
    for(int k=y0+2;k<yz;k++)
    {
      Vec3b color = destino.at<Vec3b>(Point(temp,k));
      
      if( (color[0] + color[1]+ color[2]) >0 )
      {
          cout<<"diametro interno :"<< k-y0<<endl;
          break;
      }
    destino.at<Vec3b>(Point(temp,k)) = 255;
    }




    //line( destino, Point( temp, 0 ), Point( temp, 500), Scalar( 255, 0, 0 ),  1, 1 );
    imwrite("salidas/salida_"+nombreImagen,destino);

    cout<<nombreImagen<<endl;
    cout<<"x medio:"<<temp<<endl;
    cout<<"diametro externo:"<<yz-y0<<endl;
    cout<<"#################"<<endl;
  }


}
