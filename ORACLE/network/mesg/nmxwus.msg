/ **************** This is generated file, do not edit directly **********
//  Copyright (c) 2000 by the Oracle Corporation.  All rights reserved.
/
/ NAME
/   nmxwus.msg
/
/
/ FUNCTION
/   HTTP Server component message file, American English version.
/
/
/ NOTES
/       Message text should be limited to a maximum of 76 characters.
/       Messages containing embedded % directives should not overrun
/       the 76 character limit unless they signal unusual errors
/       which do not occur as part of user applications.  For example,
/       startup errors and system-dependent errors can probably get
/       away with longer message text. 
/
/
/       '/' at beginning-of-line for comments.
/       '//' at beginning-of-line for *Cause: And *Action: fields.
/
///////////////////////////////////////////////////////////////////////////
/
01000, 00000, "Error: Unable to locate the file : %s"
// *Cause:  The file does not exists.
// *Action: Ensure that file exists.
/
01001, 00000, "Error: Unable to execute command %s: %s"
// *Cause: 
// *Action:
/
01002, 00000, "HTTP Server startup task execution completed"
// *Cause: 
// *Action: 
/
01003, 00000, "HTTP Server shutdown task execution completed" 
// *Cause: 
// *Action: 
/
01004, 00000, "Error: HTTP Server startup task execution failed : %s"
// *Cause: 
// *Action:
/
01005, 00000, "Error: HTTP Server shutdown task execution failed : %s"
// *Cause:  
// *Action: 
/
01006, 00000, "Error: Root privilege required to execute this task"
// *Cause: 
// *Action: 
/
01007, 00000, "Error: Unable to open file %s for reading"
// *Cause:  The file does not exists.
// *Action: Ensure that file exists and has read permission.
/


////////////////////////////////////////////////////////////////////////////
/
/ 2000-2999  Oracle HTTP server Discovery, Misc Diagnostic Messages
/
///////////////////////////////////////////////////////////////////////////
/
02000, 00000, "Discovery error: Unable to locate HTTP Server configuration file : %s"
// *Cause:  The file does not exists.
// *Action: Ensure that file exists.
/
02001, 00000, "Discovery error: Unable to open HTTP Server configuration file %s"
// *Cause: File may not have read permission.
// *Action: Ensure the file exists and has read permission.
/
02002, 00000, "Discovery error: Unable to read HTTP Server configuration file %s; %s entry not found"
// *Cause: Check the syntax of the configuration file for correctness.
// *Action: Rectify the syntax error.
/
/ *********************** END OF MESSAGE FILE*******************************
