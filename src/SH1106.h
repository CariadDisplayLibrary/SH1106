#ifndef _SH1106_H
#define _SH1106_H

#include <DisplayCore.h>

class SH1106 : public DisplayCore {
    public: // Constants
        static const int Width = 132;
        static const int Height = 64;

    private: // Variables
        uint8_t _buffer[Width * Height / 8];
        int32_t _bufferDepth;
        int _offset_x;
        int _offset_y;
   
    public:  // Methods
        void updateDisplay();
        void initDevice();
        void setPage(uint8_t);
        void setY(uint8_t);

    public: // Methods
        SH1106() : _bufferDepth(0) {};
        void setPixel(int x, int y, color_t c);
        void startBuffer();
        void endBuffer();

        void setRotation(int r);

        virtual void data(uint8_t) = 0;
        virtual void data(uint8_t*, uint32_t) = 0;
        virtual void command(uint8_t) = 0;
        virtual void command(uint8_t, uint8_t) = 0;

        virtual void initializeDevice() = 0;

        void setDisplayOffset(int x, int y) { 
            _offset_x = x;
            _offset_y = y;
        }

        void displayOn();
        void displayOff();
        void invertDisplay(bool i);

        void setBacklight(uint8_t b);

};

#endif
