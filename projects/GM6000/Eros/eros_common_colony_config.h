#ifndef EROS_COMMON_COLONY_CONFIG_H_
#define EROS_COMMON_COLONY_CONFIG_H_
////////////////////////////////////////////////
//
// Common to ALL platforms for Ajax
//
////////////////////////////////////////////////

#define I_AM_EROS

#define EROS_SEMANTIC_VERSION_STR                   "0.0.1"

// No console password required!
#define OPTION_TSHELL_CMD_COMMAND_DEFAULT_PERMISSION_LEVEL	Cpl::TShell::Security::ePUBLIC

// CLI
#define OPTION_CPL_TSHELL_PROCESSOR_INPUT_SIZE      (4*1024)
#define OPTION_CPL_TSHELL_PROCESSOR_OUTPUT_SIZE     (4*1024)
#define OPTION_CPL_TSHELL_PROCESSOR_PROMPT          "$$> "

// Data Model
#define OPTION_CPL_DM_MODEL_DATABASE_MAX_CAPACITY_JSON_DOC  (4*1024)
#define OPTION_CPL_DM_MODEL_DATABASE_TEMP_STORAGE_SIZE      (4*1024)

#endif
