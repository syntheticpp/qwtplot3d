#ifndef __openglhelper_2003_06_06_15_49__
#define __openglhelper_2003_06_06_15_49__

#include <qgl.h>

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



#endif  // __openglhelper_2003_06_06_15_49__
