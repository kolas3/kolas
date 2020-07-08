// Copyright (c) 1991, 1997, 2001 by the Oracle Corporation.  All rights reserved.
/
/
/ $Header: /K2UP_CVS/Repository/KOLAS III/ORACLE/network/mesg/vpsqlsrvus.msg,v 1.1 2009/12/09 02:30:30 ╧за╬©Н Exp $
/
/ NAME                                          
/   svsqlsrvus.msg                                            
/                                                     
/
/ FUNCTION
/       SQL Server Collection Cartridge error message file, American English 
/       version. 
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
/       In all explanations below, the words "specified service" refer
/       to either the database subagent, OR other SNMP-capable service.
/
/
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
/
/
/ 
/ OWNER
/   System Management Products Division
/
/ MODIFIED
/    ndutko     12/19/01 - Provide specific tags for DO NOT TRANSLATE for each
/    glavash    09/15/97 - remove NT messages
/
///////////////////////////////////////////////////////////////////////////
/
/
/
///////////////////////////////////////////////////////////////////////////
1,0,"Error: SQL_SRV_get_sample: SQL_SRVPRIV_get_datapoint failed"
2,0,"Error: NULL state information struct"
3,0,"Error: vppalloc of instance resource lines failed" 
4,0,"Warning: resource id not found in resouce list"
5,0,"Error: Can't free memory, vppfree"
6,0,"SQL Server"
7,0, "ERROR: vp_send_instance failed"
8,0, "Callback not implemented"
9,0, "Comprehensive Chart"
10,0, "Performance Class from SQL Server"
11,0,"Error: 0 resources found for Class"
12,0,"SQL Server Statistics"
13,0,"This cartridge externalizes the SQL Server entries in the Windows NT Performance Registry"
14,0,"Buffer Request Rates"
15,0,"Buffer Page Request Rates"
16,0,"Buffer Cache Hit Ratios"
17,0,"Buffer Cache Size"
18,0,"Buffer Page Counts"
19,0,"Cache Usage Rate"
20,0,"Cache Hit Ratio"
21,0,"Cache Object and Page Counts"
22,0,"Latch Rate"
23,0,"Latch Average Wait"
24,0,"Total Latch Time"
25,0,"Lock Rates"
26,0,"Lock Average Wait"
27,0,"Total Lock Time"
28,0,"Page Access Rates"
29,0,"Scan Access Rates"
30,0,"Misc Access Rates"
31,0,"Work Access Creation Rates"
32,0,"Worktable Access Ratio"
33,0,"Transaction Rates"
34,0,"Transaction Counts"
35,0,"Throughput Rates"
36,0,"Log Bytes Flushed Rate"
37,0,"Log File Rates"
38,0,"Log Cache Ratio"
39,0,"Log File Usage"
40,0,"Log File Counts"
41,0,"Files Sizes"
42,0,"Miscellaneous Rates"
43,0,"Bulk Copy Rows Rate"
44,0,"Logon Rates"
45,0,"Active User Counts"
46,0,"Memory Usage"
47,0,"Memory Lock Block Counts"
48,0,"Memory Grant Counts"
49,0,"SQL Rates"
50,0,"Automatic Parameter Rates"
51,0,"Replication Agent Active Count"
52,0,"Distribution Rates"
53,0,"Distribution Latency"
54,0,"Logreader Rates"
55,0,"Logreader Latency"
56,0,"Merge Rates"
57,0,"Snapshot Rates"
58,0,"Backup Rate"
59,0,"SQL Server at a Glance"
60,0,"Replication Rates"
61,0,"Replication Latency"
/
///////////////////////////////////////////////////////////
/
/ IMPORTANT NOTE TO TRANSLATORS:
/
/ The remainder of the messages in this file should map
/ to entries in the Windows NT performance registry.
/
/ These items should be translated to correspond to the
/ entries in the Windows NT performance registry in the
/ language to which this catalog is being translated.
/
/ This can be done by running the Windows NT performance
/ monitor (see the Administrative Tools program group)
/ and clicking on the "+" button in the toolbar.  The
/ first message after each break "/" below is identified
/ as an "Object" in the NT Performance Monitor, and then
/ all items below that 1st line up to the next break "/"
/ are the "Counters" for that "Object".
/
/ The major object names are listed below and also commented out just prior to the
/ list of resources for each of the objects
/
/
/ SQLSRV 6 classes
/
// NLS_DO_NOT_TRANSLATE [100,4400]
/
100,0,"SQLServer"
101,0,"SQLServer-Locks"
102,0,"SQLServer-Procedure Cache"
103,0,"SQLServer-User Defined Counters"
104,0,"SQLServer-Users"
105,0,"SQLServer-Log"
106,0,"SQLServer Replication-Published DB"
/
200,0,"Read Ahead Manager"
201,0,"Cache Utilization"
202,0,"Network Throughput"
203,0,"I/O Overview"
204,0,"Batch I/O"
205,0,"Summary I/O"
206,0,"Transaction I/O"
207,0,"Outstanding I/O"
208,0,"User Summary"
209,0,"User I/O Utilization" 
210,0,"Log Utilization"
211,0,"Top Locks"

