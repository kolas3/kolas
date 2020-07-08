/ Copyright (c) 1995, 1997, 1998 by the Oracle Corporation.  All rights reserved.
/
/
/ $Header: /K2UP_CVS/Repository/KOLAS III/ORACLE/network/mesg/naukus.msg,v 1.1 2009/12/09 02:29:48 ╧за╬©Н Exp $
/
/ NAME     
/   naukus.msg
/
/ FUNCTION 
/   Network nAtive aUthentication Kerberos trace US message file
/
/ NOTES
/       Message text should be limited to a maximum of 76 characters.
/	Messages containing embedded % directives should not overrun
/	the 76 character limit unless they signal unusual errors
/	which do not occur as part of user applications.  For example,
/	startup errors and system-dependent errors can probably get
/	away with longer message text.
/
/-----------------------------------------------------------------
/
/ List error messages in (numerical) ascending order here.
/ Precede each block of errors with a comment.
/ Pick a range for your component messages and document above
/ ========================================================
/ All messages have the number in the first column. the second
/ set of 00000 is not used. Do not preface your message number in 
/ your code with 0's.  I.e. message 00001, 00000 defined in this
/ message file is simply message 1 in your code.
/
/
// NLS_DO_NOT_TRANSLATE [0,65535]
00000, 00000, "Success, no errors."
00001, 00000, "\tfor client %s"
00002, 00000, "\t"
00003, 00000, ", "
00004, 00000, "renew until %s" 
00005, 00000, "Flags: %s"
00006, 00000, "\n"
00007, 00000, "Unable to identify user from password file\n"
00008, 00000, "Error while reading password for '%s'\n"
00009, 00000, "%s: Password incorrect\n"
00010, 00000, "%s: %s.\n"
00011, 00000, "Usage:%s [-f] [-p] [-l life] [-r renew] [-c cache] [-k] [-t table]\n"
00012, 00000, "Usage:\n\t%s [-f] [[-c] cachename]\n"
00013, 00000, "\t%s -k [keytabname]\n"
00014, 00000, "%s: Credential cache %s not found.\n"
00015, 00000, "Ticket cache: %s\nDefault principal: %s\n\n"
00016, 00000, "   Valid Starting           Expires            Principal\n"
00017, 00000, "Service Key Table: %s\n\n"
00018, 00000, "Ver      Timestamp                    Principal\n"
00019, 00000, "Usage: %s [[-c] cachename]\n"
00020, 00000, "Password for %s: "
00021, 00000, "%s  %s  %s"
00022, 00000, "%2d  %s  %s\n"

/
/ 2100 and up are reserved for error and trace messages.
/ 
/ NOTE: These correspond to messages in nltus.msg and have the same
/ message number - make sure this is always the case.
/

/
/ TRANSFER, IN ENGLISH, ALL MESSAGES BEYOND THIS POINT.
/

/
/ Miscellaneous
/
02100, 00000, "%s\n"
02199, 00000, "%s function failed.\n"
02273, 00000, "%s: %s\n"

/
/ Function names
/
06100, 00000, "<Unknown function>"
06101, 00000, "nauk5b1_fcc_open_file"
06102, 00000, "nauk5ba_fcc_retrieve"
06103, 00000, "nauk5bb_fcc_read"
06104, 00000, "nauk5bc_fcc_read_principal"
06105, 00000, "nauk5bf_fcc_read_data"
06106, 00000, "nauk5bh_fcc_read_int32"
06107, 00000, "nauk5bn_fcc_write"
06108, 00000, "nauk5bo_fcc_store_principal"
06109, 00000, "nauk5bz_fcc_close_file"
06110, 00000, "nauk5cb_cc_default"
06111, 00000, "nauk5cd_fcc_destroy"
06112, 00000, "nauk5cj_fcc_initialize"
06113, 00000, "nauk5cn_fcc_skip_principal"
06114, 00000, "nauk5fq_free_principal"
06115, 00000, "nauk5hnmkcred"
06116, 00000, "nauk5hrdcred"
06117, 00000, "nauk5k1_send_tgs"
06118, 00000, "nauk5ky_rd_req_decoded"
06119, 00000, "nauk5kz_rd_req_simple"
06120, 00000, "nauk5la_get_in_tkt"
06121, 00000, "nauk5ld_parse_name"
06122, 00000, "nauk5ll_generate_authenticator"
06123, 00000, "nauk5ln_find_preauthenticator"
06124, 00000, "nauk5lp_verify_unixtime_padata"
06125, 00000, "nauk5lr_createpath"
06126, 00000, "nauk5lu_get_host_realm"
06127, 00000, "nauk5lv_sname_to_principal"
06128, 00000, "nauk5lx_get_krbhst"
06129, 00000, "nauk5ra_rcinit"
06130, 00000, "nauk5rb_rcstore"
06131, 00000, "nauk5ru_create"
06132, 00000, "nauk5rv_write"
06133, 00000, "nauk5ry_next"
06134, 00000, "nauk5rz_validate"
06135, 00000, "nauk5wj_ktfileint_open"
06136, 00000, "nauk5z0_show_credential"
06137, 00000, "nauk5zd_kdestroy"
06138, 00000, "nauk5zi_kinit"
06139, 00000, "nauk5zl_klist"
06140, 00000, "nauk5lz_get_default_realm"
06141, 00000, "snauk5g_open_file"
06142, 00000, "snauk5h_gettmp"
06143, 00000, "snauk5i_os_localaddr"
06144, 00000, "snauk5k_lock_file"
06145, 00000, "snauk5l_sendto_kdc"
06146, 00000, "snauk5t_close_file"
06147, 00000, "snaukz_getpassword"
06148, 00000, "nauk5_get_unixtime_padata"
06149, 00000, "nauk5lg_init_krb5"

