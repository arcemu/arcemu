#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <direct.h> // for _mkdir

#include "TileAssembler.h"

//=======================================================
// remove last return or LF and tailing SPACE
// remove all char after a #

void chompAndTrim(std::string& str)
{
	for(unsigned int i=0;i<str.length(); ++i) {
		char lc = str[i];
		if(lc == '#') {
			str = str.substr(0,i);
			break;
		}
	}

	while(str.length() >0) {
		char lc = str[str.length()-1];
		if(lc == '\r' || lc == '\n' || lc == ' ') {
			str = str.substr(0,str.length()-1);
		} else {
			break;
		}
	}
}

//=======================================================
/**
This callback method is called for each model found in the dir file.
return true if it should be included in the vmap
*/
bool modelNameFilter(char *pName)
{
#if 0
	bool result;
	result = !Wildcard::wildcardfit("*bush[0-9]*", pName);
	if(result) result = !Wildcard::wildcardfit("*shrub[0-9]*", pName);
	if(result) result = !Wildcard::wildcardfit("*_Bushes_*", pName);
	if(result) result = !Wildcard::wildcardfit("*_Bush_*", pName);
	if(!result) {
		printf("%s",pName);
	}
#endif
	return true;
}

//=======================================================
int main(int argc, char* argv[])
{
	bool ok = true;
	char *src = "buildings";
	char *dest = "vmaps";
	char *conffile = NULL;
	_mkdir(dest); // in case destination directory doesn't exist
	VMAP::TileAssembler* ta = new VMAP::TileAssembler(std::string(src), std::string(dest));
	ta->setModelNameFilterMethod(modelNameFilter);

	static int32 maps[999] = { 509, 469, 189, 30, 37, 33, 533, 209, 309, 560, 534, 532, 543, 568, 564, 0, 1, 530, 571, 129, 169, 559, 601, 604, 608, 616, 617, 618, 624, -1 /* keep -1 as last entry. if you need to add more, put them before -1 */};
	for(uint32 i = 0; maps[i] != -1; ++i)
		ta->addWorldAreaMapId( maps[i] );

	ok = ta->convertWorld();
	if(ok) {
		printf("Ok, all done\n");
	} else {
		printf("exit with errors\n");
		return 1;
	}
	delete ta;
	return 0;
}
