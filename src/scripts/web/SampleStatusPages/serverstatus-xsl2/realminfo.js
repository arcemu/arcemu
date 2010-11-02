var EnvVars = [];
function SetEnvVar( envVarIdent , envVarValue )
{
EnvVars[envVarIdent] = envVarValue;
}
function GetEnvVar( envVarIdent )
{
return EnvVars[envVarIdent];
}
/***********************************
** RANDOM CONSTANT INITIALISATION **
***********************************/
var ClassNames = [];
ClassNames[1] = "Warrior";
ClassNames[2] = "Paladin";
ClassNames[3] = "Hunter";
ClassNames[4] = "Rogue";
ClassNames[5] = "Priest";
ClassNames[7] = "Shaman";
ClassNames[8] = "Mage";
ClassNames[9] = "Warlock";
ClassNames[11] = "Druid";
var RaceNames = [];
RaceNames[1] = "Human";
RaceNames[2] = "Orc";
RaceNames[3] = "Dwarf";
RaceNames[4] = "Night Elf";
RaceNames[5] = "Undead";
RaceNames[6] = "Tauren";
RaceNames[7] = "Gnome";
RaceNames[8] = "Troll";
RaceNames[10] = "Blood Elf";
RaceNames[11] = "Draenei";
var FactionName = [];
FactionName[1] = "alliance";
FactionName[2] = "horde";
FactionName[3] = "alliance";
FactionName[4] = "alliance";
FactionName[5] = "horde";
FactionName[6] = "horde";
FactionName[7] = "alliance";
FactionName[8] = "horde";
FactionName[10] = "horde";
FactionName[11] = "alliance";
var InstanceTypeNames = [];
InstanceTypeNames[0] = "Static";
InstanceTypeNames[1] = "Raid";
InstanceTypeNames[2] = "Instance";
InstanceTypeNames[3] = "Continent 3";
InstanceTypeNames[4] = "Continent 4";

/**************************************
** DATA INITIALISATION (ARRAYS, ETC) **
**************************************/
var PlayerMap = [];
var FieldInfo = [];
var PlayerCount = 0;
for( i = 0 ; i <= 7 ; i ++ ) { FieldInfo[i] = []; }
FieldInfo[0][0] = "Name"; 			FieldInfo[0][1] = "15%";
FieldInfo[1][0] = "Level"; 		FieldInfo[1][1] = "7%";
FieldInfo[2][0] = "Race"; 			FieldInfo[2][1] = "10%";
FieldInfo[3][0] = "Class"; 		FieldInfo[3][1] = "10%";
FieldInfo[4][0] = "Map"; 			FieldInfo[4][1] = "10%";
FieldInfo[5][0] = "Zone"; 			FieldInfo[5][1] = "15%";
FieldInfo[6][0] = "Online Time"; 	FieldInfo[6][1] = "23%";
FieldInfo[7][0] = "Latency"; 	FieldInfo[7][1] = "20%";


var InstanceMap = [];
var FieldInfoInstances = [];
for( i = 0; i <= 6 ; i ++ ) { FieldInfoInstances[i] = []; }
FieldInfoInstances[0][0] = "Name";						FieldInfoInstances[0][1] = "15%";
FieldInfoInstances[1][0] = "Map ID";					FieldInfoInstances[1][1] = "10%";
FieldInfoInstances[2][0] = "Players";					FieldInfoInstances[2][1] = "10%";
FieldInfoInstances[3][0] = "Player Limit";				FieldInfoInstances[3][1] = "10%";
FieldInfoInstances[4][0] = "Instance Type";			FieldInfoInstances[4][1] = "15%";
FieldInfoInstances[5][0] = "Creation Time";			FieldInfoInstances[5][1] = "15%";
FieldInfoInstances[6][0] = "Expiry Time";				FieldInfoInstances[6][1] = "30%";

var GMMap = [];
var FieldInfoGM = [];
for( i = 0 ; i <= 6 ; i ++ ) { FieldInfoGM[i] = []; }
FieldInfoGM[0][0] = "Name";		FieldInfoGM[0][1] = "15%";
FieldInfoGM[1][0] = "Level";		FieldInfoGM[1][1] = "10%";
FieldInfoGM[2][0] = "Race";		FieldInfoGM[2][1] = "10%";
FieldInfoGM[3][0] = "Class";		FieldInfoGM[3][1] = "10%";
FieldInfoGM[4][0] = "Online Time";	FieldInfoGM[4][1] = "30%";
FieldInfoGM[5][0] = "Latency";		FieldInfoGM[5][1] = "15%";
FieldInfoGM[6][0] = "Permissions";	FieldInfoGM[6][1] = "15%";

