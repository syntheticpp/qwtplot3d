#ifndef __openglhelper_2003_06_06_15_49__
#define __openglhelper_2003_06_06_15_49__

#include <qgl.h>

namespace Qwt3D
{

class GLStateBewarer
{
public:
	
	GLStateBewarer(GLenum what, bool on, bool persist=false)
	{
		stateval_ = glIsEnabled(what);	
		if (on)
			turnOn(persist);
		else
			turnOff(persist);
	}

	~GLStateBewarer() 
	{
		if (stateval_)
			glEnable(state_);
		else
			glDisable(state_);
	}
	
	void turnOn(bool persist = false)
	{
		glEnable(state_);
		if (persist)
			stateval_ = true;
	}
	
	void turnOff(bool persist = false)
	{
		glDisable(state_);
		if (persist)
			stateval_ = false;
	}


private:
	
	GLenum state_;
	bool stateval_;

};

inline const GLubyte* gl_error()
{
	GLenum errcode;
	const GLubyte* err = 0;
	
	if ((errcode = glGetError()) != GL_NO_ERROR)
	{
		err = gluErrorString(errcode);
	}
	return err;
}

} // ns

#endif  // __openglhelper_2003_06_06_15_49__
