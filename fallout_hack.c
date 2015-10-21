/*----------------------------------------------------------------------------*/
#include <stdio.h>
/*----------------------------------------------------------------------------*/
#define SIZEOF_HEAD 0x7563
#define SIZEOF_FUN0 0x0000
#define SIZEOF_FUN1 0x0004
#define SIZEOF_GVAR_FALLOUT1 0x09A9
#define SIZEOF_GVAR_FALLOUT2 0x0AE0
/* 0x00,0x00,'F','P' */
#define KNOWN_DATACK 0x00004650
/*----------------------------------------------------------------------------*/
typedef struct _player_t /* 0x80 @128 bytes */
{
	unsigned char dummy0[0x48];
	unsigned int item_count;
	unsigned char dummy1[0x28];
	unsigned int current_hp;
	unsigned int radiation_lvl;
	unsigned int poison_lvl;
}
player_t;
/*----------------------------------------------------------------------------*/
typedef struct _item_t
{
	unsigned int quantity;
	unsigned int unknown0; /* changes when one is in use? */
	unsigned int location; /* -1 for inventory */
	unsigned int unknowns_0[2];
	unsigned int specials_0[2];
	unsigned int unknowns_1[2];
	unsigned int image_onground;
	/* bitflag msb 0x01 (r) 0x02 (l) 0x04 (body) : unknown0 changes */
	/* (why lsb 0x08?) => assume for weapon? */
	unsigned int inuse_location;
	unsigned int map_level; /* zero for inventory? */
	unsigned int objectid;
	unsigned int checkFF0; /* always -1? */
	unsigned int unknowns_2[3];
	unsigned int scriptid;
	unsigned int checkFF1; /* always -1? */
	unsigned int bag_count; /* 0 for non-containers */
	unsigned int bag_items[2]; /* 0 for non-containers */
	unsigned int check000; /* always 0? */
	unsigned int ammo_count; /* for ammo:  */
	unsigned int ammo_type; /* -1 for no ammo */
}
item_t;
/*----------------------------------------------------------------------------*/
#define ITEM_TYPE_UNKNOWN -1
#define ITEM_TYPE_ARMOR 0
#define ITEM_TYPE_CONTAINER 1
#define ITEM_TYPE_DRUG 2
#define ITEM_TYPE_WEAPON 3
#define ITEM_TYPE_AMMO 4
#define ITEM_TYPE_MISC 5
#define ITEM_TYPE_KEY 6
/*----------------------------------------------------------------------------*/
int item_type(unsigned int type)
{
	int which = ITEM_TYPE_UNKNOWN;
	switch(type)
	{
		case 0x04: /* knife */
		case 0x07: /* spear */
		case 0x08: /* 10mm pistol */
		case 0x2d: /* throwing knife */
		case 0x4f: /* flare */
		case 0x78: /* alien blaster */
			which = ITEM_TYPE_WEAPON; break;
		case 0x1d: /* 10mm jhp : fullclip=24! */
		case 0x1e: /* 10mm ap */
		case 0x26: /* laser ammo? */
			which = ITEM_TYPE_AMMO; break;
		case 0x28: /* stimpak */
		case 0x31: /* antidote */
			which = ITEM_TYPE_DRUG; break;
		case 0x29: /* bottle caps */
		case 0x2f: /* first aid kit */
		case 0x4b: /* tools */
			which = ITEM_TYPE_MISC; break;
		case 0x54: /* lock picks */
			which = ITEM_TYPE_KEY; break;
/*
			which = ITEM_TYPE_ARMOR; break;
			which = ITEM_TYPE_CONTAINER; break;
*/
	}
	return which;
}
/*----------------------------------------------------------------------------*/
#define ITEM_BASE_COUNT 23
#define ITEM_MORE_COUNT 2
/*----------------------------------------------------------------------------*/
typedef struct _item_check_t
{
	unsigned int dummy[ITEM_BASE_COUNT+ITEM_MORE_COUNT];
}
item_check_t;
/*----------------------------------------------------------------------------*/
/**
 *  primary stats (values: 1-10)
 *  - strength, perception, endurance, charisma
 *  - intelligence, agility, luck
**/
#define STAT_ST 0
#define STAT_PE 1
#define STAT_EN 2
#define STAT_CH 3
#define STAT_IN 4
#define STAT_AG 5
#define STAT_LK 6
#define STAT_PRIMARY_COUNT 7
/**
 *  secondary stats
 *  - hit_points, action_points, armor_class (values: 0-999)
 *  - unarmed_damage, melee_damage, carry_weight (values: 0-999)
 *  - sequence, healing_rate (values: 0-99)
 *  - critical_chance, critical_hit_modifier (values: 0-100)
 *  - dmg_thresh : normal, laser, fire, plasma,
 *      electrical, emp, explosive (values: 0-999)
 *  - dmg_res : normal, laser, fire, plasma,
 *      electrical, emp, explosive (values: 0-100)
 *  - radiation resistance, poison resistance (values: 0-100)
 *  - age, gender (values: age to 35, gender 0 is male)
**/
#define STAT_HP 0
#define STAT_AP 1
#define STAT_AC 2
#define STAT_UD 3
#define STAT_MD 4
#define STAT_CW 5
#define STAT_SQ 6
#define STAT_HR 7
#define STAT_CC 8
#define STAT_CM 9
#define STAT_DT0 10
#define STAT_DT1 11
#define STAT_DT2 12
#define STAT_DT3 13
#define STAT_DT4 14
#define STAT_DT5 15
#define STAT_DT6 16
#define STAT_DR0 17
#define STAT_DR1 18
#define STAT_DR2 19
#define STAT_DR3 20
#define STAT_DR4 21
#define STAT_DR5 22
#define STAT_DR6 23
#define STAT_RR 24
#define STAT_PR 25
#define STAT_AGE 26
#define STAT_GENDER 27
#define STAT_SECOND_COUNT 28
/* skills */
#define SKILL_SMALL_GUNS 0
#define SKILL_BIG_GUNS 1
#define SKILL_ENERGY_WEAPONS 2
#define SKILL_UNARMED 3
#define SKILL_MELEE_WEAPONS 4
#define SKILL_THROWING 5
#define SKILL_FIRST_AID 6
#define SKILL_DOCTOR 7
#define SKILL_SNEAK 8
#define SKILL_LOCKPICK 9
#define SKILL_STEAL 10
#define SKILL_TRAPS 11
#define SKILL_SCIENCE 12
#define SKILL_REPAIR 13
#define SKILL_SPEECH 14
#define SKILL_BARTER 15
#define SKILL_GAMBLING 16
#define SKILL_OUTDOORSMAN 17
#define SKILL_COUNT 18
/*----------------------------------------------------------------------------*/
const char STAT_PRI_NAMES[STAT_PRIMARY_COUNT][16] =
{
	{"Strength"},{"Perception"},{"Endurance"},{"Charisma"},
	{"Intelligence"},{"Agility"},{"Luck"}
};
const char STAT_SEC_NAMES[STAT_SECOND_COUNT][24] =
{
	{"Hit Points"},{"Action Points"},{"Armor Class"},
	{"Unarmed(N/A)"},{"Melee Damage"},{"Carry Weight"},
	{"Sequence"},{"Healing Rate"},{"Critical Chance"},
	{"Critical Hit Modifier"},{"DT Normal"},{"DT Laser"},
	{"DT Fire"},{"DT Plasma"},{"DT Electrical"},{"DT EMP"},
	{"DT Explosives"},{"DR Normal"},{"DR Laser"},
	{"DR Fire"},{"DR Plasma"},{"DR Electrical"},{"DR EMP"},
	{"DR Explosives"},{"Radiation Res."},{"Poison Res."},
	{"Age"},{"Gender"}
};
const char SKILL_NAMES[SKILL_COUNT][24] =
{
	{"Small Guns"},{"Big Guns"},{"Energy Weapons"},{"Unarmed"},
	{"Melee Weapons"},{"Throwing"},{"First Aid"},{"Doctor"},{"Sneak"},
	{"Lockpick"},{"Steal"},{"Traps"},{"Science"},{"Repair"},{"Speech"},
	{"Barter"},{"Gambling"},{"Outdoorsman"}
};
/*----------------------------------------------------------------------------*/
typedef struct _stats_t
{
	unsigned int what,that,flag;
	/* primary stat 0 - 10 */
	unsigned int primary[STAT_PRIMARY_COUNT];
	/* secondary stat */
	unsigned int secondary[STAT_SECOND_COUNT];
	/* bonus for primary */
	unsigned int bonus_primary[STAT_PRIMARY_COUNT];
	/* bonus for secondary */
	unsigned int bonus_secondary[STAT_SECOND_COUNT];
	/** skills => offset: (3+7+28+7+28)x4=292 */
	unsigned int skills[SKILL_COUNT];
}
stats_t;
/*----------------------------------------------------------------------------*/
void print_stats(stats_t* pstat)
{
	int loop=0;
	printf("PRIMARY STATS\n  ");
	while(1)
	{
		printf("%s: %d (%d)",STAT_PRI_NAMES[loop],
			pstat->primary[loop],pstat->bonus_primary[loop]); loop++;
		if(loop==STAT_PRIMARY_COUNT) { putchar('\n'); break; }
		else if(loop%2==0&&loop<STAT_PRIMARY_COUNT-1) printf("\n  ");
		else printf(" , ");
	}
	loop=0;
	printf("SECONDARY STATS\n  ");
	while(1)
	{
		if(loop>=STAT_DT0&&loop<=STAT_DR6)
		{
			loop++; continue;
		}
		printf("%s: %d (%d)",STAT_SEC_NAMES[loop],
			pstat->secondary[loop],pstat->bonus_secondary[loop]); loop++;
		if(loop==STAT_SECOND_COUNT) { putchar('\n'); break; }
		else if(loop%2==0) printf("\n  ");
		else printf(" , ");
	}
	loop=0;
	printf("SKILLS\n  ");
	while(1)
	{
		printf("%s: %d",SKILL_NAMES[loop],pstat->skills[loop]); loop++;
		if(loop==SKILL_COUNT) { putchar('\n'); break; }
		else if(loop%4==0) printf("\n  ");
		else printf(" , ");
	}
}
/*----------------------------------------------------------------------------*/
char* basename(char* pname, int nodot)
{
	int index = -1, index_d = -1, size = -1, count = 0, loop;
	int path_sep = '/';
	if(pname[1]==':') path_sep = '\\';
	while(pname[count]!='\0') count++;
	for(loop=count-1;loop>=0;loop--)
	{
		if(pname[loop]=='.')
		{
			if(index_d<0) index_d = loop;
		}
		if(pname[loop]==path_sep)
		{
			index = loop + 1;
			size = count - index;
			if(nodot&&index_d>=0)
				size -= count - index_d;
			break;
		}
	}
	if(index>=0)
	{
		for(loop=0;loop<size;loop++)
		{
			pname[loop] = pname[loop+index];
		}
		pname[size] = '\0';
	}
	return pname;
}
/*----------------------------------------------------------------------------*/
void big2lit_end(void *src, int size)
{
	unsigned char *chk = (unsigned char*) src, *nxt = &chk[size-1];
	unsigned char swap;
	int loop;
	for(loop=0;loop<size/2;loop++)
	{
		swap = *chk;
		*chk = *nxt;
		*nxt = swap;
		chk++; nxt--;
	}
}
/*----------------------------------------------------------------------------*/
int main(int argc, char* argv[])
{
	FILE *pfile,*ptest;
	player_t myself;
	stats_t mystat;
	item_t myitem;
	unsigned int *pdata;
	int size, test, loop, save, debug = 0;
	unsigned int sizeof_GVAR = SIZEOF_GVAR_FALLOUT1;
	unsigned int offset_4hack = SIZEOF_HEAD + SIZEOF_FUN0 + SIZEOF_FUN1;
	putchar('\n');
	basename(argv[0],1);
	printf("%s - Fallout 1/2 SAVE.DAT File Editor\n\n", argv[0]);
	if(argc!=2)
	{
		printf("Usage: %s <filename>\n\n",argv[0]);
		printf(" Note: To hack Fallout 2, create a dummy file named FALLOUT2\n\n");
		return -2;
	}
	if((ptest=fopen("DEBUG","r")))
	{
		fclose(ptest);
		printf("DO DEBUG!\n\n");
		debug = 1;
	}
	if((ptest=fopen("FALLOUT2","r")))
	{
		fclose(ptest);
		sizeof_GVAR = SIZEOF_GVAR_FALLOUT2; /* hacking for fallout2 instead! */
		printf("Hacking Fallout 2!\n\n");
	}
	offset_4hack += sizeof_GVAR;
	pfile = fopen(argv[1],"rb+");
	if(pfile)
	{
		do
		{
			/* remove path from filename */
			basename(argv[1],0);
			/* try to get file size */
			fseek(pfile,0L,SEEK_END);
			size = ftell(pfile);
			printf("FileName: %s\n",argv[1]);
			printf("Size: %d byte(s)\n\n",size);
			/* seek known offset */
			fseek(pfile,offset_4hack,SEEK_SET);
			if(debug)
			{
				test = ftell(pfile);
				printf("Index: %d (%08X)\n",test,test);
			}
			fread((void*)&test,1,sizeof(test),pfile);
			big2lit_end((void*)&test,sizeof(test));
			if(debug) /* Map Count - Getting NULL-terminated names next */
				printf("Count1: %d\n",test);
			for(loop=0;loop<test;loop++)
			{
				while(fgetc(pfile)!=0x00);
			}
			fread((void*)&test,1,sizeof(test),pfile);
			big2lit_end((void*)&test,sizeof(test));
			if(debug) /* Ignored Value? */
				printf("Count2: %d\n",test);
			/* seek known offset */
			fseek(pfile,sizeof_GVAR,SEEK_CUR);
			if(debug)
			{
				test = ftell(pfile);
				printf("Index: %d (%08X)\n",test,test);
			}
			/* get player info! */
			fread((void*)&myself,1,sizeof(player_t),pfile);
			pdata = (unsigned int*) myself.dummy0;
			big2lit_end((void*)pdata,sizeof(unsigned int));
			if(*pdata!=KNOWN_DATACK)
			{
				printf("Check failed! (%08X)!=(%08X)\n",*pdata,KNOWN_DATACK);
				break;
			}
			big2lit_end((void*)&myself.item_count,sizeof(unsigned int));
			if(debug)
			{
				test = ftell(pfile);
				printf("Item Count: %d\n",myself.item_count);
				printf("Index0: %d (%08X)\n",test,test);
			}
			/* loop through items */
			for(loop=0;loop<myself.item_count;loop++)
			{
				fread((void*)&myitem,sizeof(int),ITEM_BASE_COUNT,pfile);
				item_check_t* pcheck = (item_check_t*) &myitem;
				for(test=0;test<ITEM_BASE_COUNT;test++)
					big2lit_end((void*)&pcheck->dummy[test],sizeof(int));
				if(debug)
				{
					printf("  Item#%02d: %08X\n",loop,myitem.objectid);
					printf("    Location: %08X\n",myitem.location);
					printf("    Quantity: %d\n",myitem.quantity);
					printf("    Unknown0: %08X\n",myitem.unknown0);
					printf("    InUseChk: %08X\n",myitem.inuse_location);
					printf("    Check000: %08X\n",myitem.check000);
				}
				pdata = &pcheck->dummy[ITEM_BASE_COUNT];
				if(item_type(myitem.objectid)>=ITEM_TYPE_WEAPON)
				{
					fread((void*)pdata,1,sizeof(unsigned int),pfile);
					if(debug)
					{
						big2lit_end((void*)pdata,sizeof(unsigned int));
						printf("    Extra000: %08X\n",*pdata);
					}
					pdata++;
				}
				if(item_type(myitem.objectid)==ITEM_TYPE_WEAPON)
				{
					fread((void*)pdata,1,sizeof(unsigned int),pfile);
					if(debug)
					{
						big2lit_end((void*)pdata,sizeof(unsigned int));
						printf("    Extra001: %08X\n",*pdata);
					}
					pdata++;
				}
			}
			save = ftell(pfile);
			if(debug)
				printf("Index1: %d (%08X)\n",save,save);
			fread((void*)&mystat,1,sizeof(stats_t),pfile);
			/* correct endian */
			pdata = (unsigned int*) &mystat;
			for(loop=0;loop<sizeof(stats_t)/sizeof(int);loop++)
			{
				big2lit_end((void*)pdata,sizeof(unsigned int));
				pdata++;
			}
			/* show me! */
			print_stats(&mystat);
			/* max all primary stats
			for(loop=0;loop<STAT_PRIMARY_COUNT;loop++)
			{
				mystat.primary[loop] = 10;
			}
			*/
			/* recalc secondary stats
			mystat.secondary[STAT_HP] = (2*mystat.primary[STAT_EN]) +
				(1*mystat.primary[STAT_ST]) + 15;
			mystat.secondary[STAT_AC] = (1*mystat.primary[STAT_AG]) + 0;
			mystat.secondary[STAT_AP] = (mystat.primary[STAT_AG]/2) + 5;
			*/
			/* all bonus secondary stats is double base stat
			for(loop=0;loop<STAT_SECOND_COUNT;loop++)
			{
				mystat.bonus_secondary[loop] = 2*mystat.secondary[loop];
			}
			*/
			/* round up all skills
			pdata = (unsigned int*) mystat.skills;
			for(loop=0;loop<SKILL_COUNT;loop++)
			{
				(*pdata) = (((*pdata)/10)+1)*10;
				pdata++;
			}
			*/
			/* simple mod for now! */
			/* max all primary stats' bonus */
			for(loop=0;loop<STAT_PRIMARY_COUNT;loop++)
				mystat.bonus_primary[loop] = 10;
			mystat.bonus_secondary[STAT_HP] = 200;
			mystat.bonus_secondary[STAT_AP] = 10;
			mystat.bonus_secondary[STAT_AC] = 20;
			mystat.bonus_secondary[STAT_CW] = 200;
			mystat.bonus_secondary[STAT_SQ] = 10;
			mystat.bonus_secondary[STAT_HR] = 10;
			mystat.bonus_secondary[STAT_CC] = 100;
			mystat.bonus_secondary[STAT_CM] = 100;
			mystat.bonus_secondary[STAT_RR] = 100;
			mystat.bonus_secondary[STAT_PR] = 100;
			if((ptest=fopen("SHOWHACK","r"))||(ptest=fopen("HACKTHIS","r")))
			{
				fclose(ptest);
				printf("\nMODDED!\n\n");
				/* show me! */
				print_stats(&mystat);
			}
			/* restore endian */
			pdata = (unsigned int*) &mystat;
			for(loop=0;loop<sizeof(stats_t)/sizeof(int);loop++)
			{
				big2lit_end((void*)pdata,sizeof(unsigned int));
				pdata++;
			}
			/* reset file pointer position - for writeback! */
			if((ptest=fopen("HACKTHIS","r")))
			{
				fclose(ptest);
				fseek(pfile,save,SEEK_SET);
				test = fwrite((void*)&mystat,1,sizeof(stats_t),pfile);
				printf("\n[HACKED!] => %d (%d)\n",test,sizeof(stats_t));
			}
		}
		while(0);
		/* that's it! */
		fclose(pfile);
	}
	else
	{
		printf("Error opening file '%s'!\n",argv[1]);
	}
	putchar('\n');
	return 0;
}
/*----------------------------------------------------------------------------*/
