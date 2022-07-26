#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

HWND Boton_GUARDAR;
HWND Boton_ABRIR;
HWND Boton_LIMPIAR;
HWND Edit;
#define ID_BUTTON_GUARDAR 1001
#define ID_BUTTON_ABRIR 1002
#define ID_BUTTON_LIMPIAR 1003
#define ID_OUTBOX 1004
wchar_t buffer[256];

void manejo_de_archivos()
{
    ofstream archivo;
    archivo.open("ejemplo.txt");
    archivo << "Escribiendo en el archivo. \n";
    archivo.close();

    string line;
    ifstream myfile;
    myfile.open("ejemplo.txt");
    if (myfile.is_open())
    {
        /*myfile >> line;*/
        while (getline(myfile, line))
        {
            cout << line << '\n';
        }
        myfile.close();
    }
    else
    {
        cout << "Unable to open file" << endl;
    }
}
void manejo_de_archivos_binario()
{
    fstream Archivo;
    struct nodo
    {
        int a;
        char b[12];
    };
    nodo* actual = new nodo;
    actual->a = 34;
    strcpy_s(actual->b, 12, "Hola mundo!");

    Archivo.open("Archivo.bin", ios::out | ios::binary);
    if (Archivo.is_open())
    {
        Archivo.write(reinterpret_cast<char*>(actual), sizeof(nodo));
    }
    else
    {
        cout << "Unable to open file" << endl;
    }
    Archivo.close();

    Archivo.open("Archivo.bin", ios::in | ios::binary);
    if (Archivo.is_open())
    {
        int size = Archivo.tellg();

        int i = 0;
        nodo* Temp = new nodo;
        Archivo.seekg(i * sizeof(nodo));
        Archivo.read(reinterpret_cast<char*>(Temp), sizeof(nodo));

        nodo lista;
        lista.a = Temp->a;
        strcpy_s(lista.b, 12, Temp->b);
        delete Temp;
    }
    else
    {
        cout << "Unable to open file" << endl;
    }
    Archivo.close();
}
void manejo_de_archivos_wchar_t()
{
    wofstream archivo;
    archivo.open("ejemplo.txt");
    wchar_t variable[20] = L"Hola mundo";
    archivo << variable;
    archivo.close();

    wstring line;
    wifstream myfile;
    myfile.open("ejemplo.txt");
    if (myfile.is_open())
    {
        while (getline(myfile, line))
        {
            wcout << line << '\n';
        }
        myfile.close();
    }
    else
    {
        cout << "Unable to open file" << endl;
    }
}

void display_file(LPWSTR path) 
{
    size_t i;
    char buffer[256];
    ZeroMemory(buffer, 256);
    wcstombs_s(&i, buffer, 256, path, 256 - 1);

    FILE* file;
    errno_t err;
    err = fopen_s(&file, buffer, "r");

    if (err == 0) 
    {
        fseek(file, 0, SEEK_END);
        int _size = ftell(file);
        rewind(file);
        char* data = new char[_size + 1];
        fread(data, _size, _size, file);
        data[_size] = '\0';
        fclose(file);

        wchar_t wtext[256];
        ZeroMemory(wtext, 256);
        size_t len;
        mbstowcs_s(&len, wtext, 256, data, strlen(data) + 1);
        LPWSTR ptr = wtext;

        SetWindowText(Edit, ptr);
    }
}

void funcion_abrir(HWND padre)
{
    OPENFILENAME openfilename;

    TCHAR szFile[MAX_PATH + 1];
    ZeroMemory(szFile, sizeof(szFile));

    ZeroMemory(&openfilename, sizeof(OPENFILENAME));
    openfilename.lStructSize = sizeof(OPENFILENAME);
    openfilename.hwndOwner = padre;
    openfilename.lpstrFile = szFile;
    openfilename.nMaxFile = MAX_PATH;
    openfilename.nFilterIndex = 1;
    openfilename.lpstrFileTitle = NULL;
    openfilename.nMaxFileTitle = 0;
    openfilename.lpstrInitialDir = NULL;
    openfilename.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

    if (GetOpenFileName(&openfilename)) 
    {
        display_file(openfilename.lpstrFile/*, paramEdit*/);
    }
    else 
    {
        MessageBox(NULL, TEXT("Could not open this file"), TEXT("File I/O Error"), MB_ICONSTOP);
    }
}

void write_file(LPWSTR path) 
{
    char buffer[256];
    ZeroMemory(buffer, 256);
    size_t i;
    wcstombs_s(&i, buffer, 256, path, 256 - 1);

    FILE* file;
    errno_t err;
    err = fopen_s(&file, buffer, "w");

    if (err == 0) 
    {
        int size = GetWindowTextLength(Edit);
        LPSTR _data = new char[size + 1];
        GetWindowTextA(Edit, _data, size+1);

        fwrite(_data, size+1, 1, file);
        fclose(file);
    }
}

void funcion_guardar(HWND padre)
{
    OPENFILENAME openfilename;

    TCHAR szFile[MAX_PATH + 1];
    ZeroMemory(szFile, sizeof(szFile));

    ZeroMemory(&openfilename, sizeof(OPENFILENAME));
    openfilename.lStructSize = sizeof(OPENFILENAME);
    openfilename.hwndOwner = padre;
    openfilename.lpstrFile = szFile;
    openfilename.nMaxFile = MAX_PATH;
    openfilename.nFilterIndex = 1;
    openfilename.lpstrFileTitle = NULL;
    openfilename.nMaxFileTitle = 0;
    openfilename.lpstrInitialDir = NULL;
    openfilename.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

    if (GetSaveFileName(&openfilename))
    {
        write_file(openfilename.lpstrFile);
    }
    else
    {
        MessageBox(NULL, TEXT("Could not save this file"), TEXT("File I/O Error"), MB_ICONSTOP);
    }
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR pCmdLine, _In_ int nCmdShow)
{
    // Register the window class.
    const wchar_t CLASS_NAME[] = L"Sample Window Class";

    WNDCLASS wc = { };

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Create the window.

    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L"Learn to Program Windows",    // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );

    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    // Run the message loop.

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
        
        Boton_GUARDAR = CreateWindow(L"BUTTON", L"Guardar", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 10, 10, 100, 30, hwnd, (HMENU)ID_BUTTON_GUARDAR, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
        Boton_ABRIR = CreateWindow(L"BUTTON", L"Abrir", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 115,10,100,30,hwnd, (HMENU)ID_BUTTON_ABRIR, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
        Boton_LIMPIAR = CreateWindow(L"BUTTON", L"Limpiar", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 220, 10, 100, 30, hwnd, (HMENU)ID_BUTTON_LIMPIAR, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);

        Edit = CreateWindow(L"EDIT", NULL, WS_BORDER | WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL,10,50,200,60, hwnd,(HMENU)ID_OUTBOX, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);

        break;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
            case ID_BUTTON_GUARDAR:
                funcion_guardar(hwnd);
                break;

            case ID_BUTTON_ABRIR:
                funcion_abrir(hwnd);
                break;

            case ID_BUTTON_LIMPIAR:
                SetWindowTextA(Edit, "");
                break;
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // All painting occurs here, between BeginPaint and EndPaint.

        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

        EndPaint(hwnd, &ps);
    }
    return 0;

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}