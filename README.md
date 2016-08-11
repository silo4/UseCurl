# 简介

本例主要是用于试用curl网络库，如接入方法，api使用方法等，学习使用。

另外一个重点，就是学习如何编译支持openssl和nghttp2的curl库。       



# 编译 curl + openssl + nghttp2


## 编译环境 



* #### Linux
Linux ubuntu 4.2.0-27-generic #32~14.04.1-Ubuntu SMP Fri Jan 22 15:32:26 UTC 2016 x86_64 x86_64 x86_64 GNU/Linux


* #### ndk
android-ndk-r11c

* #### jdk
jdk1.8.0_91

* #### 环境变量
export JAVA_HOME=/usr/local/java/jdk1.8.0_91
export JRE_HOME=$JAVA_HOME/jre
export CLASSPATH=.:$JAVA_HOME/lib:$JAVA_HOME/jre/lib
export ANDROID_NDK=/home/zhonglz/tools/android-ndk-r11c
export ANDROID_SDK=/root/Android/Sdk
export ANDROID_ABI=armeabi

	export CC=arm-linux-androideabi-gcc
export ARM_GCC=/tmp/ndk/bin
export LDFLAGS="-L$ANDROID_NDK/platforms/android-9/arch-arm/usr/lib"
export CPPFLAGS="-I$ANDROID_NDK/platforms/android-9/arch-arm/usr/include"

	export PATH=$PATH:$JAVA_HOME/bin:$ANDROID_NDK:$ANDROID_SDK:$ANDROID_SDK/platform-tools:$ANDROID_SDK/tools:$ANDROID_NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/darwin-x86_64/arm-linux-androideabi/bin:$ANDROID_NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/darwin-x86_64/bin:$ARM_GCC

* #### Linux执行编译时权限
root



## 编译openssl


