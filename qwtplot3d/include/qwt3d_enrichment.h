#ifndef qwt3d_enrichment_h__2004_02_23_19_24_begin_guarded_code
#define qwt3d_enrichment_h__2004_02_23_19_24_begin_guarded_code

#include "qwt3d_global.h"
#include "qwt3d_types.h"

namespace Qwt3D
{

class Plot3D;


//! Abstract base class for data dependent visible user objects
/*!
*/
class QWT3D_EXPORT Enrichment
{
public:
  enum {
    VERTEXENRICHMENT,
    EDGEENRICHMENT,
    FACEENRICHMENT,
    VOXELENRICHMENT
  } TYPE;
  
  Enrichment() : plot(0) {}
  virtual ~Enrichment(){}
  virtual Enrichment* clone() const = 0;
  virtual void draw(Qwt3D::Triple const&) = 0;
  virtual void drawBegin(){};
  virtual void drawEnd(){};
  virtual void assign(Plot3D const& pl) {plot = &pl;}

protected:
  const Plot3D* plot;
};

// todo EdgeEnrichment, FaceEnrichment, VoxelEnrichment

} // ns

#endif
