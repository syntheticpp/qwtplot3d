#ifndef qwt3d_scale_h__2004_06_02_22_02_begin_guarded_code
#define qwt3d_scale_h__2004_06_02_22_02_begin_guarded_code

#include <qstring.h>
#include "qwt3d_types.h"
#include "qwt3d_autoscaler.h"

namespace Qwt3D
{

/*! 
This is an encapsulation of nonvisual scales in a general manner. 
The class is utilized by Axis and also collaborates closely with AutoScaler.
She allows control over all aspects of tic generation including 
arbitrary transformations of tic values into corresponding strings. 
The strings contain what eventually will be shown as tic labels.\n
Standard linear and logarithmic scales have been integrated yet in the Axis 
interface. User-defined axes can be derived from Scale, LinearScale et al.
*/
class QWT3D_EXPORT Scale
{
  friend class Axis;
  public:    
    Scale();
    //! Derived classes should return a new heap based object here.  
    virtual Scale* clone() const = 0;
    void destroy() const {delete this;}    
    
    virtual QString ticLabel(unsigned int idx) const;
    virtual int autoscale(double& a, double& b, double start, double stop, int ivals);

  protected:
    virtual ~Scale(){}

    //! This function should setup the 2 vectors for major and minor positions;
    virtual void create() = 0;

    void setLimits(double start, double stop); 
    void setMajors(int val) {majorintervals_p=val;} //!< Sets number of major intervals
    void setMinors(int val) {minorintervals_p=val;} //!< Sets number of minor intervals per major interval
    void setMajorLimits(double start, double stop);

    int majors() const {return majorintervals_p;} //!< Returns major intervals
    int minors() const {return minorintervals_p;} //!< Returns minor intervals

    std::vector<double> majors_p, minors_p; 
    double start_p, stop_p;
    int majorintervals_p, minorintervals_p;
    double mstart_p, mstop_p;
};

//! The standard (1:1) mapping class for axis numbering
class QWT3D_EXPORT LinearScale : public Scale
{
public:

protected:
  //! Returns a new heap based object utilized from qwt3d_ptr
  Scale* clone() const {return new LinearScale(*this);}
  int autoscale(double& a, double& b, double start, double stop, int ivals);
  void create();
  LinearAutoScaler autoscaler_p;
};

//! log10 scale
class QWT3D_EXPORT LogScale : public Scale
{
public:
  //! Standard ctor
  LogScale();
protected:
  //! Returns a new heap based object utilized from qwt3d_ptr
  Scale* clone() const {return new LogScale;}
  QString ticLabel(unsigned int idx) const;
  void setMinors(int val);
  void create();
private:
  void setupCounter(int& k, int& step);
};

} // namespace Qwt3D


#endif /* include guarded */
