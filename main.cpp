#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <qtwebengineglobal.h>
#include <QDebug>
#include <QSettings>
#include <vector>
#include <map>

#ifdef WIN32
    #include <windows.h>
    #pragma comment(lib, "user32.lib")
#endif

#ifdef WIN32
    class KeyBlocker
    {
        public:

            KeyBlocker()
                :   m_hKeyboardHook()
            {

            }


            /**
              * Destructor
              *     - Prevent leaking the keyboard hook procedure handle
              */
            ~KeyBlocker()
            {
                unblock();
            }

            /**
             * This callback runs on every system wide key press
             * hence we find the onces we're intrested in and
             * disable them
             *
             * @brief LowLevelKeyboardProc
             * @param nCode
             * @param wParam
             * @param lParam
             * @return
             */
            static LRESULT CALLBACK lowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
            {
                // No need to move on unless the below is met
                if (nCode < 0 || nCode != HC_ACTION || wParam != WM_KEYDOWN)
                        return CallNextHookEx(nullptr, nCode, wParam, lParam);

                // Cast to correct object
                PKBDLLHOOKSTRUCT p = reinterpret_cast<PKBDLLHOOKSTRUCT>(lParam);

                // Check for combo left key event
                for(auto w = m_allowedCombos.begin(); w != m_allowedCombos.end(); w++)
                {
                    // Check for combo right key
                    if((w->second == p->vkCode) && (w->first == m_leftKey) && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN ))
                    {
                        // Allow combo
                        return 0;
                    }
                    else if(w->first == p->vkCode && wParam == WM_KEYDOWN) // Check combo left key
                    {
                        // Set left key
                        m_leftKey = p->vkCode;

                        // Allow combo
                        return 0;
                    }
                }

                // Reset left key if it has been lifted
                if(GetAsyncKeyState(static_cast<int>(m_leftKey)))
                {
                    return 1;
                }
                else
                {
                    m_leftKey = 0;
                }

                // Check vector of disabled codes
                for(auto i : m_bannedCodes)
                {
                    // Does the current event matched banned codes?
                    if(p->vkCode == i)
                    {
                        return 1;
                    }
                }

                // Move onto next event
                return CallNextHookEx(nullptr, nCode, wParam, lParam);
            }

            /**
             * Block keyboard hooks
             *
             * @brief block
             */
            void block()
            {
                // Set hook procedure handler with low level keyboard hooks and define call back
                m_hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, &lowLevelKeyboardProc, GetModuleHandle(nullptr), 0);
            }

            /**
             * Unblock keyboard hooks
             *
             * @brief unblock
             */
            void unblock()
            {
                // If there are keyboard hooks procedure handler unhook them
                if(m_hKeyboardHook)
                    UnhookWindowsHookEx(m_hKeyboardHook);

                // Null the hook
                m_hKeyboardHook = nullptr;
            }

        private:
            // Instance of current keyboard hook
            HHOOK m_hKeyboardHook;

            // Banned key events
            static std::vector<unsigned long> m_bannedCodes;

            // Allowed combos
            static std::vector<std::pair<unsigned long, unsigned long>> m_allowedCombos;

            // Combo press current
            static unsigned long m_leftKey;
    };

    // Blocked keys
    std::vector<unsigned long> KeyBlocker::m_bannedCodes = {
        VK_CONTROL, VK_LCONTROL, VK_RCONTROL,
        VK_LWIN, VK_RWIN,
        VK_SNAPSHOT,
        VK_MENU, 162, 165, 164,
        VK_F1, VK_F2, VK_F3, VK_F4, VK_F5, VK_F6, VK_F7, VK_F8, VK_F9, VK_F10,
        VK_F11, VK_F12, VK_F13, VK_F14, VK_F15, VK_F16, VK_F17, VK_F18, VK_F19, VK_F20,
        VK_F21, VK_F22, VK_F23, VK_F24
    };

    // Allowed combos
    std::vector<std::pair<unsigned long, unsigned long>> KeyBlocker::m_allowedCombos = {
        {VK_LCONTROL, 0x43}, // Control + C
        {VK_LCONTROL, 0x56} // Control + V
    };

    // No key combos
    unsigned long KeyBlocker::m_leftKey = 0;
#endif

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    // Merge user + program variables
    int newArgc = argc + 1 + 1;
    char** newArgv = new char*[static_cast<unsigned long long>(newArgc)];
    for(int i = 0; i < argc; i++)
    {
        newArgv[i] = argv[i];
    }

    // Disable web security
    char ARG_DISABLE_WEB_SECURITY[] = "--disable-web-security";
    newArgv[argc] = ARG_DISABLE_WEB_SECURITY;

    // Terminate
    newArgv[argc+1] = nullptr;

    // Start GUI application
    QGuiApplication app(newArgc, newArgv);

    // If windows disable methods of escaping the program
    #ifdef WIN32
        KeyBlocker blocker;
        blocker.block();
    #endif

    // Init web engine for os
    QtWebEngine::initialize();

    // Start QML engine
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    // Run program
    return app.exec();
}

