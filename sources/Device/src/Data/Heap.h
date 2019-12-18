#pragma once



class Heap
{
public:
    /// Ќачало хранилища RAM, где хран€тс€ считанные сигналы
    static uint8 *BeginStorageRAM();
    ///  онец хранилща RAM, где хран€тс€ считанные сигналы (указатель на последний байт за хранилищем)
    static uint8 *EndStorageRAM();
};
