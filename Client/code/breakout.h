#ifndef BREAKOUT_BREAKOUT_H
#define BREAKOUT_BREAKOUT_H

class breakout {
public:
    std::string name;

    breakout(const std::string &name);
    float getXMousePos();

    int start();

};
#endif
