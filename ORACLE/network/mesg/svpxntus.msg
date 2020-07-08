// Copyright (c) 1991, 1997, 2000, 2001 by the Oracle Corporation.  All rights reserved.
/
/
/ $Header: /K2UP_CVS/Repository/KOLAS III/ORACLE/network/mesg/svpxntus.msg,v 1.1 2009/12/09 02:30:18 ╧за╬©Н Exp $
/
/ NAME                                          
/   svpxntus.msg                                            
/                                                     
/
/ FUNCTION
/       Windows NT Collection Cartridge error message file, American English 
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
/    glavash    10/25/01 - put in NLS DONT TRANSLATE TAGS
/    glavash    01/09/01 - update TOT IO class name
/    gachen     11/13/00 -
/    cfowler    08/15/00 - Statspack: add common metric chart
/    glavash    08/03/99 - put ASP chart descriptions
/    glavash    03/17/97 - put in correct order
/    jriel      03/16/97 - merge from old catalog
/
///////////////////////////////////////////////////////////////////////////
/
/
/
///////////////////////////////////////////////////////////////////////////
1,0,"Error: NT_OS_get_sample: NT_OSPRIV_get_datapoint failed"
2,0,"Error: NULL state information struct"
3,0,"Error: vppalloc of instance resource lines failed" 
4,0,"Warning: resource id not found in resouce list"
5,0,"Error: Can't free memory, vppfree"
6,0,"NT Operating System"
7,0, "ERROR: vp_send_instance failed"
8,0, "Callback not implemented"
9,0, " Default Chart"
10,0, "Performance Class from NT"
11,0,"Overall Operating System Statistics"
12,0,"Processor Statistics, CPU activitiy"
13,0,"Memory Statistics, what's used and what's available"
14,0,"Cache Utilization Statistics"
15,0,"Physical Device Statitistics"
16,0,"Logical Disk Statistics (Partitions)"
17,0,"Process Statistics on Individual Processes"
18,0,"Thread Statistics on a Per-Thread basis"
19,0,"System Objects: Processes, Threads, Semaphores, etc"
20,0,"Page File Usage"
21,0,"Network Performance Statistics"
30,0,"NT Performance Statistics"
31,0,"Operating System Statistics Default Chart"
32,0, "Processor state statistics"
33,0,"All available System Wide Memory Statistics"
34,0,"System Cache statistics and ratios"
35,0,"Physical Disk I/O Statistics, Writes, Reads, etc"
36,0,"Logical Disk I/O's, broken down by partition"
37,0,"Process Statistics, lifeline information on individual processes"
38,0,"Thread Statistics on a per thread basis, cpu, user, elapsed"
39,0,"Objects: Counts of various system objects in use at the time"
40,0,"Page Flile Usage, current and peak"
41,0,"Network Interface Statistics, bytes sent/recieved, packets"
50,0,"Non-Idle System Time (all processors)"
51,0,"Processor, User, Privileged Time across all processors"
52,0,"Error: 0 resources found for Class"
53,0,"Windows NT Operating System Statistics"
54,0,"This cartridge externalizes the entries found in the Windows NT Performance Registry"
55,0,"Per-Processor Non-Idle Time"
56,0,"User, Privileged Time for each processor"
57,0,"Per-Process Processor Utilization"
58,0,"User, Privileged Time breakdown per process"
59,0,"System Wide I/O Rates"
60,0,"Read/Write Operations per second, R/W bytes per second"
61,0,"System Emulation or Fixup Operations"
62,0,"Processor emulation operations and exception handling"
63,0,"System Interrupts and DPCs"
64,0,"System Deferred Procedure Calls and Interrupt rates"
65,0,"Processor Interrupts and DPCs"
66,0,"Per Processor Deferred Procedure Calls and Interrupt rates"
67,0,"Memory Faults"
68,0,"Page, Write Copies, Cache, Transition, Demand Zero"
69,0,"Memory Usage"
70,0,"Available and Committed Memory"
71,0,"Resident Memory Use Detail"
72,0,"Catagorical Breakdown of Resident Memory"
73,0,"Paging Statistics"
74,0,"Overall Paging, Reads, Writes, Input, Output"
75,0,"Cache Hit Ratios"
76,0,"Cache Hit Statistics"
77,0,"Physical Disk Access %"
78,0,"Percentage of Disk Access Time"
79,0,"Physical Disk Operations "
80,0,"Reads, Writes, Transfer Counts"
81,0,"Physical Disk Byte Counts"
82,0,"Read, Write, Transfer Byte Counts"
83,0,"Logical Disk Access %"
84,0,"Disk Access Time Breakdown"
85,0,"Logical Disk Byte Counts"
86,0,"Bytes Read,Written,Transferred"
87,0,"Logical Disk Operations"
88,0,"Read,Write,Transfer Operation Counts"
89,0,"Thread Processor Time"
90,0,"Percentage of Time spent in User or Privilged Mode and Total"
91,0,"Object Counts"
92,0,"Counts of various objects on system"
93,0,"Network Byte Counts"
94,0,"Network Traffic In/Out in Bytes"
95,0,"Network Packet Counts"
96,0,"Network Traffic In/Out in Packets"
97,0,"Packets Recieved"
98,0,"Incoming Packets by Type"
99,0,"Packets Sent"
100,0,"Outgoing Packets by Type"
101,0,"Per Process Thread and Handle Counts"
102,0,"Counts of Threads and Handles by Process"
103,0,"Free Space Per Disk"
104,0,"Bytes available on each disk"
105,0,"System At A Glance"
106,0,"Key System Metrics"
107,0,"Page File Usage"
108,0,"Percent of Page File in Use"
109,0,"System I/O Overview"
110,0,"System I/O Operations"
111,0,"Disk Space Percent Free"
112,0,"Percentage Of Disk Available"
113,0,"Process Total Memory"
114,0,"Memory use by Process"
115,0,"Per-Process Detail Memory Use"
116,0,"Private, Page File, Pool Paged, Pool Non-Paged"
117,0,"Synch Cache Operations"
118,0,"Synch Data Maps, Pins, Copy Reads, MDL Reads"
119,0,"Asynch Cache Operations"
120,0,"Asynch Data Maps, Pins, Copy Reads, MDL Reads"
121,0,"Cache Operations"
122,0,"Data Maps, Pins, Copy Reads, MDL Reads"
123,0,"Cache Flushes"
124,0,"Lazy Write and Data Flushes"
125,0,"Cache Page Flushes"
126,0,"Page Counts for Lazy Write, Data Flushes"
127,0,"Cache at a Glance"
128,0,"Comprehensive Cache Statistics"
129,0,"Physical Disk Queue Lengths"
130,0,"Queue Lengths for each Physical Disk"
131,0,"Logical Disk Queue Lengths"
132,0,"Queue Lengths for each Logical Disk"
133,0,"Logical Disks at a Glance"
134,0,"Comprehensive Logical Disk Statistics"
135,0,"Thread Elapsed Time"
136,0,"Elasped Time Per Thread"
137,0,"Thread Context Switches"
138,0,"Thread Context Switches Per Second"
139,0,"System Calls and Context Switches"
140,0,"System Calls and Context Switches Per Second"
141,0,"Processor Queue Length"
142,0,"Current Processor Queue Length"
143,0,"Memory at a Glance"
144,0,"Comprehensive Memory Statistics"
145,0,"Physical Disks at a Glance"
146,0,"Comprehensive Physical Disk Statistics"
147,0,"Pagefaults Per Process"
148,0,"Pagefaults Per Second"
149,0,"Per Process Peak Memory"
150,0,"Peak Memory Usage Per Process"
151,0,"Processes at a Glance"
152,0,"Comprehensive Process Statistics"
153,0,"System Processer Time Saturation Analysis"
154,0,"System User Time Saturation Analysis"
155,0,"Processer Time Saturation Analysis"
156,0,"User Time Saturation Analysis"
157,0,"Memory Utilization Analysis"
158,0,"Physical Disk Read Rate Trend"
159,0,"Physical Disk Write Rate Trend"
160,0,"Logical Disk Full Analysis"
161,0,"CPU Bound Process Analysis"
162,0,"Memory Bound Process Analysis"
163,0,"Pagefile Full Analysis"
164,0,"Active Server Error Breakdown"
165,0,"Active Server Error Rate"
166,0,"Active Server Memory Usage"
167,0,"Active Server Request Bytes"
168,0,"Active Server Request Times"
169,0,"Active Server Request Counts"
170,0,"Active Server Scripts"
171,0,"Active Server Session Duration"
172,0,"Active Server Current Sessions"
173,0,"Active Server Template Cache"
174,0,"Active Server Template Statistics"
175,0,"Active Server Transaction Breakdown"
176,0,"CPU Usage Percentage"
177,0,"The percentage of the CPU being used"
178,0,"Processor CPU usage"
179,0,"The percentage of the CPU being used per processor"
180,0,"OS Statistics for Database Diagnostics"
181,0,"Operating system statistics that determine database performance"
182,0,"Average Physical Disk Access Times"
183,0,"Average times in ms to perform the various actions"
184,0,"Average Logical Disk Access Times"
185,0,"Average times in ms to perform the various actions"

