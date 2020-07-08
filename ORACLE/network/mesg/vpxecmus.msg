/ **************** This is generated file, do not edit directly **********
//  Copyright (c) 1999, 2000 by the Oracle Corporation.  All rights reserved.
/
/ NAME
/   vpxecmus.msg
/
/
/ FUNCTION
/   Oracle Data Gatherer eCommerce Management cartridge for Apache Web
/   Server message file, American English version.
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
/
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/
/    0 -  999 Critical Errors
/
//////////////////////////////////////////////////////////////////////////
00000, 00000, "inconsistent semaphore state"
// *Comment: mnemonic: APACHE_ERROR_INCONSISTENT_SEMAPHORE
/
00001, 00000, "memory allocation failure"
// *Comment: mnemonic: APACHE_ERROR_MEMORY_ALLOC_FAILED
/
//////////////////////////////////////////////////////////////////////////
/
/ 1000 - 1999 Not-So Critical Errors
/
//////////////////////////////////////////////////////////////////////////
01000, 00000, "failed to  write to socket"
// *Comment: mnemonic: APACHE_ERROR_FAILED_TO_WRITE_TO_SOCKET
01001, 00000, "failed to read from socket"
// *Comment: mnemonic: APACHE_ERROR_FAILED_TO_READ_FROM_SOCKET
01002, 00000, "unrecognized result"
// *Comment: mnemonic: APACHE_ERROR_UNRECOGNIZED_RESULT
//////////////////////////////////////////////////////////////////////////
/
/ 2000 - 2999 Informational Messages
/
//////////////////////////////////////////////////////////////////////////
02000, 00000, "Logon failed, could not get server status from URL: "
// *Comment: mnemonic: APACHE_ERROR_LOGON_FAILED_BAD_URL
// *Comment: This error also returns the Web Server Response Code
// *Cause:  The URL for the server status is not correct if the Response Code is 404.
// *Cause:  The URL for the server status is not accessible if the Response Code is 403.
// *Action: Verifying the Web Server mod_status configuration.
/
02001, 00000, "Logon failed to connect to Web server."
// *Comment: mnemonic: APACHE_ERROR_LOGON_FAILED_TO_CONNECT
// *Comment: This error message is displayed with one from the DG
// *Comment: which gives the machine name and port number
// *Cause:  Web Server is not responding on this machine and port
// *Action: Insure Web Server is running on this machine and port
/
//////////////////////////////////////////////////////////////////////////
/
/ 3000 - 3999 Labels or Captions Meta
/
//////////////////////////////////////////////////////////////////////////
03000, 00000, "Collection of Web Server Performance and Capacity Planner Statistics"
// *Comment: mnemonic: APACHE_CART_DESC
/
03001, 00000, "Web Server Performance Statistics"
// *Comment: mnemonic: APACHE_MODULE_EXTNAME
/
03002, 00000, "Time Stamp(second)"
// *Comment: mnemonic: APACHE_TIMESTAMP_LABEL
/
03003, 00000, "Web Server Response Code: "
// *Comment: mnemonic: APACHE_ERROR_WEB_SERVER_RESPONSE_CODE

