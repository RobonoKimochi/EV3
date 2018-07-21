#include "LineTracerWithStarter.h"

LineTracerWithStarter::LineTracerWithStarter(
    LineTracer* lineTracer,
    Starter* starter)
    : mLineTracer(lineTracer),
      mStarter(starter)
{
}

void LineTracerWithStarter::run() {
    if(mStarter->isPushed()) {
        mLineTracer->run();
    } else {

    }
}
