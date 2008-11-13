#include <stdio.h>
#include <string.h>
#include <libxml/tree.h>
#include <libxml/parser.h>

#include "Common.h"
#include "Main.h"


extern NameTableEntry g_worldOpcodeNames[];

unsigned char fromhex(unsigned char val) {
	if (val >= '0' && val <= '9') {
		return val - '0';
	} else {
		return (val - 'A')+0xA;
	}
}

typedef struct {
	unsigned char *data;
	int len;
} byteBuf;

unsigned char getByte(byteBuf *b) {
	if (b->len >= 0) {
		b->len--;
		return *(b->data++);
	} else {
		printf("need more data!!\n");
		return 0;
	}
}

unsigned short getShort(byteBuf *b) {
	return getByte(b) | (getByte(b)<<8);
}

unsigned int getInt(byteBuf *b) {
	return getByte(b) | (getByte(b)<<8) | (getByte(b)<<16) | (getByte(b)<<24);
}

float getFloat(byteBuf *b) {
	unsigned int f = getByte(b) | (getByte(b)<<8) | (getByte(b)<<16) | (getByte(b)<<24);
	return *(float*)&f;
}

void getString(byteBuf *b, char *dest) {
	unsigned char c;
	while ((c = getByte(b))) {
		*(dest++) = c;
	}
	*dest = 0;
}

void showPacketGuid(byteBuf *b) {
	unsigned char guid[8];
	unsigned char flags = getByte(b);
	int i;

	for (i=0; i<8; i++) {
		if (flags & (1<<i)) {
				guid[i] = getByte(b);
		} else {
			guid[i] = 0;
		}
	}

	printf("GUID = ");
	for (i=0; i<8; i++) {
		printf("%2.2x", guid[i]);
	}
	printf("\n");
}

void handleAuraUpdate(byteBuf *b) {
	unsigned int id;
	unsigned char flags;
	unsigned short flags2;

	showPacketGuid(b);
	printf("auraVisual = %2.2X\n", getByte(b));
	id = getInt(b);
	printf("spellID = %d (%8.8X)\n", id, id);

	if (id == 0) return;

	flags = getByte(b);
	flags2 = getShort(b);
	printf("flags = %2.2X flags2 = %4.4X\n", flags, flags2);
}

void handleAttackerStateUpdate(byteBuf *b) {
	unsigned int flags = getInt(b);
	printf("Flags = %8.8X\n", flags);

	showPacketGuid(b);
	showPacketGuid(b);

	printf("Damage = %d\n", getInt(b));
	printf("Overkill = %d\n", getInt(b));
	printf("Type = %d\n", getByte(b));
	printf("SchoolMask = %8.8X\n", getInt(b));
	printf("DamageFloat = %f\n", getFloat(b));
	printf("DamageAmt = %d\n", getInt(b));

	if (flags & 0x20) {
		printf("Abs = %d\n", getInt(b));
	}
	if (flags & 0x40) {
		printf("Resisted = %d\n", getInt(b));
	}
	printf("UnkByte = %d\n", getByte(b));
	printf("Unk = %8.8X\n", getInt(b));
	if (flags & 0x00800000) {
		printf("VState = %d\n", getInt(b));
	}
	printf("HitNumber = %d\n", getInt(b));

	if (b->len != 0) {
		printf("warning: remaining %d bytes left\n", b->len);
	}
}

