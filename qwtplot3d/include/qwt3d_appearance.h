#pragma once
#ifndef qwt3d_appearance_h__2009_06_10_12_11_18_begin_guarded_code
#define qwt3d_appearance_h__2009_06_10_12_11_18_begin_guarded_code

#include "qwt3d_types.h"

namespace Qwt3D
{
class Color;

class Appearance
{
public:
  PLOTSTYLE plotstyle;
  SHADINGSTYLE shadingstyle;
  //isolines
  bool smoothmesh;
  RGBA meshcolor;
  double meshlinewidth;
  Color* datacolor;
};

} //ns

#endif /* include guard */
