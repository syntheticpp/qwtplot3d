#ifndef QWT3D_GLOBAL_H
#define QWT3D_GLOBAL_H

#include <qmodules.h>
#include <qglobal.h>

#define QWT3D_VERSION       013
#define QWT3D_VERSION_STR   "0.1.3"

//
// Create Qwt3d DLL if QWT3D_DLL is defined (Windows only)
//

#if defined(Q_WS_WIN)

#if defined(_MSC_VER) /* MSVC Compiler */
/* template-class specialization 'identifier' is already instantiated */
#pragma warning(disable: 4660)
#pragma warning(disable: 4251) // dll interface required for stl templates
#endif

#if defined(QWT3D_NODLL)
#undef QWT3D_MAKEDLL
#undef QWT3D_DLL
#undef QWT3D_TEMPLATEDLL
#endif

#ifdef QWT3D_DLL
#if defined(QWT3D_MAKEDLL)     /* create a Qwt DLL library */
#undef QWT3D_DLL
#define QWT3D_EXPORT  __declspec(dllexport)
#define QWT3D_TEMPLATEDLL
#endif
#endif

#if defined(QWT3D_DLL)     /* use a Qwt DLL library */
#define QWT3D_EXPORT  __declspec(dllimport)
#define QWT3D_TEMPLATEDLL
#endif

#else // ! Q_WS_WIN
#undef QWT3D_MAKEDLL       /* ignore these for other platforms */
#undef QWT3D_DLL
#undef QWT3D_TEMPLATEDLL
#endif

#ifndef QWT3D_EXPORT
#define QWT3D_EXPORT
#endif

#if QT_VERSION < 300
// Use old QArray instead of QMemArray
#define QWT3D_NO_MEMARRAY 
#endif

#endif // QWT3D_GLOBAL_H
