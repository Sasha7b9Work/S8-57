#pragma once


/// Здесь хранятся данные 
class Buffer
{
    friend class StorageOld;

public:
    static void Init();

    static void Update();
};
