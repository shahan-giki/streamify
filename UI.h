// ============================================================================
// UI.h — Console Color & Box-Drawing Utilities for Streamify
// Provides ANSI escape codes and helper functions for a rich terminal UI.
// ============================================================================
#ifndef UI_H
#define UI_H

#include <iostream>
#include <string>
#include <windows.h>

// ---------------------------------------------------------------------------
// Enable ANSI escape sequences on Windows 10+
// ---------------------------------------------------------------------------
inline void enableANSI() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}

// ---------------------------------------------------------------------------
// ANSI Color codes
// ---------------------------------------------------------------------------
namespace Color {
    // Text colors
    constexpr const char* RESET   = "\033[0m";
    constexpr const char* BOLD    = "\033[1m";
    constexpr const char* RED     = "\033[91m";
    constexpr const char* GREEN   = "\033[92m";
    constexpr const char* YELLOW  = "\033[93m";
    constexpr const char* BLUE    = "\033[94m";
    constexpr const char* MAGENTA = "\033[95m";
    constexpr const char* CYAN    = "\033[96m";
    constexpr const char* WHITE   = "\033[97m";
    constexpr const char* GRAY    = "\033[90m";

    // Background colors
    constexpr const char* BG_DARK  = "\033[40m";
    constexpr const char* BG_RED   = "\033[41m";
    constexpr const char* BG_GREEN = "\033[42m";
    constexpr const char* BG_BLUE  = "\033[44m";
}

// ---------------------------------------------------------------------------
// Box-drawing characters (UTF-8)
// ---------------------------------------------------------------------------
namespace Box {
    constexpr const char* TL  = "\u250C"; // ┌
    constexpr const char* TR  = "\u2510"; // ┐
    constexpr const char* BL  = "\u2514"; // └
    constexpr const char* BR  = "\u2518"; // ┘
    constexpr const char* H   = "\u2500"; // ─
    constexpr const char* V   = "\u2502"; // │
    constexpr const char* ML  = "\u251C"; // ├
    constexpr const char* MR  = "\u2524"; // ┤
    constexpr const char* TT  = "\u252C"; // ┬
    constexpr const char* BT  = "\u2534"; // ┴
    constexpr const char* DBL_H = "\u2550"; // ═
    constexpr const char* DBL_V = "\u2551"; // ║
    constexpr const char* DBL_TL = "\u2554"; // ╔
    constexpr const char* DBL_TR = "\u2557"; // ╗
    constexpr const char* DBL_BL = "\u255A"; // ╚
    constexpr const char* DBL_BR = "\u255D"; // ╝
}

// ---------------------------------------------------------------------------
// Helper: print a horizontal line of width w using character ch
// ---------------------------------------------------------------------------
inline void printLine(int w, const char* ch = "\u2500", const char* color = Color::GRAY) {
    std::cout << color;
    for (int i = 0; i < w; ++i) std::cout << ch;
    std::cout << Color::RESET << "\n";
}

// ---------------------------------------------------------------------------
// Helper: center a string in a field of given width
// ---------------------------------------------------------------------------
inline std::string center(const std::string& s, int width) {
    int pad = (width - (int)s.size()) / 2;
    if (pad < 0) pad = 0;
    return std::string(pad, ' ') + s;
}

// ---------------------------------------------------------------------------
// Print the main Streamify banner
// ---------------------------------------------------------------------------
inline void printBanner() {
    std::cout << "\n";
    std::cout << Color::CYAN << Color::BOLD;
    std::cout << "  " << Box::DBL_TL;
    for (int i = 0; i < 54; ++i) std::cout << Box::DBL_H;
    std::cout << Box::DBL_TR << "\n";

    std::cout << "  " << Box::DBL_V
              << Color::YELLOW << Color::BOLD
              << center("🎬  S T R E A M I F Y", 54)
              << Color::CYAN << Color::BOLD
              << Box::DBL_V << "\n";

    std::cout << "  " << Box::DBL_V
              << Color::GRAY
              << center("C++ Console Video Streaming Platform", 54)
              << Color::CYAN << Color::BOLD
              << Box::DBL_V << "\n";

    std::cout << "  " << Box::DBL_BL;
    for (int i = 0; i < 54; ++i) std::cout << Box::DBL_H;
    std::cout << Box::DBL_BR << Color::RESET << "\n\n";
}

// ---------------------------------------------------------------------------
// Print a section header (e.g., "=== Creator Menu ===")
// ---------------------------------------------------------------------------
inline void printHeader(const std::string& title, const char* color = Color::CYAN) {
    std::cout << "\n" << color << Color::BOLD;
    std::cout << "  " << Box::TL;
    for (int i = 0; i < 44; ++i) std::cout << Box::H;
    std::cout << Box::TR << "\n";
    std::cout << "  " << Box::V << " " << Color::YELLOW << Color::BOLD
              << title << color << Color::BOLD;
    // pad to box width
    int pad = 43 - (int)title.size();
    if (pad > 0) std::cout << std::string(pad, ' ');
    std::cout << Box::V << "\n";
    std::cout << "  " << Box::BL;
    for (int i = 0; i < 44; ++i) std::cout << Box::H;
    std::cout << Box::BR << Color::RESET << "\n";
}

// ---------------------------------------------------------------------------
// Print a numbered menu option
// ---------------------------------------------------------------------------
inline void printOption(int n, const std::string& label,
                        const char* numColor  = Color::YELLOW,
                        const char* textColor = Color::WHITE) {
    std::cout << "  " << numColor << Color::BOLD << "  [" << n << "]"
              << Color::RESET << " " << textColor << label << Color::RESET << "\n";
}

// ---------------------------------------------------------------------------
// Print a success / error / info message
// ---------------------------------------------------------------------------
inline void ok(const std::string& msg) {
    std::cout << "\n  " << Color::GREEN << Color::BOLD << "✔  " << Color::RESET
              << Color::GREEN << msg << Color::RESET << "\n";
}
inline void err(const std::string& msg) {
    std::cout << "\n  " << Color::RED << Color::BOLD << "✘  " << Color::RESET
              << Color::RED << msg << Color::RESET << "\n";
}
inline void info(const std::string& msg) {
    std::cout << "\n  " << Color::CYAN << "ℹ  " << Color::RESET
              << Color::GRAY << msg << Color::RESET << "\n";
}

// ---------------------------------------------------------------------------
// Prompt
// ---------------------------------------------------------------------------
inline void prompt(const std::string& msg = "Choice") {
    std::cout << "\n  " << Color::MAGENTA << Color::BOLD << "▶  "
              << Color::RESET << Color::WHITE << msg << ": " << Color::CYAN;
}

// ---------------------------------------------------------------------------
// Open a file/video with the default Windows application (VLC, WMP, etc.)
// Uses ShellExecuteW so paths with spaces work correctly.
// ---------------------------------------------------------------------------
inline bool openWithDefaultApp(const std::string& path) {
    std::wstring wpath(path.begin(), path.end());
    HINSTANCE result = ShellExecuteW(
        nullptr,        // parent window
        L"open",        // verb
        wpath.c_str(),  // file
        nullptr,        // parameters
        nullptr,        // directory
        SW_SHOWNORMAL   // show window
    );
    // ShellExecuteW returns >32 on success
    return (reinterpret_cast<intptr_t>(result) > 32);
}

#endif // UI_H
