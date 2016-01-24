#include "soapH.h"
#include "opencertiface.nsmap"

#define DEBUG printf("> Line %d\n",__LINE__);

//! Daemon name.
char *processNameGuardian = "opencertifaceGuardian";

//! Daemon name.
char *processName = "opencertiface";

//! Daemon guardian string PID
char *pidNameGuardian = "[opencertifaceGuardian]";

//! Daemon string PID
char *pidName     = "[opencertiface]\0";



