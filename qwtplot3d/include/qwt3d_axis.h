#ifndef __AXIS_H__
#define __AXIS_H__

#include "qwt3d_autoptr.h"
#include "qwt3d_label.h"
#include "qwt3d_autoscaler.h"

namespace Qwt3D
{

//! Autoscalable axis with caption
class QWT3D_EXPORT Axis : public Drawable
{

public:

	Axis(); //!< Constructs standard axis
	Axis(Qwt3D::Triple beg, Qwt3D::Triple end); //!< Constructs a new axis with specified limits
	virtual ~Axis(); // dtor
	
	virtual void draw(); //!< Draws axis

	void setPosition(const Qwt3D::Triple& beg, const Qwt3D::Triple& end); //!< Positionate axis
	void position(Qwt3D::Triple& beg, Qwt3D::Triple& end) const {beg = beg_; end = end_;} //!< Returns axis' position
  Qwt3D::Triple begin() const { return beg_; } //!< Returns axis' beginning position
  Qwt3D::Triple end() const { return end_; } //!< Returns axis' ending position 
  double length() const { return (end_-beg_).length(); } //!< Returns axis' length

	void setTicLength(double majorl, double minorl); //!< Sets tics lengths in world coordinates
	//! Returns tics lengths
  void ticLength(double& majorl, double& minorl) const {majorl = lmaj_; minorl = lmin_;}
	void setTicOrientation(double tx, double ty, double tz); //!< Sets tic orientation
	void setTicOrientation(const Qwt3D::Triple& val); //!< Same function as above
  Qwt3D::Triple ticOrientation() const { return orientation_;} //!< Returns tic orientation
  void setSymmetricTics( bool b) { symtics_ = b;} //!< Sets two-sided tics (default is false) 
	
  //! Sets font for axis label
	void setLabelFont(QString const& family, int pointSize, int weight = QFont::Normal, bool italic = false);
	void setLabelFont(QFont const& font); //!< Sets font for axis label
  QFont const& labelFont() const {return labelfont_;}; //!< Returns current label font 
  
  void setLabelString(QString const& name);   //!< Sets label content
	void setLabelPosition(const Qwt3D::Triple& pos, Qwt3D::ANCHOR);
	void setLabelColor(Qwt3D::RGBA col);
  void setLabel(bool d) {drawLabel_ = d;} //!< Turns label drawing on or off
	void adjustLabel(int val) {labelgap_ = val;} //!< Shifts label in device coordinates dependent on anchor;

	void setScale(bool d) {drawTics_ = d;} //!< Turns scale drawing on or off
	bool scale() const {return drawTics_;} //!< Returns, if scale drawing is on or off
	void setNumbers(bool d) {drawNumbers_ = d;} //!< Turns number drawing on or off
	bool numbers() const {return drawNumbers_;} //!< Returns, if number drawing is on or off
	void setNumberColor(Qwt3D::RGBA col); //!< Sets the color for axes numbers
	Qwt3D::RGBA numberColor() const {return numbercolor_;} //!< Returns the color for axes numbers
	//! Sets font for numbering
	void setNumberFont(QString const& family, int pointSize, int weight = QFont::Normal, bool italic = false);
	void setNumberFont(QFont const&); //!< Overloaded member, works like the above function
  QFont const& numberFont() const {return numberfont_;}; //!< Returns current numbering font
  void setNumberAnchor(Qwt3D::ANCHOR a) { scaleNumberAnchor_ = a;} //!< Sets anchor position for numbers
	void adjustNumbers(int val) {numbergap_ = val;} //!< Shifts axis numbers in device coordinates dependent on anchor;

	void setAutoScale(bool val = true) {autoscale_ = val;} //!< Turns Autoscaling on or off
	bool autoScale() const { return autoscale_;} //!< actual Autoscaling mode

