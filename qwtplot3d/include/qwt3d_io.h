#ifndef __qwt3d_io_2003_07_04_23_27__
#define __qwt3d_io_2003_07_04_23_27__

#include <vector>
#include <algorithm>


namespace Qwt3D
{

/** 
IO provides a generic interface for standard and user written I/O handlers. 
The interfaces mimics roughly Qt's QImageIO functions for defining  
image input/output functions. 
*/
class QWT3D_EXPORT IO
{

public:
  /*! 
    The function type that can be processed by the define... members.
    An extension is the IO::Functor.
  */
  typedef bool (*Function)(Plot3D*, QString const& fname, QString const&);
  
  
  /*! 
    This class gives more flexibility in implementing 
    userdefined IO handlers than the simple IO::Function type. 
  */
  class Functor
  {
  public:
    virtual ~Functor() {}
    /*! Must create object of your own derived class with new and 
    return a pointer to it. 
    */
    virtual Functor* clone() const = 0;
    /*! The workhorse of the user-defined implementation. Eventually, the 
    framework will call this operator.
    */
    virtual bool operator()(Plot3D* plot, QString const& fname, QString const&) = 0;
  };

  IO() {setupHandler();}
  static bool defineInputHandler( QString const& format, Function func);
  static bool defineOutputHandler( QString const& format, Function func);
  static bool defineInputHandler( QString const& format, Functor const& func);
  static bool defineOutputHandler( QString const& format, Functor const& func);
  static bool save(Plot3D*, QString const& fname, QString const& format);
  static bool load(Plot3D*, QString const& fname, QString const& format);
  static QStringList inputFormatList();
  static QStringList outputFormatList();
  static Functor* outputHandler(QString const& format);
  static Functor* inputHandler(QString const& format);
  
private:  
  //! Lightweight Functor encapsulating an IO::Function
  class Wrapper : public Functor
  {
  public:
    //! Performs actual input
    Functor* clone() const { return new Wrapper(*this); }
    //! Creates a Wrapper object from a function pointer
    explicit Wrapper(Function h) : hdl(h) {}
    //! Returns a pointer to the wrapped function
    bool operator()(Plot3D* plot, QString const& fname, QString const& format)
    {
      return (hdl) 
        ?(*hdl)(plot, fname, format)
        :false;
    }
  private: 
    Function hdl;
  };  

  struct Entry
  {
    Entry() : iofunc(0) 
    {
    }
    
    ~Entry() 
    {
      delete iofunc;
    }
    
    Entry(Entry const& e)
    {
      if (this==&e)
        return;

      this->fmt = e.fmt;
      this->iofunc = e.iofunc->clone();
    }
    
    void operator=(Entry const& e)
    {
      if (this==&e)
        return;

      this->fmt = e.fmt;
      this->iofunc = e.iofunc->clone();
    }

    Entry(QString const& s, Functor const& f)
      : fmt(s) 
    { 
      iofunc = f.clone();
    }
    
    Entry(QString const& s, Function f)
      : fmt(s) 
    { 
      Wrapper  w(f);
      iofunc = w.clone();
    }
    
    QString fmt;
    Functor* iofunc;
  };

  struct FormatCompare
  {
    explicit FormatCompare(Entry const& e) {e_ = e;}
    bool operator() (Entry const& e)
    {
      return ( e.fmt == e_.fmt);
    }

    Entry e_;
  };
 
  struct FormatCompare2
  {
    explicit FormatCompare2(QString s) {s_ = s;}
    bool operator() (Entry const& e)
    {
      return( e.fmt == s_);
    }

    QString s_;
  };
    
  typedef std::vector<Entry> Container;
  typedef Container::iterator IT;

  static bool add_unique(Container& l, Entry const& e)
  {
    FormatCompare comp(e);
    l.erase(std::remove_if(l.begin(), l.end(), comp), l.end());
    l.push_back(e);

    return true;
  }
  
  static IT find(Container& l, QString const& fmt)
  {
    FormatCompare2 comp(fmt);
    return std::find_if(l.begin(), l.end(), comp); 
  }
  
  static Container& rlist()
  {
    static Container rl = Container();
    return rl;
  }

  static Container& wlist()
  {
    static Container rl = Container();
    return rl;
  }

  static bool QtPixmapWrite(Plot3D* plot, QString const& fname, QString const& format)
  {
    QImage im = plot->grabFrameBuffer(true);
	  return im.save(fname, format.local8Bit());
  }

  void setupHandler();
};

} //ns

#endif
