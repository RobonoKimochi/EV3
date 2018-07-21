#ifndef LINETRACER_WITH_STARTER_H
#define LINETRACER_WITH_STARTER_H

#include "LineTracer.h"
#include "Starter.h"

class LineTracerWithStarter {
public:
    LineTracerWithStarter(LineTracer* lineTracer, Starter* starter);
    void run();
private:
    LineTracer* mLineTracer;
    Starter* mStarter;
};

#endif