//////////////////////////////////////////////////////////////////////////
/
/ 4000 - 4999 Labels or Captions for Apache Cart
/
//////////////////////////////////////////////////////////////////////////
04001, 00000, "Performance Overview"
// *Comment: mnemonic: APACHE_SERVER_OVERVIEW_NAME
/
04002, 00000, "Web Server"
// *Comment: mnemonic: APACHE_SERVER_OVERVIEW_INST
/
04003, 00000, "Web Server Performance Statistics Overview"
// *Comment: mnemonic: APACHE_SERVER_OVERVIEW_DESC
/
04004, 00000, "Web Server at a Glance"
// *Comment: mnemonic: APACHE_CH_GROUP_SERVER_OVERVIEW_NAME
/
04005, 00000, "View of I/O and Server Utilization"
// *Comment: mnemonic: APACHE_CH_GROUP_SERVER_OVERVIEW_DESC
/
04006, 00000, "I/O"
// *Comment: mnemonic: APACHE_CLASS_IO_NAME
/
04007, 00000, "Web Server"
// *Comment: mnemonic: APACHE_CLASS_IO_INST
/
04008, 00000, "I/O"
// *Comment: mnemonic: APACHE_CLASS_IO_DESC
/
04009, 00000, "Total Kilobytes Served"
// *Comment: mnemonic: APACHE_R_IO_TOTAL_KBYTES_NAME
/
04010, 00000, "Total number of kilobytes served since the Web server was started"
// *Comment: mnemonic: APACHE_R_IO_TOTAL_KBYTES_DESC
/
04011, 00000, "Bytes per Second"
// *Comment: mnemonic: APACHE_R_IO_BYTES_PSEC_NAME
/
04012, 00000, "Rate at which Web server is serving data"
// *Comment: mnemonic: APACHE_R_IO_BYTES_PSEC_DESC
/
04013, 00000, "Requests per Second"
// *Comment: mnemonic: APACHE_R_IO_REQ_PSEC_NAME
/
04014, 00000, "Rate at which Web server is serving requests"
// *Comment: mnemonic: APACHE_R_IO_REQ_PSEC_DESC
/
04015, 00000, "Bytes per Request"
// *Comment: mnemonic: APACHE_R_IO_BYTES_PREQ_NAME
/
04016, 00000, "Average number of bytes served per request served"
// *Comment: mnemonic: APACHE_R_IO_BYTES_PREQ_DESC
/
04017, 00000, "Total Requests Served"
// *Comment: mnemonic: APACHE_R_IO_TOTAL_REQUESTS_NAME
/
04018, 00000, "Total number of requests served since the Web server was started"
// *Comment: mnemonic: APACHE_R_IO_TOTAL_REQUESTS_DESC
/
04019, 00000, "I/O"
// *Comment: mnemonic: APACHE_CH_IO_IO_NAME
/
04020, 00000, "Web server I/O"
// *Comment: mnemonic: APACHE_CH_IO_IO_DESC
/
04021, 00000, "Total Kilobytes Served"
// *Comment: mnemonic: APACHE_CH_IO_TOTAL_KBYTES_NAME
/
04022, 00000, "Total number of kilobytes served since the Web server was started"
// *Comment: mnemonic: APACHE_CH_IO_TOTAL_KBYTES_DESC
/
04023, 00000, "Bytes per Second"
// *Comment: mnemonic: APACHE_CH_IO_BYTES_PSEC_NAME
/
04024, 00000, "Rate at which the Web server is serving data"
// *Comment: mnemonic: APACHE_CH_IO_BYTES_PSEC_DESC
/
04025, 00000, "Requests per Second"
// *Comment: mnemonic: APACHE_CH_IO_REQ_PSEC_NAME
/
04026, 00000, "Rate at which the Web server is serving requests"
// *Comment: mnemonic: APACHE_CH_IO_REQ_PSEC_DESC
/
04027, 00000, "Bytes per Request"
// *Comment: mnemonic: APACHE_CH_IO_BYTES_PREQ_NAME
/
04028, 00000, "Average number of bytes served per request served"
// *Comment: mnemonic: APACHE_CH_IO_BYTES_PREQ_DESC
/
04029, 00000, "Server Utilization"
// *Comment: mnemonic: APACHE_CLASS_PROCESS_NAME
/
04030, 00000, "Web Server"
// *Comment: mnemonic: APACHE_CLASS_PROCESS_INST
/
04031, 00000, "Web server process related statistics"
// *Comment: mnemonic: APACHE_CLASS_PROCESS_DESC
/
04032, 00000, "CPU Load"
// *Comment: mnemonic: APACHE_R_PROC_CPU_LOAD_NAME
/
04033, 00000, "Average CPU load"
// *Comment: mnemonic: APACHE_R_PROC_CPU_LOAD_DESC
/
04034, 00000, "Busy Servers"
// *Comment: mnemonic: APACHE_R_PROC_BUSY_SRV_NAME
/
04035, 00000, "Number of servers busy serving data and requests"
// *Comment: mnemonic: APACHE_R_PROC_BUSY_SRV_DESC
/
04036, 00000, "Idle Servers"
// *Comment: mnemonic: APACHE_R_PROC_IDLE_SRV_NAME
/
04037, 00000, "Number of servers not busy"
// *Comment: mnemonic: APACHE_R_PROC_IDLE_SRV_DESC
/
04038, 00000, "Total Servers"
// *Comment: mnemonic: APACHE_R_PROC_TOTAL_SRV_NAME
/
04039, 00000, "Number of busy and idle servers"
// *Comment: mnemonic: APACHE_R_PROC_TOTAL_SRV_DESC
/
04040, 00000, "Server Utilization"
// *Comment: mnemonic: APACHE_CH_PROC_UTIL_NAME
/
04041, 00000, "Relationship between busy and total servers"
// *Comment: mnemonic: APACHE_CH_PROC_UTIL_DESC
/
04042, 00000, "CPU Load"
// *Comment: mnemonic: APACHE_CH_PROC_CPU_LOAD_NAME
/
04043, 00000, "Average CPU load"
// *Comment: mnemonic: APACHE_CH_PROC_CPU_LOAD_DESC
/
04044, 00000, "Busy Servers"
// *Comment: mnemonic: APACHE_CH_PROC_BUSY_SRV_NAME
/
04045, 00000, "Number of servers busy serving data and requests"
// *Comment: mnemonic: APACHE_CH_PROC_BUSY_SRV_DESC
/
04046, 00000, "Idle Servers "
// *Comment: mnemonic: APACHE_CH_PROC_IDLE_SRV_NAME
/
04047, 00000, "Number of servers not busy"
// *Comment: mnemonic: APACHE_CH_PROC_IDLE_SRV_DESC
/
04048, 00000, "Total Servers"
// *Comment: mnemonic: APACHE_CH_PROC_TOTAL_SRV_NAME
/
04049, 00000, "Number of busy and idle servers"
// *Comment: mnemonic: APACHE_CH_PROC_TOTAL_SRV_DESC
/
04050, 00000, "I/O"
// *Comment: mnemonic: APACHE_CH_GROUP_IO_OVERVIEW_NAME
/
04051, 00000, "Web server I/O overview"
// *Comment: mnemonic: APACHE_CH_GROUP_IO_OVERVIEW_DESC
/
04052, 00000, "Server Utilization"
// *Comment: mnemonic: APACHE_CH_GROUP_UTIL_OVERVIEW_NAME
/
04053, 00000, "Server utilization overview"
// *Comment: mnemonic: APACHE_CH_GROUP_UTIL_OVERVIEW_DESC
/
04054, 00000, "Total Requests Served"
// *Comment: mnemonic: APACHE_CH_IO_TOTAL_REQUESTS_NAME
/
04055, 00000, "Total number of requests served since the Web werver was started"
// *Comment: mnemonic: APACHE_CH_IO_TOTAL_REQUESTS_DESC
/
/ *********************** END OF MESSAGE FILE*******************************