* #### 下载并解压源码(本例使用openssl-1.0.1t)
	[download here](https://www.openssl.org/source/old)
* #### 将编译脚本移至源码目录
	[android-config-for-openssl.sh](https://github.com/silo4/UseCurl/blob/master/android-config-for-openssl.sh)
	
	cp android-config-for-openssl.sh openssl-1.0.1t/
* #### 配置脚本
	将 [android-config-for-openssl.sh](https://github.com/silo4/UseCurl/blob/master/android-config-for-openssl.sh) 打开，对里面的参数进行相关配置，如下部分所示
	
	 ANDROID_NDK_ROOT=$ANDROID_NDK
	 
	 _ANDROID_NDK="android-ndk-r11c"	
	 
	 _ANDROID_EABI="arm-linux-androideabi-4.9"
	 
	 _ANDROID_ARCH=arch-arm
	 
	 _ANDROID_API="android-9"
	 
* #### 执行脚本
	 ./android-config-for-openssl.sh.sh
	 
* #### 执行成功后
	这时在/usr/local/ssl/里有android-9文件夹生成，所需的libssl.so libcrypto.so 等动态库、静态库和include 头文件均在这里生成
	
## 编译nghttp2

* #### 下载并解压源码(本例使用nghttp2-1.13.0)
	[download here](http://www.nghttp2.org/)
* #### 将编译脚本移至源码目录
	[android-config-for-nghttp2.sh](https://github.com/silo4/UseCurl/blob/master/android-config-for-nghttp2)
	
	cp android-config-for-nghttp2.sh nghttp2-1.13.0/

* #### 执行脚本
	 ./android-config-for-nghttp2.sh
	 
* #### 执行成功后
	没有指定路径，所以默认在/usr/local/lib里生成库文件libnghttp2.so, libnghttp2.a, 头文件则在/usr/loca/include/nghttp2里

## 编译curl


* #### 下载并解压源码(本例使用curl-7.50.1)
	[download here](https://curl.haxx.se/download.html)
* #### 将编译脚本移至源码目录
	[android-config-for-curl.sh](https://github.com/silo4/UseCurl/blob/master/android-config-for-curl.sh)
	
	cp android-config-for-curl.sh curl-7.50.1/

* #### 执行脚本
	注意：由于要让curl支持 ssl 和 nghttp2，所以编译时要打开开关，并指定相应的库路径，如下部分所示
	
	./configure --host=arm-linux-androideabi \
	
	--with-ssl=/usr/local/ssl/android-9\

	--with-nghttp2=/usr/local \
	
	--disable-ftp \
	
	--disable-gopher \
	
	--disable-file \

	这也是脚本文件里的部分内容.
	
	执行:
	
	 ./android-config-for-curl.sh
	 
	 
* #### 执行成功后
	没有指定路径，所以默认在/usr/local/lib里生成库文件libcurl.so libcurl.a, 头文件则在/usr/loca/include/curl里;
	或者在当前目录即curl-7.50.1/lib/.libs 里也有；
	
## 使用注意 

##### 说明

* 编译完的curl库已经支持ssl和http2了，jni 将使用动态库是libcurl.so，头文件则是include/curl路径，由于是动态链接，使用时也需要将libssl.so, libcrypto.so和libnghttp2.so也一并拷贝.

* Android 在调用 so 库时是用的System.loadLibrary(),由于libcurl.so与libssl.so,libcrypto.so,libnghttp2.so相关联，所以会libcurl.so被加载时，这些库也会被加载。

* linux 编译动态库是会带版本号,如libssl.so.1.0.0 ，而名为libssl.so的文件其实是libssl.so.1.0.0的引用，也就是快捷方式。

##### 问题来了

1. 在加载so库的时候，会先去系统目录/system/lib下找寻，而恰巧里面有系统自带的libssl.so和libcrypto.so，这会导致崩溃，而且与我们需要的不一致（比如版本不一样）。这就使得我们必须去改变这些名字，好让加载so库时能加载到我们自己编译的版本而不是系统版本。

2. 带版本号的库libssl.so.1.0.0, 会使jni在加载so库时出现问题;


##### 下面两种方法可以解决上述问题（动态库静态库都一样改法，这里就讲动态库）

###### 1. 编译libssl.so libcrypto.so libnghttp2.so前，修改相关配置或脚本，让其生成指定的名字， 比如改为 libsss.so libccc.so 。然后，编译libcurl.so时，也要修改相关脚本或配置，使其依赖libsss.so libccc.so 这些库，而不是先前名字那些库；另外，也可以修改相关脚本或配置，使其编译时创建引用库和不出现版本号在.so后面，这样就能解决上述问题。


###### 2. 对so二进制文件进行修改以达到改名目的

	* 按前面的所说的方法进行编译，得到了libssl libcrypto libnghttp2等相关的库；
	
	* 使用命令 readelf -a libssl.so > ssl.txt ,打开ssl.txt分析so相关信息。
	
		so文件的.plt段记录了文件名称和依赖库名称，如 libssl.so库的.plt 字段可能有信息
	
	    0x00000001 (NEEDED)                     Shared library: [libcrypto.so]
	    
	    0x00000001 (NEEDED)                     Shared library: [libdl.so]
	    
 		0x00000001 (NEEDED)                     Shared library: [libc.so]
 		
 		0x0000000e (SONAME)                     Library soname: [libssl.so]
 		
 		
 		其中(NEEDED)就是依赖的库，而(SONAME)就是自己的库名称。要修改库名和依赖库也就是对这些进行修改。
 		
 	    按这种方法分别对libcurl.so libssl.so libcrypto.so libnghttp2.so进行分析可以知道，
 	    1. libcurl.so依赖 libssl.so, libcrypto.so, libnghttp2.so
 	    2. libssl.so 依赖 libcrypto.so
 	    3. 其他无相互依赖
 	 
 	 * 鉴于以上对依赖的分析，以及要解决名字问题，得出以下改so的方案
 		
 		1. 修改libcrypto.so 的[SONAME] 为 libccc.so;
 		2. 修改libssl.so 的 [SONAME] 为 libsss.so, [NEEDED]的libcrypto.so改为libccc.so;
 		3. 修改libcurl.so的[NEEDED], libcrypto.so 改为 libccc.so, libssl.so 改为libsss.so
 		4. libnghttp2.so可以不做任何修改；
 		
 	 * 修改so的工具，由于是二进制修改，可以搜到很多二进制文件修改工具。如高级到windowns版的IDA(高级反编译工具，这里也能使用)，
 	   
 	   简单到Mac版的Hex Fiend，甚至文本编辑器也可以，只要有二进制插件，并能找到要改的地方。这里主要说明Mac版的Hex Fiend工具.
 	 
 	   拿libssl.so举例
 	   
 	   1.用Hex Fiend打开libssl.so
 	   2.Ctrl+F 进行Text查找  (工具支持Hex和Text查找), 查找libssl.so（这是文件名)
 	   3.Hex Fiend显示是有左右两侧的，左侧是二进制内容，右侧就是Text内容，修改时对左侧进行修改，如右侧显示的libssl.so字符串，
 	    左侧对应的二进制则是6C 69 62 73 73 6C 2E 73 6F,要改为libsss.so 只需将6C 改为 73即可。 
 	    若要名字改短,如改为libss.so，则去掉6C的同时，记得要在原末尾6F后面再补一个 00。
 	    或要名字改长，自己再去琢磨吧，只要不影响其他地址即可。
 	   4.改文件名搜libssl.so进行修改，改依赖库的名就搜libcrypto.so，修改方法是一样的。这样，就完成修改了。
 	   
 	* 修改完成后就变成了
 	 
 	 	1. libcurl.so依赖 libsss.so, libccc.so, libnghttp2.so
 	    2. libssl.so 改名为libsss.so, 并依赖的是 libccc.so
 	    3. 其他不变；
 	    
 	    
 	    
# 至此完工
	