	void setMajors(int val); //!< Requests major intervals (maybe changed, if autoscaling is present)
	void setMinors(int val); //!< Requests minor intervals
	int majors() const { return majorintervals_; } //!< Returns number of major intervals
	int minors() const { return minorintervals_; } //!< Returns number of minor intervals
	Qwt3D::TripleField const& majorPositions() const {return majorpos_;} //!< Returns positions for actual major tics (also if invisible)
	Qwt3D::TripleField const& minorPositions() const {return minorpos_;} //!< Returns positions for actual minor tics (also if invisible)
	
	//! Sets line width for axis components
	void setLineWidth(double val, double majfac = 0.9, double minfac = 0.5);
	double lineWidth() const { return lineWidth_;} //!< Returns line width for axis body
	double majLineWidth() const { return majLineWidth_;} //!< Returns Line width for major tics
	double minLineWidth() const { return minLineWidth_;} //!< Returns Line width for minor tics

	void setLimits(double start, double stop) {start_=start; stop_=stop;} //!< Sets interval
	void limits(double& start, double& stop) const {start = start_; stop = stop_;} //!< Returns axis interval
  void recalculateTics(); //!< Enforces recalculation of ticmark positions

  //! A mapping class to flexibilize the axis numbering scheme
  /*!
  This ABC is a base for implementations of special forms of scale items
  instead real numbers. The key part here is operator(). The framework 
  guarantees that the arguments are filled with the index for the 
  actual major tic and his value. Additionally, tics() returns the current 
  number of major tics. The writer for the derived class can than
  map this informations to arbitrary output strings.
  \see Axis::Number implementation for an example.
  */
  class Item 
  {
  friend class Axis;
  public:
    //! Actual number of major tics
    unsigned int tics() const {return tics_;};
    //! Returns a new heap based object of the derived class.  
    virtual Item* clone() const = 0;
    /*!
    \param val provides the current axis value at major tic i
    \param idx the current major tic index
    \return to implement ...
    */
    virtual QString operator()(double val, unsigned int idx) = 0;
    //! Called from framework
    void destroy() const {delete this;}
  protected:
    virtual ~Item(){}
  private:
    unsigned int tics_;
  };
  //! The standard (1:1) mapping class for axis numbering
  class Number : public Item
  {
  public:
    //! Returns a new heap based Number object 
    Item* clone() const {return new Number;}
    /*!
    \param val provides the current axis value at major tic i
    \param idx the current major tic index
    \return val's QString representation for valid i, empty QString else
    */
    QString operator()(double val, unsigned int idx)
    {
      if (idx<tics())
        return QString::number(val);
      return QString("");
    }
  };

  void setMap(Axis::Item* map);


private:

	void init();
	void drawBase();
	void drawTics();
	void drawNumber(Qwt3D::Triple Pos, int mtic);
	Qwt3D::Triple drawTic(Qwt3D::Triple nadir, double length);
	void drawLabel();
  double TicValue(int mtic) const;

	Qwt3D::Triple biggestNumberString();
	
	
	Qwt3D::ANCHOR scaleNumberAnchor_;
	Qwt3D::Label label_;
	std::vector<Qwt3D::Label> markerLabel_;

	Qwt3D::Triple beg_, end_;
	Qwt3D::TripleField majorpos_, minorpos_; //! vectors, holding major resp. minor tic positions;

	Qwt3D::Triple ncube_beg_, ncube_end_; //!< enclosing parallelepiped for axis numbering

	double start_, stop_, autostart_, autostop_;
	double lmaj_, lmin_;
	Qwt3D::Triple orientation_;

	int majorintervals_, minorintervals_;

	double lineWidth_, majLineWidth_, minLineWidth_;
	bool symtics_;
	bool drawNumbers_, drawTics_, drawLabel_;
	bool autoscale_;
	QFont numberfont_, labelfont_;
	Qwt3D::RGBA  numbercolor_;

	int numbergap_, labelgap_; 

	Qwt3D::AutoScaler as_;
  Qwt3D::qwt3d_ptr<Item> digitmap_;
};

} // ns 

#endif
