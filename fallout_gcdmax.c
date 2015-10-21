/*----------------------------------------------------------------------------*/
#include <stdio.h>
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
/*----------------------------------------------------------------------------*/
/**
 * skills
**/
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
/* traits */
#define TRAIT_FAST_METABOLISM 0
#define TRAIT_BRUISER 1
#define TRAIT_SMALL_FRAME 2
#define TRAIT_ONE_HANDER 3
#define TRAIT_FINESSE 4
#define TRAIT_KAMIKAZE 5
#define TRAIT_HEAVY_HANDED 6
#define TRAIT_FAST_SHOT 7
#define TRAIT_BLOODY_MESS 8
#define TRAIT_JINXED 9
#define TRAIT_GOOD_NATURED 10
#define TRAIT_CHEM_RELIANT 11
#define TRAIT_CHEM_RESISTANT 12
#define TRAIT_SEX_APPEAL 13
#define TRAIT_SKILLED 14
#define TRAIT_GIFTED 15
#define TRAIT_COUNT 16
/*----------------------------------------------------------------------------*/
typedef struct _skill_comp_t
{
	int base,modw,stat1,stat2;
}
skill_comp_t;
/*----------------------------------------------------------------------------*/
const skill_comp_t SKILL_CALC[SKILL_COUNT] =
{
	{5,4,STAT_AG,-1},
	{0,2,STAT_AG,-1},
	{0,2,STAT_AG,-1},
	{30,2,STAT_AG,STAT_ST},
	{20,2,STAT_AG,STAT_ST},
	{0,4,STAT_AG,-1},
	{0,2,STAT_PE,STAT_IN},
	{5,1,STAT_PE,STAT_IN},
	{5,3,STAT_AG,-1},
	{10,1,STAT_PE,STAT_AG},
	{0,3,STAT_AG,-1},
	{10,1,STAT_PE,STAT_AG},
	{0,4,STAT_IN,-1},
	{0,3,STAT_IN,-1},
	{0,5,STAT_CH,-1},
	{0,4,STAT_CH,-1},
	{0,5,STAT_LK,-1},
	{0,2,STAT_EN,STAT_IN}
};
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
const char TRAIT_NAMES[TRAIT_COUNT][24] =
{
	{"Fast Metabolism"},{"Bruiser"},{"Small Frame"},{"One Hander"},
	{"Finesse"},{"Kamikaze"},{"Heavy Handed"},{"Fast Shot"},
	{"Bloody Mess"},{"Jinxed"},{"Good Natured"},{"Chem Reliant"},
	{"Chem Resistant"},{"Sex Appeal"},{"Skilled"},{"Gifted"}
};
/*----------------------------------------------------------------------------*/
typedef struct _stats_t
{
	unsigned int flag;
	/* primary stat 0 - 10 */
	unsigned int primary[STAT_PRIMARY_COUNT];
	/* secondary stat */
	unsigned int secondary[STAT_SECOND_COUNT];
	/* bonus for primary */
	unsigned int bonus_primary[STAT_PRIMARY_COUNT];
	/* bonus for secondary */
	unsigned int bonus_secondary[STAT_SECOND_COUNT];
	/* skills => offset: (1+7+28+7+28)x4=284 */
	unsigned int skills[SKILL_COUNT];
	/* offset: 284+(18x4)=356 */
	unsigned int unknown[4];
	/* offset: 372 */
	char name[32];
	/* offset: 404 */
	unsigned int tagged_skill[4]; /* fourth is 0xFFFFFFFF (perks) */
	/* offset: 420 */
	unsigned int traits[2];
	unsigned int end0;
	/* size: 432 */
}
stats_t;
/*----------------------------------------------------------------------------*/
#define NAME_OFFSET1 368
#define NAME_OFFSET2 372
#define SIZE_THEREST 60
/*----------------------------------------------------------------------------*/
int check_skill(stats_t *pstat, int skill_index, int set)
{
	int test = SKILL_CALC[skill_index].base +
		SKILL_CALC[skill_index].modw*SKILL_CALC[skill_index].stat1;
	if(SKILL_CALC[skill_index].stat2!=-1)
		test += SKILL_CALC[skill_index].modw*SKILL_CALC[skill_index].stat2;
	if(set)
		pstat->skills[skill_index] = test;
	return test;
}
/*----------------------------------------------------------------------------*/
void print_stats(stats_t* pstat)
{
	int loop=0,test;
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
		test = check_skill(pstat,loop,0);
		printf("%s: %d (%d)",SKILL_NAMES[loop],pstat->skills[loop],test); loop++;
		if(loop==SKILL_COUNT) { putchar('\n'); break; }
		else if(loop%3==0) printf("\n  ");
		else printf(" , ");
	}
	printf("MISC\n");
	printf("  Name: %s\n",pstat->name);
	printf("  Trait 1: %s\n",TRAIT_NAMES[pstat->traits[1]]);
	printf("  Trait 2: %s\n",TRAIT_NAMES[pstat->traits[0]]);
	printf("  Tag Skill 1: %s\n",SKILL_NAMES[pstat->tagged_skill[2]]);
	printf("  Tag Skill 2: %s\n",SKILL_NAMES[pstat->tagged_skill[1]]);
	printf("  Tag Skill 3: %s\n",SKILL_NAMES[pstat->tagged_skill[0]]);
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
void toggle_stat_end(stats_t* pstat)
{
	int loop;
	unsigned int *pdata = (unsigned int*) pstat;
	for(loop=0;loop<sizeof(stats_t)/sizeof(int);loop++)
	{
		/* skip name: 372-404 */
		if(loop<93||loop>=101)
			big2lit_end((void*)pdata,sizeof(int));
		pdata++;
	}
}
/*----------------------------------------------------------------------------*/
int main(int argc, char* argv[])
{
	FILE *pfile,*ptest;
	stats_t mystat;
	int size, test, loop, chk1 = 0;
	putchar('\n');
	basename(argv[0],1);
	printf("%s - Fallout 1/2 GCD To MAX\n\n", argv[0]);
	if(argc!=2)
	{
		printf("Usage: %s <filename>\n\n",argv[0]);
		return -2;
	}
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
			if(size==sizeof(stats_t)-sizeof(int))
			{
				chk1 = 1; /* fallout 1? */
			}
			else if(size!=sizeof(stats_t))
			{
				printf("[ERROR] Expecting %d bytes! "
					"(or %d bytes less for FO1)\n\n",
					sizeof(stats_t),sizeof(int));
				break;
			}
			/* reset file for read */
			fseek(pfile,0L,SEEK_SET);
			if(chk1)
			{
				fread((void*)&mystat,1,NAME_OFFSET1,pfile);
				mystat.unknown[3] = 0x00000000; /* just in case */
				fread((void*)&mystat.name,1,SIZE_THEREST,pfile);
			}
			else
				fread((void*)&mystat,1,sizeof(stats_t),pfile);
			/* correct endian */
			toggle_stat_end(&mystat);
			/* show me! */
			print_stats(&mystat);
			if((ptest=fopen("SHOWHACK","r"))||(ptest=fopen("HACKTHIS","r")))
			{
				fclose(ptest);
				/* max all primary stats */
				for(loop=0;loop<STAT_PRIMARY_COUNT;loop++)
					mystat.primary[loop] = 10;
				printf("\nMODDED!\n\n");
				/* show me! */
				print_stats(&mystat);
			}
			/* restore endian */
			toggle_stat_end(&mystat);
			/* reset file pointer position - for writeback! */
			if((ptest=fopen("HACKTHIS","r")))
			{
				fclose(ptest);
				fseek(pfile,0L,SEEK_SET);
				if(chk1)
				{
					test = fwrite((void*)&mystat,1,NAME_OFFSET1,pfile);
					test += sizeof(int);
					test += fwrite((void*)&mystat.name,1,SIZE_THEREST,pfile);
				}
				else
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