/ SQLSRV 7 class names
300,0, "SQLServer:Buffer Manager"
301,0, "SQLServer:Cache Manager"
302,0, "SQLServer:Latches"
303,0, "SQLServer:Locks"
304,0, "SQLServer:Access Methods"
305,0, "SQLServer:Databases"
306,0, "SQLServer:General Statistics"
307,0, "SQLServer:Memory Manager"
308,0, "SQLServer:SQL Statistics"
309,0, "SQLServer:Replication Agents"
310,0, "SQLServer:Replication Dist."
311,0, "SQLServer:Replication Logreader"
312,0, "SQLServer:Replication Merge"
313,0, "SQLServer:Replication Snapshot"
314,0, "SQLServer:Backup Device"
/
/
/ SQL Server 7 chart descriptions
/
614,0,"Rates per second associated with Buffer Manager Requests"
615,0,"Rates per second associated with Buffer Manager Page Requests"
616,0,"Percentage of hits associated with the Buffer Manager's caches"
617,0,"Size of the Buffer Manager's cache"
618,0,"Number of pages associated with the Buffer Manager"
619,0,"Rates per second associated with Cache Manager usage"
620,0,"Percentage of hits associated with the Cache Manager"
621,0,"Number of object and pages within the Cache Manager"
622,0,"Rates per second associated with the Latch Manager"
623,0,"Average rates per second associated with the Latch Manager"
624,0,"Total time in milliseconds associated with latches"
625,0,"Rates per second associated with the Lock Manager"
626,0,"Average rates per second associated with the Lock Manager"
627,0,"Total time in milliseconds associated with locks"
628,0,"Rates per second associated with Access Methods page operations"
629,0,"Rates per second associated with Access Methods Scan Access operations"
630,0,"Rates per second associated with Access Methods miscellaneous operations"
631,0,"Rates per second associated with Access Methods work creation operations"
632,0,"Percentage of hits associated with the Access Methods worktable access"
633,0,"Rates per second associated with Database transactions"
634,0,"Number of transactions within the Database"
635,0,"Rates per second associated with Database throughputs"
636,0,"Rates per second associated with Database bytes flushed to the Log Files"
637,0,"Rates per second associated with Database Log Files"
638,0,"Percentage of hits associated with the Log File cache"
639,0,"Percentage of space usage associated with the Log Files"
640,0,"Number of operations associated with the Log Files"
641,0,"The sizes of the files associated with the database"
642,0,"Rates per second associated with Database miscellaneous operations"
643,0,"Rates per second associated with Database copying of bulk rows"
644,0,"Rates per second associated with logon operations"
645,0,"Number of users currently active"
646,0,"Amount of memory allocated for use by the various compontents of the database"
647,0,"Number of lock blocks allocated and used in memory"
648,0,"Number of grants related to memory operations"
649,0,"Rates per second associated with SQL commands"
650,0,"Rates per second associated with SQL commands with automatic parameters"
651,0,"Number of active Replication agents"
652,0,"Rates per second associated with Replication distribution"
653,0,"Amount of time spent waiting for Distribution activity"
654,0,"Rates per second associated with the Replication Logreader"
655,0,"Amount of time spent waiting for the Distribution Logreader"
656,0,"Rates per second associated with Replication merging"
657,0,"Rates per second associated with Replication Snapshots"
658,0,"Rates per second associated with backup devices"
659,0,"An overall quick view of the SQL Server performance"
660,0,"Rates per second associated with all aspects of Replication"
661,0,"Amount of time spent waiting for all aspects of Replication"
/
/
/100,0,"System"
1000,0,"I/O - Log Writes/sec"
1001,0,"I/O - Batch Writes/sec"
1002,0,"I/O - Batch Average Size"
1003,0,"I/O - Batch Max Size"
1004,0,"I/O - Page Reads/sec"
1005,0,"I/O - Single page Writes/sec"
1006,0,"I/0 - Outstanding Reads"
1007,0,"I/O - Outstanding Writes"
1008,0,"I/O - Transactions/sec"
1009,0,"I/O - Trans. per Log Record"
1010,0,"Cache Hit Ratio"
1011,0,"I/O - Lazy writes/sec"
1012,0,"Cache - Avg. Free Page Scan"
1013,0,"Cache - Max. Free Page Scan"
1014,0,"Cache - Number of Free Buffers"
1015,0,"NET - Network Reads/sec"
1016,0,"NET - Network Writes/sec"
1017,0,"NET - Command Queue Length"
1018,0,"RA - Pages Found in Cache/sec"
1019,0,"RA - Pages Fetched into Cache/sec"
1020,0,"RA - Physical Reads/sec"
1021,0,"RA - Slots Used"
1022,0,"User Connections"
1023,0,"Max Users Connected"
1024,0,"Max Tempdb Space Used (MB)"
/  
/101,0,"Processor"
1100,0,"Total Locks"
1101,0,"Total Exclusive Locks"
1102,0,"Total Shared Locks"
1103,0,"Total Blocking Locks"
/
/102,0,"Memory"
1200,0,"Total Procedure Buffers"
1201,0,"Procedure Buffers Used %"
/  
/103,0,"Cache"
1300,0,"SQL User Counter 1"
1301,0,"SQL User Counter 2"
1302,0,"SQL User Counter 3"
/
/104,0,"PhysicalDisk"
1400,0,"Memory (2K pages)"
1401,0,"CPU time"
1402,0,"Physical I/O"
1403,0,"Locks held"
/
/105,0,"LogicalDisk"
1500,0,"Log Size (MB)"
1501,0,"Log Space Used(%)"
1502,0,"Max Log Space Used (MB)"
1503,0,"Max Log Space Used (%)"
/  
/106,0,"Process"
1600,0,"Replicated Transactions"
/  
/ SQLSRV 7 resource names
/
/SQLServer:Buffer Manager
3000,0, "Buffer Cache Hit Ratio"
3001,0, "Cache Size (pages)"
3002,0, "Checkpoint Writes/sec"
3003,0, "Committed Pages"
3004,0, "ExtendedMem Cache Hit Ratio"
3005,0, "ExtendedMem Cache Migrations/sec"
3006,0, "ExtendedMem Requests/sec"
3007,0, "Free Buffers"
3008,0, "Lazy Writer Buffers/sec"
3009,0, "Lazy Writes/sec"
3010,0, "Page Reads/sec"
3011,0, "Page Requests/sec"
3012,0, "Page Writes/sec"
3013,0, "Readahead Pages/sec"
3014,0, "Reserved Page Count"
3015,0, "Stolen Page Count"
/
/SQLServer:Cache Manager
3100,0, "Cache Hit Ratio"
3101,0, "Cache Object Counts"
3102,0, "Cache Pages"
3103,0, "Cache Use Counts/sec"
/
/SQLServer:Latches
3200,0, "Average Latch Wait Time (ms)"
3201,0, "Latch Waits/sec"
3202,0, "Total Latch Wait Time (ms)"
/
/SQLServer:Locks
3300,0, "Average Wait Time (ms)"
3301,0, "Lock Requests/sec"
3302,0, "Lock Timeouts/sec"
3303,0, "Lock Wait Time (ms)"
3304,0, "Lock Waits/sec"
3305,0, "Number of Deadlocks/sec"
/
/SQLServer:Access Methods
3400,0, "Extent Deallocations/sec"
3401,0, "Extents Allocated/sec"
3402,0, "Forwarded Records/sec"
3403,0, "FreeSpace Page Fetches/sec"
3404,0, "FreeSpace Scans/sec"
3405,0, "Full Scans/sec"
3406,0, "Index Searches/sec"
3407,0, "Mixed page allocations/sec"
3408,0, "Page Deallocations/sec"
3409,0, "Page Splits/sec"
3410,0, "Pages Allocated/sec"
3411,0, "Probe Scans/sec"
3412,0, "Range Scans/sec"
3413,0, "Scan Point Revalidations/sec"
3414,0, "Skipped Ghosted Records/sec"
3415,0, "Table Lock Escalations/sec"
3416,0, "Workfiles Created/sec"
3417,0, "Worktables Created/sec"
3418,0, "Worktables From Cache Ratio"
/
/SQLServer:Databases
3500,0, "Active Transactions"
3501,0, "Backup/Restore Throughput/sec"
3502,0, "Bulk Copy Rows/sec"
3503,0, "Bulk Copy Throughput/sec"
3504,0, "Data File(s) Size (KB)"
3505,0, "DBCC Logical Scan Bytes/sec"
3506,0, "Log Bytes Per Flush"
3507,0, "Log Cache Hit Ratio"
3508,0, "Log Cache Reads/sec"
3509,0, "Log File(s) Size (KB)"
3510,0, "Log Flush Waits/sec"
3511,0, "Log Flushes/sec"
3512,0, "Log Growths"
3513,0, "Log Shrinks"
3514,0, "Log Truncations"
3515,0, "Percent Log Used"
3516,0, "Repl. Pending Xacts"
3517,0, "Repl. Trans. Rate"
3518,0, "Shrink Data Movement Bytes/sec"
3519,0, "Transactions/sec"
3520,0, "Log Flush Wait Time"
/
/SQLServer:General Statistics
3600,0, "Logins/sec"
3601,0, "Logouts/sec"
3602,0, "User Connections"
/
/SQLServer:Memory Manager
3700,0, "Connection Memory (KB)"
3701,0, "Granted Workspace Memory (KB)"
3702,0, "Lock Blocks"
3703,0, "Lock Blocks Allocated"
3704,0, "Lock Memory (KB)"
3705,0, "Lock Owner Blocks"
3706,0, "Lock Owner Blocks Allocated"
3707,0, "Maximum Workspace Memory (KB)"
3708,0, "Memory Grants Outstanding"
3709,0, "Memory Grants Pending"
3710,0, "Optimizer Memory (KB)"
3711,0, "SQL Cache Memory (KB)"
3712,0, "Target Server Memory(KB)"
3713,0, "Total Server Memory (KB)"
/
/SQLServer:SQL Statistics
3800,0, "Auto-Param Attempts/sec"
3801,0, "Batch Requests/sec"
3802,0, "Failed Auto-Params/sec"
3803,0, "Safe Auto-Params/sec"
3804,0, "SQL Compilations/sec"
3805,0, "SQL Re-Compilations/sec"
3806,0, "Unsafe Auto-Params/sec"
/
/SQLServer:Replication Agents
3900,0, "Running"
/
/SQLServer:Replication Dist.
4000,0, "Dist:Delivered Cmds/sec"
4001,0, "Dist:Delivered Trans/sec"
4002,0, "Dist:Delivery Latency"
/
/SQLServer:Replication Logreader
4100,0, "Logreader:Delivered Cmds/sec"
4101,0, "Logreader:Delivered Trans/sec"
4102,0, "Logreader:Delivery Latency"
/
/SQLServer:Replication Merge
4200,0, "Conflicts/sec"
4201,0, "Downloaded Changes/sec"
4202,0, "Uploaded Changes/sec"
/
/SQLServer:Replication Snapshot
4300,0, "Snapshot:Delivered Cmds/sec"
4301,0, "Snapshot:Delivered Trans/sec"
/
/SQLServer:Backup Device
4400,0, "Device Throughput Bytes/sec"
/
/
// NLS_DO_NOT_TRANSLATE [9999,9999]
/
9999,0,"Bug fixer"
