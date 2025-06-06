enum class KeyCode : int {
    // Lettres A-Z
    A = 65, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,

    // Chiffres 0-9
    Num0 = 48, Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9,

    // Touches spéciales
    Space = 32, Enter = 13, Escape = 27, Tab = 9, Backspace = 8,
    Delete = 46, Insert = 45, Home = 36, End = 35, PageUp = 33, PageDown = 34,

    // Flèches
    Left = 37, Right = 39, Up = 38, Down = 40,

    // Touches F1-F12
    F1 = 112, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,

    // Modificateurs
    Shift = 16, LeftShift = 160, RightShift = 161,
    Control = 17, LeftControl = 162, RightControl = 163,
    Alt = 18, LeftAlt = 164, RightAlt = 165,
    Windows = 91, LeftWindows = 91, RightWindows = 92,

    // Pavé numérique
    NumPad0 = 96, NumPad1, NumPad2, NumPad3, NumPad4,
    NumPad5, NumPad6, NumPad7, NumPad8, NumPad9,
    NumPadMultiply = 106, NumPadAdd = 107, NumPadSubtract = 109,
    NumPadDivide = 111, NumPadDecimal = 110, NumPadEnter = 13, NumLock = 144,

    // Verrouillages
    CapsLock = 20, ScrollLock = 145,

    // Symboles
    Semicolon = 186, Equal = 187, Comma = 188, Minus = 189,
    Period = 190, Slash = 191, Backtick = 192,
    LeftBracket = 219, Backslash = 220, RightBracket = 221, Quote = 222,

    // Multimédia
    VolumeUp = 175, VolumeDown = 174, VolumeMute = 173,
    MediaNextTrack = 176, MediaPrevTrack = 177, MediaStop = 178, MediaPlayPause = 179,

    // Divers
    Print = 42, PrintScreen = 44, Pause = 19, Menu = 93, Sleep = 95,

    // Souris
    MouseLeft = 1, MouseRight = 2, MouseMiddle = 4, MouseX1 = 5, MouseX2 = 6
};
