#ifndef AJAX_COMMON_COLONY_CONFIG_H_
#define AJAX_COMMON_COLONY_CONFIG_H_
////////////////////////////////////////////////
//
// Common to ALL platforms for Ajax
//
////////////////////////////////////////////////


// Human readable Version Identifier
#define AJAX_SEMANTIC_VERSION_STR                   "1.0.0"


// CLI
#define OPTION_CPL_TSHELL_PROCESSOR_INPUT_SIZE      (4*1024)
#define OPTION_CPL_TSHELL_PROCESSOR_OUTPUT_SIZE     (4*1024)
#define OPTION_CPL_TSHELL_PROCESSOR_PROMPT          "$> "

// Data Model
#define OPTION_CPL_DM_MODEL_DATABASE_MAX_CAPACITY_JSON_DOC  (4*1024)
#define OPTION_CPL_DM_MODEL_DATABASE_TEMP_STORAGE_SIZE      (4*1024)

// Console password REQUIRED (except for debug builds)
#ifndef DEBUG_BUILD
#define OPTION_TSHELL_CMD_COMMAND_DEFAULT_PERMISSION_LEVEL	Cpl::TShell::Security::eADMIN
#define USE_CPL_TSHELL_PROCESSOR_SILENT_WHEN_PUBLIC
#endif

#endif
