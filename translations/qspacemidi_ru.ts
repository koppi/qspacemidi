<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="ru_RU" sourcelanguage="en">
<context>
    <name>main</name>
    <message>
        <source>Maps a 3Dconnexion SpaceNavigator to MIDI CC messages.</source>
        <translation>Преобразует движения 3Dconnexion SpaceNavigator в сообщения MIDI CC.</translation>
    </message>
    <message>
        <source>Start hidden in the system tray.</source>
        <translation>Запускаться скрытно в системном лотке.</translation>
    </message>
    <message>
        <source>Render the window to &lt;file&gt; and exit (used to generate the documentation screenshot).</source>
        <translation>Отрисовать окно в &lt;file&gt; и выйти (используется для снимка экрана в документации).</translation>
    </message>
</context>
<context>
    <name>mapping</name>
    <message>
        <source>Absolute</source>
        <translation>Абсолютный</translation>
    </message>
    <message>
        <source>Relative (Bin Offset)</source>
        <translation>Относительный (двоичное смещение)</translation>
    </message>
    <message>
        <source>Relative (2's Comp)</source>
        <translation>Относительный (доп. код)</translation>
    </message>
    <message>
        <source>Relative (Signed Bit)</source>
        <translation>Относительный (знаковый бит)</translation>
    </message>
    <message>
        <source>Momentary</source>
        <translation>Кнопка</translation>
    </message>
    <message>
        <source>Toggle</source>
        <translation>Переключатель</translation>
    </message>
    <message>
        <source>Translate X - left / right</source>
        <translation>Смещение X — влево / вправо</translation>
    </message>
    <message>
        <source>Translate Y - forward / backward</source>
        <translation>Смещение Y — вперёд / назад</translation>
    </message>
    <message>
        <source>Translate Z - up / down</source>
        <translation>Смещение Z — вверх / вниз</translation>
    </message>
    <message>
        <source>Rotate X - tilt left / right</source>
        <translation>Вращение X — наклон влево / вправо</translation>
    </message>
    <message>
        <source>Rotate Y - tilt forward / backward</source>
        <translation>Вращение Y — наклон вперёд / назад</translation>
    </message>
    <message>
        <source>Rotate Z - spin</source>
        <translation>Вращение Z — поворот</translation>
    </message>
</context>
<context>
    <name>qspacemidi::AboutDialog</name>
    <message>
        <source>About QSpaceMIDI</source>
        <translation>О программе QSpaceMIDI</translation>
    </message>
    <message>
        <source>Version %1</source>
        <translation>Версия %1</translation>
    </message>
    <message>
        <source>&lt;p&gt;Maps the six axes and the two buttons of a 3Dconnexion SpaceNavigator to MIDI control change messages, for use with Renoise and any other MIDI host.&lt;/p&gt;&lt;p&gt;No 3DxWare driver is needed: the device is read directly through evdev on Linux, IOKit HID on macOS and raw HID on Windows.&lt;/p&gt;&lt;p&gt;Copyright &amp;copy; 2013-2026 Jakob Flierl&lt;br&gt;Licensed under the &lt;a href='https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html'&gt;GNU LGPL, version 2.1 or later&lt;/a&gt;.&lt;/p&gt;&lt;p&gt;Built with Qt %1 and &lt;a href='https://github.com/atsushieno/libremidi'&gt;libremidi&lt;/a&gt;.&lt;br&gt;&lt;a href='https://github.com/koppi/qspacemidi'&gt;github.com/koppi/qspacemidi&lt;/a&gt;&lt;/p&gt;</source>
        <translation>&lt;p&gt;Преобразует шесть осей и две кнопки 3Dconnexion SpaceNavigator в сообщения MIDI control change для Renoise и любого другого MIDI-хоста.&lt;/p&gt;&lt;p&gt;Драйвер 3DxWare не нужен: устройство читается напрямую — через evdev в Linux, IOKit HID в macOS и raw HID в Windows.&lt;/p&gt;&lt;p&gt;© 2013-2026 Jakob Flierl&lt;br&gt;Распространяется по лицензии &lt;a href='https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html'&gt;GNU LGPL, version 2.1&lt;/a&gt;.&lt;/p&gt;&lt;p&gt;Собрано с Qt %1 и &lt;a href='https://github.com/atsushieno/libremidi'&gt;libremidi&lt;/a&gt;.&lt;br&gt;&lt;a href='https://github.com/koppi/qspacemidi'&gt;github.com/koppi/qspacemidi&lt;/a&gt;&lt;/p&gt;</translation>
    </message>
</context>
<context>
    <name>qspacemidi::AxisRow</name>
    <message>
        <source>MIDI control change number</source>
        <translation>Номер MIDI-контроллера</translation>
    </message>
    <message>
        <source>Sensitivity - drag up or down, double click to reset</source>
        <translation>Чувствительность — тяните вверх или вниз, двойной щелчок сбрасывает</translation>
    </message>
    <message>
        <source>Invert the direction of this axis</source>
        <translation>Инвертировать направление этой оси</translation>
    </message>
    <message>
        <source>Mute</source>
        <translation>Отключить</translation>
    </message>
    <message>
        <source>Invert direction</source>
        <translation>Инвертировать направление</translation>
    </message>
    <message>
        <source>Reset sensitivity</source>
        <translation>Сбросить чувствительность</translation>
    </message>
</context>
<context>
    <name>qspacemidi::MainWindow</name>
    <message>
        <source>QSpaceMIDI</source>
        <translation>QSpaceMIDI</translation>
    </message>
    <message>
        <source>Translate</source>
        <translation>Смещение</translation>
    </message>
    <message>
        <source>Rotate</source>
        <translation>Вращение</translation>
    </message>
    <message>
        <source>Buttons</source>
        <translation>Кнопки</translation>
    </message>
    <message>
        <source>Hardware button %1</source>
        <translation>Аппаратная кнопка %1</translation>
    </message>
    <message>
        <source>MIDI Output</source>
        <translation>Выход MIDI</translation>
    </message>
    <message>
        <source>Rescan</source>
        <translation>Обновить</translation>
    </message>
    <message>
        <source>Look for MIDI ports and 3D mice again</source>
        <translation>Заново найти MIDI-порты и 3D-мыши</translation>
    </message>
    <message>
        <source>Port:</source>
        <translation>Порт:</translation>
    </message>
    <message>
        <source>Channel:</source>
        <translation>Канал:</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Параметры</translation>
    </message>
    <message>
        <source>Deflection ignored around the centre position</source>
        <translation>Отклонение, игнорируемое вблизи центрального положения</translation>
    </message>
    <message>
        <source>Axis lock</source>
        <translation>Блокировка оси</translation>
    </message>
    <message>
        <source>Send only the axis that is deflected the most, so that parameters do not bleed into each other while MIDI mapping</source>
        <translation>Передавать только наиболее отклонённую ось, чтобы параметры не мешали друг другу при назначении MIDI</translation>
    </message>
    <message>
        <source>Start at login</source>
        <translation>Запускать при входе</translation>
    </message>
    <message>
        <source>Start QSpaceMIDI minimised to the system tray when the desktop session begins (%1)</source>
        <translation>Запускать QSpaceMIDI свёрнутым в системный лоток при начале сеанса (%1)</translation>
    </message>
    <message>
        <source>About</source>
        <translation>О программе</translation>
    </message>
    <message>
        <source>Dead zone:</source>
        <translation>Мёртвая зона:</translation>
    </message>
    <message>
        <source>Hide Window</source>
        <translation>Скрыть окно</translation>
    </message>
    <message>
        <source>About QSpaceMIDI...</source>
        <translation>О программе QSpaceMIDI…</translation>
    </message>
    <message>
        <source>Quit</source>
        <translation>Выход</translation>
    </message>
    <message>
        <source>%1 (virtual port)</source>
        <translation>%1 (виртуальный порт)</translation>
    </message>
    <message>
        <source>No MIDI output port available.</source>
        <translation>Нет доступного выхода MIDI.</translation>
    </message>
    <message>
        <source>3D mouse: %1</source>
        <translation>3D-мышь: %1</translation>
    </message>
    <message>
        <source>3D mouse: not found</source>
        <translation>3D-мышь: не найдена</translation>
    </message>
    <message>
        <source>MIDI: %1</source>
        <translation>MIDI: %1</translation>
    </message>
    <message>
        <source>MIDI: not connected</source>
        <translation>MIDI: нет подключения</translation>
    </message>
    <message>
        <source>Show Window</source>
        <translation>Показать окно</translation>
    </message>
    <message>
        <source>QSpaceMIDI will start with your desktop session.</source>
        <translation>QSpaceMIDI будет запускаться вместе с сеансом.</translation>
    </message>
    <message>
        <source>QSpaceMIDI will no longer start automatically.</source>
        <translation>QSpaceMIDI больше не будет запускаться автоматически.</translation>
    </message>
    <message>
        <source>Could not write the autostart entry (%1).</source>
        <translation>Не удалось записать элемент автозапуска (%1).</translation>
    </message>
    <message>
        <source>Still running in the system tray. Use the tray icon to show the window again or to quit.</source>
        <translation>Программа продолжает работать в системном лотке. Значок позволяет снова показать окно или выйти.</translation>
    </message>
</context>
<context>
    <name>qspacemidi::MidiEngine</name>
    <message>
        <source>MIDI output port "%1" was not found.</source>
        <translation>Выход MIDI «%1» не найден.</translation>
    </message>
    <message>
        <source>Could not open MIDI output port "%1".</source>
        <translation>Не удалось открыть выход MIDI «%1».</translation>
    </message>
    <message>
        <source>This platform does not support virtual MIDI ports. Install a loopback driver such as loopMIDI and select its port instead.</source>
        <translation>Эта платформа не поддерживает виртуальные MIDI-порты. Установите драйвер петли, например loopMIDI, и выберите его порт.</translation>
    </message>
    <message>
        <source>Could not create the virtual MIDI port "%1".</source>
        <translation>Не удалось создать виртуальный MIDI-порт «%1».</translation>
    </message>
    <message>
        <source>%1 (virtual port)</source>
        <translation>%1 (виртуальный порт)</translation>
    </message>
</context>
</TS>
