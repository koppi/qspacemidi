<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="zh_CN" sourcelanguage="en">
<context>
    <name>main</name>
    <message>
        <source>Maps a 3Dconnexion SpaceNavigator to MIDI CC messages.</source>
        <translation>将 3Dconnexion SpaceNavigator 映射为 MIDI CC 消息。</translation>
    </message>
    <message>
        <source>Start hidden in the system tray.</source>
        <translation>启动时隐藏到系统托盘。</translation>
    </message>
    <message>
        <source>Render the window to &lt;file&gt; and exit (used to generate the documentation screenshot).</source>
        <translation>将窗口渲染到 &lt;file&gt; 后退出（用于生成文档截图）。</translation>
    </message>
</context>
<context>
    <name>mapping</name>
    <message>
        <source>Absolute</source>
        <translation>绝对</translation>
    </message>
    <message>
        <source>Relative (Bin Offset)</source>
        <translation>相对（二进制偏移）</translation>
    </message>
    <message>
        <source>Relative (2's Comp)</source>
        <translation>相对（二进制补码）</translation>
    </message>
    <message>
        <source>Relative (Signed Bit)</source>
        <translation>相对（符号位）</translation>
    </message>
    <message>
        <source>Momentary</source>
        <translation>瞬时</translation>
    </message>
    <message>
        <source>Toggle</source>
        <translation>切换</translation>
    </message>
    <message>
        <source>Translate X - left / right</source>
        <translation>平移 X — 左 / 右</translation>
    </message>
    <message>
        <source>Translate Y - forward / backward</source>
        <translation>平移 Y — 前 / 后</translation>
    </message>
    <message>
        <source>Translate Z - up / down</source>
        <translation>平移 Z — 上 / 下</translation>
    </message>
    <message>
        <source>Rotate X - tilt left / right</source>
        <translation>旋转 X — 向左 / 向右倾斜</translation>
    </message>
    <message>
        <source>Rotate Y - tilt forward / backward</source>
        <translation>旋转 Y — 向前 / 向后倾斜</translation>
    </message>
    <message>
        <source>Rotate Z - spin</source>
        <translation>旋转 Z — 自转</translation>
    </message>
</context>
<context>
    <name>qspacemidi::AboutDialog</name>
    <message>
        <source>About QSpaceMIDI</source>
        <translation>关于 QSpaceMIDI</translation>
    </message>
    <message>
        <source>Version %1</source>
        <translation>版本 %1</translation>
    </message>
    <message>
        <source>&lt;p&gt;Maps the six axes and the two buttons of a 3Dconnexion SpaceNavigator to MIDI control change messages, for use with Renoise and any other MIDI host.&lt;/p&gt;&lt;p&gt;No 3DxWare driver is needed: the device is read directly through evdev on Linux, IOKit HID on macOS and raw HID on Windows.&lt;/p&gt;&lt;p&gt;Copyright &amp;copy; 2013-2026 Jakob Flierl&lt;br&gt;Licensed under the &lt;a href='https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html'&gt;GNU LGPL, version 2.1 or later&lt;/a&gt;.&lt;/p&gt;&lt;p&gt;Built with Qt %1 and &lt;a href='https://github.com/atsushieno/libremidi'&gt;libremidi&lt;/a&gt;.&lt;br&gt;&lt;a href='https://github.com/koppi/qspacemidi'&gt;github.com/koppi/qspacemidi&lt;/a&gt;&lt;/p&gt;</source>
        <translation>&lt;p&gt;将 3Dconnexion SpaceNavigator 的六个轴和两个按键映射为 MIDI control change 消息，可用于 Renoise 及任何其他 MIDI 宿主。&lt;/p&gt;&lt;p&gt;无需 3DxWare 驱动：在 Linux 上通过 evdev、在 macOS 上通过 IOKit HID、在 Windows 上通过原始 HID 直接读取设备。&lt;/p&gt;&lt;p&gt;版权所有 © 2013-2026 Jakob Flierl&lt;br&gt;依据 &lt;a href='https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html'&gt;GNU LGPL, version 2.1&lt;/a&gt;.&lt;/p&gt;&lt;p&gt;使用 Qt %1 与 &lt;a href='https://github.com/atsushieno/libremidi'&gt;libremidi&lt;/a&gt; 构建。&lt;br&gt;&lt;a href='https://github.com/koppi/qspacemidi'&gt;github.com/koppi/qspacemidi&lt;/a&gt;&lt;/p&gt;</translation>
    </message>
</context>
<context>
    <name>qspacemidi::AxisRow</name>
    <message>
        <source>MIDI control change number</source>
        <translation>MIDI 控制器编号</translation>
    </message>
    <message>
        <source>Sensitivity - drag up or down, double click to reset</source>
        <translation>灵敏度 — 上下拖动调整，双击复位</translation>
    </message>
    <message>
        <source>Invert the direction of this axis</source>
        <translation>反转该轴的方向</translation>
    </message>
    <message>
        <source>Mute</source>
        <translation>静音</translation>
    </message>
    <message>
        <source>Invert direction</source>
        <translation>反转方向</translation>
    </message>
    <message>
        <source>Reset sensitivity</source>
        <translation>重置灵敏度</translation>
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
        <translation>平移</translation>
    </message>
    <message>
        <source>Rotate</source>
        <translation>旋转</translation>
    </message>
    <message>
        <source>Buttons</source>
        <translation>按键</translation>
    </message>
    <message>
        <source>Hardware button %1</source>
        <translation>硬件按键 %1</translation>
    </message>
    <message>
        <source>MIDI Output</source>
        <translation>MIDI 输出</translation>
    </message>
    <message>
        <source>Rescan</source>
        <translation>重新扫描</translation>
    </message>
    <message>
        <source>Look for MIDI ports and 3D mice again</source>
        <translation>重新查找 MIDI 端口和 3D 鼠标</translation>
    </message>
    <message>
        <source>Port:</source>
        <translation>端口：</translation>
    </message>
    <message>
        <source>Channel:</source>
        <translation>通道：</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>选项</translation>
    </message>
    <message>
        <source>Deflection ignored around the centre position</source>
        <translation>中心位置附近忽略的偏移量</translation>
    </message>
    <message>
        <source>Axis lock</source>
        <translation>轴锁定</translation>
    </message>
    <message>
        <source>Send only the axis that is deflected the most, so that parameters do not bleed into each other while MIDI mapping</source>
        <translation>仅发送偏移最大的轴，这样在进行 MIDI 映射时参数不会相互串扰</translation>
    </message>
    <message>
        <source>Start at login</source>
        <translation>登录时启动</translation>
    </message>
    <message>
        <source>Start QSpaceMIDI minimised to the system tray when the desktop session begins (%1)</source>
        <translation>桌面会话开始时以最小化到系统托盘的方式启动 QSpaceMIDI（%1）</translation>
    </message>
    <message>
        <source>About</source>
        <translation>关于</translation>
    </message>
    <message>
        <source>Dead zone:</source>
        <translation>死区：</translation>
    </message>
    <message>
        <source>Hide Window</source>
        <translation>隐藏窗口</translation>
    </message>
    <message>
        <source>About QSpaceMIDI...</source>
        <translation>关于 QSpaceMIDI…</translation>
    </message>
    <message>
        <source>Quit</source>
        <translation>退出</translation>
    </message>
    <message>
        <source>%1 (virtual port)</source>
        <translation>%1（虚拟端口）</translation>
    </message>
    <message>
        <source>No MIDI output port available.</source>
        <translation>没有可用的 MIDI 输出端口。</translation>
    </message>
    <message>
        <source>3D mouse: %1</source>
        <translation>3D 鼠标：%1</translation>
    </message>
    <message>
        <source>3D mouse: not found</source>
        <translation>3D 鼠标：未找到</translation>
    </message>
    <message>
        <source>MIDI: %1</source>
        <translation>MIDI：%1</translation>
    </message>
    <message>
        <source>MIDI: not connected</source>
        <translation>MIDI：未连接</translation>
    </message>
    <message>
        <source>Show Window</source>
        <translation>显示窗口</translation>
    </message>
    <message>
        <source>QSpaceMIDI will start with your desktop session.</source>
        <translation>QSpaceMIDI 将随桌面会话一起启动。</translation>
    </message>
    <message>
        <source>QSpaceMIDI will no longer start automatically.</source>
        <translation>QSpaceMIDI 将不再自动启动。</translation>
    </message>
    <message>
        <source>Could not write the autostart entry (%1).</source>
        <translation>无法写入自启动项（%1）。</translation>
    </message>
    <message>
        <source>Still running in the system tray. Use the tray icon to show the window again or to quit.</source>
        <translation>程序仍在系统托盘中运行。可通过托盘图标重新显示窗口或退出。</translation>
    </message>
</context>
<context>
    <name>qspacemidi::MidiEngine</name>
    <message>
        <source>MIDI output port "%1" was not found.</source>
        <translation>未找到 MIDI 输出端口“%1”。</translation>
    </message>
    <message>
        <source>Could not open MIDI output port "%1".</source>
        <translation>无法打开 MIDI 输出端口“%1”。</translation>
    </message>
    <message>
        <source>This platform does not support virtual MIDI ports. Install a loopback driver such as loopMIDI and select its port instead.</source>
        <translation>此平台不支持虚拟 MIDI 端口。请安装 loopMIDI 之类的回环驱动，并改选其端口。</translation>
    </message>
    <message>
        <source>Could not create the virtual MIDI port "%1".</source>
        <translation>无法创建虚拟 MIDI 端口“%1”。</translation>
    </message>
    <message>
        <source>%1 (virtual port)</source>
        <translation>%1（虚拟端口）</translation>
    </message>
</context>
</TS>
