#include "includes.h"

bool IsProcessRunning(const std::string& processName) {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        return false;
    }

    PROCESSENTRY32 processEntry;
    processEntry.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(hSnapshot, &processEntry)) {
        CloseHandle(hSnapshot);
        return false;
    }

    do {
        std::string currentProcessName = processEntry.szExeFile;
        if (currentProcessName == processName) {
            CloseHandle(hSnapshot);
            return true;
        }
    } while (Process32Next(hSnapshot, &processEntry));

    CloseHandle(hSnapshot);
    return false;
}

void checkStatus() {
    static bool executado = false;

    if (!executado) {
        std::string urlCheckSt = "http://189.78.71.201:8221/BypassDx11/API/status.php";

        std::string responseSt = sendRequest(urlCheckSt);

        if (responseSt.find("API:LOADER_ONLINE") != std::string::npos) {
            login = true;
            error = false;
        }
        else if (responseSt.find("API:LOADER_OFFLINE") != std::string::npos) {
            login = false;
            error = true;
        }
        else {

        }
    }

}

// Main code
int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    mainprotect();

    WNDCLASSEX windowClass = { };
    windowClass.cbSize = sizeof(WNDCLASSEX);
    windowClass.style = CS_CLASSDC;
    windowClass.lpfnWndProc = WndProc;
    windowClass.cbClsExtra = NULL;
    windowClass.cbWndExtra = NULL;
    windowClass.hInstance = GetModuleHandleA(0); // nullptr
    windowClass.hIcon = LoadIcon(0, IDI_APPLICATION);
    windowClass.hCursor = LoadCursor(0, IDC_ARROW);
    windowClass.hbrBackground = nullptr;
    windowClass.lpszMenuName = NULL;
    windowClass.lpszClassName = Window::ClassName;
    windowClass.hIconSm = LoadIcon(0, IDI_APPLICATION);



    RegisterClassEx(&windowClass);
    hwnd = CreateWindowEx(NULL, windowClass.lpszClassName, Window::WindowName, WS_POPUP | CW_USEDEFAULT, (GetSystemMetrics(SM_CXSCREEN) / 2) - (Gui::Size.x / 2), (GetSystemMetrics(SM_CYSCREEN) / 2) - (Gui::Size.y / 2), Gui::Size.x, Gui::Size.y, 0, 0, 0, 0);

    SetWindowLongA(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
    SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 255, LWA_ALPHA);

    MARGINS margins = { -1 };
    DwmExtendFrameIntoClientArea(hwnd, &margins);

    POINT mouse;
    rc = { 0 };
    GetWindowRect(hwnd, &rc);

    //RenderBlur(hwnd);



    // Initialize Direct3D
    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
        return 1;
    }

    if (!Gui::Initialize(g_pd3dDevice))
    {
        CleanupDeviceD3D();
        ::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
        return 1;
    }

    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    // Getting some shutdown stuff
    DWORD pid;
    GetWindowThreadProcessId(hwnd, &pid);
    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pid);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImFontConfig font_config;
    font_config.OversampleH = 8;
    font_config.OversampleV = 8;
    font_config.PixelSnapH = true;
    font_config.GlyphExtraSpacing.x = 1.0f;





    static const ImWchar ranges[] =
    {
        0x0020, 0x00FF, // Basic Latin + Latin Supplement
        0x0400, 0x052F, // Cyrillic + Cyrillic Supplement
        0x2DE0, 0x2DFF, // Cyrillic Extended-A
        0xA640, 0xA69F, // Cyrillic Extended-B
        0xE000, 0xE226, // icons
        0,
    };

    font_config.GlyphRanges = ranges;

    io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\arial.ttf", 12.0f, &font_config, ranges);

    io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\arialbd.ttf", 20.0f, &font_config, ranges);

    io.IniFilename = NULL; // Remove the imgui.ini

    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();

    style.WindowRounding = 5;

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

    AddCustomDefaultFont(nullptr);

    // merge in icons from Font Awesome
    static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
    static const ImWchar icons_ranges_brands[] = { ICON_MIN_FAB, ICON_MAX_16_FAB, 0 };
    ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true;
    ImFont* FontAwesome = io.Fonts->AddFontFromMemoryCompressedTTF(fa6_solid_compressed_data, fa6_solid_compressed_size, 14.f, &icons_config, icons_ranges);
    ImFont* FontAwesomeBrands = io.Fonts->AddFontFromMemoryCompressedTTF(fa_brands_400_compressed_data, fa_brands_400_compressed_size, 14.f, &icons_config, icons_ranges_brands);

    // Main loop
    bool loaderOpen = true;

    checkStatus();

    std::string processName = "sigverif.exe";
    bool isRunning = IsProcessRunning(processName);

    if (isRunning) {
        bool loaderOpen = true;
    }
    else {
        bool loaderOpen = false;
        exit(0);
    }

    if (WinVersion::GetVersion(info) && info.Major <= 6)
    {
        MessageBox(hwnd, "your operating system is not supported", "Error", MB_ICONERROR);
    }
    else while (loaderOpen && (info.Major > 6))
    {
        // Poll and handle messages (inputs, window resize, etc.)
        // See the WndProc() function below for our to dispatch events to the Win32 backend.
        MSG msg;
        while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
            {
                TerminateProcess(hProcess, 0);
                CloseHandle(hProcess);
            }
        }
        if (!loaderOpen)
        {
            break;
        }


        // Start the Dear ImGui frame
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();


        {

            ImGuiWindowFlags main_window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar;
            ImGui::SetNextWindowPos({ 0 , 0 }, ImGuiCond_FirstUseEver);
            ImGui::SetNextWindowSize(Gui::Size);
            ImGui::Begin("Menu", &loaderOpen, main_window_flags);
            UpdateWindowLocation();
            {

                ImGui::SetWindowFontScale(1.5f); // Escala da fonte para a janela

                ImVec2 windowSize = ImGui::GetWindowSize();
                ImVec2 windowPos = ImGui::GetWindowPos();






                if (error) {


                    ImGui::Image((void*)Gui::ImageResource, Gui::SizeImage);


                    MyStyles::ButtonStyle();

                    float buttonXPosX = windowPos.x + (windowSize.x - ImGui::CalcTextSize(ICON_FA_X).x) / 1.05;
                    float buttonXPosY = windowPos.y + windowSize.y / 30;
                    ImGui::SetCursorPos({ buttonXPosX, buttonXPosY });
                    ImVec2 buttonSize(23.0f, 23.0f);

                    ButtonAnimations::HoverAnimation(10.0f);

                    if (ImGui::Button(ICON_FA_X, buttonSize))
                    {
                        exit(0);
                    }

                    MyStyles::PopStyleVars(2);
                    MyStyles::PopStyleColor(6);

                    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);

                    float text2PosX = windowPos.x + (windowSize.x - ImGui::CalcTextSize("Try again later").x) / 2.0;
                    float text2PosY = windowPos.y + windowSize.y / 8;
                    ImGui::SetCursorPos({ text2PosX, text2PosY });
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.3f, 0.3f, 0.3f, 3.5f));
                    ImGui::Text("Try again later", 6.5f);
                    ImGui::PopStyleColor();
                    ImGui::PopFont();

                    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);

                    float textWidth = ImGui::CalcTextSize("LOADER IN UPDATE").x;
                    float textPosX = windowPos.x + (windowSize.x - textWidth) / 2.0f;
                    float textPosY = windowPos.y + windowSize.y / 5.0f;
                    ImGui::SetCursorPos({ textPosX, textPosY });
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 3.5f));
                    ImGui::Text("LOADER IN UPDATE", 6.5f);
                    ImGui::PopStyleColor();

                    ImGui::PopFont();

                    float spinnerSize = 50.0f;
                    float spinnerPosX = windowPos.x + (windowSize.x - spinnerSize) / 2.4f;
                    float spinnerPosY = windowPos.y + (windowSize.y - spinnerSize) / 1.7f - ImGui::GetTextLineHeightWithSpacing() * 1.5f;

                    spinnerPosX += (spinnerSize / 2.0f) - ImGui::GetTextLineHeightWithSpacing();

                    ImGui::SetCursorPos({ spinnerPosX, spinnerPosY });
                    ImSpinner::SpinnerRainbow("Meu Spinner", spinnerSize, 2.0f, ImColor(1.0f, 0.0f, 0.0f), 1.0f);

                    ImGui::Dummy(ImVec2(0.0f, spinnerSize));

                }

                if (login) {



                    ImGui::Image((void*)Gui::ImageResource, Gui::SizeImage);

                    MyStyles::ButtonStyle();

                    float buttonXPosX = windowPos.x + (windowSize.x - ImGui::CalcTextSize(ICON_FA_X).x) / 1.05;
                    float buttonXPosY = windowPos.y + windowSize.y / 30;
                    ImGui::SetCursorPos({ buttonXPosX, buttonXPosY });
                    ImVec2 buttonSize(23.0f, 23.0f);

                    ButtonAnimations::HoverAnimation(10.0f);

                    if (ImGui::Button(ICON_FA_X, buttonSize))
                    {
                        exit(0);
                    }

                    MyStyles::PopStyleVars(2);
                    MyStyles::PopStyleColor(6);

                    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]); // define a fonte para "MinhaFonte"

                    float text2PosX = windowPos.x + (windowSize.x - ImGui::CalcTextSize("Please log in to continue").x) / 1.9;
                    float text2PosY = windowPos.y + windowSize.y / 8;
                    ImGui::SetCursorPos({ text2PosX, text2PosY });
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.3f, 0.3f, 0.3f, 3.5f));
                    ImGui::Text("Please log in to continue", 6.5f);
                    ImGui::PopStyleColor();
                    ImGui::PopFont(); // restaura a fonte anterior



                    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]); // define a fonte para "MinhaFonte"

                    float textPosX = windowPos.x + (windowSize.x - ImGui::CalcTextSize("LaBypass").x) / 1.9;
                    float textPosY = windowPos.y + windowSize.y / 5;
                    ImGui::SetCursorPos({ textPosX, textPosY });
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 3.5f));


                    ImGui::Text("LaBypass", 6.5f);
                    ImGui::PopStyleColor();



                    ImGui::PopFont(); // restaura a fonte anterior

                    MyStyles::InputTextStyle();
                    float inputPosX = windowPos.x + windowSize.x / 5;
                    float inputPosY = windowPos.y + windowSize.y / 2 - 30;
                    ImGui::SetCursorPos({ inputPosX, inputPosY });
                    ImGui::InputText("##1", username, IM_ARRAYSIZE(username));
                    user = username;


                    float passwordPosX = windowPos.x + windowSize.x / 5;
                    float passwordPosY = windowPos.y + windowSize.y / 2 + 10;
                    ImGui::SetCursorPos({ passwordPosX, passwordPosY });
                    ImGui::InputText("##", password, IM_ARRAYSIZE(password));
                    MyStyles::PopStyleVars(2);
                    MyStyles::PopStyleColor(5);
                    pass = password;


                    MyStyles::ButtonStyle();
                    float buttonPosX = windowPos.x + (windowSize.x - ImGui::CalcTextSize("Login").x) / 4.4;
                    float buttonPosY = windowPos.y + windowSize.y / 2 + 50;
                    ImGui::SetCursorPos({ buttonPosX, buttonPosY });

                    ButtonAnimations::HoverAnimation(10.0f);



                    if (ImGui::Button("Login", ImVec2(260, 0)))
                    {

                        std::string urlCheck = "http://189.78.71.201:8221/BypassDx11/API/function.php?action=accessAccount&username=" + user + "&password=" + pass;

                        std::string response = sendRequest(urlCheck);

                        if (response.find("LOGIN_GOOD:LOGGED_IN") != std::string::npos) {

                            std::string hwid = GetHWID();

                            std::string urlCheck2 = "http://189.78.71.201:8221/BypassDx11/API/function.php?action=checkHwid&username=" + user + "&hwid=" + hwid;


                            std::string response2 = sendRequest(urlCheck2);
                            if (response2.find("API:HWID_OK") != std::string::npos) {



                                std::string urlCheckBan = "http://189.78.71.201:8221/BypassDx11/API/function.php?action=banCheck&username=" + user;

                                std::string responseBan = sendRequest(urlCheckBan);

                                if (responseBan.find("API:USER_NOT_BANNED") != std::string::npos) {


                                    

                                    std::string hwid = GetHWID();
                                    std::string ip = real_ip();


                                    std::string sendDiscord = "http://189.78.71.201:8221/BypassDx11/API/function.php?action=sendDiscord&username=" + user + "&hwid=" + hwid + "&ip=" + ip;

                                    std::string responseDiscord = sendRequest(sendDiscord);
                                    if (responseDiscord.find("API:INFORMATION_SEND") != std::string::npos) {



                                        std::string hwid = GetHWID();
                                        std::string ip = real_ip();
                                        std::string cpu = GetProcessorInfo();
                                        std::string gpu = GetGPUInfo();
                                        std::string mb = GetMotherboardInfo();
                                        
                                        std::string sendInfo = "http://189.78.71.201:8221/BypassDx11/API/function.php?action=createInfo&username=" + user + "&hwid=" + hwid + "&ip=" + ip + "&cpu=" + cpu + "&gpu=" + gpu + "&mb=" + mb;

                                        std::string responseInfo = sendRequest(sendInfo);
                                        if (responseInfo.find("API:INFORMATION_SEND") != std::string::npos) {

                                            // Kernel Mode DEBUG TESTES


                                            login = false;
                                            menu = true;

                                        }
                                        else if (responseInfo.find("API:MISSING_PARAMETERS") != std::string::npos) {
                                            login = false;
                                            apiError = true;
                                        }
                                        else {
                                            login = false;
                                            apiError = true;
                                        }

                                    }
                                    else if (responseDiscord.find("API:MISSING_PARAMETERS") != std::string::npos) {
                                        login = false;
                                        apiError = true;
                                    }
                                    else {
                                        login = false;
                                        apiError = true;
                                    }

                                }
                                else if (responseBan.find("API:USER_BANNED") != std::string::npos) {
                                    loaderBan = true;
                                    login = false;

                                    
                                }
                                else {
                                    login = false;
                                    apiError = true;
                                }


                            }
                            else if (response2.find("API:HWID_CHANGE") != std::string::npos) { 

                                hwidBan = true;
                                login = false;

                                

                                std::string hwid = GetHWID();
                                std::string ip = real_ip();
                                std::string cpu = GetProcessorInfo();
                                std::string gpu = GetGPUInfo();
                                std::string mb = GetMotherboardInfo();

                                std::string sendDiscord2 = "http://189.78.71.201:8221/BypassDx11/API/function.php?action=sendDiscord2&username=" + user + "&hwid=" + hwid + "&ip=" + ip + "&cpu=" + cpu + "&gpu=" + gpu + "&mb=" + mb;

                                std::string responseDiscord2 = sendRequest(sendDiscord2);
                                if (responseDiscord2.find("API:INFORMATION_SEND") != std::string::npos){
                                }
                                else if (responseDiscord2.find("API:MISSING_PARAMETERS") != std::string::npos) {
                                    login = false;
                                    apiError = true;
                                }
                                else {
                                    login = false;
                                    apiError = true;
                                }
                            
                            }
                            else {
                                login = false;
                                apiError = true;
                            }

                        }
                        else if (response.find("API:INVALID_CREDENTIALS") != std::string::npos) {
                            login = false;
                            invalidLogin = true;
                        }
                        else {
                            login = false;
                            apiError = true;
                        }


                    }
                    MyStyles::PopStyleVars(2);
                    MyStyles::PopStyleColor(6);

                }



                if (menu) {
                    ImGui::Image((void*)Gui::ImageResource, Gui::SizeImage);


                    MyStyles::ButtonStyle();

                    float buttonXPosX = windowPos.x + (windowSize.x - ImGui::CalcTextSize(ICON_FA_X).x) / 1.05;
                    float buttonXPosY = windowPos.y + windowSize.y / 30;
                    ImGui::SetCursorPos({ buttonXPosX, buttonXPosY });
                    ImVec2 buttonSize(23.0f, 23.0f);

                    ButtonAnimations::HoverAnimation(10.0f);

                    if (ImGui::Button(ICON_FA_X, buttonSize))
                    {
                        exit(0);
                    }

                    MyStyles::PopStyleVars(2);
                    MyStyles::PopStyleColor(6);

                    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
                    float textPosX = windowPos.x + (windowSize.x - ImGui::CalcTextSize("LaBypass").x) / 2.0;
                    float textPosY = windowPos.y + windowSize.y / 8;
                    ImGui::SetCursorPos({ textPosX, textPosY });
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 3.5f));
                    ImGui::Text("LaBypass", 6.5f);

                    ImGui::PopStyleColor();
                    ImGui::PopFont(); 

                    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);



                    ImVec4 redColor(1.0f, 0.0f, 0.0f, 1.0f);  
                    ImVec4 clearColor(0.0f, 0.0f, 0.0f, 0.0f);

                    ImGuiStyle& style = ImGui::GetStyle();
                    style.Colors[ImGuiCol_Button] = clearColor;
                    style.Colors[ImGuiCol_ButtonHovered] = clearColor;
                    style.Colors[ImGuiCol_ButtonActive] = clearColor;

                    ImGui::PushStyleColor(ImGuiCol_Text, redColor);

                    float buttonWidth = 40.0f;  
                    float buttonHeight = 40.0f; 
                    float buttonMargin = 30.0f;

                    float buttonPosX = windowPos.x + buttonMargin;                    
                    float buttonPosY = windowPos.y + (windowSize.y - buttonHeight) / 2.0f;

                    ImGui::SetCursorPos({ buttonPosX, buttonPosY });

                    if (ImGui::Button("<", ImVec2(buttonWidth, buttonHeight))) {
                        if (skript) {
                            skript = false;
                            project = true;
                        }
                        else {
                            project = false;
                            skript = true;
                        }
                    }

                    float button2PosX = windowPos.x + windowSize.x - buttonWidth - buttonMargin;
                    float button2PosY = windowPos.y + (windowSize.y - buttonHeight) / 2.0f;

                    ImGui::SetCursorPos({ button2PosX, button2PosY });

                    if (ImGui::Button(">", ImVec2(buttonWidth, buttonHeight))) {
                        if (skript) {
                            skript = false;
                            project = true;
                        }
                        else {
                            project = false;
                            skript = true;
                        }
                    }


                    ImGui::PopStyleColor();

                    if (skript) {
                        float text2PosX = windowPos.x + (windowSize.x - ImGui::CalcTextSize("Skript").x) / 2.0;
                        float text2PosY = windowPos.y + windowSize.y / 4;
                        ImGui::SetCursorPos({ text2PosX, text2PosY });
                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.3f, 0.3f, 0.3f, 3.5f));
                        ImGui::Text("Skript", 6.5f);
                        ImGui::PopStyleColor();
                        ImGui::PopFont();
                        float imagePosX = windowPos.x + (windowSize.x - 250) / 2;
                        float imagePosY = windowPos.y + (windowSize.y - 130) / 2;
                        ImGui::SetCursorPos({ imagePosX, imagePosY });
                        ImGui::Image((void*)Gui::ImageResource3, Gui::SizeBanner);

                        ImVec2 rectMin = ImGui::GetItemRectMin();
                        ImVec2 rectMax = ImGui::GetItemRectMax();
                        float rounding = 5.0f;
                        ImGui::GetWindowDrawList()->AddRect(rectMin, rectMax, ImColor(1.0f, 0.0f, 0.0f), rounding, ImDrawCornerFlags_All, 3.0f);

                        MyStyles::ButtonStyle();
                        float buttonPosX = windowPos.x + (windowSize.x - ImGui::CalcTextSize("Injetar").x) / 4.5;
                        float buttonPosY = imagePosY + 130.0f + 40.0f;
                        ImGui::SetCursorPos({ buttonPosX, buttonPosY });

                        ButtonAnimations::HoverAnimation(10.0f);



                        if (ImGui::Button("Injetar", ImVec2(250, 0))) {
                            injectS = true;
                            menu = false;

                        }

                        MyStyles::PopStyleVars(2);
                        MyStyles::PopStyleColor(6);
                    }

                    if (project) {
                        float text2PosX = windowPos.x + (windowSize.x - ImGui::CalcTextSize("Project Cheats").x) / 2.0;
                        float text2PosY = windowPos.y + windowSize.y / 4;
                        ImGui::SetCursorPos({ text2PosX, text2PosY });
                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.3f, 0.3f, 0.3f, 3.5f));
                        ImGui::Text("Project Cheats", 6.5f);
                        ImGui::PopStyleColor();
                        ImGui::PopFont();
                        float imagePosX = windowPos.x + (windowSize.x - 250) / 2;
                        float imagePosY = windowPos.y + (windowSize.y - 130) / 2;
                        ImGui::SetCursorPos({ imagePosX, imagePosY });
                        ImGui::Image((void*)Gui::ImageResource2, Gui::SizeBanner);

                        ImVec2 rectMin = ImGui::GetItemRectMin();
                        ImVec2 rectMax = ImGui::GetItemRectMax();
                        float rounding = 5.0f;
                        ImGui::GetWindowDrawList()->AddRect(rectMin, rectMax, ImColor(1.0f, 0.0f, 0.0f), rounding, ImDrawCornerFlags_All, 3.0f);

                        MyStyles::ButtonStyle();
                        float buttonPosX = windowPos.x + (windowSize.x - ImGui::CalcTextSize("Injetar").x) / 4.5;
                        float buttonPosY = imagePosY + 130.0f + 40.0f;
                        ImGui::SetCursorPos({ buttonPosX, buttonPosY });

                        ButtonAnimations::HoverAnimation(10.0f);



                        if (ImGui::Button("Injetar", ImVec2(250, 0))) {
                            injectP = true;
                            menu = false;

                        }

                        MyStyles::PopStyleVars(2);
                        MyStyles::PopStyleColor(6);

                    }


                }

                if (injectS) {
                    ImGui::Image((void*)Gui::ImageResource, Gui::SizeImage);

                    MyStyles::ButtonStyle();

                    float buttonXPosX = windowPos.x + (windowSize.x - ImGui::CalcTextSize(ICON_FA_X).x) / 1.05;
                    float buttonXPosY = windowPos.y + windowSize.y / 30;
                    ImGui::SetCursorPos({ buttonXPosX, buttonXPosY });
                    ImVec2 buttonSize(23.0f, 23.0f);

                    ButtonAnimations::HoverAnimation(10.0f);

                    if (ImGui::Button(ICON_FA_X, buttonSize))
                    {
                        exit(0);
                    }

                    MyStyles::PopStyleVars(2);
                    MyStyles::PopStyleColor(6);

                    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);

                    float text2PosX = windowPos.x + (windowSize.x - ImGui::CalcTextSize("Injecting...").x) / 2.0;
                    float text2PosY = windowPos.y + windowSize.y / 8;
                    ImGui::SetCursorPos({ text2PosX, text2PosY });
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.3f, 0.3f, 0.3f, 3.5f));
                    ImGui::Text("Injecting...", 6.5f);
                    ImGui::PopStyleColor();
                    ImGui::PopFont();

                    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);

                    float textWidth = ImGui::CalcTextSize("SKRIPT").x;
                    float textPosX = windowPos.x + (windowSize.x - textWidth) / 2.0f;
                    float textPosY = windowPos.y + windowSize.y / 5.0f;
                    ImGui::SetCursorPos({ textPosX, textPosY });
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 3.5f));
                    ImGui::Text("SKRIPT", 6.5f);
                    ImGui::PopStyleColor();

                    ImGui::PopFont();

                    float spinnerSize = 50.0f;
                    float spinnerPosX = windowPos.x + (windowSize.x - spinnerSize) / 2.4f;
                    float spinnerPosY = windowPos.y + (windowSize.y - spinnerSize) / 1.7f - ImGui::GetTextLineHeightWithSpacing() * 1.5f;

                    spinnerPosX += (spinnerSize / 2.0f) - ImGui::GetTextLineHeightWithSpacing();

                    ImGui::SetCursorPos({ spinnerPosX, spinnerPosY });
                    ImSpinner::SpinnerRainbow("Meu Spinner", spinnerSize, 2.0f, ImColor(1.0f, 0.0f, 0.0f), 1.0f);

                    ImGui::Dummy(ImVec2(0.0f, spinnerSize));

                    std::thread actionsThread(ExecuteBypassSkript);

                    actionsThread.detach();
                }

                if (injectP) {
                    ImGui::Image((void*)Gui::ImageResource, Gui::SizeImage);

                    MyStyles::ButtonStyle();

                    float buttonXPosX = windowPos.x + (windowSize.x - ImGui::CalcTextSize(ICON_FA_X).x) / 1.05;
                    float buttonXPosY = windowPos.y + windowSize.y / 30;
                    ImGui::SetCursorPos({ buttonXPosX, buttonXPosY });
                    ImVec2 buttonSize(23.0f, 23.0f);

                    ButtonAnimations::HoverAnimation(10.0f);

                    if (ImGui::Button(ICON_FA_X, buttonSize))
                    {
                        exit(0);
                    }

                    MyStyles::PopStyleVars(2);
                    MyStyles::PopStyleColor(6);

                    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);

                    float text2PosX = windowPos.x + (windowSize.x - ImGui::CalcTextSize("Injecting...").x) / 2.0;
                    float text2PosY = windowPos.y + windowSize.y / 8;
                    ImGui::SetCursorPos({ text2PosX, text2PosY });
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.3f, 0.3f, 0.3f, 3.5f));
                    ImGui::Text("Injecting...", 6.5f);
                    ImGui::PopStyleColor();
                    ImGui::PopFont();

                    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);

                    float textWidth = ImGui::CalcTextSize("PROJECT").x;
                    float textPosX = windowPos.x + (windowSize.x - textWidth) / 2.0f;
                    float textPosY = windowPos.y + windowSize.y / 5.0f;
                    ImGui::SetCursorPos({ textPosX, textPosY });
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 3.5f));
                    ImGui::Text("PROJECT", 6.5f);
                    ImGui::PopStyleColor();

                    ImGui::PopFont();

                    float spinnerSize = 50.0f;
                    float spinnerPosX = windowPos.x + (windowSize.x - spinnerSize) / 2.4f;
                    float spinnerPosY = windowPos.y + (windowSize.y - spinnerSize) / 1.7f - ImGui::GetTextLineHeightWithSpacing() * 1.5f;

                    spinnerPosX += (spinnerSize / 2.0f) - ImGui::GetTextLineHeightWithSpacing();

                    ImGui::SetCursorPos({ spinnerPosX, spinnerPosY });
                    ImSpinner::SpinnerRainbow("Meu Spinner", spinnerSize, 2.0f, ImColor(1.0f, 0.0f, 0.0f), 1.0f);

                    ImGui::Dummy(ImVec2(0.0f, spinnerSize));
                }

                if (hwidBan)
                {


                    ImGui::Image((void*)Gui::ImageResource, Gui::SizeImage);

                    MyStyles::ButtonStyle();

                    float buttonXPosX = windowPos.x + (windowSize.x - ImGui::CalcTextSize(ICON_FA_X).x) / 1.05;
                    float buttonXPosY = windowPos.y + windowSize.y / 30;
                    ImGui::SetCursorPos({ buttonXPosX, buttonXPosY });
                    ImVec2 buttonSize(23.0f, 23.0f);

                    ButtonAnimations::HoverAnimation(10.0f);

                    if (ImGui::Button(ICON_FA_X, buttonSize))
                    {
                        hwidBan = false;
                        login = true;
                    }

                    MyStyles::PopStyleVars(2);
                    MyStyles::PopStyleColor(6);

                    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);

                    float text2PosX = windowPos.x + (windowSize.x - ImGui::CalcTextSize("Open ticket").x) / 2.0;
                    float text2PosY = windowPos.y + windowSize.y / 8;
                    ImGui::SetCursorPos({ text2PosX, text2PosY });
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.3f, 0.3f, 0.3f, 3.5f));
                    ImGui::Text("Open ticket", 6.5f);
                    ImGui::PopStyleColor();
                    ImGui::PopFont();

                    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);

                    float textWidth = ImGui::CalcTextSize("HWID BAN").x;
                    float textPosX = windowPos.x + (windowSize.x - textWidth) / 2.0f;
                    float textPosY = windowPos.y + windowSize.y / 5.0f;
                    ImGui::SetCursorPos({ textPosX, textPosY });
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 3.5f));
                    ImGui::Text("HWID BAN", 6.5f);
                    ImGui::PopStyleColor();

                    ImGui::PopFont();

                    float spinnerSize = 50.0f;
                    float spinnerPosX = windowPos.x + (windowSize.x - spinnerSize) / 2.4f;
                    float spinnerPosY = windowPos.y + (windowSize.y - spinnerSize) / 1.7f - ImGui::GetTextLineHeightWithSpacing() * 1.5f;

                    spinnerPosX += (spinnerSize / 2.0f) - ImGui::GetTextLineHeightWithSpacing();

                    ImGui::SetCursorPos({ spinnerPosX, spinnerPosY });
                    ImSpinner::SpinnerRainbow("Meu Spinner", spinnerSize, 2.0f, ImColor(1.0f, 0.0f, 0.0f), 1.0f);

                    ImGui::Dummy(ImVec2(0.0f, spinnerSize));
                }

                if (loaderBan)
                {
                    ImGui::Image((void*)Gui::ImageResource, Gui::SizeImage);

                    MyStyles::ButtonStyle();

                    float buttonXPosX = windowPos.x + (windowSize.x - ImGui::CalcTextSize(ICON_FA_X).x) / 1.05;
                    float buttonXPosY = windowPos.y + windowSize.y / 30;
                    ImGui::SetCursorPos({ buttonXPosX, buttonXPosY });
                    ImVec2 buttonSize(23.0f, 23.0f);

                    ButtonAnimations::HoverAnimation(10.0f);

                    if (ImGui::Button(ICON_FA_X, buttonSize))
                    {
                        loaderBan = false;
                        login = true;
                    }

                    MyStyles::PopStyleVars(2);
                    MyStyles::PopStyleColor(6);

                    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);

                    float text2PosX = windowPos.x + (windowSize.x - ImGui::CalcTextSize("Open ticket").x) / 2.0;
                    float text2PosY = windowPos.y + windowSize.y / 8;
                    ImGui::SetCursorPos({ text2PosX, text2PosY });
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.3f, 0.3f, 0.3f, 3.5f));
                    ImGui::Text("Open ticket", 6.5f);
                    ImGui::PopStyleColor();
                    ImGui::PopFont();

                    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);

                    float textWidth = ImGui::CalcTextSize("USER BAN").x;
                    float textPosX = windowPos.x + (windowSize.x - textWidth) / 2.0f;
                    float textPosY = windowPos.y + windowSize.y / 5.0f;
                    ImGui::SetCursorPos({ textPosX, textPosY });
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 3.5f));
                    ImGui::Text("USER BAN", 6.5f);
                    ImGui::PopStyleColor();

                    ImGui::PopFont();

                    float spinnerSize = 50.0f;
                    float spinnerPosX = windowPos.x + (windowSize.x - spinnerSize) / 2.4f;
                    float spinnerPosY = windowPos.y + (windowSize.y - spinnerSize) / 1.7f - ImGui::GetTextLineHeightWithSpacing() * 1.5f;

                    spinnerPosX += (spinnerSize / 2.0f) - ImGui::GetTextLineHeightWithSpacing();

                    ImGui::SetCursorPos({ spinnerPosX, spinnerPosY });
                    ImSpinner::SpinnerRainbow("Meu Spinner", spinnerSize, 2.0f, ImColor(1.0f, 0.0f, 0.0f), 1.0f);

                    ImGui::Dummy(ImVec2(0.0f, spinnerSize));
                }

                if (invalidLogin)
                {
                    ImGui::Image((void*)Gui::ImageResource, Gui::SizeImage);

                    MyStyles::ButtonStyle();

                    float buttonXPosX = windowPos.x + (windowSize.x - ImGui::CalcTextSize(ICON_FA_X).x) / 1.05;
                    float buttonXPosY = windowPos.y + windowSize.y / 30;
                    ImGui::SetCursorPos({ buttonXPosX, buttonXPosY });
                    ImVec2 buttonSize(23.0f, 23.0f);

                    ButtonAnimations::HoverAnimation(10.0f);

                    if (ImGui::Button(ICON_FA_X, buttonSize))
                    {
                        invalidLogin = false;
                        login = true;
                    }

                    MyStyles::PopStyleVars(2);
                    MyStyles::PopStyleColor(6);

                    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);

                    float text2PosX = windowPos.x + (windowSize.x - ImGui::CalcTextSize("Try again").x) / 2.0;
                    float text2PosY = windowPos.y + windowSize.y / 8;
                    ImGui::SetCursorPos({ text2PosX, text2PosY });
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.3f, 0.3f, 0.3f, 3.5f));
                    ImGui::Text("Try again", 6.5f);
                    ImGui::PopStyleColor();
                    ImGui::PopFont();

                    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);

                    float textWidth = ImGui::CalcTextSize("USER WRONG").x;
                    float textPosX = windowPos.x + (windowSize.x - textWidth) / 2.0f;
                    float textPosY = windowPos.y + windowSize.y / 5.0f;
                    ImGui::SetCursorPos({ textPosX, textPosY });
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 3.5f));
                    ImGui::Text("USER WRONG", 6.5f);
                    ImGui::PopStyleColor();

                    ImGui::PopFont();

                    float spinnerSize = 50.0f;
                    float spinnerPosX = windowPos.x + (windowSize.x - spinnerSize) / 2.4f;
                    float spinnerPosY = windowPos.y + (windowSize.y - spinnerSize) / 1.7f - ImGui::GetTextLineHeightWithSpacing() * 1.5f;

                    spinnerPosX += (spinnerSize / 2.0f) - ImGui::GetTextLineHeightWithSpacing();

                    ImGui::SetCursorPos({ spinnerPosX, spinnerPosY });
                    ImSpinner::SpinnerRainbow("Meu Spinner", spinnerSize, 2.0f, ImColor(1.0f, 0.0f, 0.0f), 1.0f);

                    ImGui::Dummy(ImVec2(0.0f, spinnerSize));
                }

                if (apiError) {
                    ImGui::Image((void*)Gui::ImageResource, Gui::SizeImage);

                    MyStyles::ButtonStyle();

                    float buttonXPosX = windowPos.x + (windowSize.x - ImGui::CalcTextSize(ICON_FA_X).x) / 1.05;
                    float buttonXPosY = windowPos.y + windowSize.y / 30;
                    ImGui::SetCursorPos({ buttonXPosX, buttonXPosY });
                    ImVec2 buttonSize(23.0f, 23.0f);

                    ButtonAnimations::HoverAnimation(10.0f);

                    if (ImGui::Button(ICON_FA_X, buttonSize))
                    {
                        apiError = false;
                        login = true;
                    }

                    MyStyles::PopStyleVars(2);
                    MyStyles::PopStyleColor(6);

                    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);

                    float text2PosX = windowPos.x + (windowSize.x - ImGui::CalcTextSize("Try again").x) / 2.0;
                    float text2PosY = windowPos.y + windowSize.y / 8;
                    ImGui::SetCursorPos({ text2PosX, text2PosY });
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.3f, 0.3f, 0.3f, 3.5f));
                    ImGui::Text("Try again", 6.5f);
                    ImGui::PopStyleColor();
                    ImGui::PopFont();

                    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);

                    float textWidth = ImGui::CalcTextSize("API ERROR").x;
                    float textPosX = windowPos.x + (windowSize.x - textWidth) / 2.0f;
                    float textPosY = windowPos.y + windowSize.y / 5.0f;
                    ImGui::SetCursorPos({ textPosX, textPosY });
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 3.5f));
                    ImGui::Text("API ERROR", 6.5f);
                    ImGui::PopStyleColor();

                    ImGui::PopFont();

                    float spinnerSize = 50.0f;
                    float spinnerPosX = windowPos.x + (windowSize.x - spinnerSize) / 2.4f;
                    float spinnerPosY = windowPos.y + (windowSize.y - spinnerSize) / 1.7f - ImGui::GetTextLineHeightWithSpacing() * 1.5f;

                    spinnerPosX += (spinnerSize / 2.0f) - ImGui::GetTextLineHeightWithSpacing();

                    ImGui::SetCursorPos({ spinnerPosX, spinnerPosY });
                    ImSpinner::SpinnerRainbow("Meu Spinner", spinnerSize, 2.0f, ImColor(1.0f, 0.0f, 0.0f), 1.0f);

                    ImGui::Dummy(ImVec2(0.0f, spinnerSize));
                }

                ImGui::End();

            }



        }
    
    

        // Rendering
        ImGui::Render();
       
        const float clear_color_with_alpha[4] = { 0 };
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        g_pSwapChain->Present(1, 0); // Never set the 1 to 0 or the panel movment will get fucked
    }
    
    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);

    return 0;
}

bool CreateDeviceD3D(HWND hWnd)
{
    // Setup swap chain
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    //createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext) != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = NULL; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = NULL; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
}

void CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void CleanupRenderTarget()
{
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = NULL; }
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
        {
            CleanupRenderTarget();
            g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
            CreateRenderTarget();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}