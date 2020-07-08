///////////////////////////////////////////////////////////////////////////////
// Copyright (c) 1991, 1997, 1999, 2000, 2001 by the Oracle Corporation.  All rights reserved.
/
/
/ $Header: /K2UP_CVS/Repository/KOLAS III/ORACLE/network/mesg/smampus.msg,v 1.1 2009/12/09 02:30:15 박종운 Exp $
/ $Header: /K2UP_CVS/Repository/KOLAS III/ORACLE/network/mesg/smampus.msg,v 1.1 2009/12/09 02:30:15 박종운 Exp $
/
/
/ NAME                                          
/   smampus.msg                                            
/                                                     
/
/ FUNCTION
/   Oracle Applications Management error message file, American English version.  
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
/
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
/
/
/ 
/ OWNER
/    Oracle Management Pack for Oracle Applications  
/
/ MODIFIED
/    yaofeng    02/08/01 - add concurrent manager name
/    yaofeng    01/03/01 - NLS for CPTA repository
/    caroy      11/03/00 - Add in missing errors
/    vtummala   08/25/99 - Bug# 942751
/    vtummala     03/26/99 
/    vtummala     08/03/99
/    vtummala     08/09/99
/    vtummala     08/10/99
/    vtummala     08/11/99
/
/
/
///////////////////////////////////////////////////////////////////////////
/
////////////////////////////////////////////////////////////////////////////
/
/ 6000-6999  Oracle Applications Events Error Messages 
/
///////////////////////////////////////////////////////////////////////////
/
06000, 00000, " %s unable to connect to concurrent processing server using the given credentials"
// *Cause: The ID, password, or service name is invalid.
// *Action: Check the preferred credentials for the
//    concurrent manager in the system preferences dialog box.
/
06001, 00000, " %s unable to proceed with the execution" 
// *Cause:  Failed to open a cursor
// *Action: Verify that the maximum cursors limit has not been exceeded.
/
06002, 00000, " %s script failure"
// *Cause:  Unable to find or run the package successfully 
// *Action: Verify that there are no invalid objects in the applications
//	    schema. 
/
06003,00000, " %s unable to find the TNS address"
// *Cause:  The agent could not find the TNS address for the target object
// *Action: Check the tnsnames.ora to make sure it has the correct 
//	    TNS address for the target object. 
/
06004,00000, " %s unable to contact the listener on the managed node"
// *Cause:  The agent could not contact the listener on the managed node.
// *Action: Check to see if the listener is up on the managed node.
/
06005,00000, " %s unable to reach the target object on the managed node"
// *Cause:  The agent could not reach the target object on the managed node.
// *Action: Check on the availability of the managed node for the target object.
/
06006,00000, " %s invalid login/passwd for the concurrent manager service"
// *Cause:  The agent could not login to the oracle applications database instance
// *Action: Check the preferred credentials for the concurrent manager service
/           in the system preferences dialog box.
/
06007, 00000, "could not locate the environment file %s"
// *Cause:  The applications environment file is not to be found in the 
//          applications file system. 
// *Action: Check that the APPLSYS.env file is present in APPL_TOP
/
06008, 00000, "could not open the environment file %s for reading"
// *Cause:  The environment file could not be opened. 
// *Action: Check the permissions of the environment file.
/
06009, 00000, "target %s not found in discovery file %s"
// *Cause:  The target could not be found in the discovery file. 
// *Action: Contact Oracle Customer Support.
/
06010, 00000, "could not find APPL_TOP, APPLOG and APPLOUT"
// *Cause:  The environment file does not contain the corresponding 
//	    definitions.
// *Action: Define all the neccessary variables in the environment file
//	    or specify the complete disk name using the Other option. 
/
06011, 00000, "Unable to compute diskusage. Check if %s is valid"
// *Cause:  The disk usage could not be computed.
// *Action: Check if the disk name is accurate.
/
06012, 00000, "No valid managers found."
// *Cause:  Managers could not be checked for status.
// *Action: Check if the managers are disabled or deactivated.
/
06013, 00000, "Invalid rows fetched from query."
// *Cause:  Managers could not be checked for status.
// *Action: Check that all managers are not disabled or deactivated.
/
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
/
/ 7000 - 7999 Result Strings to be sent to the Console 
/
/ Only the first 3 of these messages are still used 
/   The others are commented out and should not be translated.
/ 
/
///////////////////////////////////////////////////////////////////////////
/
07000, 00000, "Request ID             Requested By           Application Name                                Concurrent Program Name                         Hours Runalone"
// *Cause:  
// *Action:
/
07001, 00000, "Request ID             Requested By           Application Name                                Concurrent Program Name                         Hours Inactive"
// *Cause:  
// *Action:
/
07002, 00000, "Request ID             Requested By           Application Name                                Concurrent Program Name                         Hours Pending"
// *Cause:  
// *Action:
/
/07000, 00000, " The following session exceeded the Warning threshold -
/Session id: %s , OS Process id: %s ,OS User name : %s ,Lock Mode  : %s,
/Machine name  : %s , Terminal name : %s" 
// *Cause: 
// *Action:
/
/
/07001, 00000, " The following session exceeded the Alert threshold -
/Session id: %s , OS Process id: %s ,OS User name : %s ,Lock Mode  : %s,
/Machine name  : %s , Terminal name : %s" 
// *Cause: 
// *Action:
/
/07002, 00000, " The request error rate is %s and exceeds the alert threshold of %s"
// *Cause: 
// *Action:
/
/
/07003, 00000, " The request error rate is %s and exceeds the warning threshold of %s"
// *Cause: 
// *Action:
/
/
/07004, 00000, " The request warning rate is %s and exceeds the alert threshold of %s"
// *Cause: 
// *Action:
/
/
/07005, 00000, " The request warning rate is %s and exceeds the warning threshold of %s"
// *Cause: 
// *Action:
/
/
/07006, 00000, " The Internal Concurrent Manager  has been blocked by the session %s for 
/ a time period of %s , longer than the Warning threshold %s . The Details of the blocking 
/ session are - OS process id  %s, OS User name %s, Lock mode %s, Machine name %s, 
/ Terminal name %s"
// *Cause:  The Internal Concurrent Manager is blocked and is waiting for a lock
// *Action: If the waiting time is not acceptable, kill the session that's blocking the 
/  Internal Concurrent Manager
/
/
/07007, 00000, " The Internal Concurrent Manager has been blocked by the session %s for a 
/ time period of %s , longer than the Alert threshold %s . The Details of the blocking 
/ session are - OS process id  %s, OS User name %s, Lock mode %s, Machine name %s, 
/ Terminal name %s"
// *Cause: The Internal Concurrent Manager is blocked and is waiting for a lock 
// *Action:If  the waiting time is not acceptable, kill the session that's blocking the 
/  Internal Concurrent Manager 
/
/
/07008, 00000, " The Conflict Resolution Manager has been blocked by the session %s for a 
/ time period of %s , longer than the Warning threshold %s . The Details of the blocking 
/ session are - OS process id  %s, OS User name %s, Lock mode %s, Machine name %s, 
/ Terminal name %s"
// *Cause: The Conflict Resolution Manager is blocked and is waiting for a lock 
// *Action:If  the waiting time is not acceptable, kill the session that's blocking 
/  the Conflict Resolution Manager
/
/
/07009, 00000, " The Conflict Resolution Manager has been blocked by the session %s for a 
/ time period of %s , longer than the Alert threshold %s . The Details of the blocking 
/ session are - OS process id  %s, OS User name %s, Lock mode %s, Machine name %s, 
/ Terminal name %s"
// *Cause: The Conflict Resolution Manager is blocked and is waiting for a lock 
// *Action:If  the waiting time is not acceptable, kill the session that's blocking the 
/  Conflict Resolution Manager
/
/
/07010, 00000, " The Internal Concurrent manager %s is down"
// *Cause: 
// *Action: Restart the Concurrent Manager by submitting a Concurrent Manager Start up job
/
/
/07011, 00000, " %s Run Alone requests existing in the Concurrent manager Queues"
// *Cause: 
// *Action: A Run alone request blocks out all other requests. Make sure that
//    this request is submitted on purporse 
/
/
/07012, 00000, " %s inactive requests existing in the Concurrent manager Queues"
// *Cause: 
// *Action: 
/
/
/07013, 00000, " %s concurrent requests have been pending for a time period %s longer 
/ than the specified threshold %s"
// *Cause: 
// *Action:
/
/
/07014, 00000, " The following partition(s) %s , %s, %s have exceeded the alert threshold 
/ and their disk consumption is %s, %s, %s against the set thresholds 
/of %s , %s, %s "
// *Cause:  Disk partitions are growing in size 
// *Action: Purge the Concurrent manager log and output files 
/
/
/07015, 00000, " The following partition(s) %s , %s, %s have exceeded the warning threshold 
/ and their disk consumption is %s, %s, %s against the set thresholds 
/of %s , %s, %s "
// *Cause:  Disk partitions are growing in size
// *Action: Purge the Concurrent manager log and output files  
/
/
////////////////////////////////////////////////////////////////////////////
/
/ 8000-8999  Oracle Applications Jobs Diagnostic Messages 
/
///////////////////////////////////////////////////////////////////////////
/
08000, 00000, "target platform %s not supported"
// *Cause:  The platform in use is not supported.
// *Action: Check the product certification matrix.
/           
/
08001, 00000, "could not locate the service file %s"
// *Cause:  The services file is not to be found in the $ORACLE_HOME/network/agent
//          directory.
// *Action: Check that the agent has been started properly.
/
08002, 00000, "could not open the service file %s for reading"
// *Cause:  The services file could not be opened. 
// *Action: Check the permissions of the services file.
/
08003, 00000, "the file %s is not executable"
// *Cause:  The file does not have execute permission.
// *Action: Check the permissions of the file.
/
08004, 00000, "execution of batch file %s failed with %s "
// *Cause:  Batch file execution has failed.
// *Action: Check the output for more information.
/
08005, 00000, "Concurrent manager startup job execution has completed with output of %s"
// *Cause:  
// *Action:
/
08006, 00000, "target %s of type %s not found in discovery file %s"
// *Cause:  The discovered target disappeared from the discovery file 
// *Action: Contact Oracle Customer Support.
/
08007, 00000, "Concurrent manager shutdown job was submitted by appsuser %s, %s/<pwd>@%s"
// *Cause:  
// *Action: 
/
08008, 00000, "failed to connect to database using %s/<pwd>@%s"
// *Cause:  The Agent could not connect to the database using the credentials supplied.
// *Action: Check the credentials for accuracy. 
/
08009, 00000, "failed to open cursor after connecting to %s/<pwd>@%s"
// *Cause:  Could not open a cursor. 
// *Action: Verify that the open cursors limit has not been exceeded.
/
08010, 00000, "[%s] PL/SQL call failed, using: %s" 
// *Cause:  The PL/SQL call failed   
// *Action: Check the error text and follow the instructions.
/
08011, 00000, "The value of error text field is %s" 
// *Cause:  
// *Action: 
/
08012, 00000, "The value of ocifunc is %s" 
// *Cause:  
// *Action: 
/
08013, 00000, "The value of sqlfunc is %s" 
// *Cause:  
// *Action: 
/
08014, 00000, "The return value is: %s" 
// *Cause:  
// *Action: 
/
08015, 00000, "expected return value is %s, and the actual value is %s" 
// *Cause:  
// *Action: 
/
08016, 00000, " %s commit operation failed" 
// *Cause:  
// *Action: 
/
08017, 00000, "Concurrent manager shutdown job was committed successfully at %s" 
// *Cause:  
// *Action: 
/
08018, 00000, "Kill session job was submitted by applications user %s, %s/<pwd>@%s" 
// *Cause:  
// *Action: 
/
08019, 00000, "Kill session job was committed successfully at %s" 
// *Cause:  
// *Action: 
/
08020, 00000, "Kill session invoked as a standalone job"
// *Cause:  
// *Action: 
/
08021, 00000, "net stop failed with output of: %s"
// *Cause:  
// *Action: 
/
08022, 00000, "service stopped using net stop with output of: %s"
// *Cause:  
// *Action: 
/
08023, 00000, "[%s] PL/SQL call did not report success, using: %s" 
// *Cause:  The PL/SQL call did not report success.
// *Action: Check the return list. 
/
08024, 00000, " %s commit operation succeeded" 
// *Cause:  
// *Action: 
/
08025, 00000, "net start failed with output of: %s"
// *Cause:  
// *Action: 
/
08026, 00000, "service started using net start with output of: %s"
// *Cause:  
// *Action: 
/
08027, 00000, "Kill session invoked on a warning severity"
// *Cause:  
// *Action: 
/
08028, 00000, "[%s] SQL call failed, using: %s" 
// *Cause:  The SQL call failed   
// *Action: Check the error text and follow the instructions.
/
08029, 00000, "The pre-requisite patch not applied to the Applications instance"
// *Cause:  The pre-requisite patch for the Management Pack for Oracle Applications
//          has not been applied to the Applications instance.
// *Action: Refer to the doc for details and apply the pre-requisite patch.
/
08030, 00000, "This repository is already being used by a different concurrent manager: %s"
// *Cause:  A different concurrent manager is using this repoistory.
// *Action: Drop all tables in this repository or use another repository.
/
08031, 00000, "Connecting to the application schema %s/<pwd>@%s"
// *Cause:  
// *Action: 
/
08032, 00000, "Query the database global name"
// *Cause: 
// *Action:
/
08033, 00000, "Close the connection"
// *Cause:
// *Action:
/
08034, 00000, "Connecting to the Concurrent Processing Tuning Assistant repository schema %s/<pwd>@%s"
// *Cause:
// *Action:
/
08035, 00000, "Create link to the applications database"
// *Cause:
// *Action:
/
08036, 00000, "Download data from the applications instance to the Concurrent Processing Tuning Assistant schema"
// *Cause:
// *Action:
/
08037, 00000, "Job completed successfully at %s"
// *Cause:
// *Action:
/
////////////////////////////////////////////////////////////////////////////
/
/ 9000-9999  Oracle Applications Discovery, Misc Diagnostic Messages 
/
///////////////////////////////////////////////////////////////////////////
/
09000, 00000, "DBSNMP discovery warning: file open failure"
// *Cause:  The file could not be opened. 
// *Action: Check if the file has read permission.
/
09001, 00000, "DBSNMP discovery warning: errors in %s"
// *Cause:  
// *Action: 
/
09002, 00000, "There are %s line(s) with incorrect first field."
// *Cause: 
// *Action: 
/
09003, 00000, "There are %s line(s) with incorrect number of fields: %s."
// *Cause: 
// *Action: 
/
09004, 00000, "There are %s duplicate name(s) in oapps.ora. Only the first occurance in %s is valid."
// *Cause: 
// *Action: 
/
// *Action: 
/
09005, 00000, "There are %s entry(s) in oapps.ora that duplicate other discovered service names(ignored): %s"
// *Cause: 
// *Action: 
/
09006, 00000, "Oracle internal error"
// *Cause: An internal error occured. 
// *Action:Contact Oracle Customer Support.
/