function OnlinePlayer( Name , Level , Race , Class , Map , Zone , OnlineTime , Gender , Latency )
{
	/*this.Name = Name; // String
	this.level = Level; // Int
	this.race = Race; // Int
	this.classz = Class; // Int
	this.map = Map; // String
	this.zone = Zone; // String
	this.onlinetime = OnlineTime; // String
	*/
	this.Fields = [];
	this.Fields[0] = Name; // String
	this.Fields[1] = Level; // Int
	this.Fields[2] = Race; // Int
	this.Fields[3] = Class; // Int
	this.Fields[4] = Map; // String
	this.Fields[5] = Zone; // String
	this.Fields[6] = OnlineTime; // String
	this.Fields[7] = Latency;
	this.Gender = Gender;
}

function OpenInstance( Name , MapID , Players , PlayerLimit , MapType , CreationTime , ExpiryTime )
{
	this.Fields = [];
	this.Fields[0] = Name;
	this.Fields[1] = MapID;
	this.Fields[2] = Players;
	this.Fields[3] = PlayerLimit;
	this.Fields[4] = MapType;
	this.Fields[5] = CreationTime;
	this.Fields[6] = ExpiryTime;
}

function OnlineGM( Name , Race , Class , Level , OnlineTime , Latency , Permissions , Gender )
{
	this.Fields = [];
	this.Fields[0] = Name;
	this.Fields[1] = Level;
	this.Fields[2] = Race;
	this.Fields[3] = Class;
	this.Fields[4] = OnlineTime;
	this.Fields[5] = Latency;
	this.Fields[6] = Permissions;
	this.Fields[7] = Gender;
}
function AddPlayer( Name , Level , Race , Class , Map , Zone , OnlineTime , Gender , Latency )
{
	//PlayerCount += 1;
	//document.writeln('Added player with name ' + Name + ' [' + ( PlayerMap.length + 1 ) + ',' + PlayerCount + ']<br />');
	PlayerMap[PlayerMap.length] = new OnlinePlayer( Name , Level , Race , Class , Map , Zone , OnlineTime , Gender , Latency );
}
function AddInstance( Name , MapID , Players , PlayerLimit , State , CreationTime , ExpiryTime )
{
	InstanceMap[InstanceMap.length] = new OpenInstance( Name , MapID , Players , PlayerLimit, State, CreationTime , ExpiryTime );
}
function AddGM( Name , Race , Class , Level , OnlineTime , Latency , Permissions , Gender )
{
	GMMap[GMMap.length] = new OnlineGM( Name , Race , Class , Level , OnlineTime , Latency , Permissions , Gender );

}
/**********************
** SORTING FUNCTIONS **
**********************/

/** INIT **/
/** 1 = Ascending, 0 = Descending **/
FieldSortDirection = [];
FieldSortDirection2 = [];
FieldSortDirection3 = [];
for( i = 0 ; i <= 6 ; i += 1 )
{
FieldSortDirection[i] = 1;
FieldSortDirection2[i] = 1;
FieldSortDirection3[i] = 1;

}
ActiveSortField = -1; /* Only used to bring 'FieldIndex' from DoSort into the sort function */
ActiveSortField2 = -1;
ActiveSortField3 = -1;
function DoSort( FieldIndex , TableID )
{
	/** Bleh.. we're using classes now, so.. **/
	/** 0 = Name
	*** 1 = level
	*** 2 = race
	*** 3 = classz
	*** 4 = map
	*** 5 = zone
	*** 6 = onlinetime **/
	if( TableID == 0 )
	{
	ActiveSortField = FieldIndex;
	FieldSortDirection[FieldIndex] = !FieldSortDirection[FieldIndex];
	
	if( FieldSortDirection[FieldIndex] )
	PlayerMap.sort( sort_asc );
	else
	PlayerMap.sort( sort_desc );

	//ActiveSortField = -1; We may actually need this later..
	Display();
	}
	else if( TableID == 1 )
	{
	ActiveSortField2 = FieldIndex;
	FieldSortDirection2[FieldIndex] = !FieldSortDirection2[FieldIndex];
	if( FieldSortDirection2[FieldIndex] )
	InstanceMap.sort( sort_asc2 );
	else
	InstanceMap.sort( sort_desc2 );
	DisplayInstance();
	}
	else if( TableID == 2 )
	{
	ActiveSortField3 = FieldIndex;
	FieldSortDirection3[FieldIndex] = !FieldSortDirection3[FieldIndex];
	if( FieldSortDirection3[FieldIndex] )
	GMMap.sort( sort_asc3 );
	else
	GMMap.sort( sort_desc3 );
	DisplayGMInfo();
	}
}

