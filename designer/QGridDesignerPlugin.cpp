#include "QGridDesignerPlugin.h"
#include "QGrid/GridView.h"
#include "ColumnsPropertyEditor.h"

#include <QtDesigner/QDesignerTaskMenuExtension>
#include <QtDesigner/QExtensionManager>
#include <QtGui/QIcon>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>

QGridDesignerPlugin::QGridDesignerPlugin(QObject *parent)
    : QObject(parent)
    , mInitialized(false)
{
}

bool QGridDesignerPlugin::isContainer() const
{
    return false;
}

bool QGridDesignerPlugin::isInitialized() const
{
    return mInitialized;
}

QIcon QGridDesignerPlugin::icon() const
{
    return QIcon();
}

QString QGridDesignerPlugin::domXml() const
{
    return QStringLiteral(R"(
<ui language="c++">
    <widget class="GridView" name="gridView">
        <property name="geometry">
            <rect>
                <x>0</x>
                <y>0</y>
                <width>400</width>
                <height>300</height>
            </rect>
        </property>
        <property name="headerText">
            <string>Header text</string>
        </property>
        <property name="headerVisible">
            <bool>false</bool>
        </property>
        <property name="toolboxVisible">
            <bool>false</bool>
        </property>
        <property name="searchbarVisible">
            <bool>false</bool>
        </property>
        <property name="columnsData">
            <string></string>
        </property>
    </widget>
</ui>
)");
}

QString QGridDesignerPlugin::group() const
{
    return QStringLiteral("QGrid Widgets");
}

QString QGridDesignerPlugin::includeFile() const
{
    return QStringLiteral("QGrid/GridView.h");
}

QString QGridDesignerPlugin::name() const
{
    return QStringLiteral("GridView");
}

QString QGridDesignerPlugin::toolTip() const
{
    return QStringLiteral("A grid view widget for displaying and editing tabular data");
}

QString QGridDesignerPlugin::whatsThis() const
{
    return QStringLiteral("GridView widget provides a flexible way to display and edit tabular data with support for filtering, searching, and custom delegates");
}

QWidget *QGridDesignerPlugin::createWidget(QWidget *parent)
{
    auto gridView = new GridView(parent);
    return gridView;
}

void QGridDesignerPlugin::initialize(QDesignerFormEditorInterface *core)
{
    if (mInitialized)
    {
        return;
    }

    QExtensionManager *manager = core->extensionManager();
    Q_ASSERT(manager != nullptr);
    
    manager->registerExtensions(new ColumnsTaskMenuFactory(core, manager),
    Q_TYPEID(QDesignerTaskMenuExtension));

    mInitialized = true;
}

