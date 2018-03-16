---
title: apache+php搭建经历
date: 2017-03-29 23:39:38
tags:
---

最近公司使用的嵌入式网页服务器来做web开发，boa+cgi的，用起来太恶心了，所以。。。
网上转了一圈，现在比较流行apache+php呀，那咱也造一个呗

先下载些材料 ，列表如下：
apache 2.4.6一份,地址：http://archive.apache.org/dist/httpd/httpd-2.4.6.tar.gz
apr 1.5.2一份，地址：https://mirrors.tuna.tsinghua.edu.cn/apache//apr/apr-1.5.2.tar.gz
apr-util 1.5.4 一份，地址：https://mirrors.tuna.tsinghua.edu.cn/apache//apr/apr-util-1.5.4.tar.gz
pcre-8.37一份，地址：https://ftp.pcre.org/pub/pcre/pcre-8.37.tar.gz
php-5.5.38一份，地址：http://cn.php.net/distributions/php-5.5.38.tar.gz
libxml2