function sort_asc( ElementA , ElementB ) {

    var x = ElementA.Fields[ActiveSortField];
    var y = ElementB.Fields[ActiveSortField];
	var result = ( ( x < y ) ? -1 : ( ( x > y ) ? 1 : 0 ) );
    return result;
}
function sort_desc( ElementA , ElementB ) {
    var x = ElementA.Fields[ActiveSortField];
    var y = ElementB.Fields[ActiveSortField];
    return ( ( x < y ) ? 1 : ( ( x > y ) ? -1: 0 ) );
}
function sort_asc2( ElementA , ElementB ) {

    var x = ElementA.Fields[ActiveSortField2];
    var y = ElementB.Fields[ActiveSortField2];
	var result = ( ( x < y ) ? -1 : ( ( x > y ) ? 1 : 0 ) );
    return result;
}
function sort_desc2( ElementA , ElementB ) {
    var x = ElementA.Fields[ActiveSortField2];
    var y = ElementB.Fields[ActiveSortField2];
    return ( ( x < y ) ? 1 : ( ( x > y ) ? -1: 0 ) );
}
function sort_asc3( ElementA , ElementB ) {

    var x = ElementA.Fields[ActiveSortField3];
    var y = ElementB.Fields[ActiveSortField3];
	var result = ( ( x < y ) ? -1 : ( ( x > y ) ? 1 : 0 ) );
    return result;
}
function sort_desc3( ElementA , ElementB ) {
    var x = ElementA.Fields[ActiveSortField3];
    var y = ElementB.Fields[ActiveSortField3];
    return ( ( x < y ) ? 1 : ( ( x > y ) ? -1: 0 ) );
}


