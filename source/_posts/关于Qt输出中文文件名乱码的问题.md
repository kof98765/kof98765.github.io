---
title: 关于Qt输出中文文件名乱码的问题
date: 2017-03-07 12:21:26
tags:
---

最近为项目添加一个电能数据导出的功能
导出的格式为csv,一开始文件名是1.csv,2.csv这样的，不过客户不满意呀〜〜
那只能改成带有中文的呗

环境：嵌入式linux
文件编码:utf-8

QFile f;
QString head=QString("time,current,voltage,activePower,reactivePower,energy\n");
QString name=QString("%1/直流支路%2数据.csv").arg("data").arg(i+1);
QString energyData="12344";
f.setFileName(name);
f.open(QIODevice::WriteOnly);
f.write(head.toLocal8Bit());
f.write(energyData.toUtf8());
f.close();

在嵌入式linux下，输出的文件名是乱码〜，因为在嵌入式linux下没有做中文处理
不要紧，把文件输出到windows就行
有一点要注意，这块代码的编码必须是utf-8的。。。
平时我处理中文的时候，一般都会使用gbk编码来存代码，结果嘛，各种乱码输出。。。

这段代码唯一不足的就是head不能是中文，残念。。
