# QGrid Designer Plugin

Плагин для Qt Designer, позволяющий использовать виджет GridView в визуальном редакторе форм.

## Сборка

Плагин автоматически собирается вместе с основной библиотекой QGrid при наличии компонента Qt5::Designer.

## Установка плагина

После сборки плагин будет находиться в папке `build/plugins/designer/`:
- Windows: `QGridDesignerPlugin.dll`
- Linux: `libQGridDesignerPlugin.so`
- macOS: `libQGridDesignerPlugin.dylib`

### Способ 1: Копирование в системную папку Qt Designer

Скопируйте файл плагина в папку плагинов Qt Designer:

**Windows:**
```
C:\Qt\<версия>\<компилятор>\plugins\designer\
```

**Linux:**
```
~/.local/share/QtDesigner/plugins/
или
/usr/lib/qt5/plugins/designer/
```

**macOS:**
```
~/Library/Application Support/QtDesigner/plugins/
или
/usr/local/lib/qt5/plugins/designer/
```

### Способ 2: Указание пути через переменную окружения

Установите переменную окружения `QT_PLUGIN_PATH`:

**Windows (PowerShell):**
```powershell
$env:QT_PLUGIN_PATH = "C:\путь\к\проекту\build\plugins\designer"
```

**Linux/macOS:**
```bash
export QT_PLUGIN_PATH=/путь/к/проекту/build/plugins/designer
```

### Способ 3: Использование в Qt Creator

1. Откройте Qt Creator
2. Перейдите в `Tools` → `Options` → `Designer`
3. В разделе "Additional Designer Plugins" добавьте путь к папке с плагином

## Проверка установки

1. Запустите Qt Designer или Qt Creator
2. Откройте любой `.ui` файл
3. В палитре виджетов должна появиться группа "QGrid Widgets"
4. Внутри группы должен быть виджет "GridView"
5. Перетащите GridView на форму
6. В Property Editor должно быть видно свойство `tableName`

## Использование

После установки плагина виджет GridView будет доступен в палитре виджетов Qt Designer. Вы можете:

- Перетаскивать GridView на форму
- Настраивать свойство `tableName` в Property Editor
- Использовать виджет в `.ui` файлах

## Добавление новых свойств

Чтобы добавить новое свойство, видимое в QDesigner:

1. Добавьте `Q_PROPERTY` в класс `GridView` (файл `include/QGrid/GridView.h`)
2. Обновите `domXml()` в `QGridDesignerPlugin.cpp`, добавив новое свойство в XML-шаблон

Пример:
```cpp
Q_PROPERTY(bool searchBarShow READ getSearchBarShow WRITE setSearchBarShow)
```

