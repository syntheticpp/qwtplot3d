#ifndef __CURVE_H__
#define __CURVE_H__


#include "qwt3d_plot.h"

namespace Qwt3D
{

//! A class representing  Surfaces
/**
    A SurfacePlot ...
    
*/
class QWT3D_EXPORT Curve
{
public:
    Curve();
    virtual ~Curve();

    void setPlot(Plot3D*);
    Plot3D* plot() const { return plot_p; }
    virtual void draw();

    void updateNormals(); //!< Recalculates surface normals;
    int    resolution() const {return resolution_p;} //!< Returns data resolution (1 means all data)
    std::pair<int,int> facets() const; //!< Returns the number of mesh cells for the ORIGINAL data
    bool loadFromData(Qwt3D::Triple** data, unsigned int columns, unsigned int rows
    , bool uperiodic = false, bool vperiodic = false);
    bool loadFromData(double** data, unsigned int columns, unsigned int rows
                                                                        ,double minx, double maxx, double miny, double maxy);
    bool loadFromData(Qwt3D::TripleField const& data, Qwt3D::CellField const& poly);
     
    void setPlotStyle( Qwt3D::PLOTSTYLE val);
    Qwt3D::Enrichment* setPlotStyle( Qwt3D::Enrichment const& val);
    Qwt3D::PLOTSTYLE plotStyle() const { return plotstyle_; }//!< Returns plotting style
    //! Returns current Enrichment object used for plotting styles (if set, zero else)
    Qwt3D::Enrichment* userStyle() const { return userplotstyle_p; }
    void setShading( Qwt3D::SHADINGSTYLE val );
    Qwt3D::SHADINGSTYLE shading() const { return shading_; }//!< Returns shading style
    void setIsolines(int isolines);
    int isolines() const { return isolines_;} //!< Returns number of isolines
    
    void setSmoothMesh(bool val) {smoothdatamesh_p = val;} //!< Enables/disables smooth data mesh lines. Default is false
    bool smoothDataMesh() const {return smoothdatamesh_p;} //!< True if mesh antialiasing is on
    void setMeshColor(Qwt3D::RGBA rgba); //!< Sets color for data mesh
    Qwt3D::RGBA meshColor() const {return meshcolor_;} //!< Returns color for data mesh
    void setMeshLineWidth(double lw); //!< Sets line width for data mesh
    double meshLineWidth() const {return meshLineWidth_;} //!< Returns line width for data mesh
    void setDataColor(Color* col); //!< Sets new data color object
    const Color* dataColor() const {return datacolor_p;} //!< Returns data color object
    virtual Qwt3D::Enrichment* addEnrichment(Qwt3D::Enrichment const&); //!< Add an Enrichment
    virtual bool degrade(Qwt3D::Enrichment*); //!< Remove an Enrichment

    void setPolygonOffset(double d);
    double polygonOffset() const {return polygonOffset_;} //!< Returns relative value for polygon offset [0..1]

    Qwt3D::ParallelEpiped hull() const { return hull_;} //!< Returns rectangular hull   
  
    Qwt3D::FLOORSTYLE floorStyle() const { return floorstyle_;} //!< Return floor style
    void    setFloorStyle( Qwt3D::FLOORSTYLE val ) {floorstyle_ = val;} //!< Sets floor style
    void showNormals(bool); //!< Draw normals to every vertex
    bool normals() const { return datanormals_p;} //!< Returns \c true, if normal drawing is on
    
    void setNormalLength(double val); //!< Sets length of normals in percent per hull diagonale
    double normalLength() const { return normalLength_p; }//!< Returns relative length of normals
    void setNormalQuality(int val); //!< Increases plotting quality of normal arrows
    int normalQuality() const { return normalQuality_p; }//!< Returns plotting quality of normal arrows

    //! Returns true if valid data available, false else
    bool hasData() const { return (actualData_p) ? !actualData_p->empty() : false;}
    //! the data pointer
    const Qwt3D::Data* data() const { return actualData_p; }

    void queueUpdate();

//signals:
    void resolutionChanged(int);

//public slots:
    void setResolution( int );
protected:
    void updateData();
    virtual void drawImplementation();
    typedef std::list<Qwt3D::Enrichment*> EnrichmentList;
    typedef EnrichmentList::iterator ELIT;

    bool update_displaylists_;
    Qwt3D::Color* datacolor_p;
    Qwt3D::Enrichment* userplotstyle_p;
    EnrichmentList elist_p;

    Qwt3D::RGBA meshcolor_;
    double meshLineWidth_;
    Qwt3D::RGBA bgcolor_;
    Qwt3D::PLOTSTYLE plotstyle_;
    Qwt3D::SHADINGSTYLE shading_;
    Qwt3D::FLOORSTYLE floorstyle_;
    double polygonOffset_;
    int isolines_;
    bool smoothdatamesh_p;

    bool datanormals_p;
    double normalLength_p;
    int normalQuality_p;
  
    void setHull(Qwt3D::ParallelEpiped p) {hull_ = p;}
    virtual void calculateHull();
    virtual void createData();
    void createEnrichments();
    virtual void createEnrichment(Qwt3D::Enrichment& p);
    virtual void createFloorData();
    void createNormals();
    void createPoints();    

    int resolution_p;

    void readIn(Qwt3D::GridData& gdata, Triple** data, unsigned int columns, unsigned int rows);
    void readIn(Qwt3D::GridData& gdata, double** data, unsigned int columns, unsigned int rows, 
                double minx, double maxx, double miny, double maxy);
    void calcNormals(GridData& gdata);
    void sewPeriodic(GridData& gdata);

  //void calcLowResolution();
private:
    Plot3D* plot_p;
    enum OBJECTS
    {
        DataObject=0,
        NormalObject,
        DisplayListSize // only to have a vector length ...
    };
    std::vector<GLuint> displaylists_p;

    void Data2Floor();
    void Isolines2Floor();
    
    Qwt3D::ParallelEpiped hull_;

    Qwt3D::Data* actualData_p;

    // grid plot
    
    Qwt3D::GridData* actualDataG_;
    virtual void createDataG();
    virtual void createFloorDataG();

    void createNormalsG();
    void Data2FloorG();
    void Isolines2FloorG();
    void setColorFromVertexG(int ix, int iy, bool skip = false);


    // mesh plot

    Qwt3D::CellData* actualDataC_;
    virtual void createDataC();
    virtual void createFloorDataC();
    void createNormalsC();
    void Data2FloorC();
    void Isolines2FloorC();
    void setColorFromVertexC(int node, bool skip = false);
};

}

#endif
