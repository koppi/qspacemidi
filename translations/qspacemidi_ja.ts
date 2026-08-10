<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="ja_JP" sourcelanguage="en">
<context>
    <name>main</name>
    <message>
        <source>Maps a 3Dconnexion SpaceNavigator to MIDI CC messages.</source>
        <translation>3Dconnexion SpaceNavigator を MIDI CC メッセージに割り当てます。</translation>
    </message>
    <message>
        <source>Start hidden in the system tray.</source>
        <translation>システムトレイに隠した状態で起動します。</translation>
    </message>
    <message>
        <source>Render the window to &lt;file&gt; and exit (used to generate the documentation screenshot).</source>
        <translation>ウィンドウを &lt;file&gt; に描画して終了します（ドキュメント用のスクリーンショット生成に使用）。</translation>
    </message>
</context>
<context>
    <name>mapping</name>
    <message>
        <source>Absolute</source>
        <translation>絶対値</translation>
    </message>
    <message>
        <source>Relative (Bin Offset)</source>
        <translation>相対（バイナリオフセット）</translation>
    </message>
    <message>
        <source>Relative (2's Comp)</source>
        <translation>相対（2 の補数）</translation>
    </message>
    <message>
        <source>Relative (Signed Bit)</source>
        <translation>相対（符号ビット）</translation>
    </message>
    <message>
        <source>Momentary</source>
        <translation>モーメンタリ</translation>
    </message>
    <message>
        <source>Toggle</source>
        <translation>トグル</translation>
    </message>
    <message>
        <source>Translate X - left / right</source>
        <translation>平行移動 X — 左 / 右</translation>
    </message>
    <message>
        <source>Translate Y - forward / backward</source>
        <translation>平行移動 Y — 前 / 後</translation>
    </message>
    <message>
        <source>Translate Z - up / down</source>
        <translation>平行移動 Z — 上 / 下</translation>
    </message>
    <message>
        <source>Rotate X - tilt left / right</source>
        <translation>回転 X — 左 / 右に傾ける</translation>
    </message>
    <message>
        <source>Rotate Y - tilt forward / backward</source>
        <translation>回転 Y — 前 / 後に傾ける</translation>
    </message>
    <message>
        <source>Rotate Z - spin</source>
        <translation>回転 Z — ひねる</translation>
    </message>
</context>
<context>
    <name>qspacemidi::AboutDialog</name>
    <message>
        <source>About QSpaceMIDI</source>
        <translation>QSpaceMIDI について</translation>
    </message>
    <message>
        <source>Version %1</source>
        <translation>バージョン %1</translation>
    </message>
    <message>
        <source>&lt;p&gt;Maps the six axes and the two buttons of a 3Dconnexion SpaceNavigator to MIDI control change messages, for use with Renoise and any other MIDI host.&lt;/p&gt;&lt;p&gt;No 3DxWare driver is needed: the device is read directly through evdev on Linux, IOKit HID on macOS and raw HID on Windows.&lt;/p&gt;&lt;p&gt;Copyright &amp;copy; 2013-2026 Jakob Flierl&lt;br&gt;Licensed under the &lt;a href='https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html'&gt;GNU LGPL, version 2.1 or later&lt;/a&gt;.&lt;/p&gt;&lt;p&gt;Built with Qt %1 and &lt;a href='https://github.com/atsushieno/libremidi'&gt;libremidi&lt;/a&gt;.&lt;br&gt;&lt;a href='https://github.com/koppi/qspacemidi'&gt;github.com/koppi/qspacemidi&lt;/a&gt;&lt;/p&gt;</source>
        <translation>&lt;p&gt;3Dconnexion SpaceNavigator の 6 軸と 2 つのボタンを MIDI コントロールチェンジに割り当て、Renoise をはじめとする任意の MIDI ホストで使えます。&lt;/p&gt;&lt;p&gt;3DxWare ドライバーは不要です。Linux では evdev、macOS では IOKit HID、Windows では raw HID を通じてデバイスを直接読み取ります。&lt;/p&gt;&lt;p&gt;Copyright © 2013-2026 Jakob Flierl&lt;br&gt;ライセンス: &lt;a href='https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html'&gt;GNU LGPL, version 2.1&lt;/a&gt;.&lt;/p&gt;&lt;p&gt;Qt %1 と &lt;a href='https://github.com/atsushieno/libremidi'&gt;libremidi&lt;/a&gt; でビルドされています。&lt;br&gt;&lt;a href='https://github.com/koppi/qspacemidi'&gt;github.com/koppi/qspacemidi&lt;/a&gt;&lt;/p&gt;</translation>
    </message>
</context>
<context>
    <name>qspacemidi::AxisRow</name>
    <message>
        <source>MIDI control change number</source>
        <translation>MIDI コントロールチェンジ番号</translation>
    </message>
    <message>
        <source>Sensitivity - drag up or down, double click to reset</source>
        <translation>感度 — 上下にドラッグして調整、ダブルクリックでリセット</translation>
    </message>
    <message>
        <source>Invert the direction of this axis</source>
        <translation>この軸の方向を反転します</translation>
    </message>
    <message>
        <source>Mute</source>
        <translation>ミュート</translation>
    </message>
    <message>
        <source>Invert direction</source>
        <translation>方向を反転</translation>
    </message>
    <message>
        <source>Reset sensitivity</source>
        <translation>感度をリセット</translation>
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
        <translation>平行移動</translation>
    </message>
    <message>
        <source>Rotate</source>
        <translation>回転</translation>
    </message>
    <message>
        <source>Buttons</source>
        <translation>ボタン</translation>
    </message>
    <message>
        <source>Hardware button %1</source>
        <translation>ハードウェアボタン %1</translation>
    </message>
    <message>
        <source>MIDI Output</source>
        <translation>MIDI 出力</translation>
    </message>
    <message>
        <source>Rescan</source>
        <translation>再スキャン</translation>
    </message>
    <message>
        <source>Look for MIDI ports and 3D mice again</source>
        <translation>MIDI ポートと 3D マウスを再検索します</translation>
    </message>
    <message>
        <source>Port:</source>
        <translation>ポート:</translation>
    </message>
    <message>
        <source>Channel:</source>
        <translation>チャンネル:</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>オプション</translation>
    </message>
    <message>
        <source>Deflection ignored around the centre position</source>
        <translation>中央位置付近で無視する傾き量</translation>
    </message>
    <message>
        <source>Axis lock</source>
        <translation>軸ロック</translation>
    </message>
    <message>
        <source>Send only the axis that is deflected the most, so that parameters do not bleed into each other while MIDI mapping</source>
        <translation>最も大きく傾いた軸だけを送信し、MIDI マッピング中にパラメーターが混ざらないようにします</translation>
    </message>
    <message>
        <source>Start at login</source>
        <translation>ログイン時に起動</translation>
    </message>
    <message>
        <source>Start QSpaceMIDI minimised to the system tray when the desktop session begins (%1)</source>
        <translation>デスクトップセッションの開始時に QSpaceMIDI をシステムトレイに最小化した状態で起動します (%1)</translation>
    </message>
    <message>
        <source>About</source>
        <translation>情報</translation>
    </message>
    <message>
        <source>Dead zone:</source>
        <translation>デッドゾーン:</translation>
    </message>
    <message>
        <source>Hide Window</source>
        <translation>ウィンドウを隠す</translation>
    </message>
    <message>
        <source>About QSpaceMIDI...</source>
        <translation>QSpaceMIDI について…</translation>
    </message>
    <message>
        <source>Quit</source>
        <translation>終了</translation>
    </message>
    <message>
        <source>%1 (virtual port)</source>
        <translation>%1 (仮想ポート)</translation>
    </message>
    <message>
        <source>No MIDI output port available.</source>
        <translation>利用できる MIDI 出力ポートがありません。</translation>
    </message>
    <message>
        <source>3D mouse: %1</source>
        <translation>3D マウス: %1</translation>
    </message>
    <message>
        <source>3D mouse: not found</source>
        <translation>3D マウス: 見つかりません</translation>
    </message>
    <message>
        <source>MIDI: %1</source>
        <translation>MIDI: %1</translation>
    </message>
    <message>
        <source>MIDI: not connected</source>
        <translation>MIDI: 未接続</translation>
    </message>
    <message>
        <source>Show Window</source>
        <translation>ウィンドウを表示</translation>
    </message>
    <message>
        <source>QSpaceMIDI will start with your desktop session.</source>
        <translation>QSpaceMIDI はデスクトップセッションとともに起動します。</translation>
    </message>
    <message>
        <source>QSpaceMIDI will no longer start automatically.</source>
        <translation>QSpaceMIDI は自動的に起動しなくなります。</translation>
    </message>
    <message>
        <source>Could not write the autostart entry (%1).</source>
        <translation>自動起動の設定を書き込めませんでした (%1)。</translation>
    </message>
    <message>
        <source>Still running in the system tray. Use the tray icon to show the window again or to quit.</source>
        <translation>システムトレイで動作を続けています。トレイアイコンからウィンドウを再表示したり終了したりできます。</translation>
    </message>
</context>
<context>
    <name>qspacemidi::MidiEngine</name>
    <message>
        <source>MIDI output port "%1" was not found.</source>
        <translation>MIDI 出力ポート「%1」が見つかりません。</translation>
    </message>
    <message>
        <source>Could not open MIDI output port "%1".</source>
        <translation>MIDI 出力ポート「%1」を開けませんでした。</translation>
    </message>
    <message>
        <source>This platform does not support virtual MIDI ports. Install a loopback driver such as loopMIDI and select its port instead.</source>
        <translation>このプラットフォームは仮想 MIDI ポートに対応していません。loopMIDI などのループバックドライバーを導入し、そのポートを選択してください。</translation>
    </message>
    <message>
        <source>Could not create the virtual MIDI port "%1".</source>
        <translation>仮想 MIDI ポート「%1」を作成できませんでした。</translation>
    </message>
    <message>
        <source>%1 (virtual port)</source>
        <translation>%1 (仮想ポート)</translation>
    </message>
</context>
</TS>
