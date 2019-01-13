#pragma once
#include "Utils/String.h"


namespace Display
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class Primitive
    {
    public:
        Primitive(int x, int y);
        void Draw();

    protected:
        int x;
        int y;
        Color color;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class Region
    {
    public:
        Region(int width, int height);
        void Draw(int x, int y, Color color = Color::NUMBER);
        void DrawBounded(int x, int y, Color colorFill, Color colorBound);
    private:
        int width;
        int height;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class Rectangle
    {
    public:
        Rectangle(int width, int height);
        void Draw(int x, int y, Color color = Color::NUMBER);
    private:
        int width;
        int height;
    };


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class HLine
	{
	public:
		HLine(int width);
		void Draw(int x, int y, Color color = Color::NUMBER);
	private:
		int width;
	};


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class VLine
    {
    public:
        VLine(int height);
        void Draw(int x, int y, Color color = Color::NUMBER);
    private:
		int height;
    };


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class Line
    {
    public:
        Line(int x0, int y0, int x1, int y1);
        void Draw(Color color = Color::NUMBER);
    private:
        int x0;
        int y0;
        int x1;
        int y1;
    };


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class Char
    {
    public:
        Char(char ch);
        int Draw(int x, int y, Color color = Color::NUMBER);
    private:
		char ch;
    };


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class Point
    {
    public:
		Point() {};
        void Draw(int x, int y, Color color = Color::NUMBER);
    private:
    };


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class Text
    {
    public:
        Text(const char *text, uint8 size = 1);
		Text(const String &string, uint8 size = 1);

        int Draw(int x, int y, Color color = Color::NUMBER);

        int DrawInCenterRect(int x, int y, int width, int height, Color color = Color::NUMBER);

        int DrawWithLimitation(int x, int y, int limitX, int limitY, int limitWidth, int limitHeight);
        /// Выводит текст на прямоугольнике цвета colorBackgound
        int DrawOnBackground(int x, int y, Color colorBackground);

        void DrawRelativelyRight(int xRight, int y, Color color = Color::NUMBER);
        /// Пишет текст с переносами
        int DrawInRectWithTransfers(int x, int y, int width, int height, Color color = Color::NUMBER);
        /// Возвращает нижнюю координату прямоугольника
        int DrawInBoundedRectWithTransfers(int x, int y, int width, Color colorBackground, Color colorFill);
    private:
        const char *text;
        uint8 size;
        int DrawCharWithLimitation(int eX, int eY, char _symbol, int limitX, int limitY, int limitWidth, int limitHeight);
        void DrawBig(int x, int y, Color color);
        int DrawSmall(int x, int y, Color color);
        bool ByteFontNotEmpty(uint eChar, int byte);
        bool BitInFontIsExist(int eChar, int numByte, int bit);
    };
}
