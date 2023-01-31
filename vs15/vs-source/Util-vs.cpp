#include "TraceService.h"

void TraceInLogFile(char* myBuff)
{
    TraceService* pTracer = TraceService::Instance();
    pTracer->SetOutputChannel(0, TraceService::eOutType::OT_FILE, "C:/temp/solitario.log");
    pTracer->AddSimpleTrace(0, myBuff);
}
