#pragma once

#define E_OK		10	/* No error */
#define E_EOF		11	/* End Of File */
#define E_INTR		12	/* Interrupted */
#define E_TOKEN		13	/* Bad token */
#define E_SYNTAX	14	/* Syntax error */
#define E_NOMEM		15	/* Ran out of memory */
#define E_DONE		16	/* Parsing complete */
#define E_ERROR		17	/* Execution error */
#define E_TABSPACE	18	/* Inconsistent mixing of tabs and spaces */
#define E_OVERFLOW      19	/* Node had too many children */
#define E_TOODEEP	20	/* Too many indentation levels */
#define E_DEDENT	21	/* No matching outer block for dedent */

