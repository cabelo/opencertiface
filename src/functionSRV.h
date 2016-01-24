#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h> 
   /**
     * @brief Create pid file in the /var/run (unix format).
     * @param serviceName name of the damemon.
     */
void savePIDrun(char *serviceName);

   /**
     * @brief Create de log file in the /var/log (unix format).
     * @param filename name of the log file.
     * @param message String to save in the file.
     */
void log_message(char *filename ,char *message);

   /**
     * @brief Create daemon binary (unix format).
     * @param serviceName name of the damemon.
     */
void daemonizeGuardian(char *serviceName);

   /**
     * @brief Create daemon binary (unix format).
     */
void daemonize(void);

