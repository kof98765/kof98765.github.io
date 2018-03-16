#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include "game.h" //关于游戏的定义

Mission mission[50];
void initGame()
{
	memset(mission,0,sizeof(mission));
}
void play()
{
	printf("我打，我打，我打打打\n");
}
int loadMission()
{
	FILE * fp;
	fp = fopen("missionList.txt", "r");//读取关卡列表文件
    if (NULL== fp)    
    {
        return 0;
    }

    int ret=0;
    char namelist[50][50];//最多50个插件，每个插件的名字长度最多50
    memset(namelist,0,sizeof(namelist));
    int count=0;
    while(fgets((char *)&namelist[count], 50, fp)) 
    {
        ret=strlen(namelist[count]);//计算实际字符串长度
        if(namelist[count][ret-1]=='\n')
            namelist[count][ret-1]='\0';//fgets会读多一个换行，所以需要替换掉
        count++;
    }	 
    fclose(fp);

    ret=0;
    for(int i=0;i<count;i++)
    {
    	const char *errmsg;

        dlerror();  // 清除错误
        void *m_hLib = (void *)dlopen(namelist[i], RTLD_LAZY);
        
        if( (errmsg = dlerror()) != NULL )
        {
        	//printf("err=%s\n",errmsg);打印错误
        	ret++;//累加错误次数
            continue;
        }

        if( m_hLib == NULL )
        {
        	ret++;//累加错误次数
            continue;
        }
        
        dlerror();  // 清除错误
        Info info = (Info)dlsym( m_hLib, "gameInfo" );
        if( (errmsg = dlerror()) != NULL )
        {
            dlclose(m_hLib);
			printf("err=%s\n",errmsg);
            ret++;
            continue;
        }
        info(&mission[i]);
    }
    
    count=count-ret;//计算有效关卡数
    return count;

}
int main()
{
	initGame();//初始化游戏	
	int missionNum=loadMission();//读取关卡数
	if(missionNum==0)
	{
		printf("游戏异常退出\n");
		return -1;
	}
    //初始化一个角色，并满血
	Player p;
    p.life=100;

	while(1)
	{
		printf("请选择关卡\n");
		for(int i=0;i<missionNum;i++)
		{
			printf("%d.%s\n",i+1,mission[i].missionName);
		}
		int missionNo=getchar();//选择关卡

		missionNo=missionNo-'0';
		if(missionNo>missionNum || missionNo<1)
		{
			printf("没有此关卡!\n");
			continue;
		}
		//读取来的关卡肯定是有难度等级的，没有就是无效关卡
		if(mission[missionNo-1].level!=0)
        {
            while(p.life>0)
                mission[missionNo-1].process(&p);
        }
        printf("Game Over!\n\n\n");
		
	}
	
}
