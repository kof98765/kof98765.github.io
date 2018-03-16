---
title: 'Linux C编程:打造一个插件系统'
date: 2017-03-07 21:16:36
tags: linux
comments: true
---

运行环境：linux
使用语言：c 或者c++


插件，很多人用过，比如游戏插件，编辑器插件这些，
最著名的就数魔兽大脚插件啦，还有vim插件啦，eclipse插件啦，等等
插件有很多种形式，最常见的就是so文件，在windows上就是各种的dll啦

下面就让我们来了解一下插件式编程吧〜〜

举个例子，你开发了一个游戏，一开始只有三个关卡，分享给了你的小伙伴玩
大家玩得很爽，但很快，游戏通关了，还想玩，这时候怎么办呢？
常规做法就是再加两关，然后编译之后，再发给大家，那么问题就来了
每次想加新关卡，都需要重新编译一次，再下载一次，这个流量嘛，1+1.1+1.2+1.3,每次加个0.1,都够你受的^_^
这个时候嘛，就要引入插件系统了

咱先来写个小程序
```C++
//main.c
#include <stdio.h>
#include "game.h" //关于游戏的定义

void initGame()
{
	
}
void play()
{
	printf("我打，我打，我打打打\n");
}
void loadMission()
{

}
int main()
{
	initGame();//初始化游戏	
	int missionNum=loadMission();//读取关卡数

	while(1)
	{
		
		int missionNo=getchar();//选择关卡
		missionNo=missionNo-'0';
		if(missionNo>missionNum)
		{
			printf("没有此关卡!\n");
		}
		play();
		
	}
	
}
```
以上就是一个可扩展的游戏架构，够简单吧〜

下面咱就开始设计这个游戏系统的插件吧

在这里，游戏关卡就是我们的插件，一个插件就是一个关卡
先讲讲一个插件的构造吧
首先是这个关卡的描述，比如名称，难度等
其次就是这个关卡游戏过程了
最后，就是玩关卡的人

按照以上描述，咱定几个结构体吧
```c
//game.h
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

好了，下面就是一个插件的具体内容
```c
//game.c
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
	memcpy(m->missionName,name,len+1);//注意游戏名称不要太长
	m->process=firstMission;
}
```
Ok,以上就是一个关卡的所有东西了，虽然看着简单，不过还是建议动手敲敲^_^

那我们先把这个插件制作出来吧，免得一会忘记了，在终端下执行以下命令：
```
gcc  game.c -fPIC -shared -o firstMission.so -ldl
```
linux和mac都一样

关卡设计好了，接下来就是怎么样读取我们做好的关卡了

现在实现咱们就loadMission()函数

```1c
Mission mission[50];
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

    ret=0;//用于累加错误次数
    for(int i=0;i<count;i++)
    {
    	const char *errmsg;

        dlerror();  // 清除错误
        void *m_hLib = (void *)dlopen(namelist[i], RTLD_LAZY);//读取插件
        
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
        Info info = (Info)dlsym( m_hLib, "gameInfo" );//提取出插件里面的函数
        if( (errmsg = dlerror()) != NULL )
        {
            dlclose(m_hLib);
			printf("err=%s\n",errmsg);
            ret++;
            continue;
        }

        info(&mission[i]);//获取到关卡数据
    }
    ret=0;//关卡号
    //展示下关卡
    for(int i=0;i<count;i++)
    {
    	if(mission[i].level!=0)
    	{
    		printf("%d.%s\n",ret+1,mission[i].missionName);/正式发布的时候就不要啦〜〜
    		ret++;
    	}
    }
    count=count-ret;//计算有效关卡数
    return count;

}
```

好了，上面的代码，不难吧？咱编译一下，就可以跑了
linux下
```
gcc -o game main.c -Wl,rpath=. 
```
mac下
```
gcc -o game main.c -Wl,rpath . -dynamic
```

再准备一个游戏菜单文件，执行
```
echo firstMission.so >missionlist.txt
```
好了，再执行
```
./game 
```

一切顺利的话，你应该能看到自己的关卡列表了〜
接下来干嘛呢？我们现在仅仅是提取出了关卡的名字，还没开始玩游戏呢
以下是改动后的main.c文件

```
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
    //展示下关卡
    for(int i=0;i<count;i++)
    {
        if(mission[i].level!=0)
        {
            printf("%d.%s\n",i+1,mission[i].missionName);//正式发布的时候就不要啦〜〜
        }
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
    

    while(1)
    {
        p.life=100;
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

```

这就是一个完整的游戏啦，感兴趣的同学可以继续扩展哟〜〜后面就是不断得出扩展啦

你问我怎么扩展？
好吧，自己把game.c里面的内容改改，然后编译出来，换个名字，然后在missionlist.txt里面加一行，就ok了

还不懂？那你只能给我打赏了^_^




