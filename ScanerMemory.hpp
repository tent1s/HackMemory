#ifndef SCANERMEMORY_H
#define SCANERMEMORY_H
#include <QCoreApplication>
#include <iostream>
#include <QtWidgets>
#include <windows.h>
#include <string>
#include <tlhelp32.h>
#include "ui_mainwindow.h"
#include "mainwindow.h"
#include <iomanip>
#include <exception>
#include <cstdint>
#include <vector>
#include <sstream>
#include <fstream>


HANDLE OutputAttribute = nullptr;
enum protectType : DWORD
{
    Readable = (PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY | PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY),
    Writable = (PAGE_EXECUTE_READWRITE | PAGE_READWRITE)
};

enum TypeV : DWORD
{
    Bytes = 0x0,
    D2Bytes = 0x1,
    D4Bytes = 0x3,
    D8Bytes = 0x7
};

std::vector<MEMORY_BASIC_INFORMATION>mapp;

bool mapmemory(HANDLE phandle, protectType pTopy) {

    DWORD ban = (PAGE_NOCACHE | PAGE_GUARD | PAGE_NOACCESS);

    MEMORY_BASIC_INFORMATION mbi;
    mapp.clear();
    uint8_t* Addrs = NULL;

    while (VirtualQueryEx(phandle, Addrs, &mbi, sizeof(mbi)) != 0) {

        if (mbi.State & MEM_COMMIT && !(mbi.State & MEM_RELEASE) && mbi.AllocationProtect & pTopy && mbi.Protect & pTopy && !(mbi.Protect & ban))
            mapp.push_back(mbi);


        Addrs = reinterpret_cast<uint8_t*>(mbi.BaseAddress) + mbi.RegionSize;
    }


    return true;
}


template <typename T>
std::vector<uintptr_t> scanner(T valor, HANDLE phandle, DWORD pid, int x, TypeV TheValue, int tp_s) {
    std::vector<uintptr_t> Addres;
    Addres.clear();
    HANDLE hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
    for (auto& adr : mapp) {
        auto bf = reinterpret_cast<uint8_t*>(LocalAlloc(NULL, adr.RegionSize));
        auto ret = ReadProcessMemory(phandle, adr.BaseAddress, bf, adr.RegionSize, NULL);

        if (!ret)
        {
            continue;
        }
        for (int i = 0; i < adr.RegionSize; ++i) {
            auto allocadPoint = uintptr_t(&bf[i]);

            auto vVal = *reinterpret_cast<T*>(allocadPoint);

            if (tp_s == 3) {
                if (vVal == valor || vVal <= (valor + 1) && vVal >= (valor - 1))
                {
                    Addres.push_back((uintptr_t)adr.BaseAddress + i);
                    i += TheValue;
                }
            }
            else {
                if (vVal == valor)
                {
                    Addres.push_back((uintptr_t)adr.BaseAddress + i);
                    i += TheValue;
                }
            }
        }

        LocalFree(bf);
    }

    printf("Found : %d\n", Addres.size());



    CloseHandle(hModuleSnap);
    return Addres;
}
template <typename T>
std::vector<uintptr_t> NxtScan(T valor, HANDLE phandle, std::vector<uintptr_t>Old, int x, DWORD pid, int tp_s) {
    x = 0;
    std::vector<uintptr_t> Addres;
    Addres.clear();
    T OldValue = valor;
    HANDLE hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
    for (int i = 0; i < Old.size(); ++i) {
        ReadProcessMemory(phandle, (LPVOID)Old[i], &OldValue, sizeof(OldValue), NULL);
        if (tp_s == 3) {
            if (OldValue == valor || OldValue >= (valor - 1) && OldValue <= (valor + 1)) {
                Addres.push_back(Old[i]);
            }
        }
        else {
            if (OldValue == valor) {
                Addres.push_back(Old[i]);
            }
        }
    }   
    CloseHandle(hModuleSnap);
    return Addres;
}


#endif // SCANERMEMORY_H
