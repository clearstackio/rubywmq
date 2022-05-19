#include <dlfcn.h>
#include "wmq.h"

#ifndef RTLD_LAZY
    #define RTLD_LAZY 1
#endif
#ifndef RTLD_GLOBAL
    #define RTLD_GLOBAL 0
#endif

#define MQ_LOAD(LIBRARY)                                                             \
    void* handle = (void*)dlopen(LIBRARY, RTLD_LAZY|RTLD_GLOBAL);                    \
    if (!handle)                                                                     \
    {                                                                                \
        rb_raise(wmq_exception,                                                      \
                    "WMQ::QueueManager#connect(). Failed to load MQ Library:%s, rc=%s", \
                    LIBRARY,                                                            \
                    dlerror());                                                         \
    }

#define MQ_RELEASE dlclose(pqm->mq_lib_handle);

#define MQ_FUNCTION(FUNC, CAST) \
    pqm->FUNC = (CAST)dlsym(handle, #FUNC);                                  \
    if (!pqm->FUNC)                                                          \
    {                                                                        \
        rb_raise(wmq_exception, "Failed to find API "#FUNC" in MQ Library"); \
    }

void Queue_manager_mq_load(PQUEUE_MANAGER pqm)
{
    PMQCHAR library;
    if(pqm->is_client_conn)
    {
        library = "libmqic_r" SOEXT;
        if(pqm->trace_level) printf("WMQ::QueueManager#connect() Loading MQ Client Library:%s\n", library);
    }
    else
    {
        library = "libmqm_r" SOEXT;
        if(pqm->trace_level) printf("WMQ::QueueManager#connect() Loading MQ Server Library:%s\n", library);
    }

    {
        MQ_LOAD(library)

        if(pqm->trace_level>1) printf("WMQ::QueueManager#connect() MQ Library:%s Loaded successfully\n", library);

        MQ_FUNCTION(MQCONNX,void(*)(PMQCHAR,PMQCNO,PMQHCONN,PMQLONG,PMQLONG))
        MQ_FUNCTION(MQCONN, void(*)(PMQCHAR,PMQHCONN,PMQLONG,PMQLONG))
        MQ_FUNCTION(MQDISC,void(*) (PMQHCONN,PMQLONG,PMQLONG))
        MQ_FUNCTION(MQBEGIN,void(*)(MQHCONN,PMQVOID,PMQLONG,PMQLONG))
        MQ_FUNCTION(MQBACK,void(*) (MQHCONN,PMQLONG,PMQLONG))
        MQ_FUNCTION(MQCMIT,void(*) (MQHCONN,PMQLONG,PMQLONG))
        MQ_FUNCTION(MQPUT1,void(*) (MQHCONN,PMQVOID,PMQVOID,PMQVOID,MQLONG,PMQVOID,PMQLONG,PMQLONG))

        MQ_FUNCTION(MQOPEN,void(*) (MQHCONN,PMQVOID,MQLONG,PMQHOBJ,PMQLONG,PMQLONG))
        MQ_FUNCTION(MQCLOSE,void(*)(MQHCONN,PMQHOBJ,MQLONG,PMQLONG,PMQLONG))
        MQ_FUNCTION(MQGET,void(*)  (MQHCONN,MQHOBJ,PMQVOID,PMQVOID,MQLONG,PMQVOID,PMQLONG,PMQLONG,PMQLONG))
        MQ_FUNCTION(MQPUT,void(*)  (MQHCONN,MQHOBJ,PMQVOID,PMQVOID,MQLONG,PMQVOID,PMQLONG,PMQLONG))

        MQ_FUNCTION(MQINQ,void(*)  (MQHCONN,MQHOBJ,MQLONG,PMQLONG,MQLONG,PMQLONG,MQLONG,PMQCHAR,PMQLONG,PMQLONG))
        MQ_FUNCTION(MQSET,void(*)  (MQHCONN,MQHOBJ,MQLONG,PMQLONG,MQLONG,PMQLONG,MQLONG,PMQCHAR,PMQLONG,PMQLONG))

        MQ_FUNCTION(mqCreateBag,void(*)(MQLONG,PMQHBAG,PMQLONG,PMQLONG))
        MQ_FUNCTION(mqDeleteBag,void(*)(PMQHBAG,PMQLONG,PMQLONG))
        MQ_FUNCTION(mqClearBag,void(*)(MQHBAG,PMQLONG,PMQLONG))
        MQ_FUNCTION(mqExecute,void(*)(MQHCONN,MQLONG,MQHBAG,MQHBAG,MQHBAG,MQHOBJ,MQHOBJ,PMQLONG,PMQLONG))
        MQ_FUNCTION(mqCountItems,void(*)(MQHBAG,MQLONG,PMQLONG,PMQLONG,PMQLONG))
        MQ_FUNCTION(mqInquireBag,void(*)(MQHBAG,MQLONG,MQLONG,PMQHBAG,PMQLONG,PMQLONG))
        MQ_FUNCTION(mqInquireItemInfo,void(*)(MQHBAG,MQLONG,MQLONG,PMQLONG,PMQLONG,PMQLONG,PMQLONG))
        MQ_FUNCTION(mqInquireInteger,void(*)(MQHBAG,MQLONG,MQLONG,PMQLONG,PMQLONG,PMQLONG))
        MQ_FUNCTION(mqInquireString,void(*)(MQHBAG,MQLONG,MQLONG,MQLONG,PMQCHAR,PMQLONG,PMQLONG,PMQLONG,PMQLONG))
        MQ_FUNCTION(mqAddInquiry,void(*)(MQHBAG,MQLONG,PMQLONG,PMQLONG))
        MQ_FUNCTION(mqAddInteger,void(*)(MQHBAG,MQLONG,MQLONG,PMQLONG,PMQLONG))
        MQ_FUNCTION(mqAddString,void(*)(MQHBAG,MQLONG,MQLONG,PMQCHAR,PMQLONG,PMQLONG))

        pqm->mq_lib_handle = (void*)handle;

        if(pqm->trace_level>1) printf("WMQ::QueueManager#connect() MQ API's loaded successfully\n");
    }
}

void Queue_manager_mq_free(PQUEUE_MANAGER pqm)
{
    if(pqm->mq_lib_handle)
    {
        if(pqm->trace_level>1) printf("WMQ::QueueManager#gc() Releasing MQ Library\n");
        MQ_RELEASE
        pqm->mq_lib_handle = 0;
    }
}
