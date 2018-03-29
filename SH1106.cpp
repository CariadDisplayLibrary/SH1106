#include <SH1106.h>

#define C2B(X, Y) ((Y) * Width + (X))

void SH1106::setPage(uint8_t row) {
    command(0xb0 | (row & 0xF));
}

void SH1106::setY(uint8_t y) {
    command(0x00 | (y & 0xf));
    command(0x10 | (y >> 4));
}

void SH1106::setPixel(int x, int y, color_t c) {
    uint8_t row;
    uint8_t pixel;
    uint8_t mask;

    x += _offset_x;
    y += _offset_y;

    translateCoordinates(&x, &y);

    if((x < 0) ||(x >= Width) || (y < 0) || (y >= Height))
        return;

    row = y>>3;
    pixel = y & 0x07;
    mask = 1<<pixel;
    if (c) {
        _buffer[C2B(x, row)] |= mask;
    } else {
        _buffer[C2B(x, row)] &= ~mask;
    }

    if (_bufferDepth == 0) {
        setPage(row);
        setY(x);
        data(_buffer[C2B(x, row)]);
    }
}

void SH1106::setRotation(int r) {

    rotation = r & 0x03;

    switch (r & 0x03) {
        case 0:
            _width = Width;
            _height = Height;
            break;
        case 1:
            _width = Height;
            _height = Width;
            break;
        case 2:
            _width = Width;
            _height = Height;
            break;
        case 3:
            _width = Height;
            _height = Width;
            break;
    }
    clearClipping();
}

void SH1106::initDevice() {
    _width = Width;
    _height = Height;
    command(0xAE);          // Display off
    command(0xD5, 0x50);    // Set divide ratio / oscillator frequency set
    command(0xA8, 63);      // Multiplex ratio
    command(0xD3, 0x00);    // Display offset mode set
    command(0x40);          // Display start line
    command(0xAD, 0x8B);    // DC-DC Control Mode Set
    command(0xA1);          // Set segment re-map (ADC)
    command(0xC8);          // Set common output scan direction
    command(0xDA, 0x12);    // Common pads hardware configuration mode set
    command(0x81, 0x80);    // Contrast control mode set
    command(0xD9, 0x22);    // Discharge / precharge mode set
    command(0xDB, 0x35);    // VCOM Deselect Level Mode Set
    command(0xA6);          // Normal display
    command(0xAF);          // Display on
}

void SH1106::displayOn() {
    command(0xAF);
}

void SH1106::displayOff() {
    command(0xAE);
}

void SH1106::invertDisplay(bool i) {
    if (i) {
        command(0xA7);
    } else {
        command(0xA6);
    }
}

void SH1106::startBuffer() {
    _bufferDepth++;
}

void SH1106::endBuffer() {
    _bufferDepth--;
    if (_bufferDepth <= 0) {
        _bufferDepth = 0;
        updateDisplay();
    }
}

void SH1106::updateDisplay() {
    for (int page = 0; page < (Height/8); page++) {
        setPage(page);
        setY(0);
        int tt = Width;
        int d = 0;
        while (tt > 0) {
            int t = tt > 16 ? 16 : tt;
            data(&_buffer[page * Width + d], t);
            tt -= t;
            d += t;
        }
    }
}

void SH1106::setBacklight(uint8_t b) {
    command(0x81, b);    // Contrast control mode set
}
