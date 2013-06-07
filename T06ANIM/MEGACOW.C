/* FILE NAME: MEGACOW.C
 * PROGRAMMER: IK1
 * DATE: 06.06.2013
 * PURPOSE: Cow animation module.
 */

#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "anim.h"
#include "vec.h"


/* Vertex array */
extern VEC *Vertexes, *VertexesProj;
extern INT NumOfVertexes;

/* Facet array */
extern INT (*Facets)[3];
extern INT NumOfFacets;

/* w, h of window */
extern INT w, h;


typedef struct
{
  IK1_UNIT_BASE_FUNCS;
  INT X, Y; 
  INT RandShift;
  DBL RandScale;
  INT Who;      
} COW;

static VOID CowRender( COW *Unit, ik1ANIM *Ani )
{
  DBL t = (DBL)clock() / CLOCKS_PER_SEC;
  INT
    i, j, y;
  MATR m = 
  {
    {
      {1, 0, 0 ,0},
      {0, 1, 0, 0},
      {0, 0, 1, 0},
      {0, 0, 0, 1}
    }
  };

  SelectObject(Ani->hDC, GetStockObject(NULL_BRUSH));
  SelectObject(Ani->hDC, GetStockObject(DC_PEN));
  SetDCPenColor(Ani->hDC, RGB(255, 255, 255));
  SetDCBrushColor(Ani->hDC, RGB(0, 111, 0));

  m = MatrMulMatr(MatrScale(30, 30, 30), MatrRotate( 0, Ani->Jx, Ani->Jy, Ani->Jz));

  for (i = 0; i < NumOfVertexes; i++)
  {
    VEC v;
    v = VecMulMatr(VecSet(Vertexes[i].X, Vertexes[i].Y, Vertexes[i].Z), m);
    VertexesProj[i].X = v.X + /*Unit->X*/+900;
    VertexesProj[i].Y = - v.Y + /*Unit->Y*/+500;
    VertexesProj[i].Z = v.Z /*+ Unit->Y+500*/;
  }

  for (i = 0; i < NumOfFacets; i++)
  {
    POINT p[3];
    for (j = 0; j < 3; j++)
    {
      p[j].x = VertexesProj[Facets[i][j]].X;         
      p[j].y = VertexesProj[Facets[i][j]].Y;
    }
    Polygon(Ani->hDC, p, 3);
  }  

  for (i = 0; i < NumOfVertexes; i++)
  {
    INT
      x = VertexesProj[i].X;
      y = VertexesProj[i].Y;
    SetPixelV(Ani->hDC, x, y, RGB(255, 255, 255));
  }
}

ik1UNIT * CowCreate( INT X, INT Y )
{
  COW *Unit;

  if ((Unit = (COW *)IK1_UnitCreate(sizeof(COW))) == NULL)
    return NULL;

  Unit->Render = (ik1UNIT_RENDER)CowRender;
  Unit->X = X;
  Unit->Y = Y;
  Unit->Who = 0;
  Unit->RandShift = rand() % 1000;
  Unit->RandScale = 0.75 + 5.5 * rand() / (DBL)RAND_MAX;
  return (ik1UNIT *)Unit;
} /* End of 'CowCreate' function */

/* END OF 'MEGACOW.C' FILE */