﻿#include <winsock2.h>
#include <iostream>
#include <string>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma warning(disable: 4996)
#pragma comment (lib,"Ws2_32.lib")

#define PORT 666
#define SERVERADDR "127.0.0.1"
using namespace std;
int main() {
    setlocale(LC_ALL, "Rus");
    char buff[1024];
    cout << "TCP DEMO CLIENT\n";
    // Шаг 1 - инициализация библиотеки Winsock
    if (WSAStartup(0x202, (WSADATA*)&buff[0]))
    {
        cout << "WSAStart_error:\n" << WSAGetLastError();
        cin.get();   return -1;
    }
    // Шаг 2 - создание сокета
    SOCKET my_sock; my_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (my_sock < 0)
    {
        cout << "Socket()_error:\n" << WSAGetLastError();
        cin.get();  return -1;
    }
    // Шаг 3 - установка соединения
    /* заполнение структуры sockaddr_in,  указание адреса и порта сервера  */
    sockaddr_in dest_addr;
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(PORT);
    HOSTENT* hst;
    // преобразование IP адреса из символьного в  сетевой формат
    if (inet_addr(SERVERADDR) != INADDR_NONE)
        dest_addr.sin_addr.s_addr = inet_addr(SERVERADDR);
    else
        // попытка получить IP адрес по доменному имени сервера
        if (hst = gethostbyname(SERVERADDR))
            // hst->h_addr_list содержит массив указателей на адреса 
            ((unsigned long*)&dest_addr.sin_addr)[0] =
            ((unsigned long**)hst->h_addr_list)[0][0];
        else {
            cout << "Invalid address \n" << SERVERADDR;
            closesocket(my_sock);    WSACleanup();
            cin.get();         return -1;
        }
    // адрес сервера получен – пытаемся установить    соединение 
    if (connect(my_sock, (sockaddr*)&dest_addr, sizeof(dest_addr)))
    {
        cout << "Connect error\n" << WSAGetLastError();
        cin.get();       return -1;
    }
    cout << "Соединение с" << SERVERADDR << "успешно установлено\n"; cout << "Type quit for exit\n\n";
    // Шаг 4 - чтение и передача сообщений
    int nsize;
    while ((nsize = recv(my_sock, &buff[0], 80, 0)) != SOCKET_ERROR) { // ставим завершающий ноль в конце строки 
        buff[nsize] = '\0'; cout << "S=>C:";  cout << buff << endl;
        // читаем пользовательский ввод с клавиатуры
        cout << "S<=C:"; string strr;  getline(cin, strr);
        // передаем строку клиента серверу
        send(my_sock, (char*)&strr[0], strr.size(), 0);
        if (strr == "quit")   // проверка на "quit"
        {
            cout << "Exit..."; // Корректный выход
            closesocket(my_sock);  WSACleanup(); return 0;
        }
    }
    cout << "Recv error\n" << WSAGetLastError();
    closesocket(my_sock);
    WSACleanup();
    cin.get();
    return -1;
}