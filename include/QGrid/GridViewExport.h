#pragma once

#include <QtCore/qglobal.h>

#ifndef QGRID_BUILD_STATIC
# if defined(QGRID_LIB)
#  define QGRID_EXPORT Q_DECL_EXPORT
# else
#  define QGRID_EXPORT Q_DECL_IMPORT
# endif
#else
# define QGRID_EXPORT   
#endif