// CSF: StatsPack names/descriptions
00250, 00000, "System Common Metrics Chart Group"
// *Comment: mnemonic: GEN_CHARTGROUP_NAME
00251, 00000, "Overview of critical system metrics common to most platforms"
// *Comment: mnemonic: GEN_CHARTGROUP_DESC

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
// NLS_DO_NOT_TRANSLATE [1001,1001]
/
1001,0,"System"
/
// NLS_DO_NOT_TRANSLATE [1002,1002]
/
1002,0,"Processor"
/
// NLS_DO_NOT_TRANSLATE [1003,1003]
/
1003,0,"Memory"
/
// NLS_DO_NOT_TRANSLATE [1004,1004]
/
1004,0,"Cache"
/
// NLS_DO_NOT_TRANSLATE [1005,1005]
/
1005,0,"PhysicalDisk"
/
// NLS_DO_NOT_TRANSLATE [1006,1006]
/
1006,0,"LogicalDisk"
/
// NLS_DO_NOT_TRANSLATE [1008,1007]
/
1007,0,"Process"
/
// NLS_DO_NOT_TRANSLATE [1008,1008]
/
1008,0,"Thread"
/
// NLS_DO_NOT_TRANSLATE [1009,1009]
/
1009,0,"Objects"
/
// NLS_DO_NOT_TRANSLATE [1010,1010]
/
1010,0,"Paging File"
/
// NLS_DO_NOT_TRANSLATE [1011,1011]
/
1011,0,"Network Interface"
/
// NLS_DO_NOT_TRANSLATE [1012,1012]
/
1012,0,"Active Server Pages"
/
/1001,0,"System"
/
// NLS_DO_NOT_TRANSLATE [1100,1100]
/
1100,0,"File Read Operations/sec"
/
// NLS_DO_NOT_TRANSLATE [1101,1101]
/
1101,0,"File Write Operations/sec"
/
// NLS_DO_NOT_TRANSLATE [1102,1102]
/
1102,0,"File Control Operations/sec"
/
// NLS_DO_NOT_TRANSLATE [1103,1103]
/
1103,0,"File Read Bytes/sec"
/
// NLS_DO_NOT_TRANSLATE [1104,1104]
/
1104,0,"File Write Bytes/sec"
/
// NLS_DO_NOT_TRANSLATE [1105,1105]
/
1105,0,"File Control Bytes/sec"
/
// NLS_DO_NOT_TRANSLATE [1106,1106]
/
1106,0,"Context Switches/sec"
/
// NLS_DO_NOT_TRANSLATE [1107,1107]
/
1107,0,"System Calls/sec"
/
// NLS_DO_NOT_TRANSLATE [1108,1108]
/
1108,0,"% Total Processor Time"
/
// NLS_DO_NOT_TRANSLATE [1109,1109]
/
1109,0,"% Total User Time"
/
// NLS_DO_NOT_TRANSLATE [1110,1110]
/
1110,0,"% Total Privileged Time"
/
// NLS_DO_NOT_TRANSLATE [1111,1111]
/
1111,0,"Total Interrupts/sec"
/
// NLS_DO_NOT_TRANSLATE [1112,1112]
/
1112,0,"File Data Operations/sec"
/
// NLS_DO_NOT_TRANSLATE [1113,1113]
/
1113,0,"System Up Time"
/
// NLS_DO_NOT_TRANSLATE [1114,1114]
/
1114,0,"Processor Queue Length"
/
// NLS_DO_NOT_TRANSLATE [1115,1115]
/
1115,0,"Alignment Fixups/sec"
/
// NLS_DO_NOT_TRANSLATE [1116,1116]
/
1116,0,"Exception Dispatches/sec"
/
// NLS_DO_NOT_TRANSLATE [1117,1117]
/
1117,0,"Floating Emulations/sec"
/
// NLS_DO_NOT_TRANSLATE [1118,1118]
/
1118,0,"% Total DPC Time"
/
// NLS_DO_NOT_TRANSLATE [1119,1119]
/
1119,0,"% Total Interrupt Time"
/
// NLS_DO_NOT_TRANSLATE [1120,1120]
/
1120,0,"Total DPCs Queued/sec"
/
// NLS_DO_NOT_TRANSLATE [1121,1121]
/
1121,0,"Total DPC Rate"
/
// NLS_DO_NOT_TRANSLATE [1122,1122]
/
1122,0,"Total DPC Bypasses/sec"
/
// NLS_DO_NOT_TRANSLATE [1123,1123]
/
1123,0,"Total APC Bypasses/sec"
/
// NLS_DO_NOT_TRANSLATE [1124,1124]
/
1124,0,"% Registry Quota In Use"
/
/1002,0,"Processor"
/
// NLS_DO_NOT_TRANSLATE [1200,1200]
/
1200,0,"% Processor Time"
/
// NLS_DO_NOT_TRANSLATE [1201,1201]
/
1201,0,"% User Time"
/
// NLS_DO_NOT_TRANSLATE [1202,1202]
/
1202,0,"% Privileged Time"
/
// NLS_DO_NOT_TRANSLATE [1203,1203]
/
1203,0,"Interrupts/sec"
/
// NLS_DO_NOT_TRANSLATE [1204,1204]
/
1204,0,"% DPC Time"
/
// NLS_DO_NOT_TRANSLATE [1205,1205]
/
1205,0,"% Interrupt Time"
/
// NLS_DO_NOT_TRANSLATE [1206,1206]
/
1206,0,"DPCs Queued/sec"
/
// NLS_DO_NOT_TRANSLATE [1207,1207]
/
1207,0,"DPC Rate"
/
// NLS_DO_NOT_TRANSLATE [1208,1208]
/
1208,0,"DPC Bypasses/sec"
/
// NLS_DO_NOT_TRANSLATE [1209,1209]
/
1209,0,"APC Bypasses/sec"
/
/1003,0,"Memory"
/
// NLS_DO_NOT_TRANSLATE [1300,1300]
/
1300,0,"Available Bytes"
/
// NLS_DO_NOT_TRANSLATE [1300,1300]
/
1301,0,"Committed Bytes"
/
// NLS_DO_NOT_TRANSLATE [1301,1301]
/
1302,0,"Commit Limit"
/
// NLS_DO_NOT_TRANSLATE [1302,1302]
/
1303,0,"Page Faults/sec"
/
// NLS_DO_NOT_TRANSLATE [1303,1303]
/
1304,0,"Write Copies/sec"
/
// NLS_DO_NOT_TRANSLATE [1305,1304]
/
1305,0,"Transition Faults/sec"
/
// NLS_DO_NOT_TRANSLATE [1306,1306]
/
1306,0,"Cache Faults/sec"
/
// NLS_DO_NOT_TRANSLATE [1307,1308]
/
1307,0,"Demand Zero Faults/sec"
/
// NLS_DO_NOT_TRANSLATE [1308,1308]
/
1308,0,"Pages/sec"
/
// NLS_DO_NOT_TRANSLATE [1309,1309]
/
1309,0,"Pages Input/sec"
/
// NLS_DO_NOT_TRANSLATE [1310,1310]
/
1310,0,"Page Reads/sec"
/
// NLS_DO_NOT_TRANSLATE [1311,1311]
/
1311,0,"Pages Output/sec"
/
// NLS_DO_NOT_TRANSLATE [1312,1312]
/
1312,0,"Page Writes/sec"
/
// NLS_DO_NOT_TRANSLATE [1313,1313]
/
1313,0,"Pool Paged Bytes"
/
// NLS_DO_NOT_TRANSLATE [1314,1314]
/
1314,0,"Pool Nonpaged Bytes"
/
// NLS_DO_NOT_TRANSLATE [1315,1315]
/
1315,0,"Pool Paged Allocs"
/
// NLS_DO_NOT_TRANSLATE [1316,1316]
/
1316,0,"Pool Nonpaged Allocs"
/
// NLS_DO_NOT_TRANSLATE [1317,1317]
/
1317,0,"Free System Page Table Entries"
/
// NLS_DO_NOT_TRANSLATE [1318,1318]
/
1318,0,"Cache Bytes"
/
// NLS_DO_NOT_TRANSLATE [1319,1319]
/
1319,0,"Cache Bytes Peak"
/
// NLS_DO_NOT_TRANSLATE [1320,1320]
/
1320,0,"Pool Paged Resident Bytes"
/
// NLS_DO_NOT_TRANSLATE [1321,1321]
/
1321,0,"System Code Total Bytes"
/
// NLS_DO_NOT_TRANSLATE [1322,1322]
/
1322,0,"System Code Resident Bytes"
/
// NLS_DO_NOT_TRANSLATE [1323,1323]
/
1323,0,"System Driver Total Bytes"
/
// NLS_DO_NOT_TRANSLATE [1324,1324]
/
1324,0,"System Driver Resident Bytes"
/
// NLS_DO_NOT_TRANSLATE [1325,1325]
/
1325,0,"System Cache Resident Bytes"
/
// NLS_DO_NOT_TRANSLATE [1326,1326]
/
1326,0,"% Committed Bytes In Use"
/
/1004,0,"Cache"
/
// NLS_DO_NOT_TRANSLATE [1400,1400]
/
1400,0,"Data Maps/sec"
/
// NLS_DO_NOT_TRANSLATE [1401,1401]
/
1401,0,"Sync Data Maps/sec"
/
// NLS_DO_NOT_TRANSLATE [1402,1402]
/
1402,0,"Async Data Maps/sec"
/
// NLS_DO_NOT_TRANSLATE [1403,1403]
/
1403,0,"Data Map Hits %"
/
// NLS_DO_NOT_TRANSLATE [1404,1404]
/
1404,0,"Data Map Pins/sec"
/
// NLS_DO_NOT_TRANSLATE [1405,1405]
/
1405,0,"Pin Reads/sec"
/
// NLS_DO_NOT_TRANSLATE [1406,1406]
/
1406,0,"Sync Pin Reads/sec"
/
// NLS_DO_NOT_TRANSLATE [1407,1407]
/
1407,0,"Async Pin Reads/sec"
/
// NLS_DO_NOT_TRANSLATE [1408,1408]
/
1408,0,"Pin Read Hits %"
/
// NLS_DO_NOT_TRANSLATE [1409,1409]
/
1409,0,"Copy Reads/sec"
/
// NLS_DO_NOT_TRANSLATE [1410,1410]
/
1410,0,"Sync Copy Reads/sec"
/
// NLS_DO_NOT_TRANSLATE [1411,1411]
/
1411,0,"Async Copy Reads/sec"
/
// NLS_DO_NOT_TRANSLATE [1412,1412]
/
1412,0,"Copy Read Hits %"
/
// NLS_DO_NOT_TRANSLATE [1413,1413]
/
1413,0,"MDL Reads/sec"
/
// NLS_DO_NOT_TRANSLATE [1414,1414]
/
1414,0,"Sync MDL Reads/sec"
/
// NLS_DO_NOT_TRANSLATE [1415,1415]
/
1415,0,"Async MDL Reads/sec"
/
// NLS_DO_NOT_TRANSLATE [1416,1416]
/
1416,0,"MDL Read Hits %"
/
// NLS_DO_NOT_TRANSLATE [1417,1417]
/
1417,0,"Read Aheads/sec"
/
// NLS_DO_NOT_TRANSLATE [1418,1418]
/
1418,0,"Fast Reads/sec"
/
// NLS_DO_NOT_TRANSLATE [1419,1419]
/
1419,0,"Sync Fast Reads/sec"
/
// NLS_DO_NOT_TRANSLATE [1420,1420]
/
1420,0,"Async Fast Reads/sec"
/
// NLS_DO_NOT_TRANSLATE [1421,1421]
/
1421,0,"Fast Read Resource Misses/sec"
/
// NLS_DO_NOT_TRANSLATE [1422,1422]
/
1422,0,"Fast Read Not Possibles/sec"
/
// NLS_DO_NOT_TRANSLATE [1423,1423]
/
1423,0,"Lazy Write Flushes/sec"
/
// NLS_DO_NOT_TRANSLATE [1424,1424]
/
1424,0,"Lazy Write Pages/sec"
/
// NLS_DO_NOT_TRANSLATE [1425,1425]
/
1425,0,"Data Flushes/sec"
/
// NLS_DO_NOT_TRANSLATE [1426,1426]
/
1426,0,"Data Flush Pages/sec"
/
/1005,0,"PhysicalDisk"
/
// NLS_DO_NOT_TRANSLATE [1500,1500]
/
1500,0,"Current Disk Queue Length"
/
// NLS_DO_NOT_TRANSLATE [1501,1501]
/
1501,0,"% Disk Time"
/
// NLS_DO_NOT_TRANSLATE [1502,1502]
/
1502,0,"Avg. Disk Queue Length"
/
// NLS_DO_NOT_TRANSLATE [1503,1503]
/
1503,0,"% Disk Read Time"
/
// NLS_DO_NOT_TRANSLATE [1504,1504]
/
1504,0,"Avg. Disk Read Queue Length"
/
// NLS_DO_NOT_TRANSLATE [1505,1505]
/
1505,0,"% Disk Write Time"
/
// NLS_DO_NOT_TRANSLATE [1506,1506]
/
1506,0,"Avg. Disk Write Queue Length"
/
// NLS_DO_NOT_TRANSLATE [1507,1507]
/
1507,0,"Avg. Disk sec/Transfer"
/
// NLS_DO_NOT_TRANSLATE [1508,1508]
/
1508,0,"Avg. Disk sec/Read"
/
// NLS_DO_NOT_TRANSLATE [1509,1509]
/
1509,0,"Avg. Disk sec/Write"
/
// NLS_DO_NOT_TRANSLATE [1510,1510]
/
1510,0,"Disk Transfers/sec"
/
// NLS_DO_NOT_TRANSLATE [1511,1511]
/
1511,0,"Disk Reads/sec"
/
// NLS_DO_NOT_TRANSLATE [1512,1512]
/
1512,0,"Disk Writes/sec"
/
// NLS_DO_NOT_TRANSLATE [1513,1513]
/
1513,0,"Disk Bytes/sec"
/
// NLS_DO_NOT_TRANSLATE [1514,1514]
/
1514,0,"Disk Read Bytes/sec"
/
// NLS_DO_NOT_TRANSLATE [1515,1515]
/
1515,0,"Disk Write Bytes/sec"
/
// NLS_DO_NOT_TRANSLATE [1516,1516]
/
1516,0,"Avg. Disk Bytes/Transfer"
/
// NLS_DO_NOT_TRANSLATE [1517,1517]
/
1517,0,"Avg. Disk Bytes/Read"
/
// NLS_DO_NOT_TRANSLATE [1518,1518]
/
1518,0,"Avg. Disk Bytes/Write"
/
/1006,0,"LogicalDisk"
/
// NLS_DO_NOT_TRANSLATE [1600,1600]
/
1600,0,"% Free Space"
/
// NLS_DO_NOT_TRANSLATE [1601,1601]
/
1601,0,"Free Megabytes"
/
// NLS_DO_NOT_TRANSLATE [1602,1602]
/
1602,0,"Current Disk Queue Length"
/
// NLS_DO_NOT_TRANSLATE [1603,1603]
/
1603,0,"% Disk Time"
/
// NLS_DO_NOT_TRANSLATE [1604,1604]
/
1604,0,"Avg. Disk Queue Length"
/
// NLS_DO_NOT_TRANSLATE [1605,1605]
/
1605,0,"% Disk Read Time"
/
// NLS_DO_NOT_TRANSLATE [1606,1606]
/
1606,0,"Avg. Disk Read Queue Length"
/
// NLS_DO_NOT_TRANSLATE [1607,1607]
/
1607,0,"% Disk Write Time"
/
// NLS_DO_NOT_TRANSLATE [1608,1608]
/
1608,0,"Avg. Disk Write Queue Length"
/
// NLS_DO_NOT_TRANSLATE [1609,1609]
/
1609,0,"Avg. Disk sec/Transfer"
/
// NLS_DO_NOT_TRANSLATE [1610,1610]
/
1610,0,"Avg. Disk sec/Read"
/
// NLS_DO_NOT_TRANSLATE [1611,1611]
/
1611,0,"Avg. Disk sec/Write"
/
// NLS_DO_NOT_TRANSLATE [1612,1612]
/
1612,0,"Disk Transfers/sec"
/
// NLS_DO_NOT_TRANSLATE [1613,1613]
/
1613,0,"Disk Reads/sec"
/
// NLS_DO_NOT_TRANSLATE [1614,1614]
/
1614,0,"Disk Writes/sec"
/
// NLS_DO_NOT_TRANSLATE [1615,1615]
/
1615,0,"Disk Bytes/sec"
/
// NLS_DO_NOT_TRANSLATE [1616,1616]
/
1616,0,"Disk Read Bytes/sec"
/
// NLS_DO_NOT_TRANSLATE [1617,1617]
/
1617,0,"Disk Write Bytes/sec"
/
// NLS_DO_NOT_TRANSLATE [1618,1618]
/
1618,0,"Avg. Disk Bytes/Transfer"
/
// NLS_DO_NOT_TRANSLATE [1619,1619]
/
1619,0,"Avg. Disk Bytes/Read"
/
// NLS_DO_NOT_TRANSLATE [1620,1620]
/
1620,0,"Avg. Disk Bytes/Write"
/
// NLS_DO_NOT_TRANSLATE [1700,1700]
/
1700,0,"% Processor Time"
/
// NLS_DO_NOT_TRANSLATE [1701,1701]
/
1701,0,"% User Time"
/
// NLS_DO_NOT_TRANSLATE [1702,1702]
/
1702,0,"% Privileged Time"
/
// NLS_DO_NOT_TRANSLATE [1703,1703]
/
1703,0,"Virtual Bytes Peak"
/
// NLS_DO_NOT_TRANSLATE [1704,1704]
/
1704,0,"Virtual Bytes"
/
// NLS_DO_NOT_TRANSLATE [1705,1705]
/
1705,0,"Page Faults/sec"
/
// NLS_DO_NOT_TRANSLATE [1706,1706]
/
1706,0,"Working Set Peak"
/
// NLS_DO_NOT_TRANSLATE [1707,1707]
/
1707,0,"Working Set"
/
// NLS_DO_NOT_TRANSLATE [1708,1708]
/
1708,0,"Page File Bytes Peak"
/
// NLS_DO_NOT_TRANSLATE [1709,1709]
/
1709,0,"Page File Bytes"
/
// NLS_DO_NOT_TRANSLATE [1710,1710]
/
1710,0,"Private Bytes"
/
// NLS_DO_NOT_TRANSLATE [1711,1711]
/
1711,0,"Thread Count"
/
// NLS_DO_NOT_TRANSLATE [1712,1712]
/
1712,0,"Priority Base"
/
// NLS_DO_NOT_TRANSLATE [1713,1713]
/
1713,0,"Elapsed Time"
/
// NLS_DO_NOT_TRANSLATE [1714,1714]
/
1714,0,"ID Process"
/
// NLS_DO_NOT_TRANSLATE [1715,1715]
/
1715,0,"Pool Paged Bytes"
/
// NLS_DO_NOT_TRANSLATE [1716,1716]
/
1716,0,"Pool Nonpaged Bytes"
/
// NLS_DO_NOT_TRANSLATE [1717,1717]
/
1717,0,"Handle Count"
/
/1008,0,"Thread"
/
// NLS_DO_NOT_TRANSLATE [1800,1800]
/
1800,0,"% Processor Time"
/
// NLS_DO_NOT_TRANSLATE [1801,1801]
/
1801,0,"% User Time"
/
// NLS_DO_NOT_TRANSLATE [1802,1802]
/
1802,0,"% Privileged Time"
/
// NLS_DO_NOT_TRANSLATE [1803,1803]
/
1803,0,"Context Switches/sec"
/
// NLS_DO_NOT_TRANSLATE [1804,1804]
/
1804,0,"Elapsed Time"
/
// NLS_DO_NOT_TRANSLATE [1805,1805]
/
1805,0,"Priority Current"
/
// NLS_DO_NOT_TRANSLATE [1806,1806]
/
1806,0,"Priority Base"
/
// NLS_DO_NOT_TRANSLATE [1807,1807]
/
1807,0,"Start Address"
/
// NLS_DO_NOT_TRANSLATE [1808,1808]
/
1808,0,"Thread State"
/
// NLS_DO_NOT_TRANSLATE [1809,1809]
/
1809,0,"Thread Wait Reason"
/
// NLS_DO_NOT_TRANSLATE [1810,1810]
/
1810,0,"ID Process"
/
// NLS_DO_NOT_TRANSLATE [1811,1811]
/
1811,0,"ID Thread"
/
/1009,0,"Objects"
/
// NLS_DO_NOT_TRANSLATE [1900,1900]
/
1900,0,"Processes"
/
// NLS_DO_NOT_TRANSLATE [1901,1901]
/
1901,0,"Threads"
/
// NLS_DO_NOT_TRANSLATE [1902,1902]
/
1902,0,"Events"
/
// NLS_DO_NOT_TRANSLATE [1903,1903]
/
1903,0,"Semaphores"
/
// NLS_DO_NOT_TRANSLATE [1904,1904]
/
1904,0,"Mutexes"
/
// NLS_DO_NOT_TRANSLATE [1905,1905]
/
1905,0,"Sections"
/1010,0,"Paging File"
/
// NLS_DO_NOT_TRANSLATE [2000,2000]
/
2000,0,"% Usage"
/
// NLS_DO_NOT_TRANSLATE [2001,2001]
/
2001,0,"% Usage Peak"
/
// NLS_DO_NOT_TRANSLATE [2102,2102]
/
/1011,0,"Network Interface"
/
// NLS_DO_NOT_TRANSLATE [2100,2100]
/
2100,0,"Bytes Total/sec"
/
// NLS_DO_NOT_TRANSLATE [2101,2101]
/
2101,0,"Packets/sec"
/
// NLS_DO_NOT_TRANSLATE [2102,2102]
/
2102,0,"Packets Received/sec"
/
// NLS_DO_NOT_TRANSLATE [2103,2103]
/
2103,0,"Packets Sent/sec"
/
// NLS_DO_NOT_TRANSLATE [2104,2104]
/
2104,0,"Current Bandwidth"
/
// NLS_DO_NOT_TRANSLATE [2105,2105]
/
2105,0,"Bytes Received/sec"
/
// NLS_DO_NOT_TRANSLATE [2106,2106]
/
2106,0,"Packets Received Unicast/sec"
/
// NLS_DO_NOT_TRANSLATE [2107,2107]
/
2107,0,"Packets Received Non-Unicast/sec"
/
// NLS_DO_NOT_TRANSLATE [2108,2108]
/
2108,0,"Packets Received Discarded"
/
// NLS_DO_NOT_TRANSLATE [2109,2109]
/
2109,0,"Packets Received Errors"
/
// NLS_DO_NOT_TRANSLATE [2110,2110]
/
2110,0,"Packets Received Unknown"
/
// NLS_DO_NOT_TRANSLATE [2111,2111]
/
2111,0,"Bytes Sent/sec"
/
// NLS_DO_NOT_TRANSLATE [2112,2112]
/
2112,0,"Packets Sent Unicast/sec"
/
// NLS_DO_NOT_TRANSLATE [2113,2113]
/
2113,0,"Packets Sent Non-Unicast/sec"
/
// NLS_DO_NOT_TRANSLATE [2114,2114]
/
2114,0,"Packets Outbound Discarded"
/
// NLS_DO_NOT_TRANSLATE [2115,2115]
/
2115,0,"Packets Outbound Errors"
/
// NLS_DO_NOT_TRANSLATE [2116,2116]
/
2116,0,"Output Queue Length"
//ASP resources
/
// NLS_DO_NOT_TRANSLATE [2200,2200]
/
2200,0,"Debugging Requests"
/
// NLS_DO_NOT_TRANSLATE [2201,2201]
/
2201,0,"Errors During Script Runtime"
/
// NLS_DO_NOT_TRANSLATE [2202,2202]
/
2202,0,"Errors From ASP Preprocessor"
/
// NLS_DO_NOT_TRANSLATE [2203,2203]
/
2203,0,"Errors From Script Compilers"
/
// NLS_DO_NOT_TRANSLATE [2204,2204]
/
2204,0,"Errors/Sec"
/
// NLS_DO_NOT_TRANSLATE [2205,2205]
/
2205,0,"Memory Allocated"
/
// NLS_DO_NOT_TRANSLATE [2206,2206]
/
2206,0,"Request Bytes In Total"
/
// NLS_DO_NOT_TRANSLATE [2207,2207]
/
2207,0,"Request Bytes Out Total"
/
// NLS_DO_NOT_TRANSLATE [2208,2208]
/
2208,0,"Request Execution Time"
/
// NLS_DO_NOT_TRANSLATE [2209,2209]
/
2209,0,"Request Wait Time"
/
// NLS_DO_NOT_TRANSLATE [2210,2210]
/
2210,0,"Requests Disconnected"
/
// NLS_DO_NOT_TRANSLATE [2211,2211]
/
2211,0,"Requests Executing"
/
// NLS_DO_NOT_TRANSLATE [2212,2212]
/
2212,0,"Requests Failed Total"
/
// NLS_DO_NOT_TRANSLATE [2213,2213]
/
2213,0,"Requests Not Authorized"
/
// NLS_DO_NOT_TRANSLATE [2214,2214]
/
2214,0,"Requests Not Found"
/
// NLS_DO_NOT_TRANSLATE [2215,2215]
/
2215,0,"Requests Queued"
/
// NLS_DO_NOT_TRANSLATE [2216,2216]
/
2216,0,"Requests Rejected"
/
// NLS_DO_NOT_TRANSLATE [2217,2217]
/
2217,0,"Requests Succeeded"
/
// NLS_DO_NOT_TRANSLATE [2218,2218]
/
2218,0,"Requests Timed Out"
/
// NLS_DO_NOT_TRANSLATE [2219,2219]
/
2219,0,"Requests Total"
/
// NLS_DO_NOT_TRANSLATE [2220,2220]
/
2220,0,"Requests/Sec"
/
// NLS_DO_NOT_TRANSLATE [2221,2221]
/
2221,0,"Script Engines Cached"
/
// NLS_DO_NOT_TRANSLATE [2222,2222]
/
2222,0,"Session Duration"
/
// NLS_DO_NOT_TRANSLATE [2223,2223]
/
2223,0,"Sessions Current"
/
// NLS_DO_NOT_TRANSLATE [2224,2224]
/
2224,0,"Sessions Timed Out"
/
// NLS_DO_NOT_TRANSLATE [2225,2225]
/
2225,0,"Sessions Total"
/
// NLS_DO_NOT_TRANSLATE [2226,2226]
/
2226,0,"Template Cache Hit Rate"
/
// NLS_DO_NOT_TRANSLATE [2227,2227]
/
2227,0,"Template Notifications"
/
// NLS_DO_NOT_TRANSLATE [2228,2228]
/
2228,0,"Templates Cached"
/
// NLS_DO_NOT_TRANSLATE [2229,2229]
/
2229,0,"Transactions Aborted"
/
// NLS_DO_NOT_TRANSLATE [2230,2230]
/
2230,0,"Transactions Committed"
/
// NLS_DO_NOT_TRANSLATE [2231,2231]
/
2231,0,"Transactions Pending"
/
// NLS_DO_NOT_TRANSLATE [2232,2232]
/
2232,0,"Transactions Total"
/
// NLS_DO_NOT_TRANSLATE [2233,2233]
/
2233,0,"Transactions/Sec"


/ class
2300, 00000, "I/O Total"

/ resource

2301, 00000, "Disk Reads/sec"
2302, 00000, "Disk Writes/sec"
2303, 00000, "Disk Transfers/sec"
2304, 00000, "Average Response Time (ms)"
2305, 00000, "Maximum Response Time (ms)"

/ chart name
2306, 00000, "Host Disk Use"
2307, 00000, "I/O Activity"
2308, 00000, "I/O Response Time"

/ resource

2310, 00000, "Available Memory (%)"
2311, 00000, "Pagefile available (kb)"
2312, 00000, "Memory Available (kb)" 

/ chart name

2315, 00000, "Host Memory Use"
2316, 00000, "Available Memory (%)"
2317, 00000, "Free Memory"

// New class name to replace 2300 above
2318, 00000, "Cumulative Disk"

/ resource

2320, 00000,  "% Total Idle Time"

/ chart 

2321, 00000, "Host CPU Use"
2322, 00000, "CPU Utilization (%)"
2323, 00000, "CPU Usage Table"




/
// NLS_DO_NOT_TRANSLATE [9999,9999]
/
9999,0,"Bug fixer"
