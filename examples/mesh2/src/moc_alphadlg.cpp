/****************************************************************************
** alphadlgBase meta object code from reading C++ file 'alphadlg.h'
**
** Created: Di 1. Apr 15:07:32 2003
**      by: The Qt MOC ($Id$)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "alphadlg.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 19)
#error "This file was generated using the moc from 3.0.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *alphadlgBase::className() const
{
    return "alphadlgBase";
}

QMetaObject *alphadlgBase::metaObj = 0;
static QMetaObjectCleanUp cleanUp_alphadlgBase;

#ifndef QT_NO_TRANSLATION
QString alphadlgBase::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "alphadlgBase", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString alphadlgBase::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "alphadlgBase", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* alphadlgBase::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"alphadlgBase", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_alphadlgBase.setMetaObject( metaObj );
    return metaObj;
}

void* alphadlgBase::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "alphadlgBase" ) ) return (alphadlgBase*)this;
    return QDialog::qt_cast( clname );
}

bool alphadlgBase::qt_invoke( int _id, QUObject* _o )
{
    return QDialog::qt_invoke(_id,_o);
}

bool alphadlgBase::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool alphadlgBase::qt_property( int _id, int _f, QVariant* _v)
{
    return QDialog::qt_property( _id, _f, _v);
}
#endif // QT_NO_PROPERTIES