void handleItemQuerySingleResponse(byteBuf *b) {
	char name[1024];
	int count;
	int i;

	printf("Id = %d\n", getInt(b));
	printf("Class = %d\n", getInt(b));
	printf("SubClass = %d\n", getInt(b));
	printf("unk = %d\n", getInt(b));
	getString(b, name);
	printf("name = %s\n", name);
	getByte(b); getByte(b); getByte(b);

	printf("displayinfoid = %d\n", getInt(b));
	printf("quality = %d\n", getInt(b));
	printf("flags = 0x%8.8x\n", getInt(b));
	printf("buyprice = %d\n", getInt(b));
	printf("sellprice = %d\n", getInt(b));
	printf("inventorytype = %d\n", getInt(b));
	printf("allowableclass = %d\n", getInt(b));
	printf("allowablerace = %d\n", getInt(b));
	printf("itemlevel = %d\n", getInt(b));
	printf("requiredlevel = %d\n", getInt(b));
	printf("requiredskill = %d\n", getInt(b));
	printf("requiredskillrank = %d\n", getInt(b));
	printf("requiredskillsubrank = %d\n", getInt(b));
	printf("requiredplayerrank1 = %d\n", getInt(b));
	printf("requiredplayerrank2 = %d\n", getInt(b));
	printf("requiredfaction = %d\n", getInt(b));
	printf("requiredfactionstanding = %d\n", getInt(b));
	printf("unique = %d\n", getInt(b));
	printf("maxcount = %d\n", getInt(b));
	printf("containerslots = %d\n", getInt(b));

	count = getInt(b);
	for (i=0; i<count; i++) {
		printf("Stats[%d].Type = %d\n", i, getInt(b));
		printf("Stats[%d].Value = %d\n", i, getInt(b));
	}

	printf("Unk = %d\n", getInt(b));
	printf("Unk = %d\n", getInt(b));

	for (i=0; i<5; i++) {
		printf("Damage[%d].Min = %f\n", i, getFloat(b));
		printf("Damage[%d].Max = %f\n", i, getFloat(b));
		printf("Damage[%d].Type = %d\n", i, getInt(b));
	}

	printf("Armor = %d\n", getInt(b));
	printf("HolyRes = %d\n", getInt(b));
	printf("FireRes = %d\n", getInt(b));
	printf("NatureRes = %d\n", getInt(b));
	printf("FrostRes = %d\n", getInt(b));
	printf("ShadowRes = %d\n", getInt(b));
	printf("ArcaneRes = %d\n", getInt(b));
	printf("Delay = %d\n", getInt(b));
	printf("AmmoType = %d\n", getInt(b));
	printf("Range = %d\n", getInt(b));
	for (i=0; i<5; i++) {
		printf("Spells[%d].Id = %d\n", i, getInt(b));
		printf("Spells[%d].Trigger = %d\n", i, getInt(b));
		printf("Spells[%d].Charges = %d\n", i, getInt(b));
		printf("Spells[%d].Cooldown = %d\n", i, getInt(b));
		printf("Spells[%d].Category = %d\n", i, getInt(b));
		printf("Spells[%d].CategoryCooldown = %d\n", i, getInt(b));
	}
	printf("Bonding = %d\n", getInt(b));
	getString(b, name);
	printf("Description = %s\n", name);
	printf("PageId = %d\n", getInt(b));
	printf("PageLanguage = %d\n", getInt(b));
	printf("PageMaterial = %d\n", getInt(b));
	printf("QuestId = %d\n", getInt(b));
	printf("LockId = %d\n", getInt(b));
	printf("LockMaterial = %d\n", getInt(b));
	printf("Field108 = %d\n", getInt(b));
	printf("RandomPropId = %d\n", getInt(b));
	printf("RandomSuffixId = %d\n", getInt(b));
	printf("Block = %d\n", getInt(b));
	printf("ItemSet = %d\n", getInt(b));
	printf("MaxDurability = %d\n", getInt(b));
	printf("ZoneNameId = %d\n", getInt(b));
	printf("MapId = %d\n", getInt(b));
	printf("BagFamily = %d\n", getInt(b));
	printf("TotemCategory = %d\n", getInt(b));
	for (i=0; i<3; i++) {
		printf("Sockets[%d].SocketColor = %d\n", i, getInt(b));
		printf("Sockets[%d].Unk = %d\n", i, getInt(b));
	}
	printf("SocketBonus = %d\n", getInt(b));
	printf("GemProperties = %d\n", getInt(b));
	printf("DisenchantReqSkill = %d\n", getInt(b));
	printf("ArmorDamageModifier = %d\n", getInt(b));
	printf("Unk = %d\n", getInt(b));
	printf("Unk2 = %d\n", getInt(b));

	if (b->len != 0) {
		printf("warning: remaining %d bytes left\n", b->len);
	}
}

void parsePacket(xmlNode *node) {
	xmlChar *sdir = xmlGetProp(node, (const xmlChar*)"direction");
	xmlChar *sop = xmlGetProp(node, (const xmlChar*)"opcode");
	unsigned int op = strtol((const char*)sop, (char**)NULL, 0);
	unsigned char data[64*1024];
	const char *content;
	byteBuf b;
	int i, len;

	if (op != 0x14A) return;
//	if (op != 0x496 && op != 0x495) return;
/*	if (op == 0xdd || op == 0x1f6 || op == 0x390 || op == 0x391) return;
	if (op == 0x3b || op == 0x463 || op == 0x20b || op == 0x38c) return;
	if (op == 0xb5 || op == 0xbd || op == 0xbe || op == 0x2e6) return;
	if (op == 0xb7 || op == 0xa9 || op == 0x13c || op == 0x250) return;
	if (op == 0xee || op == 0xc9 || op == 0xbc || op == 0x143) return;
	if (op == 0x1dd || op == 0x1dc || op == 0x4c || op == 0xe8) return;*/
	if (node->children == NULL) return;

	content = (const char*)node->children->content;
	for (i=0, len=0; i<(int)strlen(content); i+= 2) {
		data[len++] = (fromhex(content[i])<<4) |
					  fromhex(content[i+1]);
	}

	b.data = data;
	b.len = len;

	printf("\n%s; opcode = %4.4X %s\n", sdir, op, g_worldOpcodeNames[op].name);
	printf("%s\n", content);

	switch (op) {
		case 0x0058:
			handleItemQuerySingleResponse(&b);
			break;

		case 0x014A:
			handleAttackerStateUpdate(&b);
			break;

		case 0x0496:
			handleAuraUpdate(&b);
			break;
	}
}

int main(int argc, char *argv[]) {
	xmlDocPtr doc;
	xmlNode *node;

    doc = xmlReadFile(argv[1], NULL, 0);
    if (doc == NULL) {
        fprintf(stderr, "Failed to parse %s\n", argv[1]);
		return 0;
    }

	node = xmlDocGetRootElement(doc);
	node = node->children;
	while (node->next) {
        if (node->type == XML_ELEMENT_NODE) {
			if (!strcmp((const char*)node->name, "packet")) {
				parsePacket(node);
			}
        }

//        print_element_names(cur_node->children);
		node = node->next;
    }

	xmlFreeDoc(doc);

	return 0;
}
