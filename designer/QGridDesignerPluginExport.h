#pragma once

#include <QtCore/qglobal.h>

#ifndef QGRIDDESIGNERPLUGIN_BUILD_STATIC
# if defined(QGRIDDESIGNERPLUGIN_LIB)
#  define QGRIDDESIGNERPLUGIN_EXPORT Q_DECL_EXPORT
# else
#  define QGRIDDESIGNERPLUGIN_EXPORT Q_DECL_IMPORT
# endif
#else
# define QGRIDDESIGNERPLUGIN_EXPORT
#endif

