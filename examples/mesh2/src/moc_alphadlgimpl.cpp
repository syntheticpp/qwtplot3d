/****************************************************************************
** alphadlg meta object code from reading C++ file 'alphadlgimpl.h'
**
** Created: Di 1. Apr 15:07:32 2003
**      by: The Qt MOC ($Id$)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "alphadlgimpl.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 19)
#error "This file was generated using the moc from 3.0.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *alphadlg::className() const
{
    return "alphadlg";
}

QMetaObject *alphadlg::metaObj = 0;
static QMetaObjectCleanUp cleanUp_alphadlg;

#ifndef QT_NO_TRANSLATION
QString alphadlg::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "alphadlg", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString alphadlg::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "alphadlg", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* alphadlg::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = alphadlgBase::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"alphadlg", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_alphadlg.setMetaObject( metaObj );
    return metaObj;
}

void* alphadlg::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "alphadlg" ) ) return (alphadlg*)this;
    return alphadlgBase::qt_cast( clname );
}

bool alphadlg::qt_invoke( int _id, QUObject* _o )
{
    return alphadlgBase::qt_invoke(_id,_o);
}

bool alphadlg::qt_emit( int _id, QUObject* _o )
{
    return alphadlgBase::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool alphadlg::qt_property( int _id, int _f, QVariant* _v)
{
    return alphadlgBase::qt_property( _id, _f, _v);
}
#endif // QT_NO_PROPERTIES
