#!/bin/sh
#############################################################################
## Credits 
## Base Script by NCDB http://www.nc-db.info
## Modified by MrHooHa and Maven ArcEmu http://www.arcemu.org
## Linux Build by CoolAcid http://www.arcemu.org
#############################################################################
## 
##      B A S I C   U S E R   C O N F I G U R A T I O N   A R E A
## 
#############################################################################
##########################################
## server - MySql Host
## user   - MySql Username
## pass   - MySql Password
## logon  - Logon Database
## chr    - Character Database
## wdb    - World Database
##########################################
#MySQL Username and Password
user=changeme
pass=changeme
## Logon Database
logon=changeme
## Character Database
chr=changeme
## World Database
wdb=changeme
#############################################################################
##
##    A D V A N C E D   U S E R   C O N F I G U R A T I O N   A R E A
##
#############################################################################
server=localhost
port=3306
#############################################################################
##
##     D O   N O T   M O D I F Y   B E Y O N D   T H I S   P O I N T
##
#############################################################################

worldupdates=`find world_updates/*.sql`
charupdates=`find character_updates/*.sql`
logonupdates=`find logon_updates/*.sql`

menu()
{
    clear
    echo 
    echo 
    echo	 ##############################################################
    echo	 #    	   Base Script by NCDB http://www.nc-db.info          #
    echo	 # Modified by MrHooHa and Maven ArcEmu http://www.arcemu.org #
    echo	 ##############################################################
    echo		 ####################################
    echo		 #######    ArcEmu Database   #######
    echo		 ######        Revision        ######
    echo		 #######      Update Tool     #######
    echo		 ####################################
    echo 
    echo		 Please type the letter for the option:
    echo 
    echo		  W = Install World Database updates
    echo		  C = Install Character updates
    echo		  L = Install Logon updates
    echo		  A = Install All updates
    echo 
    echo 
    echo		  x - Exit
    echo 
    echo             Enter Letter:
    read select
    case "$select" in
	'x'|'X')
	    exit 0
	;;
	'c'|'C')
	    echo [Importing] Started...
	    chr
	    echo [Importing] Finished
	    read input    
	    menu
	;;
	'w'|'W')
	    echo [Importing] Started...
	    world
	    echo [Importing] Finished
	    read input    
	    menu
	;;
	'l'|'L')
	    echo [Importing] Started...
	    logon
	    echo [Importing] Finished
	    read input    
	    menu
	;;
	'a'|'A')
	    installall
	    read input    
	    menu
	;;
    esac
    error
}


world()
{
    clear
    echo [Importing] ArcEmu World Updates ...
    for table in $worldupdates
    do 
        echo [Importing] $table
        mysql -h $server --user=$user --password=$pass --port=$port $wdb < $table
    done
}

chr()
{
    clear
    echo [Importing] ArcEmu Character Updates ...
    for table in $charupdates
    do 
        echo [Importing] $table
        mysql -h $server --user=$user --password=$pass --port=$port $chr < $table
    done
    echo  Update Success.
}

logon()
{
    clear
    echo [Importing] ArcEmu Logon Updates ...
    for table in $logonupdates
    do 
        echo [Importing] $table
        mysql -h $server --user=$user --password=$pass --port=$port $logon < $table
    done
    echo  Update Success.
}

installall()
{
    echo [Importing] All Updates Started
    world
    echo
    chr
    echo
    logon
    echo [Importing] All Updates Finished
}

error()
{
    clear
    echo 
    echo 
    echo [ERROR] An error has occured, you will be directed back to the
    echo [ERROR] main menu.
    read input    
    menu
}
    
error2()
{
    clear
    echo 
    echo 
    echo [FAILURE] You did not change the proper directives in this file.
    echo [FAILURE] Please edit this script and fill in the proper MYSQL Information.
    echo [FAILURE] When the information is correct: Please Try Again.
    read input    
    exit 1    
}

if [ "$user" = "changeme" -o "$pass" = "changeme" ]; then 
	# -o "$pass"=="changeme" ]; then 
    error2
fi

menu
        