/
/ Format strings for traces and errors.
/
06200, 00000, "<Unknown trace message>.\n"
06201, 00000, "%s: Credential cache %s not found.\n"
06202, 00000, "Cache file version mismatch.\n"
06203, 00000, "Call to ioctl failed with %s.\n"
06204, 00000, "Call to snauk5j_locate_kdc failed.\n"
06205, 00000, "Call to socket failed with %s.\n"
06206, 00000, "Checking for replays.\n"
06207, 00000, "Close file: %s.\n"
06208, 00000, "Could not allocate memory filename.\n"
06209, 00000, "Could not build server principal: %s.\n"
06210, 00000, "Could not create server: %s.\n"
06211, 00000, "Could not destroy CC: %s.\n"
06212, 00000, "Could not end sequence: %s.\n"
06213, 00000, "Could not get default CC: %s.\n"
06214, 00000, "Could not get local address: %s.\n"
06215, 00000, "Could not initialize CC: %s.\n"
06216, 00000, "Could not initialize crypto systems.\n"
06217, 00000, "Could not initialize replay cache.\n"
06218, 00000, "Could not open cache.\n"
06219, 00000, "Could not open cache.\n"
06220, 00000, "Could not open input tty: %s.\n"
06221, 00000, "Could not open output tty: %s.\n"
06222, 00000, "Could not parse name: %s.\n"
06223, 00000, "Could not read password: %s.\n"
06224, 00000, "Could not read table entry: %s.\n"
06225, 00000, "Could not read version info.?"
06226, 00000, "Could not resolve CC: %s.\n"
06227, 00000, "Could not resolve keytab: %s.\n"
06228, 00000, "Could not start sequence: %s.\n"
06229, 00000, "Could not unlock file.\n"
06230, 00000, "Could not unparse client.\n"
06231, 00000, "Could not unparse client: %s.\n"
06232, 00000, "Could not unparse name: %s.\n"
06233, 00000, "Could not unparse server: %s.\n"
06234, 00000, "Could not unparse service.\n"
06235, 00000, "Could not write prompt: %s.\n"
06236, 00000, "Could not write version info.\n"
06237, 00000, "Creating replay cache.\n"
06238, 00000, "Error from select: %s.\n"
06239, 00000, "Expunging excess cache records.\n"
06240, 00000, "Failed to allocate Kerberos context.\n"
06241, 00000, "Getting TGT failed: %s.\n"
06242, 00000, "Grew RC to %s entries.\n"
06243, 00000, "Initializing cache failed: %s.\n"
06244, 00000, "snlfwrt failed: %s.\n"
06245, 00000, "Lock failed.\n"
06246, 00000, "Looking for client %s.\n"
06247, 00000, "Looking for service %s.\n"
06248, 00000, "Neither read nor write specified.\n"
06249, 00000, "snlfsek failed: %s.\n"
06250, 00000, "Open failed.\n"
06251, 00000, "Open for readonly, but no file.\n"
06252, 00000, "Opening %s.\n"
06253, 00000, "snlfdel failed: %s.\n"
06254, 00000, "Programming error.\n"
06255, 00000, "Read failed: %s.\n"
06256, 00000, "snaumihi_inithostinfo failed.\n"
06257, 00000, "Replay detected.\n"
06258, 00000, "Resetting lock.\n"
06259, 00000, "Resolving cachename failed: %s.\n"
06260, 00000, "Retrieving prinicipal failed: %s.\n"
06261, 00000, "Retrieving table entry failed: %s.\n"
06262, 00000, "Retrieving ticket failed: %s.\n"
06263, 00000, "Returning %s: %s.\n"
06264, 00000, "Setting append flag.\n"
06265, 00000, "Setting create flag.\n"
06266, 00000, "Setting exclusive flag.\n"
06267, 00000, "Setting exclusive lock.\n"
06268, 00000, "Setting read lock.\n"
06269, 00000, "Setting shared lock.\n"
06270, 00000, "Setting truncate flag.\n"
06271, 00000, "Setting write lock.\n"
06272, 00000, "Skipping long line: %s.?.?.\n"
06273, 00000, "Store failed.\n"
06274, 00000, "Storing %s in CC.\n"
06275, 00000, "Unknown lock type %s.\n"
06276, 00000, "Unknown operation %s.\n"
06277, 00000, "Unparsing client failed: %s.\n"
06278, 00000, "Unparsing principal failed.\n"
06279, 00000, "Unparsing server failed: %s.\n"
06280, 00000, "Updating RC.\n"
06281, 00000, "Validating existing replay cache.\n"
06282, 00000, "Version mismatch.\n"
06283, 00000, "Version numbers didn't match.\n"
06284, 00000, "Write failed: %s.\n"
06285, 00000, "fcntl failed with %s: %s.\n"
06286, 00000, "fdopen failed with %s: %s.\n"
06287, 00000, "flock failed with %s: %s.\n"
06288, 00000, "nauk5k4_unparse_name failed: %s.\n"
06289, 00000, "snaumgs_getseconds failed.\n"
06290, 00000, "open failed with %s: %s.\n"
06291, 00000, "snaumgh_gethostinfo failed.\n"
06292, 00000, "nauk5rb_rcstore failed.\n"
06293, 00000, "Possible replay cache corruption.\n"

// End marker so that the "Do not translate" directive does not have to be
// updated when the file changes.
65535, 00000, "LAST MESSAGE"