/**********************
** DISPLAY FUNCTIONS **
**********************/
function Display()
{
object = document.getElementById('PlayerInfo');
html = "";
html = "<table width='100%' class='stattable' cellspacing='0'>";
html += "<tr>";
html += "<th colspan='8' class='player_online_count'>Online Player Info (" + PlayerMap.length + " Players online)</th>";
html += "</tr>";
html += "<tr>";
for( i = 0 ; i <= 7 ; i ++ )
{
	sortdir_pointer = "";
	IsSelectedClass = "";
	if( ActiveSortField == i )
	{
	IsSelectedClass = " activesortfield";
	if( FieldSortDirection[ActiveSortField] == 1 )
	sortdir_pointer = " <img src='ascending.gif'>";
	else
	sortdir_pointer = " <img src='descending.gif'>";
	}
	html += "<th width='" + FieldInfo[i][1] + "' class='theader" + IsSelectedClass + "'><a href='javascript:DoSort(" + i + ",0)'>" + FieldInfo[i][0] + "</a>" + sortdir_pointer + "</th>";
}

html += "</tr>";
altclass = '';
if( PlayerMap.length )
{
for( i = 0 ; i <= 2500 ; i += 1 )
{
if( PlayerMap[i] )
{
for( j = 0 ; j <= 7 ; j += 1 )
{
	if( ActiveSortField == j )
	activeclass = '_active_row';
	else
	activeclass = '';
	
	if( PlayerMap[i].Fields[0].toLowerCase() == "supalosa" )
	activeclass += ' important';
	if( j == 0 )
	{
	TempText = "<img src='icons/faction/icon-" + FactionName[PlayerMap[i].Fields[2]] + ".png' />&nbsp;" + PlayerMap[i].Fields[j];
	}
	else if( j == 2 )
	TempText = "<img src='icons/race/" + PlayerMap[i].Fields[j] + "-" + PlayerMap[i].Gender + ".gif' />&nbsp;" + RaceNames[PlayerMap[i].Fields[j]];
	else if( j == 3 )
	TempText = "<img src='icons/class/" + PlayerMap[i].Fields[j] + ".gif' />&nbsp;" + ClassNames[PlayerMap[i].Fields[j]];
	else if( j == 7 )
	TempText = PlayerMap[i].Fields[j] + "ms";
	else
	TempText = PlayerMap[i].Fields[j];
	
	html += "<td class='tdata row" + altclass + activeclass + "'>" + TempText + "</td>";
	
}
html += "</tr>";
if( altclass == '' )
altclass = '_alternate_row';
else
altclass = '';
}
else
{
html += "</table>";
object.innerHTML = html;
return;
}
}
}
else
{
/* No players online! :O */
html += "<tr>";
html += "<td colspan='" + ( FieldInfo.length + 1 ) + "' class='row centrealign none_available'>Ouch! There are no players online!</td>";
html += "</tr>";
html += "</table>";
object.innerHTML = html;
return;
}


}
function DisplayInstance()
{
object = document.getElementById('InstanceInfo');
html = "";
html = "<table width='100%' class='stattable' cellspacing='0'>";
html += "<tr>";
html += "<th colspan='7' class='player_online_count'>Instance Info</th>";
html += "</tr>";
html += "<tr>";
for( i = 0 ; i <= 6 ; i ++ )
{
	sortdir_pointer = "";
	IsSelectedClass = "";
	if( ActiveSortField2 == i )
	{
	IsSelectedClass = " activesortfield";
	if( FieldSortDirection2[ActiveSortField2] == 1 )
	sortdir_pointer = " <img src='ascending.gif'>";
	else
	sortdir_pointer = " <img src='descending.gif'>";
	}
	html += "<th width='" + FieldInfoInstances[i][1] + "' class='theader" + IsSelectedClass + "'><a href='javascript:DoSort(" + i + ",1)'>" + FieldInfoInstances[i][0] + "</a>" + sortdir_pointer + "</th>";
}

html += "</tr>";
altclass = '';
for( i = 0 ; i <= 100 ; i += 1 )
{
if( InstanceMap[i] )
{
for( j = 0 ; j <= 6 ; j += 1 )
{
	if( ActiveSortField2 == j )
	activeclass = '_active_row';
	else
	activeclass = '';
	if( j == 4 )
	{
	TempText = InstanceTypeNames[InstanceMap[i].Fields[j]];
	}
	else
	TempText = InstanceMap[i].Fields[j];
	
	html += "<td class='tdata row" + altclass + activeclass + "'>" + TempText + "</td>";
	
}
html += "</tr>";
if( altclass == '' )
altclass = '_alternate_row';
else
altclass = '';
}
else
{
html += "</table>";
object.innerHTML = html;
return;
}
}


}
function DisplayGMInfo()
{

object = document.getElementById('GMInfo');
html = "";
html = "<table width='100%' class='stattable' cellspacing='0'>";
html += "<tr>";
html += "<th colspan='7' class='player_online_count'>GM Info</th>";
html += "</tr>";
html += "<tr>";
for( i = 0 ; i <= 6 ; i ++ )
{
	sortdir_pointer = "";
	IsSelectedClass = "";
	if( ActiveSortField3 == i )
	{
	IsSelectedClass = " activesortfield";
	if( FieldSortDirection3[ActiveSortField3] == 1 )
	sortdir_pointer = " <img src='ascending.gif'>";
	else
	sortdir_pointer = " <img src='descending.gif'>";
	}
	html += "<th width='" + FieldInfoGM[i][1] + "' class='theader" + IsSelectedClass + "'><a href='javascript:DoSort(" + i + ",2)'>" + FieldInfoGM[i][0] + "</a>" + sortdir_pointer + "</th>";
}

html += "</tr>";
altclass = '';
if( GMMap.length )
{
for( i = 0 ; i <= 100 ; i += 1 )
{
if( GMMap[i] )
{
for( j = 0 ; j <= 6 ; j += 1 )
{
	if( ActiveSortField3 == j )
	activeclass = '_active_row';
	else
	activeclass = '';
	if( j == 0 )
	{
	TempText = "<img src='icons/faction/icon-" + FactionName[GMMap[i].Fields[2]] + ".png' />&nbsp;" + GMMap[i].Fields[j];
	}
	else if( j == 2 )
	TempText = "<img src='icons/race/" + GMMap[i].Fields[j] + "-" + GMMap[i].Fields[7] + ".gif' />&nbsp;" + RaceNames[GMMap[i].Fields[j]];
	else if( j == 3 )
	TempText = "<img src='icons/class/" + GMMap[i].Fields[j] + ".gif' />&nbsp;" + ClassNames[GMMap[i].Fields[j]];
	else if( j == 5 )
	TempText = GMMap[i].Fields[j] + "ms";
	else
	TempText = GMMap[i].Fields[j];
	
	html += "<td class='tdata row" + altclass + activeclass + "'>" + TempText + "</td>";
	
}
html += "</tr>";
if( altclass == '' )
altclass = '_alternate_row';
else
altclass = '';
}
else
{
html += "</table>";
object.innerHTML = html;
return;
}
}
}
else
{
/* No game masters online! :O */
html += "<tr>";
html += "<td colspan='" + ( FieldInfo.length + 1 ) + "' class='row centrealign none_available'>Sorry, there are no Game Masters available!</td>";
html += "</tr>";
html += "</table>";
object.innerHTML = html;
return;
}


}
function HighlightRow( row_object )
{
row_object.style.backgroundColor = '#FFF';
}
