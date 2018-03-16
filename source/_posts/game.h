#include <stdio.h>
#include <string.h>
//游戏玩家描述
typedef struct _player
{
	int life;//玩家生命
}Player;
//游戏关卡描述
typedef struct _mission
{
	int level;//关卡难度
	char missionName[30];//关卡名称
	void (*process)(Player *);//游戏过程
}Mission;

 typedef void (*Info)(Mission*);