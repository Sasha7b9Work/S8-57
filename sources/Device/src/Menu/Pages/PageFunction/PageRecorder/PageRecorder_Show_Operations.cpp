#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageFunction.h"


extern const PageBase pageOperations;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnPress_Rename()
{

}

DEF_BUTTON( bRename,                                                                                                    //--- ФУНКЦИЯ - РЕГИСТРАТОР - ПРОСМОТР - ОПЕРАЦИИ - Переименовать ---
    "Переименовать", "Rename",
    "",
    "",
    pageOperations, FuncActive, OnPress_Rename, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Copy()
{

}

DEF_BUTTON( bCopy,                                                                                                         //--- ФУНКЦИЯ - РЕГИСТРАТОР - ПРОСМОТР - ОПЕРАЦИИ - Копировать ---
    "Копировать", "Copy",
    "",
    "",
    pageOperations, FuncActive, OnPress_Copy, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Move()
{

}

DEF_BUTTON( bMove,                                                                                                        //--- ФУНКЦИЯ - РЕГИСТРАТОР - ПРОСМОТР - ОПЕРАЦИИ - Переместить ---
    "Переместить", "Move",
    "",
    "",
    pageOperations, FuncActive, OnPress_Move, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Delete()
{

}

DEF_BUTTON( bDelete,                                                                                                          //--- ФУНКЦИЯ - РЕГИСТРАТОР - ПРОСМОТР - ОПЕРАЦИИ - Удалить ---
    "Удалить", "Delete",
    "",
    "",
    pageOperations, FuncActive, OnPress_Delete, FuncDraw
)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_4( pageOperations, // -V641 // -V1027                                                                                          //--- ФУНКЦИЯ - РЕГИСТРАТОР - ПРОСМОТР - ОПЕРАЦИИ ---
    "ОПЕРАЦИИ", "OPERATIONS",
    "",
    "",
    &bRename,   ///< ФУНКЦИЯ - РЕГИСТРАТОР - ПРОСМОТР - ОПЕРАЦИИ - Переименовать
    &bCopy,     ///< ФУНКЦИЯ - РЕГИСТРАТОР - ПРОСМОТР - ОПЕРАЦИИ - Копировать
    &bMove,     ///< ФУНКЦИЯ - РЕГИСТРАТОР - ПРОСМОТР - ОПЕРАЦИИ - Переместить
    &bDelete,   ///< ФУНКЦИЯ - РЕГИСТРАТОР - ПРОСМОТР - ОПЕРАЦИИ - Удалить
    Page::Name::Function_Recorder_Show_Operations, PageFunction::PageRecorder::PageShow::pointer, FuncActive, FuncPressPage, FuncDrawPage, FuncRegSetPage
)

const PageBase *PageFunction::PageRecorder::PageShow::PageOperations::pointer = &pageOperations;
