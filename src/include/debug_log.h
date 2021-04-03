#pragma once

#include <syslog.h>

#define debug_debug( message, ... ) (syslog(LOG_DEBUG, "[%s]" message, __func__, __VA_ARGS__))
#define debug_info( message, ... ) (syslog(LOG_INFO, "[%s]" message, __func__, __VA_ARGS__))
#define debug_notice( message, ... ) (syslog(LOG_NOTICE, "[%s]" message, __func__, __VA_ARGS__))
#define debug_warning( message, ... ) (syslog(LOG_WARNING, "[%s]" message, __func__, __VA_ARGS__))
#define debug_err( message, ... ) (syslog(LOG_ERR, "[%s]" message, __func__, __VA_ARGS__))
#define debug_crit( message, ... ) (syslog(LOG_CRIT, "[%s]" message, __func__, __VA_ARGS__))
#define debug_alert( message, ... ) (syslog(LOG_ALERT, "[%s]" message, __func__, __VA_ARGS__))
#define debug_emerg( message, ... ) (syslog(LOG_EMERG, "[%s]" message, __func__, __VA_ARGS__))
