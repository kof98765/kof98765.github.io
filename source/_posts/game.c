#include "game.h"
const char name[]="第一关，插件入门";
const int level=1;

void firstMission(Player *p)
{
	printf("oh my god ,somebody hurt me!\n");
	p->life--;
	printf("now my life is %d\n",p->life);
	sleep(1);
}
void gameInfo(Mission *m)
{
	m->level=level;
	int len=strlen(name);
	memcpy(m->missionName,name,len);//注意游戏名称不要太长
	m->process=firstMission;
}