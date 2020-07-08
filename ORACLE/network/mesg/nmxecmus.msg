///////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2000 by the Oracle Corporation.  All rights reserved.
/
/
/ $Header: /K2UP_CVS/Repository/KOLAS III/ORACLE/network/mesg/nmxecmus.msg,v 1.1 2009/12/09 02:29:58 ╧за╬©Н Exp $
/
/ NAME                                          
/   nmxecmus.msg                                            
/                                                     
/
/ FUNCTION
/   Oracle ECommerce Mamnagement error message file, American English version.  
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
/    Oracle ECommerce Mangement 
/
/ MODIFIED
/    jmarfati   05/25/00 - adding limited number and date formatting support
/    jebridge   05/17/00 - fix bug 1298995
/    jebridge   05/11/00 - Adding report strings
/    jmarfati   05/02/00 - adding messages
/    jebridge   04/11/00 - add a hint message for sizing
/    jmarfati   03/29/00 - change filaname in the comments.
/    jmarfati   03/30/00 - .
/    jmarfati   03/28/00 - adding empty file for NLS drop.
/
///////////////////////////////////////////////////////////////////////////
00000, 00000, "Wrong message ID was passed to retrieve the message."
// *Comment: mnemonic: NONE
/
00001, 00000, "Could not open file: %s"
// *Comment: mnemonic: NONE
/
05000, 00000, "Web Server Access Log Traffic Report"
// *Comment: mnemonic: ReportString_Title
/
05001, 00000, "Web Server"
// *Comment: mnemonic: ReportString_WebServer
/
05002, 00000, "Table of Contents"
// *Comment: mnemonic: ReportString_TableOfContents
/
05003, 00000, "Date"
// *Comment: mnemonic: ReportString_Date
/
05004, 00000, "Time Based Statistics"
// *Comment: mnemonic: ReportString_TimeBasedStatistics
/
05005, 00000, "Monthly Activity Report"
// *Comment: mnemonic: ReportString_MonthlyActivityReport
/
05006, 00000, "Activity by Day of Week"
// *Comment: mnemonic: ReportString_DayOfWeekActivityReport
/
05007, 00000, "Activity by Hour of Day"
// *Comment: mnemonic: ReportString_HourOfDayActivityReport
/
05008, 00000, "Daily Activity Report"
// *Comment: mnemonic: ReportString_DailyActivityReport
/
05009, 00000, "Hour"
// *Comment: mnemonic: ReportString_Hour
/
05010, 00000, "Day"
// *Comment: mnemonic: ReportString_Day
/
05011, 00000, "Month"
// *Comment: mnemonic: ReportString_Month
/
05012, 00000, "Year"
// *Comment: mnemonic: ReportString_Year
/
05013, 00000, "Requests"
// *Comment: mnemonic: ReportString_Hits
/
05014, 00000, "Visits"
// *Comment: mnemonic: ReportString_Visits
/
05015, 00000, "Kilobytes"
// *Comment: mnemonic: ReportString_Bytes
/
05016, 00000, "Pages"
// *Comment: mnemonic: ReportString_Pages
/
05017, 00000, "Jan"
// *Comment: mnemonic: ReportString_Jan
/
05018, 00000, "Feb"
// *Comment: mnemonic: ReportString_Feb
/
05019, 00000, "Mar"
// *Comment: mnemonic: ReportString_Mar
/
05020, 00000, "Apr"
// *Comment: mnemonic: ReportString_Apr
/
05021, 00000, "May"
// *Comment: mnemonic: ReportString_May
/
05022, 00000, "Jun"
// *Comment: mnemonic: ReportString_Jun
/
05023, 00000, "Jul"
// *Comment: mnemonic: ReportString_Jul
/
05024, 00000, "Aug"
// *Comment: mnemonic: ReportString_Aug
/
05025, 00000, "Sep"
// *Comment: mnemonic: ReportString_Sep
/
05026, 00000, "Oct"
// *Comment: mnemonic: ReportString_Oct
/
05027, 00000, "Nov"
// *Comment: mnemonic: ReportString_Nov
/
05028, 00000, "Dec"
// *Comment: mnemonic: ReportString_Dec
/
05029, 00000, "Mon"
// *Comment: mnemonic: ReportString_Mon
/
05030, 00000, "Tue"
// *Comment: mnemonic: ReportString_Tue
/
05031, 00000, "Wed"
// *Comment: mnemonic: ReportString_Wed
/
05032, 00000, "Thu"
// *Comment: mnemonic: ReportString_Thu
/
05033, 00000, "Fri"
// *Comment: mnemonic: ReportString_Fri
/
05034, 00000, "Sat"
// *Comment: mnemonic: ReportString_Sat
/
05035, 00000, "Sun"
// *Comment: mnemonic: ReportString_Sun
/
05036, 00000, "Error: the directory: %s does not exist."
// *Comment: mnemonic: ReportString_ErrorTheDirectory__STRING__DoesNotExist
/
05037, 00000, "Error: failed to open file: %s"
// *Comment: mnemonic: ReportString_ErrorfailedToOpenFile__STRING__
/
05038, 00000, "Error: failed to create directory: %s"
// *Comment: mnemonic: ReportString_ErrorfailedToCreateDirectory__STRING__
/
05039, 00000, "The report was successfully generated."
// *Comment: mnemonic: ReportString_TheReportWasSuccessfullyGenerated
/
05040, 00000, "Total number of log lines"
// *Comment: mnemonic: ReportString_TotalNumberOfLogLines
/
05041, 00000, "Number of log lines not processed"
// *Comment: mnemonic: ReportString_NumberOfLogLinesNotProcessed
/
05042, 00000, ","
// *Comment: mnemonic: ReportString_ThousandsSeparator
/  NOTE :Please change this to the thousands separator used 
/        for numbers on a particular locale.
05043, 00000, "."
// *Comment: mnemonic: ReportString_DecimalPoint
/  NOTE :Please change this to the decimal point used 
/        for numbers on a particular locale.
05044, 00000, "dy, mon dd YYYY"
// *Comment: mnemonic: ReportString_DateFormat
/  NOTE :Please change this to the date format preferred
/        for a particular locale.
/        Please use the dy, mon, dd and YYYY only.
/        others are not supported.
/        U can choose to omitt any of these.
05045, 00000, "% Requests"
// *Comment: mnemonic: ReportString_Requests_Chart
/  
/ *********************** END OF MESSAGE FILE*******************************
