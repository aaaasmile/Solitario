#include "TraceService.h"

typedef TraceService* LPTraceService;

typedef struct {
    bool _initialized;
    LPTraceService _pTracer;
    void* _self;
    bool _debugEnabled;
}Tracer, *LPTracer;

static Tracer _tracer = { false, NULL, NULL, false };

static void traceInitialize() {
    TraceService* pTracer = TraceService::Instance();
    _tracer._pTracer = pTracer;
    _tracer._self = &_tracer;
    pTracer->SetOutputChannel(0, TraceService::eOutType::OT_FILE, "C:/temp/solitario.log");
    pTracer->SetOutputChannel(1, TraceService::eOutType::OT_MSVDEBUGGER, "");
    _tracer._initialized = true;
#ifdef _DEBUG
    _tracer._debugEnabled = true;
#endif
}

void TraceInServiceINFO(char* myBuff){
    if (!_tracer._initialized) {
        traceInitialize();
    }
    _tracer._pTracer->AddTrace(EntryTraceDetail::eType::TR_INFO, myBuff);
}

void TraceInServiceDEBUG(char* myBuff) {
    if (!_tracer._initialized) {
        traceInitialize();
    }
    if (!_tracer._debugEnabled) {
        return;
    }
    _tracer._pTracer->AddTrace(EntryTraceDetail::eType::TR_DEBUG, myBuff);
}

