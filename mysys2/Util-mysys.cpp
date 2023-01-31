#include "TraceService.h"

typedef TraceService* LPTraceService;

typedef struct {
    bool _initialized;
    LPTraceService _pTracer;
    bool _debugEnabled;
} Tracer, *LPTracer;

static Tracer _tracer = {false, NULL, false};

static void traceInitialize() {
    char logpath[PATH_MAX];
    snprintf(logpath, sizeof(logpath), "%s/.solitario/solitario.log",
             getenv("HOME"));

    LPTraceService pTracer = TraceService::Instance();
    _tracer._pTracer = pTracer;
    pTracer->SetOutputChannel(0, TraceService::eOutType::OT_FILE, logpath);
    _tracer._initialized = true;
#ifdef _DEBUG
    _tracer._debugEnabled = true;
#endif
    char buff[1024];
    snprintf(buff, sizeof(buff), "Using log file located in %s", logpath);
    _tracer._pTracer->AddTrace(EntryTraceDetail::eType::TR_INFO, buff);
}

void TraceInServiceINFO(char* myBuff) {
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
