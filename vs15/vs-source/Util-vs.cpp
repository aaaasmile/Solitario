#include "TraceService.h"

typedef TraceService* LPTraceService;

typedef struct {
    bool _initialized;
    LPTraceService _pTracer;
    void* _self;
}Tracer, *LPTracer;

Tracer _tracer = { false, NULL, NULL };

void TraceInitialize() {
    TraceService* pTracer = TraceService::Instance();
    _tracer._pTracer = pTracer;
    _tracer._self = &_tracer;
    pTracer->SetOutputChannel(0, TraceService::eOutType::OT_FILE, "C:/temp/solitario.log");
    _tracer._initialized = true;
}

void TraceInService(char* myBuff){
    if (!_tracer._initialized) {
        TraceInitialize();
    }
    _tracer._pTracer->AddSimpleTrace(0, myBuff);
}
