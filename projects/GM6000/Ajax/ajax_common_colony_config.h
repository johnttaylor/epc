#ifndef AJAX_COMMON_COLONY_CONFIG_H_
#define AJAX_COMMON_COLONY_CONFIG_H_
////////////////////////////////////////////////
//
// Common to ALL platforms for Ajax
//
////////////////////////////////////////////////


// Human readable Version Identifier
#define AJAX_SEMANTIC_VERSION_STR                   "0.0.1"

// Model Number
#define AJAX_MODEL_NUMBER_STR                       "gm6000-xb"

// CLI
#define OPTION_CPL_TSHELL_PROCESSOR_INPUT_SIZE      (4*1024)
#define OPTION_CPL_TSHELL_PROCESSOR_OUTPUT_SIZE     (4*1024)
#define OPTION_CPL_TSHELL_PROCESSOR_PROMPT          "$> "

// Data Model
#define OPTION_CPL_DM_MODEL_DATABASE_MAX_CAPACITY_JSON_DOC  (4*1024)
#define OPTION_CPL_DM_MODEL_DATABASE_TEMP_STORAGE_SIZE      (4*1024)

#endif
